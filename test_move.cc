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

int main() {
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
    int&& e = '1';
    cout << "6 e=" << e << endl;
    e = '2';
    cout << "7 e=" << e << endl;
    int&& f = std::move(a); // 变成右值引用


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
}