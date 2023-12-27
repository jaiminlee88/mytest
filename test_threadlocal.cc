#include <iostream>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <iterator>
#include <map>
#include <queue>

#include <chrono>
#include <assert.h>
#include <new>
#include <functional>
#include <type_traits>

#include <atomic>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <future>
#include <unistd.h>
#include <numeric>
#include <unistd.h>
#include <algorithm>
#include <climits>
#include <future>
#include <math.h>
#include <exception>
#include <atomic>

std::mutex mlock;

// 使用 static thread_local 声明一个线程本地存储的变量
static thread_local std::thread::id g_id; // 多个线程，多个副本
// 打印线程本地存储的变量值
void print_thread_id() {
    std::lock_guard<std::mutex> m(mlock);
    std::cout << "Thread ID: " << g_id << std::endl;
}
// 线程函数
void thread_func() {
    g_id = std::this_thread::get_id(); // 设置线程本地存储的变量值
    print_thread_id(); // 打印线程本地存储的变量值
}


class MyClass {
public:
    static thread_local std::thread::id thread_id; // 声明一个静态的线程本地存储变量，但是static必须类外初始化
public:
    void print_thread_id() {
        std::lock_guard<std::mutex> m(mlock);
        std::cout << "Thread ID: " << thread_id << std::endl;
    }

    void thread_func() {
        thread_id = std::this_thread::get_id(); // 设置线程本地存储的变量值
        print_thread_id(); // 打印线程本地存储的变量值
    }
};

thread_local std::thread::id MyClass::thread_id{0};  // 类外定义并初始化

int main() {
    MyClass obj;
    // initialize_thread_id();

    std::thread t1(thread_func);
    std::thread t2(thread_func);
    std::thread t3(thread_func);
    // t1.join();
    // t2.join();
    // t3.join(); // 如果在这里调用，下面的线程id可能相同

    // 创建多个线程并启动
    std::thread t4(&MyClass::thread_func, &obj);
    std::thread t5(&MyClass::thread_func, &obj);
    std::thread t6(&MyClass::thread_func, &obj);
    // 等待线程结束
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    return 0;
}



