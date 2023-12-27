#include<iostream>
using namespace std;


int main() {
    int a = 1;
    int b = 2;
    cout << "before scope: a=" << a << " b:" << b << endl;
    {
        int a = 2; // 只在该局部有效
        b = 3;
        cout << "sub scope: a=" << a << " b:" << b << endl;
    }
    cout << "after scope: a=" << a << " b:" << b << endl;
}