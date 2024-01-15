#include<iostream>

using namespace std;

// void test1()=delete; // delete后不能定义
void test1() {
    cout << "test1" << endl;
}

class A {
public:
    // A() = delete; // 显示声明使用编译器生成的默认构造函数,如果delete，则不能用A::A()来调用
    A(int a) : a_(a) {}
private:
    int a_;
};

class B : public A {
public:
    B() : A::A(2) {} // 显示声明使用编译器生成的默认构造函数
    // B() : A::A(){} // 不能用A::A()，因为A的构造函数被delete了
    // A::A; // 继承A的构造函数
};

template <typename T>
void processPointer(T* ptr) {
    cout << "processPointer(T* ptr)" << endl;
}

template <>
void processPointer<void>(void*) = delete;

template <>
void processPointer<char>(char*) = delete;


// void processPointer (int* a) {
//     cout << "processPointer(int*)" << endl;
// }

// void processPointer (char* b) {
//     cout << "processPointer(char*)" << endl;
// }

void test2() {
    B b;
    char c = 'a';
    processPointer(&c);
}
int main(){
    test1();
    test2();
}
