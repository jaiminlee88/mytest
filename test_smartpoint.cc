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

int main() {
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
        if (!wp.expired()) {
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

    struct MyClass {
        MyClass() { std::cout << "MyClass constructed" << std::endl; }
        ~MyClass() { std::cout << "MyClass destroyed" << std::endl; }

        void foo() {
            std::cout << "foo called" << std::endl;
        }
    };
    // scope_ptr<MyClass> ptr1(new MyClass()); // this is boost


    class MyClass_1 : public std::enable_shared_from_this<MyClass_1> {
    private:
        int a{3};
    public:
        void doSomething() {
            std::cout << "doSomething() called." << std::endl;
            std::shared_ptr<MyClass_1> sharedPtr = shared_from_this();
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