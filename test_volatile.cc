#include <iostream>
#include <atomic>
#include <thread>
using namespace std;

void test1() {
    cout << __func__ << " start..." << endl;
    std::atomic<int> num{0};
    std::thread t1([&num](){
        for (int i = 0; i < 100000000; ++i) {
            ++num;
        }
    });
    std::thread t2([&num](){
        for (int i = 0; i < 100000000; ++i) {
            ++num;
        }
    });
    std::thread t3([&num](){
        for (int i = 0; i < 100000000; ++i) {
            ++num;
        }
    });
    t1.join();
    t2.join();
    t3.join();
    std::cout << num << std::endl;
    cout << __func__ << " end..." << endl;
}
int main() {
    // volatile 不保证操作的原子性以及对代码重排顺序没有足够限制——解释了为什么volatile在多线程编程中没用，但是没有解释它应该用在哪
    // 不要用volatile，它避免了优化
    std::atomic<int> ai(0);         //初始化ai为0
    ai = 10;                        //原子性地设置ai为10
    std::cout << ai << endl;                //原子性地读取ai的值
    ++ai;                           //原子性地递增ai到11
    --ai;                           //原子性地递减ai到10

    // volatile在多线程中实际上不保证任何事情：数据竞争的定义
    volatile int vi(0);             //初始化vi为0
    vi = 10;                        //设置vi为10 
    std::cout << vi;                //读vi的值
    ++vi;                           //递增vi到11 ,其他线程可能读到任何值
    --vi;                           //递减vi到10

    // 编译时优化
    int x = 1;
    auto y = x;                             //读x，
    y = x;                                  //再次读x，会被优化删掉
                                            // 注意此时 y的类型为 int
    x = 10;                                 //写x，会被优化删掉
    x = 20;                                 //再次写x

    volatile int x1 = 1; // 如果用这句，就不会有下面的优化
    auto y1 = x1;                             //读x，
                                              // 注意此时 y的类型为 int
                                              // volatile或const会被过滤掉
    y1 = x1;                                  //再次读x，不会优化
    x1 = 10;                                 //写x，不会优化
    x1 = 20;                                 //再次写x

    // std::atomic 用在并发编程中，对访问特殊内存没用。
    // volatile    用于访问特殊内存，对并发编程没用。
    std::atomic<int> x2;
    std::atomic<int> y2(x2.load());                             //概念上会读x（见下）
    y2.store(x2);                                  //概念上会再次读x（见下）
    // x2 = 10;                                 //写x
    // x2 = 20;                                 //再次写x

    // test1();
}