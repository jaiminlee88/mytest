#include<iostream>
using namespace std;

class A{
public:
    A(){
        cout << "A::A()" << endl;
    }
    virtual ~A(){
        cout << "A::~A()" << endl;
    }

    void over_load() {
        cout << "A::over_load()" << endl;
    }

    void over_load_param(int a) {
        cout << "A::over_load_param(int a) a=" << a << endl;
    }

    void over_ride_novirtural() {
        cout << "A::over_ride_novirtural()" << endl;
    }

    virtual void over_ride_virtual() {
        cout << "A::over_ride_virtual()" << endl;
    }

    void over_ride() {
        cout << "A::over_ride()" << endl;
    }

    void over_write_member() {
        cout << "A::over_write_member() member=" << member << endl;
    }

    void over_write_func() {
        cout << "A::over_write_func()" << endl;
    }

    void over_write_func(double a) {
        cout << "A::over_write_func(double a) a=" << a << endl;
    }

    void over_write_func(int a) {
        cout << "A::over_write_func(int a) a=" << a << endl;
    }

    int member{0};
    static const int mem=1;
    static int mem=1;

};

class B : public A{
public:
    B(){
        cout << "B::B()" << endl;
    }
    virtual ~B(){
        cout << "B::~B()" << endl;
    };

    void over_load() {
        cout << "B::over_load(int a)" << endl;
    }

    void over_load_param(int a) {
        cout << "B::over_load_param(int a) a=" << a << endl;
    }

    void over_ride_novirtural() {
        cout << "B::over_ride_novirtural()" << endl;
    }
    virtual void over_ride_virtual() {
        cout << "B::over_ride_virtual()" << endl;
    }

    void over_write_member() {
        cout << "B::over_write_member() member=" << member << endl;
    }

    void over_write_func() {
        cout << "B::over_write_func()" << endl;
    }

    void over_write_func(double a) {
        cout << "B::over_write_func(double a) a=" << a << endl;
    }

    void over_write_func(int a) {
        cout << "B::over_write_func(int a) a=" << a << endl;
    }

    int member{1};
};


int main(int argc, char* argv[]) {
    // 注意，如果不virtual，表明该类不会作为基类，内部成员如果是32位的可以直接塞进64位寄存器，移植性很好
    // 如果是virtual，表明该类会作为基类，会有vptr指针指向vtbl，结构变化，不再具有移植性
    // virtual有很多替代方案，可以查看 test_design_declaration.cc 的test9和test10
    A* pa = new B();
    B* pb = dynamic_cast<B*>(pa);
    cout << endl;
    pa->over_load();
    pb->over_load();
    pa->over_load_param(1);
    pb->over_load_param(1);
    cout << endl;
    pa->over_ride_novirtural();
    pb->over_ride_novirtural();
    pa->over_ride_virtual();
    pb->over_ride_virtual();
    cout << endl;
    pa->over_write_member();
    pa->member = 2;
    pa->over_write_member();
    pb->over_write_member();
    pb->member = 3;
    pb->over_write_member();
    cout << endl;
    pa->over_write_func();
    pb->over_write_func();
    pa->over_write_func(1);
    pb->over_write_func(2);
    pa->over_write_func(0.5);
    pb->over_write_func(0.8);

    delete pa;
    return 0;
}