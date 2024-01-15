#include <iostream>

inline void inlineFunction() {
    /*
    编译器在处理inline函数中的static变量时，通常会将其视为局部变量。
    在C++中，当一个函数被声明为inline时，在编译过程中会尝试将函数的调用地点直接替换为函数体内的代码。而对于函数体内的static变量，编译器会为每个调用该函数的地方创建一个独立的实例。
    这种处理方式保证了每个调用点都拥有自己独立的static变量，避免了多个调用点之间的变量冲突问题。同时，由于每个调用点都会创建一个新的实例，函数的局部static变量也会在每次函数调用结束后被销毁和重建，而不会保留其上一次调用的值。
    需要注意的是，inline函数内的static变量只会在第一次调用时被初始化，之后的调用会继续使用上一次的值。这种行为与普通函数内的static变量是一致的。如果需要保证每次调用时static变量都重新初始化，可以通过将该变量声明为非static来实现。
    */
    // inline会展开多次，每次展开都有自己静态变量,独立实例
    
    static int count = 0; // 声明一个静态变量，注意，第一次调用时才会初始化，之后就不会初始化
    // 每次调用inlineFunction函数时，count的值都是独立的，且会保留在函数调用之间。
    // 这是因为编译器为每个调用点创建了一个独立的count实例。

    count++; // 每次调用将count加1
    std::cout << "Inline function called " << count << " times." << std::endl;
}

int main() {
    inlineFunction(); // 第一次调用
    inlineFunction(); // 第二次调用
    inlineFunction(); // 第三次调用

    return 0;
}
