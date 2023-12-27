#include<iostream>
#include<typeinfo>

#define DBG 1
using namespace std;
int main() {
    static long long abc=0; // static或全局.bss段才有体现
    auto& abc_1="123456"; // 实际是"123456"的大小
    auto  abc_2="12345690"; // 实际b是指针
    std::cout << "sizeof(\"123456\")=" << sizeof("123456") << " sizeof(a)=" << sizeof(abc_1) << " sizeof(b)=" << sizeof(abc_2) << std::endl;
    string tmp = "def";

    int rats = 101;
    int &rodents = rats;
    cout << "before rats=" << rats << " address(rats)=" << &rats 
    << " rodents=" << rodents << " address(rodents)" << &rats << endl;
    int bunny = 50;
    rodents = bunny;
    cout << "before bunny=" << bunny << " address(bunny)=" << &bunny 
    << " rodents=" << rodents << " address(rodents)" << &rodents << endl;
    return 0;
}