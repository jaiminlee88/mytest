#include<iostream>
#include<functional>
using namespace std;

template<typename T, typename F>
T use_f(T v, F f) {
    static int count = 0;
    ++count;
    cout << "use_f count=" << count << ", static &count=" << &count << " ";
    return f(v);
}

template<typename T>
T use_fdd(T v, std::function<T(T)> f) {
    static int count = 0;
    ++count;
    cout << "use_f count=" << count << ", static &count=" << &count << " ";
    return f(v);
}


class Fp {
public:
    Fp(double _z = 1.0) : z(_z) {}
    double operator()(double p) { return z*p; }
private:
    double z;
};

class Fq {
public:
    Fq(double _z = 1.0) : z(_z) {}
    double operator()(double p) { return z+p; }
private:
    double z;
};

double dub(double x) { return 2.0*x; }
double square(double x) { return x*x; }

int main(){
    double y = 1.21;
    cout << "use_f(y,dub): " << use_f(y,dub) << endl;
    cout << "use_f(y,square): " << use_f(y,square) << endl;
    cout << "use_f(y,Fq(10)): " << use_f(y,Fq(10)) << endl;
    cout << "use_f(y,Fp(10)): " << use_f(y,Fp(10)) << endl;
    cout << "use_f(y,[](double u){return u*u;}): " << use_f(y,[](double u){return u*u;}) << endl; // 匿名函数))"
    cout << "use_f(y,[](double u){return u+u/2.0;}): " << use_f(y,[](double u){return u+u/2.0;}) << endl; // 匿名函数))"
    cout << "以上use_f共5个实例" << endl;

    // std::function<double(char,int)> fdci; // 函数类型，参数为char,int，返回值为double
    std::function<double(double)> ef1 = dub;
    std::function<double(double)> ef2 = square;
    std::function<double(double)> ef3 = Fq(10);
    std::function<double(double)> ef4 = Fp(10);
    std::function<double(double)> ef5 = [](double u){return u*u;};
    std::function<double(double)> ef6 = [](double u){return u+u/2.0;};
    typedef std::function<double(double)> fdd;
    cout << "use_f(y,ef1): " << use_f(y,ef1) << endl; // dub
    cout << "use_f(y,ef2): " << use_f(y,ef2) << endl; // square
    cout << "use_f(y,ef3): " << use_f(y,ef3) << endl; // Fq
    cout << "use_f(y,ef4): " << use_f(y,ef4) << endl; // Fp
    cout << "use_f(y,ef5): " << use_f(y,ef5) << endl; // lambda
    cout << "use_f(y,std::function<double(double)> ef6 = [](double u){return u+u/2.0;};): " << use_f(y,(std::function<double(double)>) [](double u){return u+u/2.0;}) << endl; // lambda
    cout << "use_f(y,fdd(ef6)): " << use_f(y,fdd(ef6)) << endl; // lambda
    cout << "以上use_f共1个实例,缩小代码规模" << endl;

    cout << "use_fdd(y, ef5): " << use_fdd(y, ef5) << endl; // lambda)"
    cout << "use_fdd(y, ef6): " << use_fdd(y, ef6) << endl; // lambda)"
    cout << "以上use_fdd共2个实例,缩小代码规模" << endl;
    return 0;
}