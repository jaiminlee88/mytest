#include <iostream>
#include <string.h>

using namespace std;


class base {
public:
    base(){
        cout << "base::base() construct" << endl;
        func();
    }
    virtual ~base(){
        cout << "base::~base() destruct" << endl;
    }
    virtual void func(){
        cout << "virtual base::func()" << endl;
    }
    void set_mem(int i, int j) {
        cout << "base::set_mem" << endl;
        base_a=i;
        base_b=j;
    }
    void print_mem() {
        cout << "base::print_mem base_a=" << base_a << " base_b=" << base_b << endl;
    }
private:
    int base_a{0};
    int base_b{0};
};

class derived : public base {
public:
    derived(){
        cout << "derived::derived() construct" << endl;
        // func(); // virtual 调用的是derived的func，很奇怪，书上说这个时候虚函数表还没有初始化，但是这里可以调用，但是最好不要再构造函数中调用虚函数
                   // 或者把func设置为non-virtual的
    }
    void init () {
        cout << "derived::init()" << endl;
        func(); // 这里调用安全一些
    }
    ~derived(){
        // 析构函数不应该抛出异常，noexcept，否则资源无法释放，导致内存泄漏
        // cout << "derived::~derived() throw error" << endl;
        // throw "aaaaaa";
        try {
            cout << "derived::~derived() destruct" << endl;
            func();
            throw "aaaaaa";
        }
        catch(...){
            cout << "derived::~derived() destruct catch error" << endl;
            // std::abort(); // 直接扼杀在摇篮， 并记录失败（吞噬错误不好），这样做不影响析构函数释放资源
        }
    }
    derived& operator=(const derived& i) { /// 最好这么写
        if (&i == this) {
            cout << "derived::operator= self" << endl;
            return *this;
        }
        cout << "derived::operator=" << endl;
        a = i.a;
        b = i.b;
        base::operator=(i); // 调用基类的赋值函数,这样基类成员才能被赋值，编译器不会试图修改基类成员
        return *this;
    }
    void func() override { // override使得编译器检查是否有对应的基类函数，如果没有则报错
        cout << "virtual derived::func()" << endl;
    }
    void set_mem(int i, int j, int k=0, int l=0) {
        cout << "derived::set_mem" << endl;
        base::set_mem(k,l);
        a=i;
        b=j;
    }
    void print_mem() {
        base::print_mem();
        cout << "derived::print_mem a=" << a << " b=" << b << endl;
    }
private:
    int a{0};
    int b{0};
};

int main (){
    // operator最佳实践
    cout << "derived d======================" << endl;
    derived d;
    d.init();

    d.set_mem(1,2, 3, 4);
    d = d; // 自我赋值
    d.print_mem();

    cout << "derived d1======================" << endl;
    derived d1;
    d1.print_mem();
    d1 = d; // 赋值。base类的成员函数并没有赋值
    d1.print_mem();

    // 不要再构造函数中调用虚函数，因为此时虚函数表还没有被初始化，调用虚函数会导致程序崩溃
    // 不要再析构函数中调用虚函数，因为析构函数是最后一个被调用的，此时虚函数表已经被销毁，调用虚函数会导致程序崩溃

}