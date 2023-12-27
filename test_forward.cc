#include <iostream>
#include <utility>

// 接受右值参数的函数
void processRValue(int&& value) {
    std::cout << "Processing rvalue: " << value << std::endl;
}

// 接受左值参数的函数
void processLValue(const int& value) {
    std::cout << "Processing lvalue: " << value << std::endl;
}

// 模板函数，以完美转发方式将参数传递给其他函数
template<typename T>
void forwardArgument(T&& value) {
    // 使用std::forward将参数以完美转发方式传递给其他函数
    // 根据原始参数的值类别和引用修饰符，选择调用processLValue或processRValue函数
    if constexpr(std::is_lvalue_reference_v<T>) {
        processLValue(std::forward<T>(value));
    } else {
        processRValue(std::forward<T>(value));
    }
}

// std::forward是一个用于完美转发（perfect forwarding）的模板函数。
// 它用于将函数参数传递给另一个函数，保持原始的值类别（value category）和引用修饰符（cv-qualifiers）。

// std::forward通常在模板中使用，以保留参数的值类别，并将参数以完美转发的方式传递给其他函数。
// 它用于解决以下问题：当模板函数的参数是一个右值引用时，我们希望保持右值的属性；
// 当参数是一个左值引用时，我们希望保持左值的属性。

// 采用std::forward可以有效地保留原始参数的值类别和引用修饰符，以完美转发的方式将参数传递给其他函数。
// 这样，我们可以在模板函数中正确处理不同类型的参数，避免不必要的拷贝或移动操作，并且保持参数的原始属性

int main() {
    int x = 42;
    forwardArgument(x); // 传递左值
    forwardArgument(10); // 传递右值

    return 0;
}
