#include<iostream>
using namespace std;

void f(void* ptr) {
    cout << "f(void* ptr)" << endl;
}
void f(int i) {
    cout << "f(int i)" << endl;
}
void f(bool b) {
    cout << "f(bool b)" << endl;
}

int main(){
    int guest = 0;
    cout << "before " << guest << endl;
    while(guest++<10) {
        cout << "after " << guest << endl;
    }

    f(0);
    f(nullptr);
    // f(NULL);
    cout << "sizeof(nullptr)=" << sizeof(nullptr) << " sizeof(int)=" << sizeof(int) << " sizeof(void*)=" << sizeof(void*)<< endl;
    // cout << *(std::nullptr_t) << endl;
}