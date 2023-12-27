#include<iostream>
using namespace std;

bool (*func)(int,int) = nullptr;
using func1=bool(*)(int,int);
typedef bool(*func2)(int, int);

bool gt(int a, int b) {
    return a >= b? true:false;
}

int main() {
    int a = 6;
    int b = 5;
    cin >> a >> b;
    func = gt;
    cout << *func << " " << (void*)(&gt) << " func(a,b)=" << func(a,b) << endl;

    func1 f1 = gt;
    cout << *f1 << " " << (void*)(&gt) << " func(a,b)=" << f1(a,b) << endl;

    func2 f2 = gt;
    cout << *f2 << " " << (void*)(&gt) << " func(a,b)=" << f2(a,b) << endl;
}

// #include <iostream>
// #include <string>
// using namespace std;

// int myTest(int a, string b)
// {
// 	cout << "a = " << a << " b = " << b << endl;
// 	return 0;
// }

// typedef int(*func)(int, string);//定义了一个函数指针类型。

// using func1 = int(*)(int, string);


// int main()
// {
// 	func f = myTest;
// 	func1 f1 = myTest;
// 	f(10, "hello");
// 	f1(10, "tom");
// 	(*f)(99, "jack");
// 	return 0;
// }
