#include<iostream>

using namespace std;

class action{
public:
    action(int i=0) : a(i) { cout << "action constructor" << endl; }
    virtual ~action() {
        cout << "action destructor" << endl;
    }
    virtual void run(int i) const {
        cout << "action run " << i << endl;
    }
    int val() const { return a; }
    virtual void f(char c) { cout << "action f(char i): a=" << val() << " i:" << c << endl; }
    virtual void f1(double c) final { cout << "action f1(double i): a=" << val() << " i:" << c << endl; } // final标识符, 子类不能重写，编译期间报错，用于禁止子类重写
    // void f3() final { cout << "action f3" << endl; } // 错误，final只能用于virtual
private:
    int a;
};

class action1 : public action {
public:
    action1(int i=0) : action(i) { cout << "action1 constructor" << endl; }
    ~action1() {
        cout << "action1 destructor" << endl;
    }
    // void run() const override { // 添加override标识符，编译器会检查是否有对应的基类函数，如果没有则报错，用于检查参数匹配
    //     cout << "action1 run" << endl;
    // }
    virtual void f(char *i)  { cout << "action1 f(char *i): a=" << val() << " i:" << *i << endl; } // 参数不用于识别重载函数，只用于确定调用哪个函数，并隐藏基类的同名函数
    // virtual void f1(double c) { cout << "action1 f1(double i): a=" << val() << " i:" << c << endl; } // 重写基类的final函数，编译期间会报错
};

void test1() {
    cout << "test1()" << endl;
    class Widget {
    public:
        Widget(){
            cout << "Widget constructor" << endl;
        }
    public:
        // 引用限定符语法
        void doWork() &{
            cout << "Widget::doWork &" << endl;
        }    //只有*this为左值的时候才能被调用
        void doWork() && {
            cout << "Widget::doWork &&" << endl;
        }   //只有*this为右值的时候才能被调用
    };

    Widget w;               //普通对象（左值）
    w.doWork();             //调用被左值引用限定修饰的Widget::doWork版本
                            //（即Widget::doWork &）
    Widget().doWork();      //调用被右值引用限定修饰的Widget::doWork版本
                            //（即Widget::doWork &&）

    class Base {
    public:
        virtual void mf1() const {
            cout << "Base::mf1" << endl;
        }
        virtual void mf2(int x) {
            cout << "Base::mf2" << endl;
        }
        virtual void mf3() & {
            cout << "Base::mf3" << endl;
        }
        void mf4() const {
            cout << "Base::mf4" << endl;
        }
    };

    class Derived: public Base {
    public:
        virtual void mf1() { // 检测不到，因为参数不同，不是override
            cout << "Derived::mf1" << endl;
        }
        // virtual void mf1() override { // override使得编译器检查是否有对应的基类函数，如果没有则报错
        //     cout << "Derived::mf1" << endl;
        // }
        virtual void mf2(unsigned int x) {
            cout << "Derived::mf2" << endl;
        }
        virtual void mf3() && {
            cout << "Derived::mf3 &&" << endl;
        }
        virtual void mf3() & {
            cout << "Derived::mf3 &" << endl;
        }
        void mf4() const {
            cout << "Derived::mf4" << endl;
        }
    };

    Derived d;
    d.mf1(); // Derived::mf1
    d.mf2(1); // Derived::mf2
    d.mf3(); // Derived::mf3
    d.mf4(); // Derived::mf4
}

int main() {
    action1 a1(10);
    // a1.f('@');
    // a1::action.f('@');
    char c = '#';
    // a1.f(c); // invalid 基类的f(char)被隐藏了，所以这里调用的是子类的f(char*)
    a1.f(&c); // valid, 子类的f(char*)，隐藏了基类的f(char)，但是不影响子类的f(char*)

    test1();
}