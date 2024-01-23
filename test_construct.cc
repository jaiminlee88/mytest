#include <iostream>
#include <string.h>

using namespace std;

class func{
public:
    func(){ 
        cout << "=================construct func" << endl;
    }
private:
    int a{0};
};
class func1 {
public:
    // 没有构造函数，编译器默认创建
    // 没有析构函数，编译器默认创建, 注意是 non-virtual的，除非 该类的 基类的 析构函数 是virtual的，否则不会生成virtual析构函数
    // 没有复制构造函数，编译器默认创建，注意，默认行为可能与预期不同， func1(const func1 & i)
    // 没有移动构造函数，而又需要使用到，则编译器自动创建一个 func1(func1 &&i)，注意不能有const
    // 没有赋值运算符，编译器默认创建，注意，默认行为可能与预期不同 func1& operator=(func1 & i)
    // 没有地址运算符，编译器默认创建，注意，默认行为可能与预期不同 func1* operator&() 返回this
    
    // 特殊成员函数是编译器可能自动生成的函数：默认构造函数，析构函数，拷贝操作，移动操作。
    // 移动操作仅当类没有显式声明移动操作，拷贝操作，析构函数时才自动生成。
    // 拷贝构造函数仅当类没有显式声明拷贝构造函数时才自动生成，并且如果用户声明了移动操作，拷贝构造就是delete。拷贝赋值运算符仅当类没有显式声明拷贝赋值运算符时才自动生成，并且如果用户声明了移动操作，拷贝赋值运算符就是delete。当用户声明了析构函数，拷贝操作的自动生成已被废弃。
    // 成员函数模板不抑制特殊成员函数的生成。
    func1() {  // 如果没有func1 double构造函数，会默认生成一个func1()
        cout << "construct func1" << endl;
    }
    // func1() = default; // 没有主动实现时，这个可以强制显示声明默认版本，default只能用于6个特殊成员函数
    func1(int i[]) = delete; // 禁止编译器使用该特定方法，可以适用于任何成员函数

    func1(double i) : m1(i) {
        /*
        注意,如果m1是简单的变量
            如int mi时,可以在声明时初始化,如int mi = 0;
            但是如果mi是一个类对象,则不能在声明时初始化,如func f1("abd");是错误的,因为编译器不知道如何初始化f1.
            参看 test_thread_pool.cc 中的sorter类
        直接在声明时初始化成员变量通常用于对基本类型和简单类型的初始化，
        而通过构造函数的成员初始化列表进行初始化更适用于复杂类型或需要根据构造函数参数进行初始化的情况。
        不同的初始化方式可以根据实际需求选择使用。
        */
        // 只有这种单一参数的构造函数才会隐式转换 c=2.8才能用，如果使用explict限制，就不能c=2.8了
        // 注意，使用初始化列表可以对const成员变量进行初始化，但是不能对static成员变量进行初始化
        // 初始化列表过程是，直接把实参赋值为形参，形参直接传递给构造函数，构造函数创建一个嵌套对象，并在该对象中存储这些数据，相当于只赋值了一次
        // 如果使用构造函数初始化，这种方式会先调用默认构造函数进行初始化，然后再通过赋值操作符或成员初始化语法来改变成员变量的值。相当于两次
        // 如果成员变量是类成员，会先调用类成员构造函数，然后再执行本构造函数
        cout << "construct func1_double" << endl;
        obj_cnt += 1;
    }
    func1(func1& i) : func1(){ // 委托构造函数，调用自己的构造函数，叫委托使用，也可以放到初始化列表中
        m1 = i.m1;
        cout << "construct func1_copy" << endl;
        obj_cnt += 1;
        func1(); // 委托构造函数，调用自己的构造函数，叫委托使用，也可以放到初始化列表中
    }
    // func1(double i = 0) : m1(i) { // invalid 比上方构造函数多了一个默认参数，call of overloaded ‘func1()’ is ambiguous， func1 c; 会报错，不能区别到底是func1half还是func1double
    //     cout << "construct func1_double" << endl;
    // }
    func1(string i) {
        pstr = new char[i.size() + 1];
        strcpy(pstr, i.c_str());
        cout << "construct func1_string " << pstr << endl;
        obj_cnt += 1;
    }
    virtual ~func1() {
        cout << "destruct func1" << endl;
        obj_cnt -= 1;
        if (pstr != nullptr) {
            delete pstr;
            pstr = nullptr;
        }
    }
    operator int() const { // 转换函数用法，最好不要用隐式转换
        return (int) m1;
    }
    operator double() const {
        return m1;
    }
    char& operator[](int i){
        return pstr[i];
    }
    const char& operator[](int i) const{ // 允许这样的重载，一遍const变量时，能够通过[]访问，但是不能修改
        return pstr[i];
    }

    static int get_obj_cnt() {
        // return func1::obj_cnt; // valid
        return obj_cnt; // valid
    }

    void print(int i) { cout << "func1 print_int" << endl;}
    void print(double i){ cout << "func1 print_double" << endl;}
private:
    static int obj_cnt;
    double m1;
    char *pstr{nullptr};
    func f1;
};

class func2 : public func1 {
public:
    func2(){}
    ~func2(){}
    using func1::print; //继承构造函数
    using func1::func1; // 继承基类所有构造函数（默认、复制、移动构造函数除外）
    void print(double i) { cout << "func2 print_double" << endl;}
};

int func1::obj_cnt = 0;

class func3 {
public:
    // func3() {} // default
    // ~func3() {}// default
    // func3(func3& i);// default
    // operator=(func3& i);// default
    void print_mem() { cout << "func3 print_mem a=" << a << " b=" << b << endl;}
    void set_b(int i) { b = i;}
private:
    const static int a = 4;
    int b = 5;
};

class func4 {
public:
    // func4() {} // default
    // ~func4() {}// default
    // operator=(func4& i);// default
    func4(int& i, const int& j) : a(i),b(j) {} // invalid
    void print_mem() { cout << "func4 print_mem a=" << a << " b=" << b << endl;}
private:
    func4(func4& i);// derivative class不会生成默认的复制构造函数,可以用来禁止复制
    int& a;
    const int b = 5;
};

// class func5 : public func4 {
// public:
//     func5(func5& i) {} // invalid derivative class不会生成默认的复制构造函数
// };
void test1() {
    func3 f3;
    f3.print_mem();
    f3.set_b(10);
    func3 f4(f3); // 复制构造函数
    func3 f5 = f3; // 复制构造函数
    f4.print_mem();
    f5.print_mem();

    int a = 10;
    int b = 11;
    // func4 f6(a); // invalid
    func4 f7(a,b);
    f7.print_mem();
    int c = 12;
    int d = 13;
    func4 f8(c,d);
    f8.print_mem();
    // f7 = f8; // invalid, 更改const不合法，引用成员a不会自动赋值，需要自己实现赋值操作符

    // func5 f9(); // derivative class不会生成默认的复制构造函数
}

int main() {
    func1 c; // 调用默认构造函数
    c = 2.8; // 用func1_double初始化一个func1，再复制到c中
    double i = c;
    int j = c;
    cout << i << " " << j << endl;

    cout << "0================" << endl;
    func1 c1(c);
    cout << "1================" << endl;
    func1 c2 = c;
    cout << "2================" << endl;
    func1 c3 = func1(c);
    cout << "3================" << endl;
    func1 *pc4 = new func1(c);

    cout << "4================" << endl;
    func1 c5("abcdedg");
    cout << "c5[3]=" << c5[3] << endl;
    const func1 c6("abcdedg");
    cout << "c6[4]=" << c6[4] << endl;
    cout << "5================" << endl;
    cout << func1::get_obj_cnt() << endl;

    cout << "6=====================" << endl;
    func2 f;
    f.print(3.4); // 用的func2的print
    f.print(3); // 用的func1的print。如果不用using func1::print，会统一用func2的print
    func2 f1(10.0); // using强制继承基类构造函数，只初始化基类成员，派生成员需要使用初始化列表方法（派生构造函数，初始化列表调用基类构造函数），

    test1();
    
    return 0;
}