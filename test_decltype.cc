


#include<iostream>
#include<functional>
#include<type_traits>
#include <vector>
#include <deque>

using namespace std;

int abc(){
    int a;
    return a;
}

// decltype(abc) def() {
//     int b;
//     return b;
// }

class Foo {
public:
    static const int Number = 0;
    int x;
};

// 只能在block外定义
// c++11 函数名称前面的auto不会做任何的类型推导工作，必须有类型后置
// c++14 c++17的auto允许自动推导所有的lambda表达式和函数，可以不需要类型后置
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) { // 返回类型后置，执行前并不知道返回类型，只有执行后才知道
    return t + u;
}

// result_of测试用例
struct test2_MyAdder {
    int operator()(int a, int b) const {
        return a + b;
    }
};
int test2_fun(int x) {
    return x + 1;
}
void test2 (){
    // std::result_of编译时推断函数或者对象的返回类型
    // std::result_of 需要一个函数类型或函数指针类型作为模板参数，
    std::function<std::string(int)> f = [](int x) { return std::to_string(x + 1); }; // 注意,此时f是一个函数对象,而不是一个函数
    // std::result_of<f(int)>::type ret_type = "abc"; // invalid
    std::result_of<decltype(f)&(int)>::type ret_type2 = "abc"; // & 的作用是用来获取函数对象的引用
    cout << ret_type2 << endl;
    // std::result_of<test2_fun(int)>::type dd = 10;    // invalid
    std::result_of<decltype(&test2_fun)(int)>::type result_type3 = 3; // 注意,此时test2_fun是一个函数,而不是一个函数对象
    std::result_of<test2_MyAdder(int, int)>::type result_type4 = 0;
}

#if __cplusplus < 201402L

template<typename Container, typename Index>    //可以工作，
auto authAndAccess(Container& c, Index i) ->decltype(c[i]){// c++11 支持
    return c[i]; // auto推导出来的类型是int&，而不是int
}

template<typename Container, typename Index>    //最终的C++11版本
auto authAndAccess(Container&& c, Index i) ->decltype(std::forward<Container>(c)[i]){
    return std::forward<Container>(c)[i];
}

#endif

#if __cplusplus >= 201402L

template<typename Container, typename Index>    //可以工作，
auto authAndAccess(Container& c, Index i) {// c++11 支持
    return c[i]; // auto推导出来的类型是int&，而不是int
}

// auto说明符表示这个类型将会被推导，
// decltype说明decltype的规则将会被用到这个推导过程中
template<typename Container, typename Index>    //可以工作，
decltype(auto) authAndAccess_decltype(Container& c, Index i) {// c++14 支持
    return c[i]; // auto推导出来的类型是int&，而不是int
}

template<typename Container, typename Index>    //最终的C++14版本
decltype(auto) authAndAccess(Container&& c, Index i){
    return std::forward<Container>(c)[i]; // 保证返回值的类型和传入的类型一致
}

#endif

void test3() {
    std::deque<int> d{1,2,3};
    std::vector<bool> vec{true, false, false};
    cout << __FUNCTION__ << "| typeid(vec[0]).name()=" << typeid(vec[0]).name() << endl;
    cout << __FUNCTION__ << "| typeid(authAndAccess(vec, 0)).name()=" << typeid(authAndAccess(vec, 0)).name() << endl; // reference
    authAndAccess(vec, 0) = false; // 返回int&
    for (auto i : vec) {
        cout << i << " ";
    }
    cout << endl;
    cout << "authAndAccess(d, 1)=" << authAndAccess(d, 1) << endl;
    // authAndAccess(d, 1) = 5; // invalid,剥离了reference，返回的是int
    cout << endl;
    #if __cplusplus == 201402L
    {
        authAndAccess_decltype(d, 1) = 5; // valid,相比于c++11，c++14中decltype(auto)会保留reference
        cout << "authAndAccess(d, 1)=" << authAndAccess(d, 1) << endl;

        cout << "test authAndAccess_decltype: ";
        authAndAccess_decltype(vec, 1) = true;
        for (auto i : vec) {
            cout << i << " ";
        }
        cout << endl;
    }
    #endif
    

    int w = 3;
    const int& cw = w;
    auto myWidget1 = cw; //auto类型推导    //myWidget1的类型为Widget cw的类型为const Widget&
    // cw = 1; // error: assignment of read-only reference ‘cw’
    cout << "w=" << w << " cw=" << cw << " &cw=" << &cw << " myWidget1=" << myWidget1 << " &myWidget1=" << &myWidget1 << endl;
    #if __cplusplus == 201402L
    {
        decltype(auto) myWidget2 = cw; //decltype类型推导  //myWidget2的类型是const Widget&
        cout << "w=" << w << " cw=" << cw << " &cw=" << &cw << " myWidget2=" << myWidget2 << " &myWidget2=" << &myWidget2 << endl;
    }
    #endif

    // decltype特殊性
    int x = 1;
    decltype(x) y = x; // y -> int
    cout << "typeid(y).name()=" << typeid(y).name() << endl; // i
    decltype((x)) z = x; // z -> int &，注意是左值引用
    cout << "typeid(z).name()=" << typeid(z).name() << endl; // i
    z = 3; // 明确表明这是引用，所以x也会被修改，在函数中也是如此
    cout << "x=" << x << " y=" << y << " z=" << z << endl; // x=3 y=1 z=3
    #if __cplusplus >= 201402L
    {
        decltype(auto) f0 = [](){
            int x = 1;
            return x;
        };
        cout << "typeid(f0()).name()=" << typeid(f0()).name() << endl; // i
        decltype(auto) f1 = [](){
            int x = 1;
            return (x);
        };
        cout << "typeid(f1()).name()=" << typeid(f1()).name() << endl; // i
        // std::function<decltype(auto)()> f2 = [](){ // invalid
        //     int x = 1;
        //     return (x);
        // };
        // cout << "typeid(f2()).name()=" << typeid(f2()).name() << endl; // i
        auto f3 = []() -> decltype(auto) {
            int x = 1;
            return (x);
        };
        cout << "typeid(f3()).name()=" << typeid(f3()).name() << endl; // return int&
    }
    #endif
}

template<typename T>                //只对TD进行声明
class TD;                           //TD == "Type Displayer"

void test4() {
    // decltype(auto) f3 = []() {
    //     int x = 1;
    //     return (x);
    // };
    // TD<decltype(f3)> yType;

    // int x = 0;                        //x是int
    // int& y = x;                       //y是int&
    // TD<decltype(x)> xType;              //引出包含x和y
    // TD<decltype(y)> yType;              //的类型的错误消息
}

int main () {
    // decltype最主要的用途就是用于声明函数模板，而这个函数返回类型依赖于形参类型

    // int a = 1;
    // decltype(a) b =3;
    // float c;
    // decltype(a+c) d=4;
    // cout << "a=" << a << " b=" << b << " d=" << d << " typeid(d)="<< typeid(d).name() << endl;

    // int x = 0;
    // decltype(x) y = 1; // y -> int
    // decltype(x + y) z = 0; // z -> int
    // const int& i = x;
    // decltype(i) j = y; // j -> const int &
    // const decltype(z) * p = &z; // *p -> const int, p -> const int *
    // decltype(z) * pi = &z; // *pi -> int , pi -> int *
    // decltype(pi)* pp = &pi; // *pp -> int * , pp -> int * *

    int n = 0;
    volatile const int & x = n;
    decltype(n) a = n; // a -> int
    decltype(x) b = n; // b -> const volatile int &
    decltype(Foo::Number) c = 0; // c -> const int，注意是static
    Foo foo;
    decltype(foo.x) d = 0; // d -> int，类访问表达式

    // int& func_int_r(void); // 左值（lvalue，可简单理解为可寻址值）
    // int&& func_int_rr(void); // x 值（xvalue，右值引用本身是一个xvalue）
    // int func_int(void); // 纯右值（prvalue，将在后面的章节中讲解）
    // const int& func_cint_r(void); // 左值
    // const int&& func_cint_rr(void); // x 值
    // const int func_cint(void); // 纯右值
    // const Foo func_cfoo(void); // 纯右值
    // // 下面是测试语句
    // int x = 0;
    // decltype(func_int_r()) a1 = x; // a1 -> int &
    // decltype(func_int_rr()) b1 = 0; // b1 -> int &&
    // decltype(func_int()) c1 = 0; // c1 -> int
    // decltype(func_cint_r()) a2 = x; // a2 -> const int &
    // decltype(func_cint_rr()) b2 = 0; // b2 -> const int &&
    // decltype(func_cint()) c2 = 0; // c2 -> int 对于纯右值而言，只有类类型可以携带cv 限定符，此外则一般忽略掉cv 限定
    // decltype(func_cfoo()) ff = Foo(); // ff -> const Foo 注意纯右值

    // struct Foo { int x; };
    // const Foo foo = Foo();
    // decltype(foo.x) a = 0; // a -> int
    // decltype((foo.x)) b = a; // b -> const int &
    // int n = 0, m = 0;
    // decltype(n + m) c = 0; // c -> int
    // decltype(n += m) d = c; // d -> int & ， n+=m 返回一个左值，按照推导规则3，decltype 的结果为int &

    cout << add(1, 2) << endl;
    cout << add(2, 3.2) << endl;

    test2(); // result_of测试用例
    test3(); // decltype(auto)测试用例
    // test4(); // 查看模板推导结果方法
}