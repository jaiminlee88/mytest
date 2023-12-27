#include<iostream>
#include<cstring>
#include<vector>
using namespace std;

template<typename Type>
class Stack{
private:
    enum{MAX = 10};
    Type items[MAX];
    int top;
public:
    Stack();
    bool isempty();
    bool isfull() {
        return top == MAX;
    }
    bool push(const Type &item);
    bool pop(Type &item);
};

template<typename Type>
Stack<Type>::Stack(){
    top = 0;
}

template<typename Type>
bool Stack<Type>::isempty() {
    return top == 0;
}

// template<typename Type>
// bool Stack<Type>::isfull() {
//     return top == MAX;
// }

template<typename Type>
bool Stack<Type>::push(const Type &item) {
    if (top < MAX) {
        items[top++] = item;
        return true;
    }
    return false;
}

template<typename Type>
bool Stack<Type>::pop(Type &item) {
    if (top > 0) {
        item = items[--top];
        return true;
    }
    return false;
}

// 使用关键字typename代替关键字class指定模板类型形参也许更为直观，毕竟可
// 以使用内置类型（非类类型）作为实际的类型形参，而且，typename更清楚地指
// 明后面的名字是一个类型名。但是，关键字typename是作为标准C++的组成部分
// 加入到C++中的，因此旧的程序更有可能只用关键字class。
// 关于依赖名，不管是直接还是间接，只要依赖于模板参数，该名称就是依赖名
// https://blog.csdn.net/XMJYever/article/details/110313452
struct MyClass {
    static int iterator; // 静态数据成员 MyClass::iterator，
    // 如果std::vector<T>的T是MyClass，那么T::iterator到底是迭代器还是静态成员变量int
    static int A; // 静态数据成员 Myclass::A
    static int B(); // 静态成员函数 Myclass::B()
    typedef int C; // 嵌套 Myclass::C
};
template<typename T>
std::vector<T>& arrayRotate(const std::vector<T>& src) {
	std::vector<T> dst(src.size()); // vector<T> 本身是依赖名，所以不需要加typename，dependant name
    // std::vector<T>::iterator iter; // invalid, because compiler doesn't know that std::vector<T>::iterator is a type
    // 在模板中，编译器没法知道std::vector<T>::iterator是一个类型，所以需要加上typename
    // class std::vector<T>::iterator iter; // valid
    // typename只能用于模板内
    typename T::iterator * _iter; // valid，依赖名，表明T::iterator是一个类型，而非变量
    // 如果不加typename之前 T::iterator * iter可能会被当成乘法表达式 (T::iterator)*iter

    vector<int>::iterator iter_int; // valid，非依赖名
    typename std::vector<T>::iterator iter; // valid，依赖名，typname将该名称当成是类型，而非变量，消除歧义
	for (iter = src.begin(); src.end() != iter; iter++) {
		dst.push_back(*iter);
	}
	return dst;
}

int main() {
    Stack<int> kernels;
    Stack<std::string> colonels;
    int num[] = {1,2,3,4,5,6};
    cout << "push ";
    for (auto &it:num) {
        cout << it << " ";
        kernels.push(it);
    }
    cout << "to stack kernels" << endl;
    cout << "pop ";
    while (!kernels.isempty()) {
        int temp = 0;
        kernels.pop(temp);
        cout << temp << " ";
    }
    cout << "from stack kernels;" << endl;

    std::string str[] = {"you", "are", "super", "star"};
    cout << "push [";
    for (auto &it:str) {
        cout << it << " ";
        colonels.push(it);
    }
    cout << "]to stack colonels" << endl;
    cout << "pop [";
    while (!colonels.isempty()) {
        std::string temp{""};
        colonels.pop(temp);
        cout << temp << " ";
    }
    cout << "] from stack colonels;" << endl;

    cout << "done" << endl;
}