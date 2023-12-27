#include<iostream>
#include<functional>
#include<vector>
#include<math.h>
#include<algorithm>
#include<iterator>
using namespace std;

int main() {
    auto l = []{ cout << "i am l" << endl;};
    l();

    auto m = [](std::string&& s) -> void {
        cout << "i am " << s << endl;
    };
    m("haha");

    auto n = [](std::string&& s) -> int {
        return s.length();
    };
    cout << "len(n) = " << n("abc") << " type(n)" << typeid(n).name()<< endl;

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
    cout << "final a=" << a << endl;

    int x = 0;
    int y = 42;
    auto qqq = [x, &y] () { // 注意，此处捕捉到的x=0，传入的是x的拷贝，后续x修改不会影响此处拷贝，似乎编译期间就确定了
        ++y;
        cout << "x:" << x << endl;
        cout << "y:" << y << endl; // 外部跟着改
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
        cout << "lambda x:" << x << " y:" << y << endl;
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
}