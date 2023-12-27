#include<iostream>
#include<cstring>
using namespace std;

struct job{
    char name[40];
    double salary;
    int floor;
};

template<typename T>
void Swap(T &a, T &b);

// template<> 为显式具体化开头，必须，且优先于常规模板
// template<> void Swap(job &j1, job &j2)
template<> void Swap<job>(job &j1, job &j2); // explicit specialization，优先使用这个  是void Swap(T &a, T &b)的具体化
template void Swap<job>(job &j1, job &j2); // instantiation,会直接生成定义，

// template void Swap<int>(int &j1, int &j2); // instantiation


template<typename T>
void Swap(T *a, T *b, int n);

template<typename T>
void show(T *a, int n) {
    for (int i = 0; i < n; i++) {
        cout << a[i] << ",";
    }
}

int main() {
    int a = 1;
    int b = 2;
    cout << "before: a=" << a << " b=" << b << endl;
    Swap(a,b);
    cout << "after : a=" << a << " b=" << b << endl;

    constexpr int len = 5;
    int a1[len] = {1,2,3,4,5};
    int b1[len] = {6,7,8,8,10};
    cout << "before: \na1=";
    show(a1, len);
    cout << " b1=";
    show(b1, len);
    cout << endl;
    Swap(a1, b1, len); /////
    cout << "after: \na1=";
    show(a1, len);
    cout << " b1=";
    show(b1, len);
    cout << endl;

    job j1;
    string name = "abc"; ///
    strcpy(j1.name, name.c_str());
    j1.salary = 1000.0;
    j1.floor = 500;
    job j2;
    name = "def"; ///
    strcpy(j2.name, name.c_str());
    j2.salary = 5000.0;
    j2.floor = 800;
    cout << "before j1:" << j1.name << " " << j1.salary << " " << j1.floor << endl;
    cout << "before j2:" << j2.name << " " << j2.salary << " " << j2.floor << endl;
    Swap(j1, j2); //////
    cout << "after j1:" << j1.name << " " << j1.salary << " " << j1.floor << endl;
    cout << "after j2:" << j2.name << " " << j2.salary << " " << j2.floor << endl;

    cout << "before j1:" << j1.name << " " << j1.salary << " " << j1.floor << endl;
    cout << "before j2:" << j2.name << " " << j2.salary << " " << j2.floor << endl;
    Swap(j1, j2); //////
    cout << "after j1:" << j1.name << " " << j1.salary << " " << j1.floor << endl;
    cout << "after j2:" << j2.name << " " << j2.salary << " " << j2.floor << endl;
}


// template<class Type>
template<typename Type>
void Swap(Type &a, Type &b) {
    Type temp = a;
    a = b;
    b = temp;
}

template<typename T>
void Swap(T *a, T *b, int n) {
    T temp;
    for (int i = 0; i < n ; i++) {
        temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
}

template <> void Swap<job>(job &j1, job &j2) {
    double salary;
    int floor;
    salary = j1.salary;
    j1.salary = j2.salary;
    j2.salary = salary;

    floor = j1.floor;
    j1.floor = j2.floor;
    j2.floor = floor;
}