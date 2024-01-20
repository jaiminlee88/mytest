#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <map>
#include <queue>

#include <chrono>
#include <assert.h>
#include <functional>

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
// #include <experimental/future>

using namespace std;

std::mutex mlock;
std::queue<int> data_queue;
std::condition_variable data_cond;
bool job_done = false;

void test1(){
    std::thread t1([](){
        int cnt = 3;
        while(cnt) {
            int limit = (cnt + 1) * 10;
            {
                std::lock_guard<std::mutex> g(mlock);
                for (int i = cnt*10; i < limit; ++i) {
                    data_queue.push(i);
                }
                --cnt;
                if (!cnt) {
                    job_done = true;
                }
                // data_cond.notify_all();
                data_cond.notify_one();
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        cout << "t1 exit" << endl;
    });

    std::thread t2([](){
        while(true) {
            {
                std::unique_lock<std::mutex> ul(mlock); // 比lock_guard更为灵活（会锁住queue），可以加锁解锁
                data_cond.wait(ul, [](){ return !data_queue.empty(); });
                // 1 unique lock加锁
                // 2 data_cond判断条件是否满足，不满足则主动释放unique_lock ul，进入阻塞或等待状态挂起；若满足则转4
                // 3 在阻塞状态，data cond收到notify,从睡眠状态苏醒，重新获取unique lock，转2
                // 4 如果条件满足，继续执行以下代码
                cout << "reveive: ";
                while(data_queue.size()) {
                    int tmp = data_queue.front();
                    data_queue.pop();
                    cout << tmp << ",";
                }
                cout << endl;
                ul.unlock();
            }
            if(job_done && !data_queue.size()) {
                break;
            }
        }
        cout << "t2 exit" << endl;
    });

    t1.join();
    t2.join();
}

template<typename T>
class threadsafe_queue{
private:
    mutable std::mutex mut; // 必须可变
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue(){}
    threadsafe_queue(threadsafe_queue const& other) {
        std::lock_guard<std::mutex> lk(mut);
        data_queue = other.data_queue;
    }
    ~threadsafe_queue(){
        // cout << "threadsafe_queue deconstruct." << endl;
    }

    void push(T& new_value){
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this](){ return data_queue.size(); });
        value = data_queue.front();
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this](){ return data_queue.size(); });
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool try_pop(T& value){
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty()) {
            return false;
        }
        value = data_queue.front();
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};
threadsafe_queue<int> iqueue;
void test2(){
    std::thread t1([](){
        int cnt = 0;
        int limit = 500;
        while(cnt < limit){
            iqueue.push(cnt);
            // std::this_thread::sleep_for(std::chrono::seconds(1));
            cnt++;
        }
        job_done = true;
        cout << "thread t1 exit" << endl;
    });

    std::thread t3([](){
        int cnt = 600;
        int limit = 1100;
        while(cnt < limit){
            iqueue.push(cnt);
            // std::this_thread::sleep_for(std::chrono::seconds(1));
            ++cnt;
        }
        job_done = true;
        cout << "thread t3 exit" << endl;
    });

    std::thread t2([](){
        int value = 0;
        while(true) {
            iqueue.wait_and_pop(value);
            cout << "receive1: " << value << endl;
            if (job_done) {
                while(!iqueue.empty()) {
                    value = *iqueue.try_pop();
                    cout << "t2 receive: " << value << endl;
                }
                break;
            }
        }
        cout << "thread t2 exit" << endl;
    });
    t1.join();
    t2.join();
    t3.join();
}

double square_root(double x) {
    if (x < 0) {
        throw std::out_of_range("x<0");
    }
    return sqrt(x);
}
void test3(){
    // 基于任务的方法总是好于基于线程的方法
    // 基于任务的方法可以捕捉异常，基于线程的方法不能捕捉异常
    // 基于线程的编程方式需要手动的线程耗尽、资源超额、负责均衡、平台适配性管理。
    // 通过带有默认启动策略的std::async进行基于任务的编程方式会解决大部分问题
    // 基于任务的方法可以自行管理资源，不会oversubscription
    int cnt = 1;
    std::future<int> the_answer = std::async([](int& cnt){
        while(cnt--) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        return 10;
    }, std::ref(cnt));
    cout << "the answer=" << the_answer.get() << " cnt=" << cnt << endl;

    struct X{
        void foo(int a, std::string const& str) {}
        std::string bar(std::string const& str) { std::this_thread::sleep_for(std::chrono::seconds(1)); return str;}
    };
    X x;
    // 当不着急要任务结果时， 可以使用 std::async 启动一个异步任务，阻塞等待计算结果
    auto f1 = std::async(&X::foo, &x, 42, "hello"); // x被引用
    cout << "f1=" << endl; // f1.get()返回void
    auto f2 = std::async(&X::bar, x, "goodbye"); // x被拷贝,会阻塞
    cout << "f2=" << f2.get() << endl; // f2.get()返回string,所以输出"goodbye"

    struct Y{
        double operator()(double d) { return d; }
    };
    Y y;
    auto f3 = std::async(Y(), 3.141); // 临时Y,通过Y的移动构造函数传递给f3
    cout << "f3=" << f3.get() << endl;
    auto f4 = std::async(std::ref(y), 2.718); // y被引用
    cout << "f4=" << f4.get() << endl;

    auto baz = [](X& x, std::string str)->X { cout << "run baz " << str << endl; return x; };
    auto f5 = std::async(baz, std::ref(x), "f5"); // 调用baz(x)

    class move_only{
    public:
        move_only() {}
        move_only(move_only&&) {}
        move_only(move_only const&) = delete;
        move_only& operator=(move_only&&) { return *this;}
        move_only& operator=(move_only const&) = delete;
        void operator()(){}
    };
    auto f6 = std::async(move_only()); // 临时move_only,通过move_only的移动构造函数传递给f6 即std::move(move_only())构造得到

    /*
    std::async是C++11标准中提供的一个函数模板，用于实现多线程的异步操作。它可以通过不同的参数配置来指定是同步执行还是异步执行。

    同步执行（std::launch::deferred）：当使用std::launch::deferred参数调用std::async时，任务不会在调用std::async的线程中立即执行，而是在调用std::async的get()函数时，由调用get()函数的线程执行。也就是说，任务的执行将被延迟到调用get()函数时，且在调用get()函数的线程中执行。

    异步执行（std::launch::async）：当使用std::launch::async参数调用std::async时，任务会立即在线程池中启动一个新的线程执行，并返回一个std::future<T>对象，用于获取异步操作的结果。通过调用std::future<T>的get()函数，可以等待并获取任务执行完成后返回的结果。

    总结来说，同步执行是在调用get()函数时执行任务，而异步执行是在调用std::async函数时立即开启一个新线程执行任务，并返回一个std::future<T>对象。可以通过std::launch::deferred和std::launch::async参数来控制使用哪种执行方式。
    */

    auto f7 = std::async(std::launch::async, Y(), 1.2); // 强制在新线程执行, async必须在新线程执行, 不能在当前线程执行
    cout << "init f7=" << f7.get() << endl;
    auto f8 = std::async(std::launch::deferred, baz, std::ref(x), "f8"); // 延迟调用,直到调用f8.get()或f8.wait()才执行
    cout << "init deferred f8=" << endl;
    auto f9 = std::async(std::launch::async | std::launch::deferred, baz, std::ref(x), "f9"); // 由系统决定
    cout << "init async|deferred f9=" << endl;
    auto f10 = std::async(baz, std::ref(x), "f10");
    cout << "deferred f8 wait." << endl;
    f8.wait(); // 等待延迟调用执行. f8.get()也可以

    std::future<double> f = std::async(square_root, -1); // show throw error，会被future捕捉
    std::this_thread::sleep_for(std::chrono::seconds(1));
    double ret = f.get(); // 只有使用get时才能捕捉错误，调用后直接抛出
}


void test4(){
    auto f = [](int i){ cout << "hahahah" << endl; return i*2; };
    std::packaged_task<int(int)> task(f); // 函数返回int，输入int
    cout << "set get_future" << endl;
    std::future<int> f1 = task.get_future(); // 等待
    // cout << "try to get result=" << f1.get() << endl; // block，没有运行不能放在这
    std::thread t(std::move(task), 10); // 放入线程
    cout << "task result=" << f1.get() << endl; // block to wait for return
    t.join();

    std::packaged_task<int(int)> task2([](int i){ return i*2; });
    std::future<int> f2 = task2.get_future();
    std::thread t2(std::move(task2), 10);
    cout << "task2 result=" << f2.get() << endl;
    t2.join();

    std::packaged_task<int(int)> task3([](int i){ return i*2; });
    std::future<int> f3 = task3.get_future();
    std::thread t3(std::move(task3), 10);
    cout << "task3 result=" << f3.get() << endl;
    t3.join();

    std::packaged_task<int(int)> task4([](int i){ throw std::runtime_error("test"); return i*2;});
    std::future<int> f4 = task4.get_future();
    std::thread t4(std::move(task4), 10);
    // cout << "task4 result=" << f4.get() << endl; // 抛出异常，会被future捕捉, 不get则不会抛出
    t4.join();

    {
        std::packaged_task<int(int)> task5([](int i){
            // cout << "task5 start...." << endl;
            return i*2;
        });
        std::future<int> f5 = task5.get_future();
        std::thread t5(std::move(task5), 10);
        cout << "task5 result=" << f5.get() << endl;
        t5.join(); // still need to join
    }
    cout << __func__ << " end..." << endl;
}

void calculateResult(std::promise<int>& promiseObj) {
    // 模拟计算
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int result = 42;
    // 设置promise的值
    promiseObj.set_value(result);
}
void calculateResult2(std::promise<int>& promiseObj) {
    try{
        throw std::runtime_error("test"); // 如果抛出错误，会被future捕捉，如果不尝试捕捉，会直接在线程中报错
    } catch(...) {
        cout << "catch exception." << endl;
        // promiseObj.set_exception(std::current_exception()); // 设置异常，直接抛出
        promiseObj.set_exception(std::make_exception_ptr(std::logic_error("error foo..."))); // 存储异常，不直接抛出,std::copy_exception更名为 std::make_exception_ptr在 <exception>用于最终的 C++11 标准。
        return;
    }
    // 模拟计算
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int result = 42;
    // 设置promise的值
    promiseObj.set_value(result);
}
void test5(){
    // 创建promise对象
    std::promise<int> promiseObj;
    // 获取与promise关联的future对象
    std::future<int> futureObj = promiseObj.get_future();
    // 创建线程，执行计算
    std::thread workerThread(calculateResult, std::ref(promiseObj)); // 抛出异常，会直接报错
    // 主线程等待计算结果
    futureObj.wait(); // block for the value
    std::cout << "Waiting for result..." << std::endl;
    // 获取计算结果
    int result = futureObj.get();
    std::cout << "Result: " << result << std::endl;

    // 异常抛出
    // 创建promise对象
    std::promise<int> promiseObj2;
    // 获取与promise关联的future对象
    std::future<int> futureObj2 = promiseObj2.get_future();
    // 创建线程，执行计算
    std::thread workerThread2(calculateResult2, std::ref(promiseObj2)); // 抛出异常，会直接报错
    // 主线程等待计算结果
    futureObj2.wait(); // block for the value
    std::cout << "Waiting for result2..." << std::endl;
    // 获取计算结果
    int result2 = futureObj2.get(); // 抛出异常，会直接报错,不调用get则不会抛出
    std::cout << "Result2: " << result2 << std::endl;

    // 等待线程结束
    workerThread.join();
    workerThread2.join();
}

std::mutex loglock;
void threadsafe_log(std::string msg) {
    std::lock_guard<std::mutex> g(loglock);
    cout << msg << endl;
}


void test6(){
    /*
    future 和 promise之间有个共享状态（shared state），存放在堆中
    引用了共享状态——使用std::async启动的 未延迟任务 建立的那个——的最后一个future的析构函数会阻塞住，直到任务完成。本质上，这种future的析构函数对执行异步任务的线程执行了隐式的join。
    其他所有future的析构函数简单地销毁future对象。对于异步执行的任务，就像对底层的线程执行detach。对于延迟任务来说如果这是最后一个future，意味着这个延迟任务永远不会执行了。
    */
    std::promise<int> p;
    // 例子1，以下所有例子都可以用
    // std::future<int> f = p.get_future();
    // std::shared_future<int> sf = f.share(); // 共享future
    // std::shared_future<int> sf(std::move(f)); // 共享future, 转移所有权
    // std::shared_future<int> sf(std::move(p.get_future())); // invalid，p已经被move了，不能再get_future
    std::shared_future<int> sf(p.get_future().share()); // move future

    // 例子2
    // std::future<int> f = p.get_future(); // 这句不能有
    // std::shared_future<int> sf(p.get_future()); // move future

    std::vector<std::thread> v;
    std::thread t1([](std::promise<int>& mp){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        mp.set_value(10); // 设置promise的值, shared future get只能get一次
    }, std::ref(p)); // 传入promise

    for (int i=0; i<5; ++i) {
        v.emplace_back([](std::shared_future<int> sf, int tname){
            threadsafe_log("tname " + std::to_string(tname) + " start.");
            threadsafe_log("tname " + std::to_string(tname) + " receive: " + std::to_string(sf.get())); // block for the value
        }, sf, i); // 传入shared_future
    }
    for(auto& t : v) {
        t.join();
    }
    t1.join();

    // std::thread t1([](std::shared_future<int> sf){
    //     cout << "t1 receive: " << sf.get() << endl;
    // }, sf);
    // std::thread t2([](std::shared_future<int> sf){
    //     cout << "t2 receive: " << sf.get() << endl;
    // }, sf);
    // std::thread t3([](std::shared_future<int> sf){
    //     cout << "t3 receive: " << sf.get() << endl;
    // }, sf);
    // std::thread t4([](std::shared_future<int> sf){
    //     cout << "t4 receive: " << sf.get() << endl;
    // }, sf);
}


std::condition_variable cv;
std::mutex mylock;
void test7(){
    std::chrono::milliseconds ms(54802);
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(ms); // 被截断

    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto end = std::chrono::high_resolution_clock::now();
    cout << "f11 consumes: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << "s" << endl;

    // 利用时间等待
    std::future<int> f1 = std::async([](){
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 11;
    });
    int timeout_cnt = 0;
    while (true) {
        if(f1.wait_for(std::chrono::milliseconds(500)) == std::future_status::ready) { //wait_for是相对市场，wait_until是绝对时间点
            cout << "wait_for the f1.get()=" << f1.get() << endl;
            break;
        } else if(f1.wait_for(std::chrono::milliseconds(500)) == std::future_status::timeout) {
            cout << "wait_for f1 timeout " << timeout_cnt++ << " times." << endl; // 可以一直等待
        }
    }

    std::future<int> f2 = std::async([](){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cv.notify_one();
        return 11;
    });
    auto const timeout_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
    while (true) {
        std::unique_lock<std::mutex> lk(mylock);
        if(cv.wait_until(lk, timeout_time) == std::cv_status::timeout) { //wait_for是相对市场，wait_until是绝对时间点
            cout << "wait_until f2 timeout" << endl; // 可以一直等待
            break;
        } else {
            cout << "wait_until f2.get()=" << f2.get() << endl;
        }
    }
}


template<typename T>
std::list<T> sequence_quick_sort(std::list<T> input) {
    if (input.empty()) {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin()); // 将input的第一个元素移动到result的第一个元素, input就少了一个元素
    T const& pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(), [&](T& i){ return i < pivot;}); // 将input分成两部分，小于pivot的在前面，大于pivot的在后面，返回一个切分节点，可能是下标

    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point); // 将input的前半部分移动到lower_part的后面
    auto new_lower(sequence_quick_sort(std::move(lower_part))); // 递归
    auto new_higher(sequence_quick_sort(std::move(input))); // 递归
    result.splice(result.end(),new_higher); // 将new_higher的所有元素移动到result的后面
    result.splice(result.begin(),new_lower); // 将new_lower的所有元素移动到result的前面
    // copy(result.begin(), result.end(), ostream_iterator<T>(cout, ","));
    // cout << endl;
    return result;
}

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
    if (input.empty()) {
        return input;
    }
    std::list<T> result;
    input.splice(result.begin(), input, input.begin()); // 将input的第一个元素移动到result的第一个元素, input就少了一个元素
    T const& pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(), [&](T& i){ return i < pivot;}); // 将input分成两部分，小于pivot的在前面，大于pivot的在后面，返回一个切分节点，可能是下标

    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point); // 将input的前半部分移动到lower_part的后面

    std::future<std::list<T>> new_lower(std::async(parallel_quick_sort<T>, std::move(lower_part))); // 递归
    auto new_higher(parallel_quick_sort(std::move(input))); // 递归
    result.splice(result.end(), new_higher); // 将new_higher的所有元素移动到result的后面
    result.splice(result.begin(), new_lower.get()); // 将new_lower的所有元素移动到result的前面
    return result;
}

void test8(){
    std::list<int> l{5,7,3,4,1,9,2,8,10,6};
    cout << "sequence_quick_sort: ";
    auto sqs_ret = sequence_quick_sort(l);
    copy(sqs_ret.begin(), sqs_ret.end(), ostream_iterator<int>(cout, ","));
    cout << endl;

    cout << "parallel_quick_sort: ";
    auto pqs_ret = parallel_quick_sort(l);
    copy(pqs_ret.begin(), pqs_ret.end(), ostream_iterator<int>(cout, ","));
    cout << endl;

}

void process_int(int& i ) { cout << "process_int: " << i << endl;}
void process_string(const std::string& str) { cout << "process_string: " << str << endl;}
void process_double(double d) { cout << "process_double: " << d << endl;}
template <typename T>
class Handle {
public:
    Handle() {}
    ~Handle() {}

    // 处理函数，根据不同模板参数类型执行不同的处理逻辑
    template <typename U>
    Handle& handle(const U& param) {
        // static_assert(std::is_same<T, U>::value, "Template parameter mismatch");
        std::cout << "Processing with type: " << typeid(U).name() << " param:" << param << std::endl;
        // 实现不同类型的处理逻辑
        // ...
        return *this;
    }

    // 处理函数，根据不同模板参数类型执行不同的处理逻辑
    // 判断 C++ 版本
    #if __cplusplus >= 201703L // C++11 或更高版本
        template <auto func, typename... Args> // c++17可以实现 h.handle_func<process_int>(38);
        Handle& handle_func(Args... args) {
            func(args...);
            return *this;
        }
    #else
        // // 处理函数，根据不同模板参数类型执行不同的处理逻辑,不能实现 h.handle_func<process_int>(38); 在 C++11 中，无法使用非类型模板参数推断来实现以 handle_func<process_int>(value) 方式调用的函数模板。
        // template <typename func, typename... Args>
        // Handle& handle_func(Args&&... args) {
        //     func(std::forward<Args>(args)...);
        //     return *this;
        // }
        // std::cout << "低于 C++11 的版本" << std::endl;
        // 在这里编写针对低于 C++11 版本的代码
    #endif
};

void test9(){
    // actor model
    // 创建一个Handle对象
    Handle<int> h;
    // 调用handle函数，根据不同的模板参数实现不同处理
    h.handle<int>(10);  // 处理int型参数
    // 可以按照需要多次调用handle函数，进行不同的处理
    h.handle<const char*>("Hello");  // 处理字符串参数
    h.handle<double>(1.234).handle<char>('9');  // 处理double型参数

    #if __cplusplus >= 201703L
        // 以函数名作为模板参数
        h.handle_func<process_int>(38);
        h.handle_func<process_string>("abc").handle_func<process_double>(2.88);
    #endif
}

class atomic_flag_lock {
private:
    std::atomic_flag m_flag; // 自旋锁
public:
    atomic_flag_lock() : m_flag(ATOMIC_FLAG_INIT) {}
    ~atomic_flag_lock() {}

    void lock() {
        while(m_flag.test_and_set(std::memory_order_acquire)) {}
    }
    void unlock() {
        m_flag.clear(std::memory_order_release);
    }
};

std::atomic_flag mflag = ATOMIC_FLAG_INIT; // 初始化为false
void test10(){
    // atomic
    std::atomic<int> ai(0);
    // std::atomic<int> ai1(ai); // invalid,拷贝构造函数
    // std::atomic<int> ai1=ai.load(); // invalid,拷贝构造函数
    std::atomic_long ad(12345); // std::atomic<long> ad(12345);
    if (ai.is_lock_free()) { cout << "atomic<int> is lock free." << endl;// 是否lock free
    } else { cout << "atomic<int> is not lock free." << endl;}
    if (std::is_trivially_copyable<std::string>::value) { // 是否是trivially copyable
        cout << "is_trivially_copyable<string> is true." << endl;
    } else { cout << "is_trivially_copyable<string> is false." << endl; }
    if(std::is_trivially_copyable<double>::value) {
        cout << "is_trivially_copyable<double> is true." << endl;
    } else { cout << "is_trivially_copyable<double> is false." << endl;}
    std::atomic<double> dou(3.8);
    if (dou.is_lock_free()) { cout << "atomic<double> is lock free." << endl; // 是否lock free
    } else { cout << "atomic<double> is not lock free." << endl;}

    cout << endl;
    auto func0 = [](std::string str){
        threadsafe_log(str + ": func0 start.");
        while(true) { // 自旋锁
            if (mflag.test_and_set(std::memory_order_acquire)) {
                threadsafe_log(str + ": func0 test_and_set");
                // std::this_thread::sleep_for(std::chrono::seconds(1));
                mflag.clear(std::memory_order_release);
                break;
            } else {
                threadsafe_log(str + ": func0 test_and_set failed.");
            }
        }
        threadsafe_log(str + ": func0 end.");
    };
    std::thread t1(func0, "t1");
    std::thread t2(func0, "t2");
    t1.join();
    t2.join();

    atomic_flag_lock atl;
    std::thread t3([&atl](){
        atl.lock();
        cout << "t3 lock. 12e4567890-sdfghjkopxcvnm,;fghjkl" << endl;
        cout << "t3 dfgjkl;dfghjkl;fghjkl;'" << endl;
        atl.unlock();
    });
    std::thread t4([&atl](){
        atl.lock();
        cout << "t4 lock. 12e4567890-sdfghjkopxcvnm,;fghjkl" << endl;
        cout << "t4 dfgjkl;dfghjkl;fghjkl;'" << endl;
        atl.unlock();
    });
    t3.join();
    t4.join();
}

void test11() {
    bool b1 = true; // set true to b1
    bool b2 = b1; // set b2 with true, not through b1
    // bool b4 = std::atomic_load(&b1); // invalid, b1 is not atomic
    // store: memory_order_relaxed, memory_order_release, memory_order_seq_cst
    // load: memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_seq_cst'
    // read-modify-write: memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_release, memory_order_acq_rel, memory_order_seq_cst
    // compare_exchange_weak: memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_release, memory_order_acq_rel, memory_order_seq_cst
    // compare_exchange_strong: memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_release, memory_order_acq_rel, memory_order_seq_cst
    std::atomic<bool> b3{false};
    bool x = b3.load(std::memory_order_acquire); // 读取b3的值并返回
    cout << "x=" << x << endl;
    b3.store(true); // store
    x = b3.exchange(false, std::memory_order_acq_rel); // 存入false，返回原来的值true
    cout << "true=" << true << " false=" << false << endl;
    cout << "origins 1--> x=" << x << " b3=" << b3.load(std::memory_order_acq_rel) << endl; // x==true
    // 当前x == true != b3 == false，不相等不能修改，x被替换b3当前值，b3不变，返回false（代表这是一个替换操作）
    bool x1 = b3.compare_exchange_weak(x, true, std::memory_order_acq_rel);
    cout << "compare_exchange_weak 2--> x=" << x << "(changed) x1=" << x1 << "(ret false) b3=" << b3.load(std::memory_order_acq_rel) << "(not change)" << endl; // x1==false
    // 当前x==false,b3==true，相等可以修改，b3修改为true，返回true（代表这是一个存入操作）
    bool x2 = b3.compare_exchange_weak(x, true, std::memory_order_acq_rel);
    cout << "compare_exchange_weak 3--> x=" << x << "(not change) x2=" << x2 << "(ret true) b3=" << b3.load(std::memory_order_acq_rel) << "(change) " << endl; // x2==true
    cout << endl;

    std::atomic<bool> b4{false};
    bool y = b4.load(std::memory_order_acquire); // 读取b3的值并返回
    cout << "y=" << y << endl;
    b4.store(true); // store
    y = b4.exchange(false, std::memory_order_acq_rel); // 存入false，返回原来的值true
    cout << "true=" << true << " false=" << false << endl;
    cout << "origins 1--> y=" << y << " b4=" << b4.load(std::memory_order_acq_rel) << endl; // x==true
    // compare_exchange_strong线程会被挂起吗？不会，它会一直尝试直到成功或者失败
    // 当前x == true != b4 == false，不相等不能修改，x被替换b4当前值，b4不变，返回false（代表这是一个替换操作）
    bool x3 = b4.compare_exchange_strong(y, true, std::memory_order_acq_rel);
    cout << "compare_exchange_strong 4--> y=" << y << "(changed!!!-->2) x3=" << x3 << "(ret false) b4=" << b4.load(std::memory_order_acq_rel) << "(not change)" << endl; // x1==false
    // 当前x==false,b4==true，相等可以修改，b3修改为true，返回true（代表这是一个存入操作）
    bool x4 = b4.compare_exchange_strong(y, true, std::memory_order_acq_rel);
    cout << "compare_exchange_strong 5--> y=" << y << "(not change) x4=" << x4 << "(ret true) b4=" << b4.load(std::memory_order_acq_rel) << "(change) " << endl; // x2==true
    cout << "b4.is_lock_free()=" << b4.is_lock_free() << endl;
    cout << endl;

    std::atomic<int> value{0};
    std::thread t1([&value](){
        for (int i = 0; i < 5; ++i) {
            value.store(i);
            // std::this_thread::sleep_for(std::chrono::seconds(1));
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            threadsafe_log("t1: value=" + std::to_string(value.load()));
        }
    });
    std::thread t2([&value](){
        int expected = 4;
        int cnt = 0;
         // // weak版本的CAS允许偶然出乎意料的返回（比如在字段值和期待值一样的时候却返回了false，并且没有将字段值设置成desire的值），不过在一些循环算法中，这是可以接受的。通常它比起strong有更高的性能。
        while (!value.compare_exchange_weak(expected, 6)) {
            expected = 4; // 必须重置expected，因为会被修改为value当前值
            if (value.load() == expected) {
                threadsafe_log("t2: attention!!!! value=" + std::to_string(value.load())); // 此时value与expected相等，但是仍然返回false，因为weak版本允许偶然出乎意料的返回，不一定能复现,多跑几次
            }
            // threadsafe_log("t2: value=" + std::to_string(value.load()));
            cnt++;
        }

        // while (!value.compare_exchange_strong(expected, 6)) {
        //     expected = 4; // 必须重置expected，因为会被修改为value当前值
        //     if (value.load() == expected) {
        //         threadsafe_log("t2: attention!!!! value=" + std::to_string(value.load())); // 一定不会出现,strong会耗费一定资源确保返回false时两者是不同的
        //     }
        //     // threadsafe_log("t2: value=" + std::to_string(value.load()));
        //     cnt++;
        // }
        threadsafe_log("t2: value=" + std::to_string(value.load()) + " cnt=" + std::to_string(cnt));
    });
    t1.join();
    t2.join();
    cout << endl;

    // std::atomic<T*>
    class Foo{};
    Foo some_arry[5];
    std:atomic<Foo*> p(some_arry); // 初始化为some_arry,专门为地址类型设计的atomic
    // Foo* x5 = p.fetch_add(2); // x5 = some_arry, p = some_arry + 2，返回p原来的值
    cout << "|||||||||fetch_add..." << endl;
    Foo* x5 = p.fetch_add(2, std::memory_order_acq_rel);
    // 返回数组第一个元素的指针，指针加2，返回原来的指针
    if(x5 == some_arry) { cout << "x5 == some_arry" << endl; } else { cout << "x5 != some_arry" << endl; }
    if (x5 == &some_arry[2]) { cout << "x5 == &some_arry[2]" << endl; } else { cout << "x5 != &some_arry[2]" << endl; }
    if (p.load() == &some_arry[2]) { cout << "p.load() == &some_arry[2]" << endl; } else { cout << "p.load() != &some_arry[2]" << endl; }
    if (p.load() == &some_arry[3]) { cout << "p.load() == &some_arry[3]" << endl; } else { cout << "p.load() != &some_arry[3]" << endl; }
    x5 = (p-=1); // x5 = some_arry + 1, p = some_arry + 1，返回p的新值
    if (x5 == &some_arry[1]) { cout << "x5 == &some_arry[1]" << endl; } else { cout << "x5 != &some_arry[1]" << endl; }
    if (p.load() == &some_arry[1]) { cout << "p.load() == &some_arry[1]" << endl; } else { cout << "p.load() != &some_arry[1]" << endl; }
    cout << "|||||||||fetch_sub..." << endl;
    x5 = p.fetch_sub(1); // x5 = some_arry + 1, p = some_arry, p = some_arry - 1（变化了），返回p原来的值
    if (x5 == &some_arry[1]) { cout << "x5 == &some_arry[1]" << endl; } else { cout << "x5 != &some_arry[1]" << endl; }
    if (p.load() == &some_arry[0]) { cout << "p.load() == &some_arry[0]" << endl; } else { cout << "p.load() != &some_arry[0]" << endl; }
    cout << endl;

    // std::atomic<T>模板使用要求：
    // 1. 用户类型，必须包含有编译器创建的拷贝构造函数，不能包含虚函数和虚基类
    // 2. 用户类型，必须是trivially copyable，即可以使用memcpy复制
    // 3. 用户类型，基类和非静态数据成员也支持拷贝赋值操作
}


///
// reference: http://www.cplusplus.com/reference/future/packaged_task/
int test_packaged_task_1() {
 
{ // constructor/get_future/operator=/valid
	std::packaged_task<int(int)> foo; // default-constructed
	std::packaged_task<int(int)> bar([](int x) { return x * 2; }); // initialized
 
	foo = std::move(bar); // move-assignment
	std::cout << "valid: " << foo.valid() << "\n";
	std::future<int> ret = foo.get_future(); // get future
	std::thread(std::move(foo), 10).detach(); // spawn thread and call task
 
	int value = ret.get(); // wait for the task to finish and get result
	std::cout << "The double of 10 is " << value << ".\n";
}
 
{ // reset/operator()
	std::packaged_task<int(int)> tsk([](int x) { return x * 3; }); // package task
 
	std::future<int> fut = tsk.get_future();
	tsk(33);
	std::cout << "The triple of 33 is " << fut.get() << ".\n";
 
	// re-use same task object:
	tsk.reset();
	fut = tsk.get_future();
	std::thread(std::move(tsk), 99).detach();
	std::cout << "Thre triple of 99 is " << fut.get() << ".\n";
}
 
{ // constructor/get_future
	auto countdown = [](int from, int to) {
		for (int i = from; i != to; --i) {
			std::cout << i << '\n';
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		std::cout << "Lift off!\n";
		return from - to;
	};
 
	std::packaged_task<int(int, int)> tsk(countdown); // set up packaged_task
	std::future<int> ret = tsk.get_future(); // get future
 
	std::thread th(std::move(tsk), 5, 0); // spawn thread to count down from 5 to 0
 
	int value = ret.get(); // wait for the task to finish and get result
	std::cout << "The countdown lasted for " << value << " seconds.\n";
 
	th.join();
}
 
	return 0;
}
 
///
// reference: https://en.cppreference.com/w/cpp/thread/packaged_task
int test_packaged_task_2() {
{ // lambda
	std::packaged_task<int(int, int)> task([](int a, int b) { return std::pow(a, b);});
	std::future<int> result = task.get_future();
 
	task(2, 9);
	std::cout << "task_lambda:\t" << result.get() << '\n';
}
 
{ // bind
    /*
    C++中的bind函数用于创建一个函数对象（也称为函数符或函数子），该函数对象可以将参数绑定到函数中。bind函数是函数式编程的重要工具，它允许我们在调用函数时，通过预先绑定一些参数值来创建一个新的可调用对象。
    */
	std::packaged_task<int()> task(std::bind([](int x, int y) { return std::pow(x, y); }, 2, 11));
	std::future<int> result = task.get_future();
 
	task();
	std::cout << "task_bind:\t" << result.get() << '\n';
}
 
{ // thread
	std::packaged_task<int(int, int)> task([](int x, int y) { return std::pow(x, y); });
	std::future<int> result = task.get_future();
 
	std::thread task_td(std::move(task), 2, 10);
	task_td.join();
	std::cout << "task_thread:\t" << result.get() << '\n';
}
 
	return 0;
}
 
///
// reference: https://thispointer.com/c11-multithreading-part-10-packaged_task-example-and-tutorial/
struct DBDataFetcher {
	std::string operator()(std::string token)
	{
		// Do some stuff to fetch the data
		std::string data = "Data From " + token;
		return data;
	}
};

int test_packaged_task_3() {
	// Create a packaged_task<> that encapsulated a Function Object
	std::packaged_task<std::string(std::string)> task(std::move(DBDataFetcher()));

	// Fetch the associated future<> from packaged_task<>
	std::future<std::string> result = task.get_future();

	// Pass the packaged_task to thread to run asynchronously
	std::thread th(std::move(task), "Arg");

	// Join the thread. Its blocking and returns when thread is finished.
	th.join();

	// Fetch the result of packaged_task<> i.e. value returned by getDataFromDB()
	std::string data = result.get();
	std::cout << data << std::endl;

	return 0;
}

///
// reference: https://stackoverflow.com/questions/18143661/what-is-the-difference-between-packaged-task-and-async
int test_packaged_task_4() {
	// sleeps for one second and returns 1
	auto sleep = []() {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		return 1;
	};

{ // std::packaged_task
	// >>>>> A packaged_task won't start on it's own, you have to invoke it
	std::packaged_task<int()> task(sleep);

	auto f = task.get_future();
	task(); // invoke the function

	// You have to wait until task returns. Since task calls sleep
	// you will have to wait at least 1 second.
	std::cout << "You can see this after 1 second\n";

	// However, f.get() will be available, since task has already finished.
	std::cout << f.get() << std::endl;
}

{ // std::async
	// >>>>> On the other hand, std::async with launch::async will try to run the task in a different thread :
	auto f = std::async(std::launch::async, sleep);
	std::cout << "You can see this immediately!\n";
	// However, the value of the future will be available after sleep has finished
	// so f.get() can block up to 1 second.
	std::cout << f.get() << "This will be shown after a second!\n";
}
	return 0;
}
void test12() {
    test_packaged_task_1();
    test_packaged_task_2();
    test_packaged_task_3();
    test_packaged_task_4();
}

void f(){                    //f休眠1秒，然后返回
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
void test13() {
    cout << __func__ << " start." << endl;
    auto fut = std::async(f);           //异步运行f（理论上）
    if (fut.wait_for(std::chrono::milliseconds(0)) == std::future_status::deferred) {
        cout << __func__ << " deferred." << endl;
    } else {
        while (fut.wait_for(std::chrono::milliseconds(100)) !=       //循环，直到f完成运行时停止...
            std::future_status::ready)   //但是有可能永远不会发生！
        {
            cout << __func__ << " waiting..." << endl;
        }
    }
    cout << __func__ << " done." << endl;
}
int main() {
    // test1(); // test condition_variable
    // test2(); // test threadsafe queue
    // test3(); // future async, 一个future只能get一次, asynck可能是硬件并发最好的
    test4(); // packaged_task
    // test5(); // std::promise , 与future配合使用，promise只能set一次，使用了堆内存存储共享状态，
    // test6(); // shared_future，多个线程共享一个future，
    // test7(); // test chrono
    // test8(); // quick sort
    // test9(); // CSP Communicating Sequential Processer 通信顺序处理 actor model
    // test10(); // atomic flag
    // test11(); // atomic 要求不能拷贝构造或拷贝赋值，因赋值和拷贝调用了两个对象，这就就破坏了操作的原子性
    // test12(); // std::packaged_task 测试
    // test13();

    // std::condition_variable cv;
    // std::mutex cv_mutex;
    // bool ready = false;
    // std::thread t1([&cv_mutex, &cv, &ready](){
    //     std::this_thread::sleep_for(std::chrono::seconds(1));
    //     {
    //         std::lock_guard<std::mutex> lk(cv_mutex);
    //         ready = true;
    //     }
    //     cv.notify_one();
    // });
    // std::unique_lock<std::mutex> lk(cv_mutex);
    // auto result = cv.wait_for(lk, std::chrono::seconds(2), [&ready](){ return ready;}); // 返回bool
    // if (result) {
    //     cout << "cv.wait_for(lk, std::chrono::seconds(1), [&ready](){ return ready;}) timeout" << endl;
    // } else {
    //     cout << "cv.wait_for(lk, std::chrono::seconds(1), [&ready](){ return ready;}) not timeout" << endl;
    // }
    // t1.join();
    // cout << endl;

    std::condition_variable cv;
    std::mutex cv_mutex;
    std::thread t1([&cv](){
        std::this_thread::sleep_for(std::chrono::seconds(3));
        cv.notify_one();
    });
    std::unique_lock<std::mutex> lk(cv_mutex);
    auto result = cv.wait_for(lk, std::chrono::seconds(2)); // 返回bool
    if (result == std::cv_status::timeout) {
        cout << "cv.wait_for(lk, std::chrono::seconds(1), [&ready](){ return ready;}) timeout" << endl;
        t1.detach();
    } else {
        cout << "cv.wait_for(lk, std::chrono::seconds(1), [&ready](){ return ready;}) not timeout" << endl;
        t1.join();
    }
    cout << endl;

    std::cout << "Main thread ends." << std::endl;

    return 0;
}