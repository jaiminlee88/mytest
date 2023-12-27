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
int main()
{
    constexpr struct myType mt { "zhangsan", 10 };
    cout << mt.name << " " << mt.age << endl;
    return 0;
}