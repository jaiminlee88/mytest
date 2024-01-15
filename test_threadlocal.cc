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
thread_local int x = 1; // 声明一个线程本地存储变量，但是static必须类外初始化

// 打印线程本地存储的变量值
void print_thread_id(std::string thread_name="") {
    std::lock_guard<std::mutex> m(mlock);
    std::cout << "thread[" << thread_name << "]: " << "Thread ID: " << g_id << std::endl;
}
// 线程函数
void thread_func(std::string thread_name="") {
    g_id = std::this_thread::get_id(); // 设置线程本地存储的变量值
    print_thread_id(thread_name); // 打印线程本地存储的变量值
}

class MyClass {
public:
    // thread_local作为类成员变量时必须是static的
    static thread_local std::thread::id m_thread_id; // 声明一个静态的线程本地存储变量，但是static必须类外初始化
    int                     mem_x = 0; // 声明一个线程本地存储变量，但是static必须类外初始化s
    static int              static_mem_x; // 声明一个静态的线程本地存储变量，但是static必须类外初始化
    static thread_local int static_thread_local_mem_x; // 声明一个静态的线程本地存储变量，但是static必须类外初始化
    int                     mem_count = 0; // 声明一个线程本地存储变量，但是static必须类外初始化s
public:
    void print_thread_id(std::string thread_name="") {
        std::lock_guard<std::mutex> m(mlock);
        std::cout << "thread[" << thread_name << "]: " << "Thread ID: " << m_thread_id << std::endl;
    }

    void increase_print_count () {
        std::lock_guard<std::mutex> m(mlock);
        std::cout << "mem_count=" << ++mem_count << std::endl;
    }

    int get_mem_count() {
        return mem_count++;
    }

    void thread_func(std::string thread_name="") {
        m_thread_id = std::this_thread::get_id(); // 设置线程本地存储的变量值
        print_thread_id(thread_name); // 打印线程本地存储的变量值
        lambda_func(thread_name);
        increase_x(thread_name);
    }

    void increase_x(std::string thread_name="") {
        for (unsigned i = 0; i < 3; ++i) {
            std::lock_guard<std::mutex> m(mlock);
            std::cout << "thread[" << thread_name << "]: "
                      << "mem_x=" << ++mem_x
                      << " static_mem_x=" << ++static_mem_x
                      << " static_thread_local_mem_x=" << ++static_thread_local_mem_x << std::endl;
        }
    }

    void lambda_func(std::string thread_name="") {
        auto func = [&thread_name]() {
            std::lock_guard<std::mutex> m(mlock);
            std::cout << "thread[" << thread_name << "]: " << "lambda_func &m_thread_id=" << &m_thread_id << std::endl;
            m_thread_id = std::this_thread::get_id(); // 设置线程本地存储的变量值
            // print_thread_id(); // 打印线程本地存储的变量值
        };
        std::thread t(func);
        t.join();
    }
};
thread_local std::thread::id MyClass::m_thread_id{0};  // 类外定义并初始化
thread_local int MyClass::static_thread_local_mem_x = 0;  // 类外定义并初始化
int MyClass::static_mem_x = 0;  // 类外定义并初始化

int main() {
    MyClass obj;
    // initialize_thread_id();

    std::thread t1(thread_func, "t1");
    std::thread t2(thread_func, "t2");
    std::thread t3(thread_func, "t3");
    // t1.join();
    // t2.join();
    // t3.join(); // 如果在这里调用，下面的线程id可能相同

    // 创建多个线程并启动
    std::thread t4(&MyClass::thread_func, &obj, "t4");
    std::thread t5(&MyClass::thread_func, &obj, "t5");
    std::thread t6(&MyClass::thread_func, &obj, "t6");
    // 等待线程结束
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    std::function<void(std::string)> func = [](std::string thread_name){
        for (int i=0; i<3; i++) {
            thread_local MyClass* m = new MyClass(); // 声明即赋值，说明m已经是一个线程本地存储变量了，相当于static，只有一个实例
            std::lock_guard<std::mutex> m_(mlock);
            std::cout << "thread[" << thread_name << "]: get_mem_count=" << m->get_mem_count() << std::endl;
        }
    };
    std::thread t7(func, "t7");
    std::thread t8(func, "t8");
    t7.join();
    t8.join();

    std::function<void(std::string)> func_0 = [](std::string thread_name){
        for (int i=0; i<3; i++) {
            thread_local MyClass* m;
            m = new MyClass(); // 每次都建立一个新的对象，所以count都是0，产生了多个实例
            std::lock_guard<std::mutex> m_(mlock);
            std::cout << "thread[" << thread_name << "]: get_mem_count=" << m->get_mem_count() << std::endl;
        }
    };
    std::thread t9(func_0, "t9");
    std::thread t10(func_0, "t10");
    t9.join();
    t10.join();

    std::function<void(std::string)> func_1 = [](std::string thread_name){
        for (int i=0; i<3; i++) {
            thread_local int y = 1; // 内部声明，只会被初始化一次，相当于static
            std::lock_guard<std::mutex> m_(mlock);
            std::cout << "thread[" << thread_name << "]: y=" << y++ << std::endl;
        }
    };
    std::thread t11(func_1, "t11");
    std::thread t12(func_1, "t12");
    t11.join();
    t12.join();

    std::function<void(std::string)> func_2 = [](std::string thread_name){
        for (int i=0; i<3; i++) {
            std::lock_guard<std::mutex> m_(mlock);
            std::cout << "thread[" << thread_name << "]: x=" << x++ << std::endl; // x外部声明，x是一个线程本地存储变量，相当于static
        }
    };
    std::thread t13(func_2, "t13");
    std::thread t14(func_2, "t14");
    t13.join();
    t14.join();
    return 0;
}



