#include <iostream>
using namespace std;

struct data {
    int a = 1;
    mutable int b = 2; // can be changed if data is const
};
int main() {
    volatile int a = 4; // 多次使用a时，强制写回内存，不要放在寄存器里
    int b = 3;
    b += a;
    cout << a << " " << b << endl;

    const data mdata;
    cout << "before mdata.b=" << mdata.b << endl;
    mdata.b++;
    // mdata.a++;// not allowed
    cout << "after mdata.b=" << mdata.b << endl;
    return 0;
}
