#include <iostream>
#include <atomic>
#include <thread>
#include <stdio.h>

using namespace std;

#define ACCESS_ONCE(x) (*(volatile decltype(x) *)&(x))
// 换句话说，它通过暂时将相关变量转换为volatile类型来工作。
// ACCESS_ONCE通常用于确保对变量的访问是原子的，避免编译器优化或指令重排等问题。
// ACCESS_ONCE宏通常在内核编程或者对硬件进行访问时使用，普通应用程序开发中很少会用到。
// ACCESS_ONCE宏应该谨慎使用，因为它会绕过编译器的优化，可能会影响代码的性能和可读性。

int data = 0;

int main() {
    auto reader = [](){
        while (true) {
            // int val = data; // 因为编译器不知道data是否会被其他线程修改，所以可能会把该语句放到循环外面，导致出错
            int val = ACCESS_ONCE(data); // ACCESS_ONCE防止被优化重排，确保读取的是最新的值
            cout << "read: " << val << endl;
            if (val >= 100000) {
                break;
            }
        }
    };

    auto writer = [](){
        for (int i = 0; i < 100000; ++i) {
            ACCESS_ONCE(data) += 1;
        }
    };

    std::thread t1(reader);
    std::thread t2(writer);
    t1.join();
    t2.join();
    return 0;
}


