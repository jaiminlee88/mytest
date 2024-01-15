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
    auto a=i, &b=i,*c=&i;//a是int，b是i的引用，c是i的指针，auto就相当于int
    // auto d=0, f=1.0;//error，0和1.0类型不同，对于编译器有二义性，没法推导
    // auto e;//error，使用auto必须马上初始化，否则无法推导类型
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
int main(int argc, char* argv[]) {
    cout << "entrance_func()" << endl;
    test1();
    test2();
    // test_template.cc test1
    test3();
    // 显式类型初始器惯用法是这样的：推荐这么用，auto就会严格按照类型推导
    // auto sum = static_cast<Matrix>(m1 + m2 + m3 + m4);

    double x,y;
    x = 1.0;
    y = 2.0;
    int sum{x+y}; // 居然允许隐式转换
    cout << "sum=" << sum << endl;
    int sum2 = x + y;
    cout << "sum2=" << sum2 << endl;
    int sum3(x+y);
    cout << "sum3=" << sum3 << endl;
    return 0;
}


