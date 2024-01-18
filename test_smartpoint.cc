#include<iostream>
#include<memory>
#include<vector>
#include<algorithm>
#include<functional>
#include<numeric>
using namespace std;


class report{
public:
    report(const std::string s):str(s){
        std::cout << "report()" << std::endl;
    }
    ~report(){
        std::cout << "~report()" << std::endl;
    }
    void comment() const {
        std::cout << "report::comment()-->str=" << str << std::endl;
    }
private:
    std::string str;
};

std::unique_ptr<int> make_int(int n) {
    return std::unique_ptr<int>(new int(n));
}
void show(std::unique_ptr<int>& p) {
    std::cout << "show: " << *p << std::endl;
}

#if __cplusplus <= 201103L
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params) {
    cout << "[make_unique] use defined make_unique" << endl;
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
#endif

int main() {
    // 智能指针内存安全，不会抛出错误 没有潜在的资源泄漏
    // std::auto_ptr<double> pd(new double(5.6));
    // std::shared_ptr<int> p1(new int(1));
    // { //auto_ptr c++11已经弃用
    //     std::auto_ptr<report> ps(new report("using auto_ptr"));
    //     ps->comment();
    // }
    { // 括号确保打印完毕后，ps就会被销毁
        cout << "===========test shared_ptr" << endl;
        std::shared_ptr<report> ps(new report("using shared_ptr"));
        ps->comment();
    }
    {
        cout << "===========test unique_ptr" << endl;
        std::unique_ptr<report> ps(new report("using unique_ptr"));
        ps->comment();
        std::unique_ptr<report> ps1;
        // ps1 = ps; // invalid, 不安全的解除， ps.get()获取原始指针
        cout << "before move ps:" << ps.get() << " ps1:" << ps1.get() << endl;
        ps1 = move(ps); // valid
        cout << "after move ps:" << ps.get() << " ps1:" << ps1.get() << endl;

        auto f = []() -> std::unique_ptr<report> {
            return std::unique_ptr<report>(new report("using unique_ptr in lambda")); // right value
        };
        std::shared_ptr<report> sp(f()); // unique_ptr转换为shared_ptr
        sp->comment();
    }

    {
        cout << "===========test shared_ptr 1" << endl;
        std::shared_ptr<double> pd;
        double *p_reg = new double(5.6);
        // pd = p_reg; // invalid conversion from ‘double*’ to ‘std::shared_ptr<double>::element_type* {aka double*}’ [-fpermissive]
        // std::shared_ptr<double> pa = p_reg; // invalid
        // std::shared_ptr<double> pa(p_reg); // valid
        cout << p_reg << endl;
        // pd.reset(p_reg); // valid, 清空原来指向对象的空间，指向新的对象,pd没有指向空间此时
        cout << p_reg << endl;
        pd = std::shared_ptr<double>(p_reg); // valid
        cout << "pd.use_count=" << pd.use_count() << endl;
        std::shared_ptr<double> pd1(pd);
        cout << "pd.use_count=" << pd.use_count() << " pd1.use_count=" << pd1.use_count() << endl;
        pd1.reset();
        cout << "pd.use_count=" << pd.use_count() << " pd1.use_count=" << pd1.use_count() << endl;
    }

    {
        cout << "===========test weak_ptr 1" << endl;
        // weak_ptr
    // weak_ptr是一种用于解决shared_ptr相互引用时产生死锁问题的智能指针。如果有两个shared_ptr相互引用，那么这两个shared_ptr指针的引用计数永远不会下降为0，
    //     资源永远不会释放。weak_ptr是对对象的一种弱引用，它不会增加对象的use_count，weak_ptr和shared_ptr可以相互转化，shared_ptr可以直接赋值给weak_ptr，
    //     weak_ptr也可以通过调用lock函数来获得shared_ptr。
    // weak_ptr指针通常不单独使用，只能和 shared_ptr 类型指针搭配使用。将一个weak_ptr绑定到一个shared_ptr不会改变shared_ptr的引用计数。
    //     一旦最后一个指向对象的shared_ptr被销毁，对象就会被释放。即使有weak_ptr指向对象，对象也还是会被释放。
    // weak_ptr并没有重载operator->和operator *操作符，因此不可直接通过weak_ptr使用对象，典型的用法是调用其lock函数来获得shared_ptr示例，进而访问原始对象
        std::shared_ptr<int> p1(new int(42));
        std::weak_ptr<int> wp(p1);
        std::cout << "wp.use_count() = " << wp.use_count() << std::endl;
        if (!wp.expired()) { // 指针仍然存在
            std::shared_ptr<int> p2 = wp.lock(); // 提升为shared_ptr，不阻塞，需要查看p2是否为空
            *p2 = 100;
            std::cout << "p2 = " << *p2 << std::endl;
        }
        std::cout << "wp.use_count() = " << wp.use_count() << std::endl;
        p1.reset();
        std::cout << "wp.use_count() = " << wp.use_count() << std::endl;
        if (!wp.expired()) {
            std::shared_ptr<int> p2 = wp.lock();
            *p2 = 100;
            std::cout << "p2 = " << *p2 << std::endl;
        } else {
            std::cout << "wp is expired." << std::endl;
        }
    }

    {
        cout << "===========test unique_ptr 1" << endl;
        // unique_ptr
        std::unique_ptr<int> p1(new int(42));
        std::unique_ptr<int> p2 = std::move(p1);
        std::cout << "p2 = " << *p2 << std::endl;
        // std::cout << "p1 = " << *p1 << std::endl; // invalid
        std::unique_ptr<int> p3;
        // p3 = p2; // invalid
        p3 = std::move(p2);
        std::cout << "p3 = " << *p3 << std::endl;
        // std::cout
    }

    {
        cout << "===========test unique_ptr 2" << endl;
        std::vector<std::unique_ptr<int>> vp(5);
        for (int i=0; i<vp.size(); i++) {
            vp[i] = make_int(rand() % 1000);
        }
        vp.push_back(make_int(rand() % 1000));
        for_each(vp.begin(), vp.end(), show); // show必须引用
    }
    // std::string s = "hello";
    // std::shared_ptr<std::string> p1(&s); // no,不可以，因为s不是new出来的，还会double free

    // make_shared函数的主要功能是在动态内存中分配一个对象并初始化它，返回指向此对象的shared_ptr;
    // 由于是通过shared_ptr管理内存，因此一种安全分配和使用动态内存的方法。
    std::shared_ptr<string> p4 = make_shared<string>("hello");
    int tmp = 9;
    std::shared_ptr<int> p5 = make_shared<int>(std::move(tmp));
    std::cout << "p4=" << *p4 << " p5=" << *p5 << " tmp=" << tmp << std::endl;
    std::cout << "done." << std::endl;

    std::shared_ptr<int> p6 = std::make_shared<int>(42);
    cout << "p6=" << *p6 << endl;
    std::shared_ptr<int> p7{nullptr};
    if (p7) {
        cout << "p6=" << *p6 << " p7=" << *p7 << endl;
    } else {
        cout << "p6=" << *p6 << " p7 is nullptr" << endl;
    }
    p7.swap(p6);
    if (p6) {
        cout << "p6=" << *p6 << " p7=" << *p7 << endl;
    } else {
        cout << "p6 is nullptr" << " p7=" << *p7 << endl;
    }

    std::shared_ptr<int> p8(new int(42), [](int* a){
        cout << "[p8] delete int " << *a << endl;
        delete a;
    });

    // 定制智能指针 析构函数
    std::shared_ptr<string> p9;
    p9.reset(new string("hello"), [](string* str) {
        cout << "[p9] delete string " << *str << endl;
        delete str;
    });
    // shared_ptr的大小不受删除器的影响，因为删除器是通过模板参数传递的，而不是存储在shared_ptr中的
    cout << "-------sizeof(p7)=" << sizeof(p7) << " sizeof(p8)=" << sizeof(p8) << " sizeof(p9)=" << sizeof(p9) << endl;

    // 重要！！！！！！！！！！！！！！！！！！！
    /*
    shared_ptr ptr，删除器是模板参数传奇，除指针外ptr->point，还有一个指针指向控制模块ptr->control_block（std::make_shared总是创建），
                    控制模块包含reference count，weak count，otherdata（删除器等），而且会多个shared_ptr会共享同一个控制模块
                    shared_ptr<int> p(new int(7)); 会分配两次内存，一个是指针，一个是控制模块
                    shared_ptr<int> p1(make_shared<int>(7)); 只会分配一次内存，指针和控制模块在一起，效率更高，打不能使用自定义删除器
                    unique_ptr<int> p2(new int(7)); 只能用new
                    shared_ptr引用结束时，引用计数减1，如果引用计数为0，释放对象的内存，但此时控制模块内存还在
                    weak_ptr引用结束时，控制模块内存才会被销毁，这样就会导致对象内存和控制内存销毁时间延长
                    make_shared，需要使用自定义删除器，希望用花括号初始化，有自定义内存管理类，或者非常大的对象，或者weak比shared声明周期更久，都不适合用make_shared
    unique_ptr ptr，删除器是模板本身一部分，删除器大小会影响ptr的大小
    */
    class deleter {
    public:
        void operator()(std::string* str) {
            cout << "delete string " << *str << endl;
            delete str;
        }
    private:
        int a{10}; // 不能有状态，否则指针ptr附带该类使得sizeof(ptr)会变大
        int b{11}; // 不能有状态，否则指针ptr附带该类使得sizeof(ptr)会变大
    };
    std::unique_ptr<std::string, deleter> p10(new std::string("[p10] hello"));
    auto delfunc = [](std::string* str){ // 尽量用lambda，注意不要有太多状态
        cout << "[delfunc] delete string :" << *str << endl;
        delete str;
    };
     // 注意，与shared_ptr不同，unique_ptr的删除器类型是作为模板参数传递的
    std::unique_ptr<std::string, decltype(delfunc)> p11(new std::string("[p11] hello"), delfunc);
    // 所以shared_ptr可以接收不同删除器并放到同一个vector中，但是unique_ptr只能用相同删除器
    cout << "------------sizeof(p10)=" << sizeof(p10) << " sizeof(p11)=" << sizeof(p11) << endl;

    std::unique_ptr<std::string> p12(new string("i am p12"));
    std::shared_ptr<std::string> p13(p12.release()); // p12释放所有权，p13接管所有权
    cout << "[p13]=" << *p13 << endl;
    // cout << "[p12]=" << *p12 << endl; //  invalid, p12已经释放所有权
    std::unique_ptr<std::string> p14(new string("--->abc"));
    std::unique_ptr<std::string,decltype(delfunc)> p15(new string("--->abc"), delfunc);
    cout << "--->sizeof(p14)=" << sizeof(p14) << " sizeof(p15)=" << sizeof(p15) << endl;

    std::shared_ptr<int> p16(new int(42)); // 引用计数动态分配
    std::shared_ptr<int> p17(std::make_shared<int>(42)); // 避免引用计数的动态分配
    // std::shared_ptr<int> p17(std::make_shared<int>(42), [](int* a){ delete a;}); // invalid，make_shared不允许使用自定义删除器
    cout << "[p16] p16=" << *p16 << " sizeof(p16)=" << sizeof(p16) << " p16.use_count=" << p16.use_count() << endl;
    cout << "[p17] p17=" << *p17 << " sizeof(p17)=" << sizeof(p17) << " p17.use_count=" << p17.use_count() << endl;
    std::weak_ptr<int> wp(p16);
    cout << "--->sizeof(int)="<< sizeof(int) << " sizeof(p16)=" << sizeof(p16) << " sizeof(wp)=" << sizeof(wp) << endl;

    std::shared_ptr<int> p18(std::move(p17)); // 移动构造函数会更快，且不改变引用计数，P18创建控制模块，P17没有控制模块
    cout << "[p18]=" << *p18 << " p18.count=" << p18.use_count() << " p17.count=" << p17.use_count()  << endl;
    // cout << "[p17]=" << *p17 << endl; // invalid, p17已经释放所有权
    // std::unique_ptr<int> p18_(std::move(p18)); // invalid，unique_ptr不能从shared_ptr转移

    int *p = new int(42);
    std::shared_ptr<int> p19(p,[](int* a){ // 尽量避免传入原始指针，最好用 make_shared，或者直接 new
        cout << "[p19] delete int" << endl;
        delete a;
    }); // 创建一个控制块
    // std::shared_ptr<int> p20(p, [](int* a){
    //     cout << "[p20] delete int" << endl;
    //     delete a;
    // }); // 创建另一个控制块，会double free
    std::shared_ptr<int> p21(p19); // 共享同一个控制块
    cout << "[p19]=" << *p19 << " p19.count=" << p19.use_count() << " p21.count=" << p21.use_count() << endl;

    // std::shared_ptr<int> p22(new int[]{1,2,3}); // invalid,不允许使用数组
    p = nullptr;
    std::shared_ptr<int> p23(p); // 也不会报错

    std::shared_ptr<int> p24(new int(42));
    std::weak_ptr<int> wp24(p24); // weak_ptr是shared_ptr衍生出来的，不会改变引用计数（std::weak_ptr操作的是第二个引用计数），与shared_ptr共享控制块
    cout << "[p24]=" << *p24 << " p24.count=" << p24.use_count() << " wp24.count=" << wp24.use_count() << endl;
    if (!wp24.expired()){
        cout << "[wp24] wp24 is not expired : " << wp24.lock() << endl; // lock直接提升为shared_ptr
    } else {
        cout << "[wp24] wp24 is expired : " << wp24.lock()<< endl;
    }
    p24.reset(); // 引用计数减1，但是不会释放内存，因为wp24还在引用
    if (!wp24.expired()){
        cout << "[wp24] wp24 is not expired : " << wp24.lock() << endl;
    } else {
        cout << "[wp24] wp24 is expired : " << wp24.lock() << endl; // wp24.lock()返回空指针nullptr
    }
    // std::weak_ptr<int> wp25(p); // invalid，不能从原始指针创建weak_ptr
    // if (!wp25.expired()){
    //     cout << "[wp25] wp25 is not expired" << endl;
    // } else {
    //     cout << "[wp25] wp25 is expired" << endl;
    // }

    std::unique_ptr<int> p26(make_unique<int>(42));
    cout << "[p26]=" << *p26 << endl;
    auto p27 = make_unique<int>(42);
    cout << "[p27]=" << *p27 << endl;

    //创建std::initializer_list，使用花括号初始化
    auto initList = { 10, 20 };
    //使用std::initializer_list为形参的构造函数创建std::vector
    auto spv = std::make_shared<std::vector<int>>(initList); // 区分了list
    cout << "[spv]: ";
    for (auto it = spv->begin(); it != spv->end(); ++it) {
        std::cout << *it << ", ";
    }
    cout << endl;


    struct MyClass {
        MyClass() { std::cout << "MyClass constructed" << std::endl; }
        ~MyClass() { std::cout << "MyClass destroyed" << std::endl; }

        void foo() {
            std::cout << "foo called" << std::endl;
        }
    };
    // scope_ptr<MyClass> ptr1(new MyClass()); // this is boost


    // 奇异递归模板模式（The Curiously Recurring Template Pattern（CRTP））
    class MyClass_1 : public std::enable_shared_from_this<MyClass_1> {
    private:
        int a{3};
    public:
        void doSomething() {
            std::cout << "doSomething() called." << std::endl;
            std::shared_ptr<MyClass_1> sharedPtr = shared_from_this(); // shared_from_this查找当前对象控制块，然后创建一个新的std::shared_ptr关联这个控制块
            // 使用sharedPtr进行其他操作
            sharedPtr->foo();
        }
        void foo() {
            std::cout << "foo() called. a=" << a << std::endl;
        }
    };
    std::shared_ptr<MyClass_1> sharedPtr(new MyClass_1);
    sharedPtr->doSomething();


    return 0;
}