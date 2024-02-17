#include<iostream>
#include<vector>
using namespace std;
// #pragma comment(linker,"/entry:entrance_func")

void test1() {
    cout << __FUNCTION__ << " start." << endl;
    auto a=10;//10是int型，可以自动推导出a是int
    int i=10;
    auto b=i;//b是int型
    auto d=2.0;//d是double型
    // auto x; // auto必须马上初始化，否则无法推导类型
}

void test2() {
    cout << __FUNCTION__ << " start." << endl;
    int i=10;
    auto a=i;
    auto &b=i,
    auto *c=&i;
    auto d = b;
    // a是int，b是i的引用，c是i的指针，auto就相当于int
    // auto d=0, f=1.0;//error，0和1.0类型不同，对于编译器有二义性，没法推导
    // auto e;//error，使用auto必须马上初始化，否则无法推导类型
    // d是int，不是引用
}

std::vector<bool> vec_bool_global{true,false,false};
std::vector<bool> features(const std::vector<bool>& w=vec_bool_global) {
    return w;
}
void test3() {
    std::vector<bool> vec_bool{true,false,false};
    cout << "vec_bool[1]=" << vec_bool[1] << endl;
    vec_bool[1] = true;
    cout << "vec_bool[1]=" << vec_bool[1] << endl;
    for (auto i : vec_bool) {
        cout << i << " ";
    }
    cout << endl;
    auto h1 = features()[1];
    cout << "h1=" << h1 << endl;
    bool h2 = features()[1];
    cout << "h2=" << h2 << endl;
}

void test4() {
    cout << __FUNCTION__ << " start.=====================" << endl;
    int x = 5;
    auto y = x;
    if (std::is_lvalue_reference<decltype(x)>::value) {
        cout << "x is lvalue" << endl;
    } else if (std::is_rvalue_reference<decltype(x)>::value) {
        cout << "x is rvalue" << endl;
    } else {
        cout << "x is neither lvalue nor rvalue" << endl;
    }
}

void test5(){
    cout << __FUNCTION__ << " start.=====================" << endl;
    int x = 5;
    int& y = x;
    auto z = y;
    if (std::is_lvalue_reference<decltype(z)>::value) {
        cout << "z is lvalue" << endl;
    } else if (std::is_rvalue_reference<decltype(z)>::value) {
        cout << "z is rvalue" << endl;
    } else {
        cout << "z is neither lvalue nor rvalue" << endl; // 这个，auto会把引用去掉
    }
}
int main(int argc, char* argv[]) {
    /*
    auto的推导规则如下：

    自动推导初始化表达式的类型：当使用auto声明变量并初始化时，编译器会自动推导变量的类型，根据变量初始化表达式的类型来确定变量的类型。
    示例：
    auto x = 10; // x的类型将自动推导为int
    auto y = 3.14; // y的类型将自动推导为double
    auto z = "Hello"; // z的类型将自动推导为const char*

    排除引用和const限定符：在类型推导时，auto会自动排除引用和const限定符。如果初始化表达式是引用类型或带有const限定符，auto会将其去除，推导为对应的非引用类型。
    示例：
    const int a = 5;
    auto b = a; // b的类型将自动推导为int，去除了const限定符
    auto &c = a;
    auto d = c; // d的类型将自动推导为int，去除了引用

    推导为原始类型或推导为表达式的类型：当初始化表达式是原始类型（如整数、浮点数、字符等）时，auto会推导为相应的原始类型。当初始化表达式是一个复杂的表达式（如函数调用、算术运算等）时，auto会推导为表达式的类型。
    示例：
    auto c = 1 + 2; // c的类型将自动推导为int，因为1 + 2的结果是int型
    auto d = get_value(); // 如果get_value()返回一个int类型的值，d的类型将自动推导为int

    需要注意的是，类型推导是在编译时完成的，因此auto声明的变量在编译阶段会被推导为具体的类型，而不是在运行时进行动态类型推导。另外，auto不能用于函数的参数列表、函数返回类型和非静态成员变量的类型推导。
    */
    cout << "entrance_func()" << endl;
    test1();
    test2();
    // test_template.cc test1
    test3();
    // 显式类型初始器惯用法是这样的：推荐这么用，auto就会严格按照类型推导
    // auto sum = static_cast<Matrix>(m1 + m2 + m3 + m4);
    test4();
    test5();
    double x,y;
    x = 1.0;
    y = 2.0;
    int sum{x+y}; // 居然允许隐式转换
    cout << "sum=" << sum << endl;
    int sum2 = x + y;
    cout << "sum2=" << sum2 << endl;
    int sum3(x+y);
    cout << "sum3=" << sum3 << endl;

    const int x1 = 5;
    // x = 6; // invalid
    auto y1 = x1;
    cout << "before y1=" << y1 << endl;
    y1 = 7; // 已经把const int x1的const属性去掉了
    cout << "after  y1=" << y1 << endl;
    return 0;
}


