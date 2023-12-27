#include <iostream>
#include <string.h>

using namespace std;

class creature {
public:
    creature(){ cout << "creature construct" << endl;  }
    virtual bool is_human() const = 0; // 必须实现
};

class father : virtual public creature{
public:
    father() { cout<< "i am father's construnct" << endl; } // 坚决不能是虚函数，因为必须调用，而且虚函数表是在构造函数之后才会生成的
    father(string _name) : name(_name) { cout<< "i am father's construnct, name=" << name << endl; }
    ~father() { cout<< "i am father's virtual destruct" << endl; } // 应该hi虚函数，确保都是从子类开始析构，即使不继承，也应该定义为虚函数，因为可能会有子类继承
    virtual bool is_human() const { return true; }
    friend void print_name(father& f); // 友元函数，可以访问类的私有成员,  但不能是虚函数 !!!!，因为本身不是类成员，需要通过引用访问
    void func2 (){ cout<< "i am father's func2"  << endl; } // 静态联编，没有设置为虚函数，则按照指针类型调用，如果是父类指针，则调用父类函数，如果是子类指针，则调用子类函数
    virtual void func3(){ cout<< "i am father's virtual func3"  << endl;} // 虚函数，动态联编，效率低些，该虚函数才虚函数
    // 虚函数表再内存的readonly区，常量
    // 每个类对象都有一个虚函数表指针，指向虚函数表，虚函数表中存储的是虚函数的地址
    // 子类会继承基类所有虚函数，如果子类重写了基类的虚函数，则会在子类的虚函数表里，直接覆盖基类的虚函数地址，如果子类没有重写基类的虚函数，则会保留基类的虚函数（子类虚函数表）
    // 如果子类新定义了虚函数，则会在子类虚函数表里，添加新的虚函数地址，但是不改变基类虚函数表，因为基类不知道子类的虚函数
    // 虚函数成本包括，虚函数地址占用，虚函数表占用，寻址成本（虚函数表是一个数组，需要寻址，顺序是定义的顺序）
    virtual void func4() { cout<< "i am father's virtual func4"  << endl;}
    virtual father& func5(int i) { cout<< "i am father's virtual func5, i=" << i << endl; return *this;}

    virtual void func6() { cout << "i am father's virtual func6" << endl; }
    virtual void func6(int) { cout << "i am father's virtual func6(int)" << endl; } // 虚函数重载，overloading

    friend ostream& operator<<(ostream& os, father& f);

    string get_name() { return name;}
private:
    void func1 (){ cout<< "i am father's func1"  << endl; }
    string name{""};
protected:
    string gender{"male"};
};

ostream& operator<<(ostream& os, father& f) {
    return os << "father name=" << f.name << " ";
}


void print_name(father& f) { cout << "[print_name] father's name=" << f.name << endl; }

class mother : virtual public creature{
public:
    mother() { cout<< "i am mother's construnct" << endl; }
    mother(string _name) : name(_name) { cout<< "i am mother's construnct, name=" << name << endl; }
    virtual bool is_human() const { return true; }
    void func2 (){ cout<< "i am mother's func2"  << endl; }
    string get_name() { return name;}

private: //protected性质只有在子类中才能表现出来，不能在类外部调用
    void func1 (){ cout<< "i am mother's func1"  << endl; }
    string name{""};
};

class son : public father { // 继承方式承上启下，son角度，对上只能用public和protect的，对下public（对孙开放父类函数）、protect（对孙关闭父类函数）、private（对孙关闭父类函数）
public:
    son() : father() { cout << "i am son's construct" << endl; } // 先基类，在本体构造函数
    // son(string sname, string fname): son_name(sname), father(fname) ; //invalid, 初始化列表不应该放在这里（需要接{}）
    son(string sname, string fname); // 正确的函数声明,初始化列表在实现中出现
    virtual ~son(){ cout << "i am son's virtual destruct" << endl; } // 确保无论是父类还是子类调用，都会调用子类虚析构函数，正确释放资源，否则如果是父类引用则只调用了父类析构函数，
    // void sfunc1(){ func1(); } // invalid, father's private func under public inheritage
    void sfunc2() { func2(); } // valid
    void func2() { cout << "i am son's func2" << endl; } // valid
    virtual void func3() { cout << "i am son's virtual func3" << endl; } // valid，重新定义了虚函数，会在子类虚函数表里，直接覆盖基类的虚函数地址，但不会改变基类的地址
    // 重载和隐藏都是编译期实现的，当发现子类虚函数与基类虚函数名称相同但参数不同，会认为这两者没有任何关联，不会重载，而对基类虚函数进行了隐藏function hiding
    virtual void func4(int i) { cout << "i am son's virtual func4(int i) i=" << i << endl; } // 基类是func4(),子类是func4(int)，该虚函数不会重载func4，而是直接隐藏func4，不管参数特征标如何
    // 返回类型协变，covariance of return type，基类虚函数返回基类指针或引用，子类则可以返回基类指针或引用
    virtual son& func5(int i) { cout<< "i am son's virtual func5, i=" << i << endl; return *this;} // 子类重写了基类的虚函数，返回值可以不同(必须是指针或引用)，但是参数特征标必须相同，否则会隐藏基类的虚函数

    virtual void func6() { cout << "i am son's virtual func6" << endl; } // 虚函数重写，override，基类func6(int)被隐藏，除非func6(int)被重新定义如下
    // virtual void func6(int) { cout << "i am son's virtual func6(int)" << endl; } // 虚函数重载，overloading

    friend ostream& operator<<(ostream& os, son& f); // 需要访问父类私有成员，就调用父类的friend函数
    string get_father_gender() { return gender; }
private:
    string son_name{""};
};

ostream& operator<<(ostream& os, son& f) {
    os << (father &)f; // 通过这种方式访问父类私有成员
    os << "son son_name=" << f.son_name << " ";
    return os;
}

class daughter : public father, public mother {
public:
    daughter(string sonname, string fname, string mname) : creature(), father(fname), mother(mname), name(sonname) { // 直接跳过father和mother，必须显示调用creature构造函数（只有虚基类可以），creature是虚基类，信息自动传递失效
        cout << "daughter construct, name=" << sonname << endl;
    }
    ~daughter() { cout << "~daughter destruct." << endl; }
    virtual bool is_human() const { return true; }
private:
    string name{""};
};