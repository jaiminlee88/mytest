#include <iostream>
#include <string.h>
#include <tuple>
using namespace std;

// 枚举类
enum class Person {
    man,
    woman,
    boy,
    girl
};

enum Pet: char { // 指定底层类型
    dog ,
    man,
    cat,
    frog
};

enum Fish: char {
    shark = 's',
    goldfish = 'g'
};



template<typename T>
constexpr typename std::underlying_type<T>::type toUType(T enumerator) noexcept {
    return static_cast<typename std::underlying_type<T>::type>(enumerator);
}
// template<typename E>                //C++14
// constexpr std::underlying_type_t<E>
//     toUType(E enumerator) noexcept
// {
//     return static_cast<std::underlying_type_t<E>>(enumerator);
// }
// template<typename E>                //C++14
// constexpr auto
//     toUType(E enumerator) noexcept
// {
//     return static_cast<std::underlying_type_t<E>>(enumerator);
// }

void test1(){
    using UserInfo =                //类型别名，参见Item9
    std::tuple<std::string,     //名字
               std::string,     //email地址
               std::size_t> ;   //声望

    UserInfo uInfo{"jack", "jack@gmail.com", 10};
    std::cout << "std::get<0>(uInfo)=" << std::get<0>(uInfo) << std::endl;
    std::cout << "std::get<1>(uInfo)=" << std::get<1>(uInfo) << std::endl;
    std::cout << "std::get<2>(uInfo)=" << std::get<2>(uInfo) << std::endl;
    enum class UserInfoFields {Name, Email, Reputation};
    std::cout << "std::get<static_cast<std::size_t>(UserInfoFields::Name)>(uInfo)=" << std::get<static_cast<std::size_t>(UserInfoFields::Name)>(uInfo) << std::endl;
    std::cout << "std::get<toUType<UserInfoFields::Email>>(uInfo)=" << std::get<toUType(UserInfoFields::Email)>(uInfo) << std::endl;
}
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
    enum egg2 { // 一旦修改可能就需要全部编译
        small,
        medium,
        large
    };
    enum class egg3 { // 可以避免冲突 ， 一旦修改，但是使用地方没有用到，就不需要全部编译
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

    egg2 egg2_1 = small; // valid，可以不带前缀，未限域enum中的枚举名会隐式转换为整型（现在，也可以转换为浮点类型）
    // egg3 egg3_1 = small; // invalid
    egg3 egg3_1 = egg3::small; // valid，enum class必须带前缀
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
    test1();
    return 0;
}