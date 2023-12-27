#include <iostream>

class MyClass {
public:
    void foo() const {
        ++count;  // 错误：const成员函数不能修改非mutable成员变量
    }

    void bar() const {
        ++mutableCount;  // 正确：const成员函数可以修改mutable成员变量
    }

private:
    int count;
    mutable int mutableCount; // 可以在const函数中修改的变量
};

int main(){
// 在C++中，关键字mutable用于修饰类的成员变量。
// 它的作用是允许在const成员函数中修改被mutable修饰的成员变量。
// 通常情况下，成员函数被声明为const表示它不会修改对象的状态。编译器会对const成员函数进行一些优化，例如不允许在其中修改非mutable成员变量，以确保对象的状态不会被修改。
// 然而，有时候我们可能希望在const成员函数中修改某个成员变量，例如记录一个计算的缓存结果，或者进行某些内部状态的更新。这种情况下，我们可以使用mutable关键字来修饰被修改的成员变量，
// 使它可以在const成员函数中被修改。
}