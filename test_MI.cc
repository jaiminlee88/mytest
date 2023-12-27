// multiple inheritage
#include<iostream>
using namespace std;

class A {
private:
    int member;
public:
    A(){} // 必须得写
    A(int i) : member(i) {
        cout << "this is A." << endl;
    }
    void show () {
        cout << "A.member=" << member << endl;
    }
};

class B : public A {
private:
    int member;
public:
    B(int i) : member(i) {
        cout << "this is B." << endl;
    }
    B(int i, int j) : member(i), A(j) {
        cout << "this is B." << endl;
    }
    void show () {
        cout << "B.member=" << member << endl;
    }
};

class C : public B {
private:
    int member;
public:
    // C(int i, int j, int k) : B(j), A(k), member(i) { // doesnt work, C不能跨越b直接调用A的构造函数
    //     cout << "this is C." << endl;
    // }
    C(int i, int j, int k) : B(j, k), member(i) { // doesnt work, C不能跨越b直接调用A的构造函数
        cout << "this is C." << endl;
    }
    ~C(){
        cout << "C release" << endl;
    }
    void show () {
        A::show();
        B::show();
        cout << "C.member=" << member << endl;
    }
};

class Worker {// an abstract base class
private:
    std::string fullname;
    long id;
public:
    Worker() : fullname("no one"),id(0L){}
    Worker(const std::string & s, long n): fullname(s),id(n){}
    Worker(const Worker& wk) {}
    // virtual ~Worker() = 0; // pure virtual destructor
    virtual void set() {   }
    virtual void show(){}
};

class Waiter : virtual public Worker{
private:
    int panache;
public:
    Waiter() : Worker(), panache(0){}
    Waiter(const std::string & s, long n, int p = 0): Worker(s,n), panache(p){}
    Waiter(const Worker & wk, int p = 0) : Worker(wk), panache(p){}
    void Set() {}
    void Show() const {}
};

class Singer : virtual public Worker {
protected:
    enum {other, alto, contralto, soprano, bass, baritone, tenor};
    enum {Vtypes = 7};
private:
    // string equivs of voice types
    static char *pv[Vtypes];
    int voice;
public:
    Singer() : Worker(), voice(other){}
    Singer(const std::string & s,long n, int v = other) : Worker(s,n), voice(v){}
    Singer(const Worker & wk, int v = other): Worker(wk), voice(v){}
    void Set(){};
    void Show() const{};
};

class singerwaiter : public Singer, public Waiter {
public:
    singerwaiter(){}
    singerwaiter(const std::string & s, long n,int p =0, int v = Singer::other) : Worker(s,n), Waiter(s,n,p), Singer(s,n,v) {}
    // singerwaiter(const Worker & wk,int p = 0,int v = Singer::other): Worker(wk), Waiter(wk,p), Singer(wk,v) {}
    // singerwaiter(const Waiter & wt,int v = Singer::other): Worker(wt), Waiter(wt), Singer(wt,v) {}
    // singerwaiter(const Singer & wt, int p = 0): Worker(wt), Waiter(wt,p), Singer(wt){}
};

class d {
public:
    d(std::string name){ cout << "i am d" << endl;}
};

class e: virtual public d {
public:
    e(std::string name): d(name) { cout << "i am e" << endl;}
};

class f: virtual public d {
public:
    f(std::string name):d(name){ cout << "i am f" << endl;}
};

class g : public e, public f {
public:
    g(std::string name): d(name), e(name), f(name){ cout << "i am g" << endl;}
};

int main() {
    C tmp(1,2,3);
    tmp.show();

    std::string name = "alyo";
    singerwaiter person(name, 123);

    g tmp1(name);
    return 1;
}