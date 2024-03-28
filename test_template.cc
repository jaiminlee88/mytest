#include<iostream>
#include<cstring>
using namespace std;

// 模板默认参数语法，如果没有传递参数，就使用默认参数，下方就是int
template <typename T = int>
void foo() {
    // 函数实现
}

struct job{
    char name[40];
    double salary;
    int floor;
};

template<typename T>
void Swap(T &a, T &b);

// template<> 为显式具体化开头，必须，且优先于常规模板
// template<> void Swap(job &j1, job &j2)
template<> void Swap<job>(job &j1, job &j2); // explicit specialization，优先使用这个  是void Swap(T &a, T &b)的具体化
template void Swap<job>(job &j1, job &j2); // instantiation,会直接生成定义，

// template void Swap<int>(int &j1, int &j2); // instantiation


template<typename T>
void Swap(T *a, T *b, int n);

template<typename T>
void show(T *a, int n) {
    for (int i = 0; i < n; i++) {
        cout << a[i] << ",";
    }
}

// 注意！！！！！！！！！！！！！！模板推导规则
// t到底是什么，由f(expr)和ParamType决定
// template<typename T>
// void f(T param) { //param是一个被推导出来的模板参数
//     cout << __FUNCTION__ << "| T param: typeid(param).name()=" << typeid(param).name() << endl;
// }
// f(expr) //expr是实参
// 方法： T的类型由expr和paramtype共同决定，
//      可以先看expr是什么，
//      再看ParamType是什么，注意如果ParamType是引用，那么T的引用性会被忽略
//      最后推导出T是什么
template<typename T>
void f_lref(T& t) { // t是一个引用
    cout << __FUNCTION__ << "| const T& t: typeid(t).name()=" << typeid(t).name() << endl;
}

template<typename T>
void f_const_lref(const T& t) { // t是一个引用
    cout << __FUNCTION__<< "| const T& t: typeid(t).name()=" << typeid(t).name() << endl;
}

template<typename T>
void f_ptr(T* t) { // t是一个引用
    cout << __FUNCTION__<< "| const T& t: typeid(*t).name()=" << typeid(*t).name() << endl;
}

template<typename T>
void f_rref(T&& t) {  //param现在是一个通用引用类型,情况很特殊，针对右值才会正常推到，左值或左值引用会被推到喂左值引用
    cout << __FUNCTION__ << "| const T& t: typeid(t).name()=" << typeid(t).name() << endl;
}

template<typename T>
void f_initlist(std::initializer_list<T> initList) {  //param现在是一个通用引用类型,情况很特殊，针对右值才会正常推到，左值或左值引用会被推到喂左值引用
    cout << __FUNCTION__ << "| const T& t: typeid(t).name()=" << typeid(initList).name() << endl;
}

template<typename T>
void f_passval(T t) {  //非指针，非引用 传值（pass-by-value） 这意味着无论传递什么param都会成为它的一份拷贝——一个完整的新对象
    // T的所有限制符都会被忽略，这很重要
    // ptr自身的常量性constness将会被省略，但是ptr所指的对象的常量性不会被忽略
    cout << __FUNCTION__ << "| const T& t: typeid(t).name()=" << typeid(t).name() << endl;
}

//在编译期间返回一个数组大小的常量值（//数组形参没有名字，
//因为我们只关心数组的大小）
template<typename T, std::size_t N>                     //关于
constexpr std::size_t arraySize(T (&)[N]) noexcept      //constexpr 强制咋编译期间计算大小
{                                                       //和noexcept
    return N;                                           //的信息
}                                                       //请看下面

double somefunc(int a, double b) {
    double c = a + b;
    return c;
}
void test1() {
    cout << "\n[test1 start]: 模板类型推导" << endl;
    auto s = R"delimiter(
    {
        template<typename T>
        void f(ParamType param) {} //param是一个被推导出来的模板参数
        f(expr);                        //从expr中推导T和ParamType
    }
请记住：
    在模板类型推导时，有引用的实参会被视为无引用，他们的引用会被忽略
    对于通用引用的推导，左值实参会被特殊对待
    对于传值类型推导，const和/或volatile实参会被认为是non-const的和non-volatile的
    在模板类型推导时，数组名或者函数名实参会退化为指针，除非它们被用于初始化引用
        )delimiter";
    std::cout << s << std::endl;
    cout << "1.ParamType是一个指针或引用，但不是通用引用:" << endl;
    int x=27;                       //x是int
    const int cx=x;                 //cx是const int
    const int& rx=x;                //rx是指向作为const int的x的引用
    f_lref(x); //T是int，param的类型是int&
    f_lref(cx); //T是const int，param的类型是const int&
    f_lref(rx); //T是const int，param的类型是const int&, 注意即使rx的类型是一个引用，T也会被推导为一个非引用 ，这是因为rx的引用性（reference-ness）在类型推导中会被忽略
    // f_lref(8); // invalid
    f_const_lref(x); //T是int，param的类型是const int&
    f_const_lref(cx); //T是int，param的类型是const int&
    f_const_lref(rx); //T是int，param的类型是const int&  rx的reference-ness在类型推导中被忽略了。

    const int *px = &x;             //注意px是指向作为 const int的x 的指针，const不是修饰px的
    f_ptr(&x);                          //T是int，param的类型是int*
    f_ptr(px);                          //T是const int，param的类型是const int*
    cout << "2.ParamType是通用引用:" << endl;
    f_rref(x);  //x是左值，所以T是int&，//param类型也是int& 如果expr是左值，T和ParamType都会被推导为左值引用。
    f_rref(cx); //cx是左值，所以T是const int&，//param类型也是const int& 如果expr是左值，T和ParamType都会被推导为左值引用。
    f_rref(rx); //rx是左值，所以T是const int&，//param类型也是const int& 如果expr是左值，T和ParamType都会被推导为左值引用。
    f_rref(27); //27是右值，所以T是int，//param类型就是int&&   如果expr是右值，就使用正常的（也就是情景一）推导规则
    cout << "3.ParamType正常传值:" << endl;
    f_passval(x); //T和param的类型都是int
    f_passval(cx); //T和param的类型都是int
    f_passval(rx); //T和param的类型都是int  expr是一个const，那就再忽略const 它是volatile，也忽略volatile
    const char* const ptr = "Fun with pointers"; //ptr是一个常量指针，指向常量对象 
    f_passval(ptr);  //(const char*) (const) ptr,  其中传递const char * const类型的实参， const ptr起修饰性的const被忽略

    int name[4] = {1,2,3,4};
    cout << "name[0]=" << name[0] << " name[1]=" << name[1] << " name[2]=" << name[2] << endl;
    f_passval(name); //  //name是一个数组，但是T被推导为const char*
    f_lref(name); // name是一个数组，但是T被推导为真正数组const char[4]，包括数组大小4， typeid(t).name()为A4_i，
                  // f_lref形参为 f_lref(const char (&)[4])  注意，4是直接推导出来的
    int mappedVals[arraySize(name)]; //mappedVals也有4个, arraySize推导出数组长度
    cout << "妙！！！ sizeof(int)=" << sizeof(int) << " sizeof(mappedVals)=" << sizeof(mappedVals) << endl;
    // 注意：
    // int* p = name; 
    // cout << "p[0]=" << p[0] << " p[1]=" << p[1] << " p[2]=" << p[2] << endl;
    // cout << "arraySize(p)=" << arraySize(p) << endl; // invalid
    // cout << "[test1 end]" << endl;

    f_passval(somefunc); //param被推导为指向 函数的指针， //类型是void(*)(int, double)
    f_lref(somefunc); //param被推导为指向 函数的引用，//类型是void(&)(int, double)

    {
        cout << "4.auto类型推导：" << endl;
        // auto类型推导和模板类型推导有一个直接的映射关系。它们之间可以通过一个非常规范非常系统化的转换流程来转换彼此。
        // auto扮演了T的角色
        // 变量类型说明符扮演了ParamType的角色
        // 编译器认为每个变量都有一个模板，然后在合适地方初始化调用这个模板
        // 以下类似于
        auto x = 27;
        // template<typename T>
        // void func_for_x(T param) {};//param的推导类型是x的类型
        // func_for_x(27);
        const auto cx = x;
        // template<typename T>
        // void func_for_cx(T param) {}; //param的推导类型是cx的类型
        // func_for_cx(x);
        const auto & rx = cx;
        // template<typename T>
        // void func_for_rx(const T & param) {};//param的推导类型是rx的类型
        // func_for_rx(x);
        auto&& uref1 = x;               //x是int左值，//所以uref1类型为int&
        auto&& uref2 = cx;              //cx是const int左值，//所以uref2类型为const int&
        auto&& uref3 = 27;              //27是int右值，//所以uref3类型为int&&
        const char name[] =  "R. N. Briggs";           //name的类型是const char[13]
        auto arr1 = name;               //arr1的类型是const char*
        auto& arr2 = name;              //arr2的类型是const char (&)[13]
        auto func1 = somefunc;          //func1的类型是void (*)(int, double)
        auto& func2 = somefunc;         //func2的类型是void (&)(int, double)

        auto x1 = 27;                   //类型是int，值是27
        auto x2(27);                    //同上
        auto x3 = { 27 };               //类型是std::initializer_list<int>，//值是{ 27 }
        auto x4{ 27 };                  //同上
        cout << __FUNCTION__ << "| auto x4{ 27 } : typeid(x4).name()=" << typeid(x4).name() << endl;

        // auto x5 = { 1, 2, 3.0 };        //错误！无法推导std::initializer_list<T>中的T, 3.0与其他元素类型不一致
        auto x5 = { 1, 2, 3 };        //x的类型是std::initializer_list<int>
        cout << __FUNCTION__ << "| typeid(x5).name()=" << typeid(x5).name() << endl;
        // f_passval({1, 2, 3}); // 错误！单纯模板类型，不能直接推导, 无法推导std::initializer_list，因为根本不知道
        f_initlist({1, 2, 3}); //T被推导为int，initList的类型为 //std::initializer_list<int>

       
    }

    cout << "\n[test1 end]" << endl;
}

auto createInitList()
    {
        return { 1, 2, 3 };         //错误！不能推导{ 1, 2, 3 }的类型
    }

// class Widget {                                  //方法1：对左值和右值重载
// public:
//     void addName(const std::string& newName)
//     { names.push_back(newName); } // rvalues
//     void addName(std::string&& newName)
//     { names.push_back(std::move(newName)); }
//     …
// private:
//     std::vector<std::string> names;
// };

// class Widget {                                  //方法2：使用通用引用，可能会展开各种各样的函数
// public:
//     template<typename T>
//     void addName(T&& newName)
//     { names.push_back(std::forward<T>(newName)); }
//     …
// };

class Widget {                                   // 方法3：使用传值，对于类可能会产生对象切片问题
public:
    void addName(std::string newName) {         //接受左值或右值；移动它，
        names.push_back(std::move(newName));
    }
private:
    std::vector<std::string> names;
};

void test2() {
    // 对于移动成本低 且 总是被拷贝的可拷贝形参，考虑按值传递
    // 对比三种方法的开销，如果差不多，就用传值，也不会函数膨胀
    // 如果开销明确很大，那就用引用
    Widget w;
    std::string name("Bart");
    w.addName(name);            //使用左值调用addName
    w.addName(name + "Jenne");  //使用右值调用addName（见下）


}
int main() {
    int a = 1;
    int b = 2;
    cout << "before: a=" << a << " b=" << b << endl;
    Swap(a,b);
    cout << "after : a=" << a << " b=" << b << endl;

    constexpr int len = 5;
    int a1[len] = {1,2,3,4,5};
    int b1[len] = {6,7,8,8,10};
    cout << "before: \na1=";
    show(a1, len);
    cout << " b1=";
    show(b1, len);
    cout << endl;
    Swap(a1, b1, len); /////
    cout << "after: \na1=";
    show(a1, len);
    cout << " b1=";
    show(b1, len);
    cout << endl;

    job j1;
    string name = "abc"; ///
    strcpy(j1.name, name.c_str());
    j1.salary = 1000.0;
    j1.floor = 500;
    job j2;
    name = "def"; ///
    strcpy(j2.name, name.c_str());
    j2.salary = 5000.0;
    j2.floor = 800;
    cout << "before j1:" << j1.name << " " << j1.salary << " " << j1.floor << endl;
    cout << "before j2:" << j2.name << " " << j2.salary << " " << j2.floor << endl;
    Swap(j1, j2); //////
    cout << "after j1:" << j1.name << " " << j1.salary << " " << j1.floor << endl;
    cout << "after j2:" << j2.name << " " << j2.salary << " " << j2.floor << endl;

    cout << "before j1:" << j1.name << " " << j1.salary << " " << j1.floor << endl;
    cout << "before j2:" << j2.name << " " << j2.salary << " " << j2.floor << endl;
    Swap(j1, j2); //////
    cout << "after j1:" << j1.name << " " << j1.salary << " " << j1.floor << endl;
    cout << "after j2:" << j2.name << " " << j2.salary << " " << j2.floor << endl;

    test1();
    test2(); // 对于移动成本低且总是被拷贝的可拷贝形参，考虑按值传递
}


// template<class Type>
template<typename Type>
void Swap(Type &a, Type &b) {
    Type temp = a;
    a = b;
    b = temp;
}

template<typename T>
void Swap(T *a, T *b, int n) {
    T temp;
    for (int i = 0; i < n ; i++) {
        temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
}

template <> void Swap<job>(job &j1, job &j2) {
    double salary;
    int floor;
    salary = j1.salary;
    j1.salary = j2.salary;
    j2.salary = salary;

    floor = j1.floor;
    j1.floor = j2.floor;
    j2.floor = floor;
}