#include<iostream>
using namespace std;
// #pragma comment(linker,"/entry:entrance_func")

class stock1 {
public:
    stock1() { // 无参构造函数，只有当不提供任何构造参数时，编译器才会默认提供
        cout << "construct func:" << __FUNCTION__ << " sym:" << sym << ", price:" << price  << endl;
    }
    stock1(string symbol, double price) : sym(symbol), price(price) {
        cout << "construct func:" << __FUNCTION__ << "(string symbol, double price). sym:" << sym << ", price:" << price  << endl;
    }
    ~stock1(){ // 若不写则默认构建一个空的析构函数
        cout << "destruct func:" << __FUNCTION__ << " sym=" << sym << endl;
    }
    void buy() { // 类内定义的都是默认内联函数
        cout << "buy stock1" << endl;
    }
    void sell() { // 类内定义的都是默认内联函数
        cout << "sell stock1" << endl;
    }
    void show(int a=10) const { // 表示结构体本身不会修改
        // price = 3.14; // 会报错
        cout <<  __FUNCTION__ << " sym=" << sym << " price=" << price << endl;
    }
    static int i; // 只能类外初始化
    static const int j{1}; // const情况下，可以在这初始化
private:
    string sym;
    double price;
};

class stock2 {
public:
    stock2(int a) {
        cout << "construct func:" << __FUNCTION__ << "(int a). a=" << a << endl;
    }
private:
    // string sym;
    double price;
};

// static int stock1::i = 0; //必须省略static，原来static变量在编译单元内的定义必须是全局的
int stock1::i = 0;

class base {
public:
    base(){
        cout << "base::base() construct" << endl;
    }
    virtual ~base(){
        cout << "base::~base() destruct" << endl;
    }
    virtual void func(){
        cout << "virtual base::func()" << endl;
    }

    virtual void func2() = 0; // 纯虚函数，派生类必须实现
    virtual void func2(int i) {
        cout << "virtual base::func2(int i)" << endl;
    }
    void func3() {
        cout << "base::func3()" << endl;
    }
    void func3(int i) {
        cout << "base::func3(int i)" << endl;
    }
};
class derived : public base {
public:
    derived(){
        cout << "derived::derived() construct" << endl;
        // func(); // virtual 调用的是derived的func，很奇怪，书上说这个时候虚函数表还没有初始化，但是这里可以调用，但是最好不要再构造函数中调用虚函数
                   // 或者把func设置为non-virtual的
    }
    virtual ~derived() {
        cout << "derived::~derived() destruct" << endl;
    }
    virtual void func() override { // override使得编译器检查是否有对应的基类函数，如果没有则报错
        cout << "virtual derived::func()" << endl;
        base::func(); // 该种方式可以调到基类的虚函数
    }
    void func1() {
        cout << "virtual derived::func1()" << endl;
        static_cast<base*>(this)->func(); // 该种方式不能调到基类func，仍然是派生类的func，关键在于this指针
        // 直接访问base基类的表中的副本
    }

    virtual void func2() override {
        cout << "virtual derived::func2()" << endl;
    }
    void func3() {
        cout << "derived::func3()" << endl;
    }

    using base::func2; // 使得基类的func2(int i)可以在派生类中使用, 导致派生类增加新的成员函数
    using base::func3; // 使得基类的func3(int i)可以在派生类中使用，导致派生类增加新的成员函数

};

class derived1 : private base {
    // private从base中继承来的成员都是private的，说白了外部对象不能直接强转base，也不能访问base成员
    // 用private继承，需要考虑哪些接口是真正需要用到，但又不属于该派生类的
public:
    derived1(){
        cout << "derived1::derived1() construct" << endl;
    }
    virtual ~derived1() {
        cout << "derived1::~derived1() destruct" << endl;
    }
    virtual void func() override {
        base::func(); // forwarding functions, 转交函数，该种方式可以调到基类的虚函数
        cout << "virtual derived1::func()" << endl;
    }
    virtual void func2() override {
        cout << "virtual derived1::func2()" << endl;
    }
};

void test5() {
    cout << __func__ << "--------------------" << endl;
    derived d;
    d.func();
    d.func1();
    base* b = static_cast<base*>(&d); // 即使用强转也是调用的derived的func
    b->func();


    // public继承
    cout << "public继承-------------" << endl;
    // 成员函数重载问题，函数名和参数必须完全一致
    d.func2(); // 调用的是derived的func2
    // (&d)->base::func2(); // invalid
    // d.func2(1); // invalid, 被func2()覆盖了，如果使用要配合类内 using base::func2;
    d.base::func2(1); // valid，该方法调用也可

    d.func3(); // 调用的是derived的func3
    // d.func3(1); // invalid，被func3()覆盖了， 如果使用要配合类内 using base::func3;
    d.base::func3(1); // valid，该方法调用也可

    // private继承
    cout << "private继承-------------" << endl;
    derived1 d1;
    d1.func(); // 调用的是derived1的func
    // d1.func2(3); // invalid, 被func2()覆盖了，如果使用要配合类内 using base::func2;
    // d1.fun3(); // invalid, 找不到
    // base* b1 = static_cast<base*>(&d1); // invalid 即使用强转也不行，因为private继承，base的成员函数在derived1中是private的

}



int main(int argc, char* argv[]) {
    cout << "test1================" << endl;
    stock1 s1;
    s1.show();
    stock1 s2("3000688", 4.12);
    s2.show();
    stock1 s3{"000001",3.14};
    s3.show();
    const stock1 s4 = {"000002", 3.15};
    s4.show(); // 如果没有const将会报错
    cout << "entrance_func()" << endl;
    cout << "test2================" << endl;
    stock2 s5(10);
    stock2 s6 = 4.5;
    stock2 s7 = stock2(11.5);
    cout << "test3================" << endl;
    stock1 stock_array[4] = {stock1("000003", 3.14), stock1("000004", 3.15)};

    cout << "test4================" << endl;
    static  int cnt = 5;
    int array_a[cnt]{0};
    cout << "sizeof(array_a)=" << sizeof(array_a) << endl;
    cnt = 6;
    int array_b[cnt]{0};
    cout << "sizeof(array_b)=" << sizeof(array_b) << endl;

    class A{}; // empty class，一个空类至少占用一个字节
    class B : public A{
        int i; // 4字节，A不占位
    };
    class C{
        A   a; // 8字节，empty class, 组合方式占位，可能有内存对齐问题，很注重空间时兼用用继承方式
        int i;
    };
    cout << " sizeof(A)=" << sizeof(A) << " sizeof(B)=" << sizeof(B) << " sizeof(C)=" << sizeof(C) << endl;

    test5();
    return 0;
}


