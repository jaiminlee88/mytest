#include<iostream>
using namespace std;

int gI = 0;

class Book {
public:
  int m{3};
  Book() {
    std::cout << gI << ":"
              << "default constructor\n";
  }
  Book(Book &&b) {
    std::cout << gI << ":"
              << "move constructor\n";
  }
  Book(const Book&& b) { // 移动构造函数是区分const的,尽量不要在移动时声明为const，否则可能会被转为拷贝操作，eg. string
    std::cout << gI << ":"
              << "const move constructor\n";
  }
  Book(const Book &b) { // 常量
    std::cout << gI << ":"
              << "copy constructor\n";
  }
  Book &operator=(const Book &) {
    std::cout << gI << ":"
              << "copy assigment\n";
              return *this;
  }
  Book &operator=(Book &&) { 
    // 必须编写移动构造函数行为，编译器会根据参数类型（右值引用）调用移动构造函数
    // 如果不定义移动构造函数，move失效，仍然使用左值引用
    // 使用移动构造函数，因为需要修改右值引用的值，不能有const，
    std::cout << gI << ":"
              << "move assignment\n";
              return *this;
  }
};

class Widget {
public:
    Widget() {}
    Widget(const Widget& rhs) { cout << "Widget(const Widget& rhs)" << endl; }
    Widget(Widget&& rhs) { cout << "Widget(Widget&& rhs)" << endl; }
    Widget& operator=(const Widget& rhs) { cout << "operator=(const Widget& rhs)" << endl; return *this; }
    Widget& operator=(Widget&& rhs) { cout << "operator=(Widget&& rhs)" << endl; return *this; }
};
void process(const Widget& lvalArg) {        //处理左值
    cout << "process(const Widget& lvalArg)" << endl;
}
void process(Widget&& rvalArg){ //处理右值
    cout << "process(Widget&& rvalArg)" << endl;
}
template<typename T>
void logAndProcess(T&& param) {//用以转发param到process的模板, 
    // auto now = std::chrono::system_clock::now(); //获取现在时间
    process(std::forward<T>(param)); // std::forward只有当它的参数被绑定到一个右值时，才将参数转换为右值。
}

void test1() {
    Widget w;
    logAndProcess(w); //传递左值，调用logAndProcess(T&),由于引用折叠规则，此时参数 param 的类型仍然是一个左值引用
    logAndProcess(std::move(w)); //传递右值，调用
}

int main() {
    //   C++11的移动语义并无优势：
    // 没有移动操作：要移动的对象没有提供移动操作，所以移动的写法也会变成复制操作。
    // 移动不会更快：要移动的对象提供的移动操作并不比复制速度更快。
    // 移动不可用：进行移动的上下文要求移动操作不会抛出异常，但是该操作没有被声明为noexcept。
    // 源对象是左值：除了极少数的情况外（例如Item25），只有右值可以作为移动操作的来源。

    // 右值引用可以绑定到一个临时对象上，本身只是一个指针，指向右值所在内存地址，与普通指针无差
    // 右值引用是一个左值，可以取地址，但是为右值引用通常用于移动语义，取地址并不符合其设计目的。
    // 与普通指针不同的是，右值引用具有生命周期，当右值引用超出作用域时，其指向对象（临时对象）会被销毁
    // 右值引用实现move和forward，可以避免不必要的独享拷贝
    int a = 1;
    int& b = a;
    cout << "1 a=" << a << " b=" << b << endl;
    b = 3;
    cout << "2 a=" << a << " b=" << b << endl;
    int&& c = std::move(b);  // 变成右值引用，右值引用本身是左值
    cout << "3 c=" << c << " b=" << b << endl;
    c = 4;
    cout << "4 c=" << c << " b=" << b << " a=" << a << endl;
    int& d = c;
    cout << "5 d=" << d << " c=" << c << " b=" << b << " a=" << a << endl;
    int&& d1 = std::move(d); // 变成右值引用
    cout << "d1 typeid(d1).name() = " << typeid(d1).name() << endl;
    int&& e = '1';
    cout << "6 e=" << e << endl;
    e = '2';
    cout << "7 e=" << e << endl;
    int&& f = std::move(a); // 变成右值引用
    cout << "f typeid(f).name() = " << typeid(f).name() << endl;
    cout << "8 f=" << f << endl;
    int&& f11 = std::move(f); // f是右值引用，move后变成右值
    cout << "f11 typeid(f11).name() = " << typeid(f11).name() << " f11=" << f11 << endl;
    cout << "address of f11=" << &f11 << endl; // 注意，右值引用本身是左值，可以取地址，但是为右值引用通常用于移动语义，取地址并不符合其设计目的。


    gI = 0;
    std::cout << gI << "- ";
    Book origin = Book(); // prvalue直接初始化 default constructor

    gI = 1;
    std::cout << gI << "- ";
    Book origin1; //  default constructor

    gI = 2;
    std::cout << gI << "- ";
    origin1 = Book(); // 默认创建book临时变量default，右值引用移动move，移动 pravlue，如果注释掉move就是就都是copy

    gI = 3;
    std::cout << gI << "-";
    origin = std::move(origin1); // 强制移动，将lvalue转换成xvalue，然后编译器会主动调用移动move,如果注释掉move就是就都是copy
    // move本身没有任何操作，类似标记

    gI = 4;
    const Book origin2(std::move(origin));
    Book origin3(std::move(origin2));

    test1();

    int g = 1;
    if (std::is_lvalue_reference<decltype(std::move(g))>::value) {
        cout << "std::move(g) is lrvalue" << endl;
    } else if (std::is_rvalue_reference<decltype((std::move(g)))>::value) {
        cout << "std::move(g) is rrvalue" << endl;
    } else {
        cout << "std::move(g) is not lvalue or rvalue" << endl;
    }
    if (std::is_lvalue_reference<decltype(std::move("a"))>::value) {
        cout << "std::move(\"a\") is lrvalue" << endl;
    } else if (std::is_rvalue_reference<decltype(std::move("a"))>::value) {
        cout << "std::move(\"a\") is rrvalue" << endl;
    } else {
        cout << "std::move(\"a\") is not lvalue or rvalue" << endl;
    }
    int&& g1 = std::move(g);
    if (std::is_lvalue_reference<decltype(g1)>::value) {
        cout << "std::move(g1) is lrvalue" << endl;
    } else if (std::is_rvalue_reference<decltype(g1)>::value) {
        cout << "std::move(g1) is rrvalue" << endl;
    } else {
        cout << "std::move(g1) is not lvalue or rvalue" << endl;
    }
}