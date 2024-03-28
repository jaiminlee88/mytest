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


// 这个函数的实现通常会使用底层的原子操作指令，比如使用内存屏障（memory barrier）来确保读取操作是原子的。具体实现可能会根据不同的架构和编译器有所不同。
// __always_inline 是一个内核宏，表示告诉编译器这个函数应该被内联，即使-O0编译器也会尝试内联这个函数。
static __always_inline void __read_once_size(volatile void *p, void *res, int size) {
    switch (size) {
    case 1: *(uint8_t  *)res = *(volatile uint8_t *)p; break;
    case 2: *(uint16_t  *)res = *(volatile uint16_t *)p; break;
    case 4: *(uint32_t  *)res = *(volatile uint32_t *)p; break;
#ifdef CONFIG_64BIT
    case 8: *(u64 *)res = *(volatile u64 *)p; break;
#endif
    }
}

/*
先定义一个临时变量，然后通过__read_once_size函数将p指向的内存拷贝到__val中，并且确保这个读取操作是原子的，最后返回__val。
使用了GNU C "Statement Expression"（语句表达式）这个语法允许在一个宏中执行多条语句，并将最后一条语句的结果作为整个表达式的结果返回。
({...}) 语法是GNU C的语法，用于定义一个语句块，可以在其中定义多条语句，并且最后一条语句的结果作为整个表达式的结果返回。
*/
#define READ_ONCE(p) \
          ({ decltype(p) __val; \
            __read_once_size(&p, &__val, sizeof(__val)); \
            __val; })

int data = 0;

int main() {
    auto reader = [](){
        while (true) {
            // int val = data; // 因为编译器不知道data是否会被其他线程修改，所以可能会把该语句放到循环外面，导致出错
            int val = READ_ONCE(data); // READ_ONCE 防止被优化重排，确保读取的是最新的值，且是原子的操作
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


