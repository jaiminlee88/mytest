// #include <iostream>
// using namespace std;
// constexpr int display(int x);


// int main()
// {
//     constexpr int num = 1 + 2 + 3;
//     int url[num] = {1,2,3,4,5,6};
//     cout<< url[1] << endl;

//     int num1 = 1 + 2 + 3;
//     int url1[num1] = {2,3,4,5,6,7};
//     cout<< url1[1] << endl;

//     cout << display(3) << endl;
//     return 0;
// }

// constexpr int display(int x) {
//     // int ret = 1 + 2 + x; // error
//     // int ret = 1 + 2 + 3; // error
//     // return ret;
//     return 1+2+x;
// }



// #include <iostream>
// using namespace std;
// int num = 3;
// constexpr int display(int x){
//     return num + x;
// }
// int main()
// {
//     //调用常量表达式函数
//     int a[display(3)] = { 1,2,3,4 };
//     return 0;
// }



// #include <iostream>
// using namespace std;
// //自定义类型的定义
// constexpr struct myType {
//     const char* name;
//     int age;
//     //其它结构体成员
// };
// int main()
// {
//     constexpr struct myType mt { "zhangsan", 10 };
//     cout << mt.name << " " << mt.age << endl;
//     return 0;
// }


#include <iostream>
using namespace std;
//自定义类型的定义
struct myType {
    constexpr myType(char *name,int age):name(name),age(age){};
    const char* name;
    int age;
    //其它结构体成员
};

constexpr int display(int x){
    return 1+2+x;
}

class Point {
public:
    constexpr Point(double xVal = 0, double yVal = 0) noexcept
    : x(xVal), y(yVal)
    {}

    constexpr double xValue() const noexcept { return x; } 
    constexpr double yValue() const noexcept { return y; }

#if __plusplus > 201103L
    // c++14
    constexpr setX(double newX) noexcept { x = newX; }
    constexpr setY(double newY) noexcept { y = newY; }
#else
    // c++11
    void setX(double newX) noexcept { x = newX; }
    void setY(double newY) noexcept { y = newY; }
#endif

private:
    double x, y;
};

constexpr Point midpoint(const Point& p1, const Point& p2) noexcept {
    return { (p1.xValue() + p2.xValue()) / 2,   //调用constexpr
             (p1.yValue() + p2.yValue()) / 2 }; //成员函数
}

int main() {
    // constexpr对象是const，它被在编译期可知的值初始化
    // 当传递编译期可知的值时，constexpr函数可以产出编译期可知的结果
    // constexpr对象和函数可以使用的范围比non-constexpr对象和函数要大
    // constexpr是对象和函数接口的一部分
    
    // 尽可能使用 constexpr，在编译阶段尽可能完成计算，减少运行时的计算量。不仅仅是常量，还是编译期可知的
    // 所有constexpr对象都是const，但不是所有const对象都是constexpr
    constexpr struct myType mt { "zhangsan", 10 };
    cout << mt.name << " " << mt.age << endl;
    cout << display(3) << endl; // 编译器决议，编译期已经算好了，运行时不需要再算了

    int sz; // sz不是常量，也没有初始化
    // constexpr auto arrsize = sz; // error: sz的值直到运行时才能确定
    const auto arrsize = sz; // ok: arrsize是一个常量，可以用于数组的维度，单纯复制

    constexpr Point p1(9.4, 27.7);  //没问题，constexpr构造函数
                                //会在编译期“运行”
    cout << "p1.xValue()=" << p1.xValue() << " p1.yValue()=" << p1.yValue() << endl;
    constexpr Point p2(28.8, 5.3);  //也没问题
    cout << "p2.xValue()=" << p2.xValue() << " p2.yValue()=" << p2.yValue() << endl;
    constexpr auto mid = midpoint(p1, p2);      //使用constexpr函数的结果
                                            //初始化constexpr对象
    cout << "mid.xValue()=" << mid.xValue() << " mid.yValue()=" << mid.yValue() << endl;
    return 0;
}