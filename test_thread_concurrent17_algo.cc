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
#include <execution>
// #include <experimental/future>

#include "myutils.hh"

using namespace std;

std::mutex mlock;
std::queue<int> data_queue;
std::condition_variable data_cond;
bool job_done = false;
utils_log_level g_utils_log_level = utils_log_level::DEBUG;

void test1 (){
    std::vector<int> v(1000);
    int count=0;
    std::for_each(std::execution::seq,v.begin(),v.end(),[&](int& x){
        x=++count; // 不能仅依赖这种方式，让数字按顺序进行存储。
    });
}

void test2() {
    std::vector<int> v(1000);
    int count=0;
    std::for_each(std::execution::par,v.begin(),v.end(),
    [&](int& x){ x=++count; // 每次调用Lambda表达式时，都会对计数器进行修改，如果有多个线程在执行Lambda表达
                            //式，那么这里就会出现数据竞争，从而导致未定义行为。
    });
}
int main() {
    // 并行库影响
    // 1. 算法复杂化
    // 2. 抛出异常
    // 3. 算法执行位置、方式和时间
    // 标注执行策略
    // std::execution::sequenced_policy，调用对象 std::execution::seq
    // 强制要求按照一定顺序执行策略
    test1();
    // std::execution::parallel_policy，调用对象 std::execution::par
    // 强制要求并行执行策略
    test2();
    // std::execution::parallel_unsequenced_policy，调用对象 std::execution::par_unseq
    // 强制要求并行执行策略，但是不保证顺序,使用并行不排序策略调用的算法，可以在任意线程上执行，这些线程彼此间没有顺序

}