#include<iostream>
using namespace std;
// #pragma comment(linker,"/entry:entrance_func")

class stock1 {
public:
    stock1() { // 无参构造函数，只有当不提供任何构造参数时，编译器才会默认提供
        cout << "construct func:" << __FUNCTION__ << " sym:" << sym << ", price:" << price  << endl;
    }
    stock1(string symbol, double price) : sym(symbol), price(price) {
        cout << "construct func:" << __FUNCTION__ << "(string symbol, double price). sym:" << sym << ", price:" << price  << endl;
    }
    ~stock1(){ // 若不写则默认构建一个空的析构函数
        cout << "destruct func:" << __FUNCTION__ << " sym=" << sym << endl;
    }
    void buy() { // 类内定义的都是默认内联函数
        cout << "buy stock1" << endl;
    }
    void sell() { // 类内定义的都是默认内联函数
        cout << "sell stock1" << endl;
    }
    void show(int a=10) const { // 表示结构体本身不会修改
        // price = 3.14; // 会报错
        cout <<  __FUNCTION__ << " sym=" << sym << " price=" << price << endl;
    }
    static int i; // 只能类外初始化
    static const int j{1}; // const情况下，可以在这初始化
private:
    string sym;
    double price;
};

class stock2 {
public:
    stock2(int a) {
        cout << "construct func:" << __FUNCTION__ << "(int a). a=" << a << endl;
    }
private:
    // string sym;
    double price;
};

// static int stock1::i = 0; //必须省略static，原来static变量在编译单元内的定义必须是全局的
int stock1::i = 0;
int main(int argc, char* argv[]) {
    cout << "test1================" << endl;
    stock1 s1;
    s1.show();
    stock1 s2("3000688", 4.12);
    s2.show();
    stock1 s3{"000001",3.14};
    s3.show();
    const stock1 s4 = {"000002", 3.15};
    s4.show(); // 如果没有const将会报错
    cout << "entrance_func()" << endl;
    cout << "test2================" << endl;
    stock2 s5(10);
    stock2 s6 = 4.5;
    stock2 s7 = stock2(11.5);
    cout << "test3================" << endl;
    stock1 stock_array[4] = {stock1("000003", 3.14), stock1("000004", 3.15)};

    cout << "test4================" << endl;
    static  int cnt = 5;
    int array_a[cnt]{0};
    cout << "sizeof(array_a)=" << sizeof(array_a) << endl;
    cnt = 6;
    int array_b[cnt]{0};
    cout << "sizeof(array_b)=" << sizeof(array_b) << endl;

    
    return 0;
}


