#include <iostream>
using namespace std;

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

template <typename T>
inline T const& Max(T const& a, T const& b) { // inline函数模板, 通常在头文件内,会导致编译时间变长，代码膨胀，inline本身是有成本的
    return a < b ? b : a;
}

class base {
public:
    base(){} // default inline, 如果不再类内定义，就不是inline了
    virtual ~base(){} // default inline
    inline virtual void func1() { // 此处inline是多余的，因为虚函数不能是inline的，virtual只有到运行时才知道是哪个函数，而inline是编译时就知道是哪个函数
        cout << __func__ << "====func1======" << endl;
    }
    void func2(); // not inlined
};

void base::func2() {
    cout << __func__ << "====test2======" << endl;
}

void test1() {
    cout << __func__ << "====start======" << endl;
    base b;
    b.func1();
    b.func2();
}
int main() {
    /*
    inline放于头文件，因为inline的展开都是在编译阶段，而非链接阶段，如果放在源文件，只有源文件内才能展开，其他源文件无法看到展开的代码

    inline通常在头文件内，因为inline函数的定义必须在调用点之前，而头文件通常会被多个源文件包含，因此将inline函数的定义放在头文件内可以保证其在所有调用点之前被定义。

    在C++中，
    类内 定义的 成员函数（不管是在头文件内还是源文件内）默认是内联的。
    构造函数默认inline，如果构造函数定义在源文件内，需要inline关键字显式声明为内联函数，但是是否展开取决于编译器。

    头文件内 定义的 非成员函数 不默认是内联的，需要使用inline关键字显式声明为内联函数。

    非成员函数的定义一般放在源文件中，根据需要使用inline关键字来声明为内联函数。

    由于内联函数的定义通常放在头文件中，可以被多个源文件共享，因此在头文件中定义的非成员函数通常会使用inline关键字来声明为内联函数。

    需要注意的是，函数是否真正内联化，取决于编译器的具体实现。
    编译器会根据一些规则来决定是否将函数内联展开，即使使用了inline关键字。因此，inline关键字只是一个建议，编译器会自行决定是否真正内联化函数。
    即使没有内敛，但是编译器也有可能为函数生成一个函数体

    注意，使用inline一旦展开，未来如果函数发生变化，可能需要编译更长时间，因为每个调用点都需要重新编译。

    80-20法则，程序将80%的执行时间花在20%的代码上，因此，只有在这20%的代码上使用inline才能带来性能提升。
    */
    inlineFunction(); // 第一次调用
    inlineFunction(); // 第二次调用
    inlineFunction(); // 第三次调用

    std::cout << "Max(3, 7): " << Max(3, 7) << std::endl;
    std::cout << "Max(3.0, 7.0): " << Max(3.0, 7.0) << std::endl;

    test1();
    return 0;
}
