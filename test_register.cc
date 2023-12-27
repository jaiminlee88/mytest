#include<iostream>
using namespace std;


int main() {
    int a = 1;
    register int b = 2; // 单纯为了加快访问速度，建议用寄存器存储
    cout << "a=" << a << " &a=" << &a << " b:" << b << " &b=" << &b << endl;
}