#include<iostream>
using namespace std;
// #pragma comment(linker,"/entry:entrance_func")

void test1() {
    cout << __FUNCTION__ << " start." << endl;
    auto a=10;//10是int型，可以自动推导出a是int
    int i=10;
    auto b=i;//b是int型
    auto d=2.0;//d是double型
}

void test2() {
    cout << __FUNCTION__ << " start." << endl;
    int i=10;
    auto a=i, &b=i,*c=&i;//a是int，b是i的引用，c是i的指针，auto就相当于int
    // auto d=0, f=1.0;//error，0和1.0类型不同，对于编译器有二义性，没法推导
    // auto e;//error，使用auto必须马上初始化，否则无法推导类型
}

int main(int argc, char* argv[]) {
    cout << "entrance_func()" << endl;
    test1();
    test2();
    return 0;
}


