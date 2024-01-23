#include<iostream>

using namespace std;

class Example{
public:
    int feet;
    int inches;
    char c;
public:
    Example(){}
    Example(int ft, int inc) : feet(ft), inches(inc) {}
    ~Example(){}
    void show_in() const { cout << "inches=" << inches << endl; }
    void show_ft() const { cout << "feet=" << feet << endl; }
    void show_ptr() const { }
};

int main(){
    int a = 0b0000000000001111; // 四个字节 15
    cout << "before a=" << a << endl;
    cout << "after a>>1 =" << (a >> 1) << endl; // 无符号右移，左边补0，右边丢弃 7
    cout << "after a<<1 =" << (a << 1) << endl; // 左移，右边补0，左边丢弃 15*2=30
    a>>=1;
    cout << "after a>>=1 =" << a << endl; // 无符号右移，左边补0，右边丢弃 7

    signed int a1 = 0b0000000000001111; // 四个字节 15
    cout << "before a1=" << a1 << endl;
    cout << "after a1>>1 =" << (a1 >> 1) << endl; // 有符号右移，左边补符号位，右边丢弃 7
    cout << "after a1<<1 =" << (a1 << 1) << endl; // 左移，右边补0，左边丢弃 15*2=30
    a1>>=1;
    cout << "after a1>>=1 =" << a1 << endl; // 有符号右移，左边补符号位，右边丢弃 7

    unsigned char a2 = 0b00001111; // 一个字节 15
    cout << "sizeof(char)=" << sizeof(char) << endl;
    cout << "before a2=" << (int)a2 << endl;
    cout << "after !a2>>=" << (int)(!a2); // 非
    cout << "after ~a2>>=" << (int)(~a2) << "  0b11110000=" <<(int)(0b11110000) << endl; // 位非

    // 特殊指针
    int Example::* pt = &Example::feet; // 指向类成员的指针,不指向特定内存单元，指向任意example对象中的位置，相当于给成员变量定义了一个特殊指针(相当于成员名)
    Example ob1(1,2);
    Example ob2(3,4);
    Example *pq = new Example(5,6);

    cout << "*pt set to feet:" << endl;
    cout << ob1.*pt << endl; // * 和 ->* 都是成员解除运算符
    cout << ob2.*pt << endl;
    cout << pq->*pt << endl;

    pt = &Example::inches;
    cout << "*pt set to inches:" << endl;
    cout << ob1.*pt << endl; // * 和 ->* 都是成员解除运算符
    cout << ob2.*pt << endl;
    cout << pq->*pt << endl;

    // pt = &Example::c; // invalid char类型与feet和inches不同，不会被自动转换为int，所以这里会报错

    void (Example::*pf)() const;
    pf = &Example::show_in;
    (ob1.*pf)();
    (ob2.*pf)();
    (pq->*pf)();
    pf = &Example::show_ft;
    (ob1.*pf)();
    (ob2.*pf)();
    (pq->*pf)();

    cout << "alignof=================" << endl;
}
