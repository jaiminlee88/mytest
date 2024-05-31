#include<iostream>
#include<functional>
#include<vector>
#include<math.h>
#include<algorithm>
#include<iterator>
#include <memory>
using namespace std;

using FilterContainer = std::vector<std::function<bool(int)>>; //“using”参见条款9，//std::function参见条款2
FilterContainer filters;                    //过滤函数
void addDivisorFilter(){
    // auto calc1 = computeSomeValue1();
    // auto calc2 = computeSomeValue2();
    // auto divisor = computeDivisor(calc1, calc2);
    auto calc1 = 10;
    auto calc2 = 5;
    auto divisor = 2; // 退出时，divisor将会被销毁，但lambda表达式中的divisor引用还存在，产生未定义行为
    filters.emplace_back(                               //危险！对divisor的引用
        [&](int value) { return value % divisor == 0; } //将会悬空！因为只是添加了表达式，产生一个闭包，等执行时divisor产生未定义行为
    );
}

class Widget {
public:
    Widget(){}                      //构造函数等
    int get_divisor() const { return divisor; }
    void set_divisor(int d) { divisor = d; }
    void addFilter() const {
        filters.emplace_back(
            [=](int value) { // 注意，此处捕获的是this指针
                cout << "-----------" << endl;
                cout << "[Widget] filters divisor=" << divisor << endl;
                return value % divisor == 0; // this->divisor
            } // divisor是non-static成员变量，能捕获，编译通过

            // [divisor](int value) { // 不能捕获，编译不通过 注意，此处捕获的是this指针，真的吗？
            //     cout << "-----------" << endl;
            //     cout << "[Widget] filters divisor=" << divisor << endl;
            //     return value % divisor == 0; // this->divisor
            // } // divisor是non-static成员变量，能捕获，编译通过

            // [](int value) { // 不能捕获，编译不通过，默认引用
            //     cout << "-----------" << endl;
            //     cout << "Widget filters divisor=" << divisor << endl;
            //     return value % divisor == 0;
            // } // divisor是non-static成员变量，说是不能捕获，编译不通过
        );
    }; //向filters添加条目
private:
    int divisor;            //在Widget的过滤器使用 non-static成员变量
};

void addDivisorFilter_1() {
    Widget w1;
    w1.set_divisor(5);
    w1.addFilter();
    cout << "[addDivisorFilter_1] filters[2](10)=" << filters[2](10) << endl;
}

void test1() {
    addDivisorFilter();
    // filters[0](10); // 未定义行为
    Widget w;
    w.set_divisor(5);
    w.addFilter();
    cout << "filters.size()=" << filters.size() << endl;
    cout << "filters[1](10)=" << filters[1](10) << endl;

    addDivisorFilter_1();
    // cout << w1.get_divisor() << endl; // 未定义行为，w1已经销毁，但是filters[2]中的lambda表达式中的this指针还存在，产生未定义行为
    cout << "filters.size()=" << filters.size() << endl;
    // cout << "filters[2](10)=" << filters[2](10) << endl; // 未定义行为，w1已经销毁，但是filters[2]中的lambda表达式中的this指针还存在，产生未定义行为
}

class Widget_1 {                          //一些有用的类型
public:
    bool isValidated() const { return validate; }
    bool isProcessed() const { return process; }
    bool isArchived() const { return archive; }
private:
    bool validate{true};
    bool process{true};
    bool archive{true};
};

class PolyWidget {
public:
    template<typename T>
    void operator()(const T& param) {
        cout << "PolyWidget::operator() param=" << endl;
    }
};

void test2() {
    cout << "[test2] start=========================" << endl;
#if __cplusplus > 201103L
    auto pw = std::make_unique<Widget_1>();   //创建Widget；使用std::make_unique
                                        //的有关信息参见条款21
    auto func = [pw = std::move(pw)] () { //pw是闭包内的变量，std::move(pw)的pw是外部声明
                return pw->isValidated() && pw->isArchived(); }; //使用std::move(pw)初始化闭包数据成员
    cout << "[test2] func()=" << func() << endl;
#endif

    std::vector<double> data{1,2,3};               //同上
    cout << "[test2] data.size()=" << data.size() << endl;
    // 使用bind方式实现初始化捕获，但是相比lambda表达式，bind方式不够简洁，不建议使用
    auto func_vec = std::bind(                              //C++11模拟初始化捕获, 通过bind方式实现初始化捕获
            [](const std::vector<double>& data) { //译者注：本行高亮 const防止修改，mutable允许修改
            // [](std::vector<double>& data) mutable { // ambda声明为mutable，则闭包类中的operator()将不会声明为const，并且在lambda的形参声明中省略const也是合适的
                for (auto elem : data) {
                    cout << elem << " ";
                }
                cout << endl;
            },
            std::move(data)                     //译者注：本行高亮
        );
    func_vec(data);
    cout << "[test2] after data.size()=" << data.size() << endl;

    PolyWidget pw_1;
    auto boundPW = std::bind(pw_1,  std::placeholders::_1); // placeholders是占位符，表示参数位置，依次类推
    boundPW(1930);              //传int给PolyWidget::operatocr()
    boundPW(nullptr);           //传nullptr给PolyWidget::operator()
    boundPW("Rosebud"); 		//传字面值给PolyWidget::operator()

#if __cplusplus > 201103L
    PolyWidget pw_2;
    auto boundPW_2 = [&pw_2](const auto& param) { 
            if constexpr (std::is_lvalue_reference<decltype(param)>::value) {
                cout << "param is lrvalue" << endl;
            } else if constexpr (std::is_rvalue_reference<decltype(param)>::value) {
                cout << "param is rrvalue" << endl;
            } else {
                cout << "param is value" << endl;
            }
            pw_2(std::forward<decltype(param)>(param));
        };  //C++14 
    int a=123;
    boundPW_2(a); 		//传字面值给PolyWidget::operator()

    // auto ffff = [pw_2](){ // 只能引用，但不能使用传值方法
    auto ffff = [&pw_2](){ // 只能引用，但不能使用传值方法
        int xxx = 1;
        int& xxx1 = xxx;
        pw_2(xxx1);
    };
    ffff();
#endif

    cout << "[test2] end=========================" << endl;
}

template <typename T>
void func_rreference(T&& param) {
    cout << "func_rreference(T&& param) param=" << param << endl;
}
void test3() {
#if __cplusplus > 201103L
    cout << "[test3] starts=======================" << endl;
    auto func = [](auto x, auto y) { return x+y;}; // c++14 auto参数支持
    cout << "func(1,2)=" << func(1,2) << endl;
    cout << "func(1.1,2.2)=" << func(1.1,2.2) << endl;


    auto f = [](auto&& param) {
        return func_rreference(std::forward<decltype(param)>(param)); // auto&&形参时，使用decltype 和 forward 来保持参数的类型
    };
    f(33);
    f("abc");

    cout << "[test3] ends=======================" << endl;
#endif
}

static int s_a = 500; // static成员变量，不能被捕获，但是可以在lambda中使用
int main() {
    // 闭包 enclosure，运行时 创建的对象，依赖捕获模式，闭包持有被捕获数据的副本或者引用。
    // 闭包类，closure class，编译期间产生的 实例化闭包的类，每个lambda表达式都有一个唯一的闭包类类型
    // ，区分什么存在于编译期（lambdas 和闭包类），什么存在于运行时（闭包）以及它们之间的相互关系是重要的。
    // 注意lambda只能捕获non-static的局部变量，包括形参，不能捕获成员变量
    auto l = []{ cout << "i am l, s_a=" << s_a << endl;}; // lambda产生的闭包副本
    l();

    auto m = [](std::string&& s) -> void {
        cout << "i am " << s << endl;
    };
    m("haha");

    auto n = [](std::string&& s) -> int {
        return s.length();
    };
    cout << "len(n) = " << n("abc") << " type(n) typeid(n).name()=" << typeid(n).name()<< endl;

    int a = 3;
    [] (int input) {
        cout << "a=" << input << endl;
    }(a);

    // [=] (int input) {
    //     cout << "before a=" << input << endl;
    //     a = 4;
    //     cout << "after a=" << input << endl;
    // };
    [&] (int input) {
        cout << "before a=" << a << endl;
        a = 4;
        cout << "after a=" << a << endl;
    }(0);
    cout << "final outside a=" << a << endl;

    int x = 0;
    int y = 42;
    auto qqq = [x, &y] () { // 注意，此处捕捉到的x=0，传入的是x的拷贝，后续x修改不会影响此处拷贝，似乎编译期间就确定了
        ++y;
        cout << "qqq() x:" << x << endl;
        cout << "qqq() y:" << y << endl; // 外部跟着改
    };
    x = y = 77;
    cout << "x=" << x << " y=" << y << endl;
    // auto qqq = [x, &y] () {
    //     cout << "x:" << x << endl;
    //     cout << "y:" << y << endl;
    //     ++y;
    // };
    qqq();
    qqq();
    cout << "final y:" << y << endl;

    x = 100;
    y = 200;
    auto f = [=] () mutable {
        x += y; // 内部独有一份，与外部无关, x is not read-only, x not affect outside
        cout << "f() lambda x:" << x << " y:" << y << endl;
    };
    cout << "before x:" << x << " y:" << y << endl;
    f();
    f();
    cout << "after x:" << x << " y:" << y << endl;
    x = 0;
    cout << "before x:" << x << " y:" << y << endl;
    f();
    f();
    cout << "after x:" << x << " y:" << y << endl;

    std::function<int(int,int)> func = [] (int x, int y) {
        return x * y;
    };
    std::function<void()> func1 = [] () {
        cout << "i am func1" << endl;
        return;
    };
    cout << "func(3,4)=" << func(3,4) << endl;

    a = 10;
    auto func2 = [&a] () {
        int a = 0; // 此处a是局部变量，不会影响外部a
        cout << "a=" << a << endl;
    };

    func2();

    auto func3 = [&a] () {
        cout << "a=" << a << endl;
    };
    std::vector<int> numbers(100);
    std::generate(numbers.begin(), numbers.end(), std::rand);
    cout << "origin numbers:" << endl;
    copy(numbers.begin(), numbers.end(), ostream_iterator<int>(cout, ", "));
    cout << endl;

    class f_n {
    public:
        f_n(int i) : n(i) {}
        bool operator()(int x) { return x%n==0; } // 函数符是一个类对象，可以有成员变量
    private:
        int n;
    };
    int count14 = std::count_if(numbers.begin(), numbers.end(), f_n(4));// 好处 lambda不会像函数指针那样阻止内敛，lambda和函数符都可以内联
    cout << "count14=" << count14 << endl;

    int count3 = std::count_if(numbers.begin(), numbers.end(), [](int x)->bool { return x%3==0; }); // 好处 lambda表达式，匿名函数，可以直接传入作用域任何变量
    cout << "count3=" << count3 << endl;

    int count13 = std::count_if(numbers.begin(), numbers.end(), [](int x)->bool { return x%13==0; });
    cout << "count13=" << count13 << endl;

    int count15 = 0;
    std::for_each(numbers.begin(), numbers.end(), [&count15](int x)->bool { return count15 += (x % 14==0); }); // 好处 lambda表达式，匿名函数，可以直接传入作用域任何变量
    cout << "count15=" << count15 << endl;

    test1();// 悬空情况，指针悬空，引用悬空，避免使用默认捕获方式
    test2(); // 初始化捕获，c++14
    test3(); // c++14 lambda参数支持auto
#if __cplusplus > 201103L
    auto func4 = [](auto a, auto b){ return a+b;}; // c++14 auto参数支持
    cout << "func4(1,2)=" << func4(1,2) << endl;

    int x11 = 3;
    auto func5 = [x11=x11](auto b) -> decltype(x11+b) { return x11+b;}; // c++14
    cout << "func5(1)=" << func5(1) << endl;
#endif
}