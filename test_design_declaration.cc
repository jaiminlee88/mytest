#include <iostream>
#include <memory>
#include <functional>
using namespace std;

namespace design_declaration {

struct Year {
public:
    explicit Year(int x) : y(x) {
        // cout << "Year::Year(int x) construct" << endl;
    }
    int y;
};
struct Month {
public:
    static Month Jan() { return Month(1); } // 这样可能更好，接口行为更为一致
    static Month Feb() { return Month(2); }
    static Month Mar() { return Month(3); }
    static Month Apr() { return Month(4); }
    static Month May() { return Month(5); }
public:
    explicit Month(int x) : m(x) {
        // cout << "Month::Month(int x) construct" << endl;
    }
    int m;
};
struct Day {
public:
    explicit Day(int x) : d(x) {
        // cout << "Day::Day(int x) construct" << endl;
    }
    int d;

};
class myData {
public:
    myData(Day d_, Month m_, Year y_) : d(d_.d), m(m_.m), y(y_.y) {
        cout << "myData::myData() construct" << endl;
    }
    void str() {
        printf("myData::str() date=%04d%02d%02d \n",y,m,d );
    }

    void clear_y(){ y = 0; }
    void clear_m(){ m = 0; }
    void clear_d(){ d = 0; }

    void clear() { // 可以这么封装
        clear_y();
        clear_m();
        clear_d();
    }
private:
    int d;
    int m;
    int y;
};

void test1(){
    cout << __func__ << "--------------------" << endl;
    myData date(Day(30), Month::May(), Year(1995)); // 不能隐式转换，必须显示转换，保证接口调用不出错
    date.str();

    // myData date(Month(3), Year(1995), Day(30)); // invalid,
}

void clear_myData(myData& date1) { // 非成员函数，可以这么封装，封装性比直接调用clear更高
                                   // 与myData在同一个namespace内，可以配合namespace使用增加封装性，包裹性，技能扩充性
    date1.clear_y();
    date1.clear_m();
    date1.clear_d();
}

void test2() {
    cout << __func__ << "--------------------" << endl;
    myData date(Day(30), Month::May(), Year(1995)); // 不能隐式转换，必须显示转换，保证接口调用不出错
    date.str();
    date.clear();
    date.str();

    myData date1(Day(30), Month::May(), Year(1995)); // 不能隐式转换，必须显示转换，保证接口调用不出错
    date1.str();
    clear_myData(date1);
    date1.str();
}


class Rational {
public:
    // explicit Rational(int numerator = 0, int denominator = 1) : num(numerator), den(denominator) {
    //     cout << "Rational::Rational() construct" << endl;
    // }
    Rational(int numerator = 0, int denominator = 1) : num(numerator), den(denominator) {
        cout << "Rational::Rational() construct" << endl;
    }
    Rational operator*(const Rational& rhs) {
        cout << "===Rational::operator*()===" << endl;
        return Rational(num * rhs.num, den * rhs.den);
    }
    int numerator() const { return num; }
    int denominator() const { return den; }
    void print() {
        cout << "Rational::print() num = " << num << "/" << den << endl;
    }
private:
    int num;
    int den;
};

const Rational operator*(const Rational& lhs, const Rational& rhs) { // 可以保证所有参数做类型转换
    cout << "===operator*()===" << endl;
    return Rational(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
}
void test3() {
    Rational oneEighth(1, 8);
    Rational oneHalf(1, 2);
    Rational result = oneHalf * oneEighth;
    result.print();
    Rational result1 = oneHalf * 2; // oneHalf.operator*(2), 隐式转换，把2转换为Rational(2,1)，如果使用explcit就会禁止隐式转换
    result1.print();
    Rational result2 = 2 * oneHalf; // 此时调用的是 const Rational operator*(const Rational& lhs, const Rational& rhs)
}




struct WidgetImpl {
public:
    WidgetImpl(int i) : val(i) {}
    int val;
};

class Widget {
public:
    Widget(int i) : pImpl(new WidgetImpl(i)) {}
    Widget& operator=(const Widget& rhs) {
        *pImpl = *(rhs.pImpl);
        return *this;
    }
    ~Widget() {
        if (pImpl != nullptr)
            delete pImpl;
    }
    void swap(Widget& rhs) { // 成员函数方法
        using std::swap;
        std::swap(pImpl, rhs.pImpl);
    }
    std::string str() {
        if (pImpl != nullptr)
            return "Widget::print() val=" + std::to_string(pImpl->val);
        else
            return "Widget::print() val=nullptr";
    }
private:
    WidgetImpl* pImpl{nullptr};
};

template<typename T>
void swap(T& a, T& b) { // non-member方法
    a.swap(b);
}



void test4() {
    cout << "[test4]====start======" << endl;

    cout << "1、swap函数的member实现" << endl;
    Widget w1(5);
    cout << "w1: before " << w1.str() << endl;
    Widget w2(10);
    cout << "w2: before " << w2.str() << endl;
    w1.swap(w2);
    cout << "w1: after " << w1.str() << endl;
    cout << "w2: after " << w2.str() << endl;

    cout << "2、swap函数的non-member实现" << endl;
    Widget w3(5);
    cout << "w3: before " << w3.str() << endl;
    Widget w4(10);
    cout << "w4: before " << w4.str() << endl;
    swap(w3, w4);
    cout << "w3: after " << w3.str() << endl;
    cout << "w4: after " << w4.str() << endl;

    cout << "[test4]====end======" << endl;
}

std::string func5(std::string& str) {
    // std::string str_encrypt; // 不要这里初始化，否则还要构造和析构，浪费资源
    if (str.size() < 5) {
        throw logic_error("string size < 5");
    }
    // std::string str_encrypt = str; // 需要用赋值函数，因为多了一步构造，如果是初始化，就不需要赋值函数了
    std::string str_encrypt(str); //最好这里才初始化，延迟定义变量，这里直接用拷贝构造函数，不需要赋值函数
    return str_encrypt;
}
void test5() {
    std::string str = "12345";
    func5(str);
}

class Point {
public:
    Point(int x_, int y_) : x(x_), y(y_) {}
    void setX(int newVal) { x = newVal; }
    void setY(int newVal) { y = newVal; }
    int getX() const { return x; }
    int getY() const { return y; }
private:
    int x;
    int y;
};
struct RectData {
    Point ulhc; // upper left-hand corner
    Point lrhc; // lower right-hand corner
};
class Rectangle {
private:
    std::shared_ptr<RectData> pData;
public:
    Rectangle(Point& ulhc, Point& lrhc) : pData(new RectData{ulhc, lrhc}) {}
    const Point& upperLeft() const { return pData->ulhc; } // 返回const引用，不能修改值，这样更好
    Point& lowerRight() const { return pData->lrhc; } // 返回引用，可以修改值，这样不好，如果外部取了引用指针，非常危险
};
void test6() {
    cout << "[test6]====start======" << endl;
    Point coord1(0, 0);
    Point coord2(100, 100);
    Rectangle rec(coord1, coord2);
    cout << "upperLeft: [" << rec.upperLeft().getX() << "," << rec.upperLeft().getY() << "]" << endl;
    cout << "lowerRight: [" << rec.lowerRight().getX() << "," << rec.lowerRight().getY() << "]" << endl;
    // rec.upperLeft().setX(50); 
    // rec.upperLeft().setY(50);
    rec.lowerRight().setX(50); // 不合理，尽量不要
    rec.lowerRight().setY(50); // 不合理，尽量不要
    cout << "upperLeft: [" << rec.upperLeft().getX() << "," << rec.upperLeft().getY() << "]" << endl;
    cout << "lowerRight: [" << rec.lowerRight().getX() << "," << rec.lowerRight().getY() << "]" << endl;
    cout << "[test6]====end======" << endl;
}

void test7() {
    /*
    异常安全的标准
    1、基本承诺：如果异常被抛出，程序内的任何事物仍然保持在有效状态下。没有任何对象或数据结构会因为异常而破坏。
    2、强烈保证：如果异常被抛出，程序状态不改变。这通常意味着，如果操作失败，对象保持不变。这也被称为事务性内存模型。
    3、不抛出异常的保证：承诺绝不抛出异常。这通常是最有用的保证，因为它允许调用者在没有备用方案的情况下使用函数。
    std::shared_ptr的构造函数和reset函数都提供了强烈保证，而std::vector的push_back函数提供了基本承诺。
    */
    cout << "[test7]====start======" << endl;
    cout << "[test7]====end======" << endl;
}


class gamecharacter {
public:
    int healthValue() const { // 有点像wrapper,装饰器
        // cout << "gamecharacter::healthValue()" << endl;
        // do some work
        int ret= doHealthValue();
        // do some work
        return ret;
    }
private:
    virtual int doHealthValue() const { // 也可以是个纯虚函数
        // cout << "gamecharacter::doHealthValue()" << endl;   
        return 0;
    }
};
void test8(){
    cout << "[test8]====start======" << endl;
    gamecharacter gc;
    cout << "gc.healthValue()=" << gc.healthValue() << endl;
}


class gamecharacter1;
int defaultHealthCalc(const int& i) {
    // cout << "defaultHealthCalc()" << endl;
    return i;
}
int defaultHealthCalc1(const int& i) {
    // cout << "defaultHealthCalc()" << endl;
    return i*i;
}
class gamecharacter1{
public:
    using healthcalcfunc = int (*)(const int&);
    explicit gamecharacter1(healthcalcfunc hcf = defaultHealthCalc) : healthfunc(hcf) {} // 绑定方法
    int healthValue(int i=0) const {
        // cout << "gamecharacter1::healthValue()" << endl;
        return healthfunc(i);
    }
private:
    healthcalcfunc healthfunc;
};
void test9(){
    cout << "[test9]====start======" << endl;
    gamecharacter1 gc(defaultHealthCalc);
    gamecharacter1 gc1(defaultHealthCalc1);
    cout << "gc.healthValue()=" << gc.healthValue(10) << endl;
    cout << "gc1.healthValue()=" << gc1.healthValue(10) << endl;
}


class gamecharacter2;
class gamecharacter2{
public:
    using healthcalcfunc = std::function<int(const int& i)>;
    explicit gamecharacter2(healthcalcfunc hcf = defaultHealthCalc) : healthfunc(hcf) {} // 绑定方法
    int healthValue(int i=0) const {
        // cout << "gamecharacter2::healthValue()" << endl;
        return healthfunc(i);
    }
private:
    healthcalcfunc healthfunc;
};
void test10(){
    cout << "[test10]====start======" << endl;
    gamecharacter2 gc(defaultHealthCalc);
    gamecharacter2 gc1(defaultHealthCalc1);
    cout << "gc.healthValue()=" << gc.healthValue(11) << endl;
    cout << "gc1.healthValue()=" << gc1.healthValue(11) << endl;
}
}


int main (){
    design_declaration::test1();
    design_declaration::test2();
    design_declaration::test3();
    design_declaration::test4();
    design_declaration::test5(); // 尽可能延迟定义变量
    design_declaration::test6(); // 尽量不要以引用方式返回对象成员
    design_declaration::test7(); // 保证异常安全是第一个位的
    // virtual 替代方案
    design_declaration::test8(); // non-virtual interface设计模式,NVI
    design_declaration::test9(); // strategy设计模式
    design_declaration::test10(); // strategy设计模式,使用std::function
}