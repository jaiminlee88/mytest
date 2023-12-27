#include "test_derivative.hh"

son::son(string sname, string fname) : son_name(sname), father(fname) {
    cout << "i am son's construct, son_name=" << sname << " father_name=" << fname << endl;
}

int main() {
    son son1{"jack", "abc"};
    father father1(son1); // 没有实现，但会默认生成一个隐式赋值构造函数 father(const facher&)
    father1.func2();
    cout << "father1.name=" << father1.get_name() << endl;
    father father2 = son1;
    father2.func2();
    cout << "father2.name=" << father2.get_name() << endl;

    son1.func2();
    ((father)son1).func2(); // valid, 但是不建议这样用，因为会有歧义，不知道是father的func1还是son的func1

    son1.func3();
    ((father)son1).func3(); // valid, father's func3

    father &f = son1;
    f.func2(); // valid
    f.func3(); // valid, son's virtural func3, not father's
    // ((son)f).func3(); // invalid

    father *pf = &son1;
    pf->func2(); // valid
    pf->func3(); // valid, son's virtural func3, not father's
    ((father*)(&son1))->func3(); // valid, son's virtural func3, not father's

    // son1.func4(); // invalid, 因为是虚函数，子类重新定义了一个int参数版本的func4，不会生成重载版本，而是会直接隐藏积累的func4，所以这里调用的是son的func4，但是没有参数，所以报错
    son1.func4(1); // valid, son's func4(int i)
    son1.func5(2); // valid, return reference of son
    pf->func5(3); // valid, son's func5, not fathers

    // cout << son1.gender << endl; // invalid, protected member
    cout << son1.get_father_gender() << endl; // valid

    // son1.func6(3); // invalid, father's func6(int) is hidden by son's func6(),除非子类也重新定义了func6(int)
    son1.func6();

    cout << "son1 is human : " << son1.is_human() << endl;
    print_name(father1);

    cout << "=========" << son1 << endl;
    daughter dau1("lucy", "jack", "alice");
    dau1.father::func2(); // 通过指定来澄清意图
    dau1.mother::func2();
    return 0;
}