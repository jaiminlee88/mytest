#include <iostream>
using namespace std;

struct chaff {
    char dross[20];
    int slag;
};

char buffer1[50];
char buffer2[500];

void test_new_failed() {
    const int a = 1000000000;
    long long *p = nullptr;
    while(true) {
        p = nullptr;
        // p = new long long[a]{0}; // throw std::bad_alloc
        p = new(std::nothrow)long long[a]{0}; // return nullptr if failed

        if (p == nullptr) {
            cout << "new failed." << endl;
            break;
        }
    }
}
int main() {
	// test_new_failed();
	chaff *p1, *p2;
    int *p3, *p4;
    p1 = new chaff;
    p3 = new int[20];

    p2 = new(buffer1) chaff; // 在buffer1中分配空间，从首地址buffer1开始
    p4 = new(buffer2) int[20]{2}; // 在buffer2中分配空间，从首地址buffer2开始

    for(int i = 0; i < 500; i++) {
        cout << buffer2[i] << ",";
    }
    cout << endl;
    int *p5 = (int*)(&buffer2);
    for(int i = 0; i < 500; i++) {
        cout << p5[i] << ",";
    }
    cout << endl;
    for(int i = 0; i < 500; i++) {
        cout << p4[i] << ",";
    }
    cout << endl;
    cout << "&buffer2=" << (void*)buffer2 << " &p4=" << &p4 << endl;

    // delete p2; // invalid， 因为new此时不是常规运算符，是定位new运算符，除非构造一个解析函数 ~chaff()，并直接调用p2->~chaff()，否则不会调用析构函数
    return 0;
}