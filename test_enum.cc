#include <iostream>
#include <string.h>

using namespace std;


enum class Person {
    man,
    woman,
    boy,
    girl
};

enum Pet: char {
    dog ,
    man,
    cat,
    frog
};

enum Fish: char {
    shark = 's',
    goldfish = 'g'
};

int main() {
    std::cout << "sizeof Person is " << sizeof(Person) << std::endl;
    Person person1{Person::boy}; // enum class枚举类型必须加作用域Person，着这在前面加上 using enum Person 或者 using Person::boy。
    // std::cout << "person1:" << person1 << std::endl;
    std::cout << "person1 int:" << (int)person1 << std::endl; // 必须做显式的强制转换
    std::cout << "person1 size:" << sizeof(person1) << std::endl;

    std::cout << "sizeof Pet is " << sizeof(Pet) << std::endl;
    Pet pet1{Pet::frog};
    std::cout << "pet1:" << pet1 << std::endl;// 不做强制转换，则默认为int。
    std::cout << "pet1 char:" << (char)(pet1) << std::endl;
    std::cout << "pet1 size:" << sizeof(pet1) << std::endl;

    std::cout << "sizeof Fish is " << sizeof(Fish) << std::endl;
    Fish fish1{Fish::goldfish};
    std::cout << "fish1:"<< fish1 << std::endl;
    std::cout << "fish1 char:" << (char)fish1 << std::endl;
    std::cout << "fish1 size: " << sizeof(fish1) << std::endl;

    char abc[4] = "abc";
    cout << strlen(abc) << endl;

    // cout << "test5================" << endl;
    // enum class egg {
    //     small,
    //     medium,
    //     large
    // };
    // enum egg1 {
    //     small, // redeclaration of 'small'
    //     medium,
    //     large
    // };
    // cout << egg::small << endl; // invalid
    // cout << egg2::small << endl;
    cout << "test6================" << endl;
    enum egg2 {
        small,
        medium,
        large
    };
    enum class egg3 { // 可以避免冲突
        small=1,
        medium,
        large
    };
    enum struct egg4 {
        small=1,
        medium,
        large
    };
    cout << "egg2::small=" << (int)egg2::small << endl; // valid
    cout << "egg3::small=" << (int)egg3::small << endl; // valid

    egg2 egg2_1 = small; // valid
    // egg3 egg3_1 = small; // invalid
    egg3 egg3_1 = egg3::small; // valid
    // egg4 egg4_1 = small; // invalid
    egg4 egg4_1 = egg4::small; // valid

    // int tmp = egg3_1; // invalid 作用域内枚举类型不能隐式转换为整型
    int tmp1 = egg2_1; // 常规枚举可以隐式转换为整型
    int tmp2 = (int)egg3_1; // valid

    // 特殊的语法
    // enum class : short pizza { // invalid
    //     Small,
    //     Medium,
    //     Large
    // };
    // cout << "pizza::small=" << (int)pizza::small << endl; // valid
    return 0;
}