#include<iostream>
#include<vector>
using namespace std;
// #pragma comment(linker,"/entry:entrance_func")

class Widget { 
public:  
    Widget() {
        cout << "Widget()" << endl;
    }      //默认构造函数
    Widget(int i, bool b){
        cout << "Widget(int i, bool b)" << " i=" << i << " b=" << b << endl;
    }      //同上
    Widget(int i, double d){
        cout << "Widget(int i, double d)" << " i=" << i << " d=" << d << endl;
    }      //同上
    Widget(std::initializer_list<long double> il) {
        cout << "Widget(std::initializer_list<long double> il) il.size()=" << il.size() << " ";
        for (auto i : il) {
            cout << i << " ";
        }
        cout << endl;
    }      //新添加的
    // Widget(std::initializer_list<bool> il) { // invalid,{}不允许变窄转换
    //     cout << "Widget(std::initializer_list<bool> il) il.size()=" << il.size() << " ";
    //     for (auto i : il) {
    //         cout << i << " ";
    //     }
    //     cout << endl;
    // }      //新添加的
    // Widget(std::initializer_list<int> il) {      //std::initializer_list构造函数
    //     cout << "Widget(std::initializer_list<int> il) il.size()=" << il.size() << " ";
    //     for (auto i : il) {
    //         cout << i << " ";
    //     }
    //     cout << endl;
    // }      //新添加的

    Widget(Widget&& rhs) { // 不写也会自动生成
        cout << "Widget(const Widget&& rhs)" << endl;
    }
    Widget(std::initializer_list<Widget>&& il) {
        cout << "Widget(std::initializer_list<long double> il) il.size()=" << il.size() << " ";
        cout << endl;
    }      //新添加的
};

int main(int argc, char* argv[]) {
    long long a = 10;
    int b = a;
    cout << "b=" << b << endl;
    int c(a);
    cout << "c=" << c << endl;
    int d{a}; // 单纯警告，不会报错
    cout << "d=" << d << endl;

    Widget w1(10, true);    //使用圆括号初始化，同之前一样
                        //调用第一个构造函数
    Widget w2{10, true};    //使用花括号初始化，但是现在
                            //调用带std::initializer_list的构造函数
                            //(10 和 true 转化为long double)

    Widget w3(10, 5.0);     //使用圆括号初始化，同之前一样
                            //调用第二个构造函数 

    Widget w4{10, 5.0};     //使用花括号初始化，但是现在
                            //调用带std::initializer_list的构造函数
                            //(10 和 5.0 转化为long double)
                            // 如果没有std::initializer_list的构造函数，调用Widget(int i, double d)

    Widget w5(std::move(w4));
    Widget w6{std::move(w4)}; ///调用带std::initializer_list的构造函数
    cout << "Widget w7--->" << endl;
    Widget w7(); // 这个是函数声明，不是构造函数
    cout << "Widget w8--->" << endl;
    Widget w8{}; //也调用默认构造函数
    cout << "Widget w9--->" << endl;
    Widget w9{{}}; 
    // cout << "Widget w10--->" << endl;
    // Widget w10({});  // 候选太多
    return 0;
}


