#include <iostream>
#include <utility>
#include<typeinfo>
#include <vector>
#include <memory>
using namespace std;
// 接受右值参数的函数
void processValue(int&& value) {
    std::cout << "Processing int&& value: " << value << std::endl;
}
// 接受左值参数的函数
void processValue(const int& value) {
    std::cout << "Processing const int& value: " << value << std::endl;
}
void processValue(int& value) {
    std::cout << "Processing int& value: " << value << std::endl;
}
void processValue(int* value) {
    std::cout << "Processing int* value: " << value << std::endl;
}
void processLValue(std::size_t sz) {
    std::cout << "processLValue(std::size_t sz)" << std::endl;
}
void processValue(const std::vector<int>& v) {
    std::cout << "f(const std::vector<int>& v) " ;
    for (auto& it:v) {
       cout << it << " ";
    }
    cout << endl;
}

// 模板函数，以完美转发方式将参数传递给其他函数
template<typename T>
void forwardArgument(T&& value) {
    // 使用std::forward将参数以完美转发方式传递给其他函数
    // 根据原始参数的值类别和引用修饰符，选择调用processLValue或processRValue函数
    // if constexpr(std::is_lvalue_reference_v<T>) {
    //     processLValue(std::forward<T>(value));
    // } else {
    //     processRValue(std::forward<T>(value));
    // }
    processValue(std::forward<T>(value));
}

template<typename... T>
void processValue_(T&&... args) {
    std::cout << "Arguments: ";

    ((std::cout << args << " "), ...); // 左折叠
    std::cout << std::endl;

    (std::cout << ... << args); // 使用折叠表达式展开参数包
    std::cout << std::endl;

    // ((std::cout << args << " ") , ... , std::cout << "End"); // 折叠表达式
}
// 模板函数，以完美转发方式将参数传递给其他函数
template<typename... T>
void forwardArgument_(T&&... args) {
    // 使用std::forward将参数以完美转发方式传递给其他函数
    // 根据原始参数的值类别和引用修饰符，选择调用processLValue或processRValue函数
    // if constexpr(std::is_lvalue_reference_v<T>) {
    //     processLValue(std::forward<T>(args));
    // } else {
    //     processRValue(std::forward<T>(args));
    // }
    cout << "sizeof(args)=" << sizeof...(args) << endl;
    std::cout << "Arguments: ";
    (std::cout << ... << args); // 使用折叠表达式展开参数包
    std::cout << std::endl;
    processValue_(std::forward<T>(args)...);
}
class Widget {
public:
    static const std::size_t MinVals = 28;  //MinVal的声明, 没有定义和初始化，本身没有地址
};

void f(void(*pf)(int&)) {
    cout << "f(void(*pf)(int&))" << endl;
}

void test1() {
    forwardArgument_(1, 2, 3);

    forwardArgument(28);
    processValue({1,2,3}); // 会把实参与函数参数进行比较，隐式转换为vector
    // 花括号推导失败情况：
    // forwardArgument({1,2,3}); // invalid,编译失效，因为参数先推导为std::initializer_list，再进行比较
    // 花括号导致完美转发失败，模板不会进行实参和参数比较，内部不会调用processValue(vector)
    // 模板会直接对参数进行推导，然后才比较推导之后的类型
    // 1 编译器不能推导出fwd的一个或者多个形参类型。 这种情况下代码无法编译。
    // 2 编译器推导“错”了fwd的一个或者多个形参类型。
    // 解决办法一
    auto il = {4,5,6};
    forwardArgument(il); // valid,编译通过，

    // 0或者NULL作为空指针失败情况：
    forwardArgument(0); // 推导为int
    forwardArgument(NULL); // 推导为int
    forwardArgument(nullptr); // 推导为std::nullptr_t

    // 仅有声明的整型static const数据成员 失败情况：
    std::vector<int> widgetData;
    widgetData.reserve(Widget::MinVals);        //使用MinVals
    processValue(Widget::MinVals);
    // forwardArgument(Widget::MinVals); // invalid,尽管代码中没有使用MinVals的地址，但是fwd的形参是通用引用，而引用，在编译器生成的代码中，通常被视作指针

    // 重载函数的名称和模板名称 失败情况：
    f(processValue);
    // forwardArgument(processValue); // 不能识别，fwd是一个函数模板，没有它可接受的类型的信息，使得编译器不可能决定出哪个函数应被传递
    // f如果是函数模板，也会报错，因为不知道是哪个
    using ProcessFuncType = void (*)(int&);//写个类型定义；见条款9
    ProcessFuncType processValPtr = processValue;     //指定所需的processVal签名
    // forwardArgument(processValPtr);                             //可以
    // forwardArgument(static_cast<ProcessFuncType>(processValue));   //也可以

    // 位域 失败情况：
    struct IPv4Header {
    std::uint32_t version:4,
                  IHL:4,
                  DSCP:6,
                  ECN:2,
                  totalLength:16;
    };
    IPv4Header h;
    processLValue(h.totalLength); // valid,编译通过
    // forwardArgument(h.totalLength); // invalid,forwardArgument的形参是引用，h.totalLength是non-const位域
    //non-const引用不应该绑定到位域。禁止的理由很充分。位域可能包含了机器字的任意部分（比如32位int的3-5位），但是这些东西无法直接寻址。我之前提到了在硬件层面引用和指针是一样的，所以没有办法创建一个指向任意bit的指针（C++规定你可以指向的最小单位是char），同样没有办法绑定引用到任意bit上。
    //拷贝位域值；参看条款6了解关于初始化形式的信息
    auto length = static_cast<std::uint16_t>(h.totalLength);
    forwardArgument(length); // valid,编译通过
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
    test1();
    return 0;
}
