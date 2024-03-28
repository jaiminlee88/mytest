#include <iostream>
#include <unordered_map>
#include <memory>
#include <any>
using namespace std;


int main() {
#if __cplusplus >= 201703L
    cout << "using C++17" << endl;

    /*
    std::any 是 C++17 中引入的一个模板类，
    位于 <any> 头文件中，用于在运行时存储任意类型的值。
    它类似于 C++11 中的 std::variant，但是 std::any 不限定于特定的类型。
    std::any 提供了一种通用的方式来处理不同类型的值，但在使用时需要注意类型的正确性，
    否则会抛出 std::bad_any_cast 异常。

    std::any 模板通常用于需要在运行时存储不同类型的值，
    但又不想使用模板或者不确定要存储的值的具体类型的情况。
    */
    std::any a = 42; // 存储整数
    std::cout << std::any_cast<int>(a) << std::endl; // 读取整数
    // std::cout << a << std::endl; // 不能这么用
    // std::cout << std::any_cast<string>(a) << std::endl; // 'std::bad_any_cast

    a = 3.14; // 存储浮点数
    std::cout << std::any_cast<double>(a) << std::endl; // 读取浮点数

    a = "Hello"; // 存储字符串
    std::cout << std::any_cast<const char*>(a) << std::endl; // 读取字符串
#endif
    return 0;
}