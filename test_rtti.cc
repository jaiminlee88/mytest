// rttil.cpp--usinq the RTTI dynamic cast operator
#include <iostream>
#include <cstdlib>
#include <ctime>
using std::cout;
using std::endl;

class Grand {
private:
    int hold;
public:
    Grand(int h=0) :hold(h) {}
    virtual void Speak() const { cout <<"I am a grand class!\n";}
    virtual int Value() const { return hold;}

    void fun()const//此时this指针相当于const Grand* const this
	{
		//this->val1 = 10;//错误
		const_cast<Grand*>(this)->hold = 1000;//OK
	}
    int get_hold () {return hold; }
};

class Superb : public Grand {
public:
    Superb(int h = 0) :Grand(h) {}
    void Speak() const { cout <<"I am a superb class!!\n";}
    virtual void Say() const {cout << "I hold the superb value of "<< Value() <<"!\n";}
};

class Magnificent : public Superb {
private:
    char ch;
public:
    Magnificent(int h= 0,char c='A') : Superb(h),ch(c) {}
    void Speak() const { cout << "I am a magnificent class!!!\n";}
    void Say() const {cout << "I hold the character " << ch <<"and the integer" << Value() <<"! \n";}

};

Grand * GetOne() {
    // generate one of three kinds of objects randomly
    Grand *p;
    switch( std::rand() % 3) {
    case 0:p= new Grand(std::rand() % 100); break;
    case 1: p = new Superb(std::rand() % 100); break;
    case 2: p = new Magnificent(std::rand() % 100, 'A' + std::rand() % 26); break;
    }
    return p;
}

Grand * GetOne();

class Base {
public:
    Base(){}
    ~Base(){}
    virtual void fun() { cout << "this is Base fun" << endl;}
	int b_val;
};
class Son : public Base {
public:
    Son(){}
    ~Son(){}
    virtual void fun() {
        cout << "this is son fun" << endl;
        cout << "use base virtual fun by Base::fun()--->";
        Base::fun();
    }

    int s_val;
};

void test_static_cast();
void test_const_cast();
void test_reinterpret_cast();

int main() {
    std::srand(std::time(0));
    Grand*pg;
    Superb *ps;
    for (int i=0; i<5; i++) {
        pg = GetOne();
        pg->Speak();
        cout << "--->| typeid(*pg).name=" << typeid(*pg).name() << std::endl;
        if(ps = dynamic_cast<Superb *>(pg)) {
            ps->Say();
        }
        // static_cast
        // const_cast
        // reinterpret_cast
    }

    Base* bptr1 = new Base();
	Son* sptr1 = new Son();
	Base* bptr;
	Son* sptr;
    sptr1->fun();
    bptr = dynamic_cast<Base*>(sptr1); // 强制转换没有改变多态
    bptr->fun(); // 仍然使用派生类虚函数
    // sptr1.Base::fun(); // invalid,不能外部直接调用虚函数，只能子类虚函数中再调用父类虚函数

    test_const_cast();
    test_static_cast();
    test_reinterpret_cast();
    return 0;
}

void test_reinterpret_cast() {
    cout << "======test reinterpret_cast ============" <<endl;
    // 这个和C语言的强制转换没什么区别，只不过C++用自己的写法替代了C语言的强制转换而已。
    // ①不同类型的指针之间的转换
    // ②指针和能容纳指针的整数类型之间的转换（比如将int类型强转成int*类型）
    // ③不同类型的引用之间的转换
    // 编译期处理执行的是逐字节复制的操作。
    // 类似于强制转换，至于强制转换会产生什么后果需要自己承担。
    // 由于和C语言的强制转换一样，这里不进行赘述。
    int a = 8;
    long long &b = reinterpret_cast<long long&>(a);
    char &c = reinterpret_cast<char&>(a);
    cout << a << " " << b << " " << (char)c << " |"<< endl;
    cout << "sizeof(int)=" << sizeof(int) << endl;
}

void test_const_cast() {
    cout << "======test const_cast ============" <<endl;
    // const int a = 1; // 出现const值，以下操作不能改变a,*const_cast<int*>(p) = 50;实际上是将50赋值给了一个临时变量，并非修改了a的值
    int a = 1;
    const int* p = &a;
	//int* d = p;//错误原因：const int*类型不能赋值或者初始化int*类型的实体
    cout << "p=" << p << " &a=" << &a << endl;
	*const_cast<int*>(p) = 50;
	cout << "*p=" << *p << endl;//50
    cout << "a=" << a << endl;
	int* d = const_cast<int*>(p);
	*d = 100;
	cout << "*p=" << *p << endl;//100ena 
    cout << "a=" << a << endl;

    int a1 = 10;
	const int& b1 = a1;
	//b1 = 20;//错误原因：常量引用，不允许修改值
	//int& c1 = b1;//错误，和常量指针不允许给普通指针赋值或者初始化一样
	int& c1 = const_cast<int&>(b1);
	c1 = 20;
	cout << "a1=" << a1 << endl;//20
	const_cast<int&>(b1) = 30;
	cout << "a1=" <<a1 << endl;//30

    Grand *pg = GetOne();
    cout << "before pg->get_hold()=" << pg->get_hold() << endl;
    pg->fun();
    cout << "after pg->get_hold()=" << pg->get_hold() << endl; // 去掉this的const限制

}

void test_static_cast() {
    cout << "======test static_cast ============" <<endl;
    char c_a = 0;
	int i_a = 0;
	float f_a = 0;
	double d_a = 1.111111;
	void* v_ptr = NULL;
	int* i_ptr = new int(1);
	char* c_ptr = new char(1);
	//下面部分没有报错，可以运行，但是平时不允许这样写，除非自己很明确自己在干什么
	//从高字节数到低字节数的转换平常肯定是不允许这样用的，因为将一个多字节的内容转换到少字节，非常容易丢失数据
	char c_sc = static_cast<char>(i_a);
	c_sc = static_cast<char>(f_a);
	c_sc = static_cast<char>(d_a);
	//类似于下面的转换不允许，因为两个不同的指针类型之间不允许相互转换
	//int* i_scptr = static_cast<int*>(c_ptr);//报错
	//下面的指针类型转换允许
	int* i_scptr = static_cast<int*>(v_ptr);
	void* v_scptr = static_cast<void*>(i_ptr);
	//下面的可取，只不过有时候精度可能会降低而已，比如float转换为int，被视为低风险
	float f_sc = static_cast<float>(i_a);
	int i_sc = static_cast<int>(c_a);
	cout << i_sc << endl;
	//父类指针和派生类指针的指向问题
	Base* bptr1 = new Base();
	Son* sptr1 = new Son();
	Base* bptr;
	Son* sptr;
	bptr = new Son();    //语句1   正确，基类指针指向派生类实体
	//sptr = new Base();//语句2   错误，派生类指针指向父类
	bptr = static_cast<Base*>(sptr1);//等同于语句1，正确
	sptr = static_cast<Son*>(bptr1); //等同于语句2，但是不安全，平时使用也不会使派生类指针指向基类,会出现访问越界，有时候会崩溃，有时候我们却没办法发现
	//对于派生类指针指向基类，会用另一个强制转换dynamic_cast
}