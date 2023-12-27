#include<iostream>
using namespace std;

void print( int a [], int n) {
    cout << __FUNCTION__ << "| a=" << a << " sizeof(a)=" << sizeof(a) << " n=" << n << endl;
    int i = 0;
    while(i < n) {
        cout << a[i] << ", ";
        ++i;
    }
    cout << endl;
}

void print1(int *a, int n) {
    cout << __FUNCTION__ << "| a=" << a << " sizeof(a)=" << sizeof(a) << " n=" << n << endl;
    int i = 0;
    while(i < n) {
        cout << a[i] << ", ";
        ++i;
    }
    cout << endl;
}

int main () {
    int temp[] = {1,2,3,4,5,6};
    print(temp, sizeof(temp) / sizeof(int));
    print1(temp, sizeof(temp) / sizeof(int));
}