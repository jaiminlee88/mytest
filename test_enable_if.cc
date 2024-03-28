#include <iostream>
#include <type_traits>


// 普通版本，用了enable_if来限制T的类型
template <typename T>
typename std::enable_if<std::is_pointer<T>::value>::type
printSize(T t) {
    std::cout << "Size of T*: " << sizeof(T) << std::endl;
}
// typename 表示 std::enable_if 的返回类型是一个类型而不是值。
// 因此，typename 的作用是告诉编译器 std::enable_if 的模板参数是一个类型而不是其他内容。


// 特殊版本，当 T 不是指针类型时，启用该函数
template <typename T>
typename std::enable_if<!std::is_pointer<T>::value>::type
printSize(T t) {
    std::cout << "Size of T: " << sizeof(T) << std::endl;
}

// 在这里，“typename =”是用于定义模板参数的默认值的语法
// 在上面模板中，如果is_integral_v<T>为true，则模板参数T有效,enable_if_t为void，模板生效
// 如果is_integral_v<T>为false，则模板参数T无效，模板参数不会被定义
template <typename T,typename = std::enable_if_t<std::is_integral_v<T>>>
void printInteger(T value) {
    std::cout << "Value: " << value << std::endl;
}

int main() {
    int i = 5;
    int* p = &i;

    printSize(i); // 调用普通版本
    printSize(p); // 调用特殊版本
    printInteger(42);  // OK，T 是整数类型
    // printInteger("hello");  // 编译错误，T 不是整数类型
    return 0;
}
