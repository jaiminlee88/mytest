#include <iostream>
using namespace std;
 
class A
{
private:
    int a;
protected:
    int b;
public:
    int c;
    void show()    {		cout << "a = " << a << "b = " << b << "c = " << c <<endl;    }

    int set(int a,int b,int c)
    {
	this->a = a;
	this->b = b;
	this->c = c;
    }

    int get_a() {        return a;    }
    int get_b() {         return b;     }
};

class B : private A{
private:
    int x;
protected:
    int y;
public:
    int z;

    void show()    {
	cout << "a = " << get_a() << "," << "b = " << b << "," << "c = " << c << "," << "x = " << x << "," << "y = " << y << "," << "z = " << z <<endl;
    }

    int set(int a,int _b,int c,int x,int y,int z) {
	    A::set(a,_b,c);
	    this->x = x;
	    this->y = y;
	    this->z = z;
    }
 
	int get_a() { 
        // return a; // invalid
        return A::get_a();
    }

	int get_b() {         return b;    }
	int get_c() {         return c;    }
    int get_x(){         return x;    }
    int get_y() {        return y;    }
};

class C {
public:
    C(){
        cout << "base c construct." << endl;
    }
    ~C(){
        cout << "base c release." << endl;
    }
    void show(){
        cout << "this is c." << endl;
    }
    friend void friend_show(C& c);

    void show3() { cout << "this is show3" << endl; }
    void show4() { cout << "this is show4" << endl; }
private:
    void show1() {
        cout << "this base c.show1" << endl;
    }
    // virtual void attach() = 0;
};

class D : private C{
public:
    D(){
        cout << "derivative d construct." << endl;
    }
    ~D(){
        cout << "derivative d release." << endl;
    }
    C::show4; // 老式方法声明，目前也是支持，未来可能摒弃
    using C::show3; // 允许外部私有继承访问，必须有，且没有括号，只适用于继承，不适用于包含

    void show(){
        cout << "this is d." << endl;
    }
};

void friend_show(C& c) {
    cout << "friend_show" << endl;
    c.show1();
}

int main(){
    B b;
    b.set(10,20,30,40,50,60);
    b.show();
    cout << "a = " << b.get_a() <<endl;
    cout << "b = " << b.get_b() <<endl;
    cout << "c = " << b.get_c() <<endl;
    cout << "x = " << b.get_x() <<endl;
    cout << "y = " << b.get_y() <<endl;
    cout << "z = " << b.z <<endl;

    D temp;
    temp.show();
    auto temp1 = ((const C &)temp);
    temp1.show();
    (( C &)temp).show();

    friend_show(*((C*)&temp)); // 子类不能访问父类私有函数，友元间接访问

    temp.show3(); // 私有继承函数访问
    temp.show4();
    return 0;
}