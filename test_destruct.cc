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
    void func() override { // override使得编译器检查是否有对应的基类函数，如果没有则报错
        cout << "virtual derived::func()" << endl;
    }
};

int main (){
    // 析构函数不应该抛出异常，noexcept，否则资源无法释放，导致内存泄漏
    derived d;
    d.init();

    // 不要再构造函数中调用虚函数，因为此时虚函数表还没有被初始化，调用虚函数会导致程序崩溃
    // 不要再析构函数中调用虚函数，因为析构函数是最后一个被调用的，此时虚函数表已经被销毁，调用虚函数会导致程序崩溃

}