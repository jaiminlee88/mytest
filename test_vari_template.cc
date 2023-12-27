#include<iostream>
#include<string>
using namespace std;

// void show_list1(int... args) {} // invalid,只能用变长参数模板

template<typename T>
void show_list0(T value) {
    cout << "value: " << value << endl;
}

template<typename... Args> // Args: template parameter pack
void show_list1(Args... args){ // args: function parameter pack

}


void show_list3() {} // 递归终止条件,必须要有

template<typename T, typename... Args>
void show_list3(T value, Args... args) {
    cout << "value: " << value << endl;
    show_list3(args...); // 每递归一次缩小一个参数，效率低下
    cout << "done" << endl;
}


// 引用的方式，省去大量复制
void show_list4(){}
template<typename T>
void show_list4(const T& value) {
    cout << value << endl;
}
template<typename T, typename... Args>
void show_list4(const T& value, const Args&... args) {
    cout << value << ", ";
    show_list4(args...);
}

int main() {
    cout << "1===========" << endl;
    int n=14;
    double x=2.71828;
    string mr = "Mr. String objects!";
    show_list3(n,x);
    cout << "2===========" << endl;
    show_list3(x*x, '!', 7, mr);

    cout << "3===========" << endl;
    show_list4(x*x, '!', 7, mr);
}