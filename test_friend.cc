#include <iostream>
#include <string.h>

using namespace std;

class abc;



class abc{
public:
    abc(string _name, int a=0):name(_name), m1(a){}
    friend void set_abc_m1(abc &a, int i);
    void print() { cout << "name=" << name << " m1=" << m1 << endl; }
    abc& operator +(abc& a) {
        m1 += a.m1;
        return *this;
    }
    abc& operator -(abc& a) { // 两个操作数，一个this，一个abc引入， 即 tmp.operator-(tmp1)
        m1 += a.m1;
        return *this;
    }
    // friend abc& operator -(abc& a, abc& b) { // 两个操作数，两个abc引入， 即 operator-(tmp,tmp1)
    //     a.m1 += b.m1;
    //     return a;
    // }

    friend ostream& operator <<(ostream& os, abc& a); // 重载操作符需要的操作数 等于 形参数书目，对于非成员函数，没有this指针，只有以os和abc引用方式引入，而成员参数可以通过this传入操作数
    // friend ostream& operator <<(ostream& os, abc& a) { // valid，实现也可以放到类定义里，但必须是friend
    //     os << " name=" << a.name << " m1=" << a.m1;
    //     return os;
    // }
    // 输出流的重载运算符<<不能定义成成员函数的原因是，成员函数的形式只能拥有一个参数，而输出流的重载运算符需要接收两个参数：一个是输出流对象，另一个是要输出的对象（即类的实例）。
    // 这两个参数都需要作为函数的参数，在成员函数的形式下无法实现，因为成员函数会自动添加一个隐式的this指针，只能作为左操作数来调用成员函数，右操作数只能是隐式的this指针。
    // 而对于输出流的重载运算符<<，我们期望能够以类实例作为左操作数，输出流对象作为右操作数来调用该运算符，例如：cout << obj。这样，如果将输出流的重载运算符<<定义为成员函数，就会和这种调用方式不匹配。
    // 为了可以正确地在全局上下文中使用重载的<<运算符，我们需要将其定义为非成员函数，并使用友元函数的方式来声明和定义该运算符的重载。这样，就可以同时接收两个参数，即输出流对象和要输出的对象，从而实现输出的功能。
    // ostream& operator <<(ostream& os, abc& a) { //invalid，输出流的重载运算符需要接收两个参数：一个是输出流对象，另一个是要输出的对象（即类的实例）。
    //     os << " name=" << a.name << " m1=" << a.m1;
    //     return os;
    // }
private:
    string name{""};
    int m1{0};
};

void set_abc_m1(abc &a, int i) {
    a.m1 = i;
}

ostream& operator <<(ostream& os, abc& a) {
    os << " name=" << a.name << " m1=" << a.m1;
    return os;
}

class func1 {
public:
    func1() {cout << "construct func1" << endl;}
    func1(double i) : m1(i) {
        cout << "construct func1_double" << endl;
    }
private:
    double m1;
};

int main() {
    abc a("a",0);
    a.print();
    set_abc_m1(a,3);
    a.print();
    abc b("b",0);
    set_abc_m1(b,4);
    b.print();
    (a + b).print();
    (a.operator+(b)).print();
    a.print();
    b.print();
    cout << a << " | " << b << endl;

    func1 c; // 调用默认构造函数
    c = 2.8; // 用func1_double初始化一个func1，再复制到c中
    return 0;
}