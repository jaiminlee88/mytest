#include<iostream>
#include<typeinfo>
#include <vector>
#include <memory>
#include <set>
#include <chrono>

#define DBG 1
using namespace std;

void f(int && param) {             //右值引用

}

template<typename T>
void f(std::vector<T>&& param){     //右值引用
    cout << "f(std::vector<T>&& param)" << endl;
}

template<typename T>
void f(T&& param){ // 通用引用，引用折叠reference collapsing
    //不是右值引用,是通用引用（universal references）第一种情况，只有对右值才会产生真正的右值引用
    // 注意，要求必须是T&&形式
    cout << "f(T&& param)" << endl;
}

template<typename T>
void f1(std::vector<T>&& param){ // 注意是右值引用,不是通用引用，因为不是T&&形式
    cout << "f1(std::vector<T>&& param)" << endl;
}

template<typename T>
void f2(const T&& param){ // 注意是右值引用,不是通用引用，应为const
    cout << "f1(std::vector<T>&& param)" << endl;
}

struct SomeDataStructure {
    int a;
    int b;
};
class Widget;
class Widget {
public:
    template<typename T>
    void setName(T&& newName) {       //通用引用可以编译，
        name = std::move(newName);  //但是代码太太太差了！
    }
    std::string getName() { return name; }
    Widget makeWidget() {                //makeWidget的移动版本
        Widget w;
        w.name = name;
        w.p = p;
        return std::move(w);            //移动w到返回值中（不要这样做！）
        // 返回值优化（return value optimization，RVO）
        // 编译器自行决定是否移动，可能拷贝，可能移动
        // 优化消除条件（1）局部对象与函数返回值的类型相同；（2）局部对象就是要返回的东西。
        // 不要试图参与拷贝消除
        // 如果局部对象可以被返回值优化消除，就绝不使用std::move或者std::forward。
    }

private:
    std::string name;
    std::shared_ptr<SomeDataStructure> p;
};

std::string getWidgetName() {
    return "123";
}
void test1() {
    // std::string getWidgetName();        //工厂函数
    Widget w;
    auto n = getWidgetName();           //n是局部变量
    cout << "before n=" << n << endl;
    w.setName(n);                       //把n移动进w！并强制转换为右值
                                        // 调用移动赋值运算符，而不是拷贝赋值运算符
                                        //现在n的值未知，
    cout << "after n=" << n << endl;
    cout << "w.getName()=" << w.getName() << endl;
    auto w1 = w.makeWidget();
    cout << "w1.getName()=" << w1.getName() << endl;
}

void log(std::chrono::system_clock::time_point t, const std::string& str) {
    cout << "[log] " << str << endl;
}
std::string nameFromIdx(int idx) {
    return "123";
}

std::multiset<std::string> names;           //全局数据结构
void logAndAdd(const std::string& name) { // 效率低下写法
    auto now = std::chrono::system_clock::now(); //获取当前时间  
    log(now, "logAndAdd " + name);                  //志记信息
    names.emplace(name);                    //把name加到全局数据结构中；
}
template<typename T>
void logAndAdd_Opt(T&& name) { // 通用引用，引用折叠reference collapsing， 效率较高
    auto now = std::chrono::system_clock::now(); //获取当前时间  
    log(now, "template logAndAdd ");                  //志记信息
    names.emplace(std::forward<T>(name));                    //把name加到全局数据结构中；
}
void logAndAdd_Opt(int idx) {
    auto now = std::chrono::system_clock::now(); //获取当前时间  
    log(now, "logAndAdd_Opt(int idx) ");                  //志记信息
}

class Person {
public:
    template<typename T>
    explicit Person(T&& n): name(std::forward<T>(n)) {
        //完美转发的构造函数，初始化数据成员
        cout << "[Person] construct Person(T&& n): name(std::forward<T>(n))" << endl;
    }

    explicit Person(int idx): name(nameFromIdx(idx)) {
        //int的构造函数
        cout << "[Person] construct Person(int idx): name(nameFromIdx(idx))" << endl;
    }
private:
    std::string name;
};

template<typename T>                            //非整型实参：添加到全局数据结构中
void logAndAddImpl(T&& name, std::false_type)	{//译者注：高亮std::false_type
    auto now = std::chrono::system_clock::now();
    log(now, "logAndAddImpl(T&& name, std::false_type)");
    names.emplace(std::forward<T>(name));
}
std::string nameFromIdx(int idx);           //与条款26一样，整型实参：查找名字并用它调用logAndAdd
void logAndAddImpl(int idx, std::true_type) {//译者注：高亮std::true_type
    auto now = std::chrono::system_clock::now();
    log(now, "logAndAddImpl(int idx, std::true_type)");
    // logAndAdd(nameFromIdx(idx));
}
template<typename T>
void logAndAdd_distinparam(T&& name) { // 通过判断参数类型来选择重载哪个logAndAddImpl函数
    // 类型std::true_type和std::false_type是“标签”（tag），其唯一目的就是强制重载解析按照我们的想法来执行
    // tag dispatch 设计方法
    logAndAddImpl(
        std::forward<T>(name),
        std::is_integral<typename std::remove_reference<T>::type>()
    );
}


class Person_1 {
public:
     template<typename T,
              typename = typename std::enable_if<  //使用std::enable_if可以使得仅在std::enable_if指定的条件满足时模板才启用
                                        //std::decay<T>::value与T是相同的，只不过会移除引用和cv限定符（cv-qualifiers，即const或volatile标识符）的修饰
                                        // !is_base_of<Person,typename std::decay<T>::type> // 判断是否是 Person 的派生类
                                        !std::is_same<Person_1,typename std::decay<T>::type> // 不相同时时才启动
                                                    ::value
                                      >::type
             >
    explicit Person_1(T&& n): name(std::forward<T>(n)) {
        //完美转发的构造函数，初始化数据成员
        cout << "[Person_1] construct Person_1(T&& n): name(std::forward<T>(n))" << endl;
    }

    explicit Person_1(int idx): name(nameFromIdx(idx)) {
        //int的构造函数
        cout << "[Person_1] construct Person_1(int idx): name(nameFromIdx(idx))" << endl;
    }
private:
    std::string name;
};

void test2() {
    std::string petName("Darla");
    logAndAdd(petName);                     //传递左值std::string, 只能拷贝
    logAndAdd(std::string("Persephone"));	//传递右值std::string, 只能拷贝
    logAndAdd("Patty Dog");                 //传递字符串字面值, 传递右值到左值引用,构造临时string，并拷贝，效率低下

    logAndAdd_Opt(petName);                     //传递左值std::string, 只能拷贝 拷贝左值到multiset
    logAndAdd_Opt(std::string("Persephone"));	//传递右值std::string, 移动右值而不是拷贝它
    logAndAdd_Opt("Patty Dog");                 //在multiset直接创建std::string  而不是拷贝一个临时std::string.

    logAndAdd_Opt(22); // 精准匹配
    int a = 22;
    logAndAdd_Opt(a); // 精准匹配
    // long b = 22;
    // logAndAdd_Opt(b); // 不能匹配int，会调用模板 logAndAdd_Opt(T&& name)，但是name本身是string的，类型不匹配而报错 模板匹配很贪婪

    // 避免对通用引用形参的函数进行重载。完美转发构造函数是糟糕的实现
    Person p("Nancy"); // 左值
    // auto cloneOfP(p); //从p创建新Person cloneOfP；这通不过编译！cloneOfP会调用 explicit Person(T&& n): name(std::forward<T>(n))
    cout << "---before auto cloneOfP(p)" << endl;
    auto cloneOfP = p; // valid， 调用 operator=(const Person& rhs);  不会调用 explicit Person(T&& n): name(std::forward<T>(n))
    cout << "---after auto cloneOfP(p)" << endl;

    const Person p1("jack"); // 右值
    cout << "---before auto cloneOfP1(p1)" << endl;
    auto cloneOfP1(p1); // valid 调用构造器自行生成的 Person(const Person& rhs);      不会调用 explicit Person(T&& n): name(std::forward<T>(n))
    cout << "---after auto cloneOfP1(p1)" << endl;

    // 通用重载的替代方法，约束模板
    // 1 放弃重载，另取新名字
    // 2 传递const T&
    // 3 将按传引用形参替换为按值传递
    // 4 使用tag dispatch约束模板，但是要注意，类可能会生成一些默认构造函数，使得行为超出预期
    logAndAdd_distinparam(petName);                     //传递左值std::string, 只能拷贝 拷贝左值到multiset
    logAndAdd_distinparam(std::string("Persephone"));	//传递右值std::string, 移动右值而不是拷贝它
    logAndAdd_distinparam("Patty Dog");                 //在multiset直接创建std::string  而不是拷贝一个临时std::string
    logAndAdd_distinparam(a); // 精准重载logAndAddImpl(int idx, std::true_type)
    short b = 22;
    logAndAdd_distinparam(b); // 精准重载logAndAddImpl(int idx, std::true_type)
    // 5 std::enable_if 可以给你提供一种强制编译器执行行为的方法，像是特定模板不存在一样。这种模板被称为被禁止（disabled）

    Person_1 p_1("Nancy"); // 左值
    auto cloneOfP_1("jack"); // 右值, 不是Person_1，用 explicit Person_1(T&& n): name(std::forward<T>(n))
    auto cloneOfP_2(p_1); // 左值,是Person_1,不能用 explicit Person_1(T&& n): name(std::forward<T>(n)), 调用默认的 Person_1(const Person_1& rhs)
    const Person_1 p_2("lam"); // 右值
    auto cloneOfP_3(p_2); // 右值,是Person_1, 用 explicit Person_1(T&& n): name(std::forward<T>(n)), p_2的const在判断时被decay移除
}

template<typename T>
void func(T&& param) {
    bool isRvalue = std::is_rvalue_reference<decltype(param)>::value;
    if (isRvalue) {
        cout << " [func(T&& param)] is_rvalue_reference : " << param << endl;
    }
    bool isLvalue = std::is_lvalue_reference<decltype(param)>::value;
    if (isLvalue) {
        cout << " [func(T&& param)] is_lvalue_reference : " << param << endl;
    }
}

void bar(int& val) {
    std::cout << "bar called with lvalue reference" << std::endl;
}

void bar(int&& val) {
    std::cout << "bar called with rvalue reference" << std::endl;
}
template<typename T>
void foo(T&& val) { // 引用折叠，答案是引用折叠（reference collapsing），禁止你声明引用的引用
    // 如果任一引用为左值引用，则结果为左值引用。否则（即，如果引用都是右值引用），结果为右值引用。
    std::cout << "foo called with universal reference" << std::endl;
    // 使用std::forward将val转发为正确的引用类型
    // std::forward的作用是当且仅当传给f的实参为右值时，即T为非引用类型，才将fParam（左值）转化为一个右值
    bar(std::forward<T>(val));
}

// template<typename T>
// class Widget_1 {
// public:
//     typedef T&& RvalueRefToT; // 也会发生引用折叠
//     RvalueRefToT a;
//     void test_mem_valtype() {
//         if (std::is_rvalue_reference<decltype(a)>::value) {
//             cout << "member a is rrvalue" << endl;
//         } else if (std::is_lvalue_reference<decltype(a)>::value) {
//             cout << "member a is lrvalue" << endl;
//         } else {
//             cout << "member a is neither lrvalue nor rrvalue" << endl;
//         }
//     }
// };
void test3() {
    // 引用折叠，即引用的引用情况
    // 引用折叠发生在四种情况下：模板实例化，auto类型推导，typedef与别名声明的创建和使用，decltype。
    // 当编译器在引用折叠环境中生成了引用的引用时，结果就是单个引用。有左值引用折叠结果就是左值引用，否则就是右值引用。

    string abc = "123";
    func(abc); // abc是左值，T是string&，param是string& &&，引用折叠，所以param是string&
    func("abc"); // 常量
    string&& abc_1 = std::move(abc); // 右值本身也是左值
    func(abc_1);
    func(std::move("111")); // 变成了右值
    func(27); // 右值引用
    const int a = 28;
    func(a); // 左值引用，常量

    int x;
    // auto& & y = x; // invalid
    auto&& z = x; // z是int&，引用折叠，所以y是int&
    // x是左值，auto推导出的类型是int&，所以z是int&，Widget& && z = x; 发生引用折叠，所以z是int&
    if (std::is_rvalue_reference<decltype(z)>::value) {
        cout << "z is rrvalue" << endl;
    } else if (std::is_lvalue_reference<decltype(z)>::value) {
        cout << "z is lrvalue" << endl;
    }
    auto&& z1 = 24; // z1是int&&，引用折叠，所以z1是int&&
    if (std::is_rvalue_reference<decltype(z1)>::value) {
        cout << "z1 is rrvalue" << endl;
    } else if (std::is_lvalue_reference<decltype(z1)>::value) {
        cout << "z1 is lrvalue" << endl;
    }
    Widget_1<int&> w; // w.a是int&&，引用折叠，所以w.a是int&&

    x = 5;
    foo(x); // foo called with universal reference，将x作为lvalue传递给foo，在foo内部的val被推导为int&类型，调用bar(int& val)
    foo(10); // foo called with universal reference，将10作为rvalue传递给foo，在foo内部的val被推导为int&&类型，调用bar(int&& val)
}

int main() {
    // 最后一次使用变量时，在右值引用上使用std::move，在通用引用上使用std::forward。
    static long long abc=0; // static或全局.bss段才有体现
    auto& abc_1="123456"; // 实际是"123456"的大小
    auto  abc_2="12345690"; // 实际b是指针
    std::cout << "sizeof(\"123456\")=" << sizeof("123456") << " sizeof(a)=" << sizeof(abc_1) << " sizeof(b)=" << sizeof(abc_2) << std::endl;
    string tmp = "def";

    int rats = 101;
    int &rodents = rats;
    cout << "before rats=" << rats << " address(rats)=" << &rats 
    << " rodents=" << rodents << " address(rodents)" << &rats << endl;
    int bunny = 50;
    rodents = bunny;
    cout << "before bunny=" << bunny << " address(bunny)=" << &bunny 
    << " rodents=" << rodents << " address(rodents)" << &rodents << endl;

    int&& var1 = 1; //右值引用
    auto&& var2 = var1; //不是右值引用，是通用引用，第二种情况
    f(var1);
    f(var2);
    std::vector<int> v{1,2,3}; // 左值
    // f1(v); // invalid，因为v是左值，不是右值，所以不会调用f1(std::vector<T>&& param)
    f1(std::move(v)); // valid，因为std::move(v)是右值，所以会调用f1(std::vector<T>&& param)

#if __cplusplus > 201103L
    auto timeFunc = [](auto&& func, auto&&... params){ // auto&& func通用引用
        std::forward<decltype(func)>(func)(     //对params调用func
            std::forward<delctype(params)>(params)...
        );
    };
#endif
    test1();
    test2(); // 通用重载
    test3(); // 引用折叠
    return 0;
}