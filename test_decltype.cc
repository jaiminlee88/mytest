


#include<iostream>
#include<functional>
#include<type_traits>

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

int main () {
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
}