#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <map>
#include <queue>

#include <chrono>
#include <assert.h>

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

    auto f7 = std::async(std::launch::async, Y(), 1.2); // 强制在新线程执行, async必须在新线程执行
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
    cout << "task3 result=" << f4.get() << endl; // 抛出异常，会被future捕捉, 不get则不会抛出
    t4.join();
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

std::shared_ptr<int> sp;
void test11() {
    auto getBool = [](bool i) { return i ? "true" : "false";};
    bool b1 = true; // set true to b1
    bool b2 = b1; // set b2 with true, not through b1
    // bool b4 = std::atomic_load(&b1); // invalid, b1 is not atomic
    // store: memory_order_relaxed, memory_order_release, memory_order_seq_cst
    // load: memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_seq_cst'
    // read-modify-write: memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_release, memory_order_acq_rel, memory_order_seq_cst
    // compare_exchange_weak: memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_release, memory_order_acq_rel, memory_order_seq_cst
    // compare_exchange_strong: memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_release, memory_order_acq_rel, memory_order_seq_cst

    // weak版本的CAS允许偶然出乎意料的返回（比如在字段值和期待值一样的时候却返回了false，并且没有将字段值设置成desire的值），不过在一些循环算法中，这是可以接受的。通常它比起strong有更高的性能。
    std::atomic<bool> b3{false};
    bool x = b3.load(std::memory_order_acquire); // 读取b3的值并返回
    cout << "x=b3.load(std::memory_order_acquire)=" << getBool(x) << endl;
    cout << "before b3=" << getBool(b3.load(std::memory_order_acquire)) << endl; // b3==false
    b3.store(true); // store
    cout << "b3=" << getBool(b3.load(std::memory_order_acquire)) << endl;
    x = b3.exchange(false, std::memory_order_acq_rel); // 存入false，返回原来的值true
    cout << "x=b3.exchange(false, std::memory_order_acq_rel)=" << getBool(x) << " b3=" << getBool(b3.load(std::memory_order_acq_rel)) << endl; // x==true
    // 当前x==true  b3==false，不相等不能修改，x被替换b3当前值，b3不变，返回false（代表这是一个替换操作
    cout << "before x=" << getBool(x) << " b3=" << getBool(b3.load(std::memory_order_acq_rel)) << endl;
    bool x1 = b3.compare_exchange_weak(x, true, std::memory_order_acq_rel);
    cout << "x1=b3.compare_exchange_weak(x, true, std::memory_order_acq_rel) [neuqal] x=" << getBool(x) << "(changed, just read b3) x1=" << getBool(x1) << "(ret false) b3=" << getBool(b3.load(std::memory_order_acq_rel)) << "(not change)" << endl; // x1==false
    // 当前x==false,b3==true，相等可以修改，b3修改为true，返回true（代表这是一个存入操作）
    cout << "before x=" << getBool(x) << " b3=" << getBool(b3.load(std::memory_order_acq_rel)) << endl;
    bool x2 = b3.compare_exchange_weak(x, true, std::memory_order_acq_rel);
    cout << "x2=b3.compare_exchange_weak(x, true, std::memory_order_acq_rel) [equal ] x=" << getBool(x) << "(not change) x2=" << getBool(x2) << "(ret true) b3=" << getBool(b3.load(std::memory_order_acq_rel)) << "(change) " << endl; // x2==true
    cout << endl;

    std::atomic<bool> b4{false};
    bool y = b4.load(std::memory_order_acquire); // 读取b3的值并返回
    cout << "y=b4.load(std::memory_order_acquire)=" << getBool(y) << endl;
    cout << "before b4=" << getBool(b4.load(std::memory_order_acquire)) << endl; // b4==false
    b4.store(true); // store
    cout << "b4.store(true)=" << getBool(b4.load(std::memory_order_acquire)) << endl; // b4==true
    y = b4.exchange(false, std::memory_order_acq_rel); // 存入false，返回原来的值true
    cout << "y=b4.exchange(false, std::memory_order_acq_rel)=" << getBool(y) << " b4=" << getBool(b4.load(std::memory_order_acq_rel)) << endl; // x==true
    // 当前x == true != b4 == false，不相等不能修改，x被替换b4当前值，b4不变，返回false（代表这是一个替换操作）
    cout << "before y=" << getBool(y) << " b4=" << getBool(b4.load(std::memory_order_acq_rel)) << endl;
    bool x3 = b4.compare_exchange_strong(y, true, std::memory_order_acq_rel);
    cout << "x3=b4.compare_exchange_strong(y, true, std::memory_order_acq_rel) [neuqal] y=" << getBool(y) << "(changed, just read b4) x3=" << getBool(x3) << "(ret false) b4=" << getBool(b4.load(std::memory_order_acq_rel)) << "(not change)" << endl; // x1==false
    // 当前x==false,b4==true，相等可以修改，b4修改为true，返回true（代表这是一个存入操作）
    cout << "before y=" << getBool(y) << " b4=" << getBool(b4.load(std::memory_order_acq_rel)) << endl;
    bool x4 = b4.compare_exchange_strong(y, true, std::memory_order_acq_rel);
    cout << "x4=b4.compare_exchange_strong(y, true, std::memory_order_acq_rel) [euqal ] y=" << getBool(y) << "(not change) x4=" << getBool(x4) << "(ret true) b4=" << getBool(b4.load(std::memory_order_acq_rel)) << "(change) " << endl; // x2==true
    cout << "b4.is_lock_free()=" << getBool(b4.is_lock_free()) << endl;
    cout << endl;

    // std::atomic<T*>
    class Foo{};
    Foo some_arry[5];
    std:atomic<Foo*> p(some_arry); // 初始化为some_arry,专门为地址类型设计的atomic,不初始化可能是任意地址
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

    sp.reset(new int(10));
    std::shared_ptr<int> local = std::atomic_load(&sp); // 读取sp的值并返回
    cout << "local=" << *local << endl;
    cout << "sp=" << *sp << endl;
    local.reset(new int(20));
    std::atomic_store(&sp, local); // 存入local，返回原来的值
    cout << "local=" << *local << endl;
    cout << "sp=" << *sp << endl;
    cout << "std::atomic_is_lock_free(&sp)=" << std::atomic_is_lock_free(&sp) << endl;
    cout << endl;
}

std::vector<int> data;
std::atomic<bool> data_ready(false);
std::atomic<bool> finish_job(false);
void foo(int a, int b) {
    cout << "foo: " << a << " " << b << endl;
}
int get_num(){
    static int i = 0;
    return ++i;
}
void test12() {
    std::thread t1([](){
        int cnt = 10;
        while(true) {
            if (!cnt) {
                break;
            }
            if (data_ready.load() == true) {
                continue;
            }
            data.push_back(cnt--);
            data_ready = true;
            // std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        finish_job.store(true);
        // std::atomic_store(&finish_job, true);
        cout << "t1 exit." << endl;
    });
    std::thread t2([](){
        while(true){
            if(data_ready.load() == true) {
                cout << "t2 receive: " << data.front() << endl;
                data.erase(data.begin());
                data_ready = false;
            }
            if (finish_job.load() == true) {
                while(data.size()) {
                    cout << "t2 receive: " << data.front() << endl;
                    data.erase(data.begin());
                }
                data_ready = false;
                break;
            }
            // std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        cout << "t2 exit." << endl;
    });
    t1.join();
    t2.join();
    cout << endl;

    foo(get_num(), get_num());
}

void test_memory_order_relaxed(){
    cout << "test_memory_order_relaxed start." << endl;
    unsigned const int loop_count = 10;
    std::atomic<int> x(0), y(0), z(0);
    std::atomic<bool> go(false);
    struct read_values{
        int x, y, z;
    };
    read_values values1[loop_count];
    read_values values2[loop_count];
    read_values values3[loop_count];
    read_values values4[loop_count];
    read_values values5[loop_count];
    auto print =[&loop_count](read_values* values, std::string str){
        cout << str << ": ";
        for (unsigned int i=0; i<loop_count; ++i) {
            if (i) {
                cout << ",";
            }
            cout << "(" << values[i].x << "," << values[i].y << "," << values[i].z << ")";
        }
        cout << endl;
    };
    auto increament = [&go, &loop_count,&x,&y,&z](std::atomic<int>* var_to_inc, read_values* values){
        while(!go) {
            std::this_thread::yield(); // 自旋，等待信号,函数用于使当前线程主动放弃CPU的执行权，将它的时间片（CPU时间）让给其他线程或进程。这个函数的目的是为了改善多线程程序的执行效率。
        }
        for(unsigned int i=0; i<loop_count; ++i) {
            values[i].x = x.load(std::memory_order_relaxed);
            values[i].y = y.load(std::memory_order_relaxed);
            values[i].z = z.load(std::memory_order_relaxed);
            var_to_inc->store(i+1, std::memory_order_relaxed);
            std::this_thread::yield();
        }
    };

    auto read_vals = [&go, &loop_count,&x,&y,&z](read_values* values){
        while(!go) {
            std::this_thread::yield(); // 自旋，等待信号
        }
        for(unsigned int i=0; i<loop_count; ++i) {
            values[i].x = x.load(std::memory_order_relaxed);
            values[i].y = y.load(std::memory_order_relaxed);
            values[i].z = z.load(std::memory_order_relaxed);
            std::this_thread::yield();
        }
    };
    std::thread t1(increament, &x, values1);
    std::thread t2(increament, &y, values2);
    std::thread t3(increament, &z, values3);
    std::thread t4(read_vals, values4);
    std::thread t5(read_vals, values5);
    go = true;
    t5.join();
    t4.join();
    t3.join();
    t2.join();
    t1.join();

    print(values1,"values1");
    print(values2,"values2");
    print(values3,"values3");
    print(values4,"values4");
    print(values5,"values5");

    cout << "test_memory_order_relaxed end." << endl;
    cout << endl;
}

void test_memory_order_release() {
    cout << "test_memory_order_release test start." << endl;
    std::atomic<bool> x(false), y(false);
    std::atomic<int> z(0);
    std::thread write_x_release([&x](){
        x.store(true, std::memory_order_release);
    });
    std::thread write_y_release([&y](){
        y.store(true, std::memory_order_release);
    });
    std::thread read_x_then_y_acquire([&x,&y,&z](){
        while(!x.load(std::memory_order_acquire)) {}
        if (y.load(std::memory_order_acquire)) {
            ++z;
        }
    });
    std::thread read_y_then_x_acquire([&x,&y,&z](){
        while(!y.load(std::memory_order_acquire)) {} // 严格按照顺序执行，不会出现x先于y
        if (x.load(std::memory_order_acquire)) {
            ++z;
        }
    });
    write_x_release.join();
    write_y_release.join();
    read_x_then_y_acquire.join();
    read_y_then_x_acquire.join();
    cout << "z=" << z << endl;
    cout << "test_memory_order_release test end." << endl;
    cout << endl;
}

void test_memory_order_release_1(){
    cout << "test_memory_order_release_1 test start." << endl;
    std::atomic<bool> x(false), y(false);
    std::atomic<int> z(0);
    std::thread write_x_then_y([&x,&y](){
        x.store(true, std::memory_order_relaxed);
        this_thread::sleep_for(std::chrono::milliseconds(1)); // 添加后z可能等于1，不等于2
        y.store(true, std::memory_order_release); // 不能放在x.store前面，否则可能出现y先于x,前面写操作不能延后
    });
    std::thread read_y_then_x([&x,&y,&z](){
        while(!y.load(std::memory_order_acquire)) {} // 自旋，严格按照顺序执行，x读不会先于y
        if (x.load(std::memory_order_relaxed)) {
            ++z;
        }
    });
    write_x_then_y.join();
    read_y_then_x.join();
    cout << "z=" << z << endl;
    cout << "test_memory_order_release_1 test end." << endl;
    cout << endl;
}

void test_memory_order_release_2(){
    cout << "test_memory_order_release_2 test start." << endl;
    std::atomic<int> data[5];
    std::atomic<bool> sync1(false), sync2(false);

    std::thread t1([&sync1](std::atomic<int>* pdata){
        pdata[0].store(42, std::memory_order_relaxed);
        pdata[1].store(97, std::memory_order_relaxed);
        pdata[2].store(17, std::memory_order_relaxed);
        pdata[3].store(-141, std::memory_order_relaxed);
        pdata[4].store(2003, std::memory_order_relaxed);
        sync1.store(true, std::memory_order_release); // set sync1 to true
    }, data);

    std::thread t2([&sync1,&sync2](){
        while(!sync1.load(std::memory_order_acquire)) {} // 自旋，等待sync1为true
        sync2.store(true, std::memory_order_release); // set sync2 to true
    });

    std::thread t3([&sync2](std::atomic<int>* pdata){
        while(!sync2.load(std::memory_order_acquire)) {} // 自旋，等待sync2为true
        if (pdata[0].load(std::memory_order_relaxed) == 42) {
            cout << "pdata[0].load(std::memory_order_relaxed) == 42" << endl;
        } else {
            cout << "pdata[0].load(std::memory_order_relaxed) != 42" << endl;
        }
        if (pdata[1].load(std::memory_order_relaxed) == 97) {
            cout << "pdata[1].load(std::memory_order_relaxed) == 97" << endl;
        } else {
            cout << "pdata[1].load(std::memory_order_relaxed) != 97" << endl;
        }
        if (pdata[2].load(std::memory_order_relaxed) == 17) {
            cout << "pdata[2].load(std::memory_order_relaxed) == 17" << endl;
        } else {
            cout << "pdata[2].load(std::memory_order_relaxed) != 17" << endl;
        }
        if (pdata[3].load(std::memory_order_relaxed) == -141) {
            cout << "pdata[3].load(std::memory_order_relaxed) == -141" << endl;
        } else {
            cout << "pdata[3].load(std::memory_order_relaxed) != -141" << endl;
        }
        if (pdata[4].load(std::memory_order_relaxed) == 2003) {
            cout << "pdata[4].load(std::memory_order_relaxed) == 2003" << endl;
        } else {
            cout << "pdata[4].load(std::memory_order_relaxed) != 2003" << endl;
        }
    }, data);

    t1.join();
    t2.join();
    t3.join();
    cout << "test_memory_order_release_2 test end." << endl;
    cout << endl;
}

void test_memory_order_release_2_1(){
    cout << "test_memory_order_release_2_1 test start." << endl;
    std::atomic<int> data[5];
    std::atomic<int> sync1(0);

    std::thread t1([&sync1](std::atomic<int>* pdata){
        pdata[0].store(42, std::memory_order_relaxed);
        pdata[1].store(97, std::memory_order_relaxed);
        pdata[2].store(17, std::memory_order_relaxed);
        pdata[3].store(-141, std::memory_order_relaxed);
        pdata[4].store(2003, std::memory_order_relaxed);
        sync1.store(1, std::memory_order_release); // set sync1 to true
    }, data);

    std::thread t2([&sync1](){
        int expected = 1;
        while(!sync1.compare_exchange_strong(expected, 2, std::memory_order_acq_rel)) {
            expected = 1;
        } // 自旋，等待sync1为true
    });

    std::thread t3([&sync1](std::atomic<int>* pdata){
        while(sync1.load(std::memory_order_acquire) < 2) {} // 自旋，等待sync2为true
        if (pdata[0].load(std::memory_order_relaxed) == 42) {
            cout << "pdata[0].load(std::memory_order_relaxed) == 42" << endl;
        } else {
            cout << "pdata[0].load(std::memory_order_relaxed) != 42" << endl;
        }
        if (pdata[1].load(std::memory_order_relaxed) == 97) {
            cout << "pdata[1].load(std::memory_order_relaxed) == 97" << endl;
        } else {
            cout << "pdata[1].load(std::memory_order_relaxed) != 97" << endl;
        }
        if (pdata[2].load(std::memory_order_relaxed) == 17) {
            cout << "pdata[2].load(std::memory_order_relaxed) == 17" << endl;
        } else {
            cout << "pdata[2].load(std::memory_order_relaxed) != 17" << endl;
        }
        if (pdata[3].load(std::memory_order_relaxed) == -141) {
            cout << "pdata[3].load(std::memory_order_relaxed) == -141" << endl;
        } else {
            cout << "pdata[3].load(std::memory_order_relaxed) != -141" << endl;
        }
        if (pdata[4].load(std::memory_order_relaxed) == 2003) {
            cout << "pdata[4].load(std::memory_order_relaxed) == 2003" << endl;
        } else {
            cout << "pdata[4].load(std::memory_order_relaxed) != 2003" << endl;
        }
    }, data);

    t1.join();
    t2.join();
    t3.join();
    cout << "test_memory_order_release_2_1 test end." << endl;
    cout << endl;
}

void test_memory_order_consume() {
    cout << "test_memory_order_consume test start." << endl;
    struct X {
    int i_;
    std::string s_;
    };

    std::atomic<int> a(1);
    std::atomic<X*> p(nullptr);
    auto create_x = [&p,&a](){
        X* x = new X;
        x->i_ = 42;
        x->s_ = "hello";
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        a.store(99, std::memory_order_relaxed);
        p.store(x, std::memory_order_release);
    };
    auto use_x = [&p,&a](){
        X* x;
        // p.load加载p中地址的值，若地址的值是nullptr，那么x也是nullptr，如果p中地址的值不是nullptr，那么x也不是nullptr
        // 有些机器会将std::memory_order_consume强制修改为acquire
        while (!(x = p.load(std::memory_order_consume))); // 不推荐用这个
        // while (!(x = p.load(std::memory_order_acquire))); // 推荐用这个
        if (x->i_ != 42) {
            cout << "x->i_ != 42" << endl;
        } else {
            cout << "x->i_ == 42" << endl;
        }
        if (x->s_ != "hello") {
            cout << "x->s_ != hello" << endl;
        } else {
            cout << "x->s_ == hello" << endl;
        }
        if (a.load(std::memory_order_relaxed) != 99) {
            cout << "a.load(std::memory_order_relaxed) != 99" << endl;
        } else {
            cout << "a.load(std::memory_order_relaxed) == 99" << endl;
        }
    };
    std::thread t1(create_x);
    std::thread t2(use_x);
    t1.join();
    t2.join();
    cout << "test_memory_order_consume test end." << endl;
    cout << endl;
}

void test_memory_order_cnt() {
    cout << "test_memory_order_cnt test start." << endl;
    std::vector<int> queue_data;
    std::atomic<int> count(0);
    std::atomic<bool> curr_job_done(false);

    std::thread t1([&queue_data, &count, &curr_job_done](){
        unsigned const int number_of_items = 100;
        queue_data.clear();
        for (unsigned int i=0; i<number_of_items; ++i) {
            queue_data.push_back(i);
        }
        count.store(number_of_items, std::memory_order_release);
        curr_job_done.store(true, std::memory_order_release);
    });
    auto consume_queue_items = [&queue_data, &count, &curr_job_done](string str){
        while(true) {
            int item_index;
            if((item_index = count.fetch_sub(1, std::memory_order_acquire)) <= 0) {
                if (curr_job_done.load(std::memory_order_acquire)) {
                    break;
                }
                // std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            cout << str << " queue_data[item_index-1]=" << queue_data[item_index-1] << endl;
        }
        cout << str << " exit." << endl;
    };
    std::thread t2(consume_queue_items, "t2");
    std::thread t3(consume_queue_items, "t3");
    t1.join();
    t2.join();
    t3.join();
    cout << "test_memory_order_cnt test end." << endl;
    cout << endl;
}

void test_memory_fences() {
    cout << "test_memory_fences test start." << endl;
    std::atomic<bool> x(false),y(false);
    std::atomic<int> z(0);

    cout << "atomic_thread_fence in middle." << endl;
    auto read_y_then_x = [&x,&y,&z](){
        try {
            while(!y.load(std::memory_order_relaxed)) {}
            std::atomic_thread_fence(std::memory_order_acquire); // load-load， 获取栅栏，与前面同步
            if (x.load(std::memory_order_relaxed)) {
                ++z;
            }
        } catch (std::exception& e) {
            cout << "error occurs." << endl;
            cout << e.what() << endl;
        }
    };

    std::thread write_x_then_y([&x,&y](){
        x.store(true, std::memory_order_relaxed);
        std::atomic_thread_fence(std::memory_order_release); // store-store, 释放栅栏，同步点， 就是栅栏本身
        y.store(true, std::memory_order_relaxed); // 即使使用memory_order_release，也会受到栅栏影响
    });
    std::thread t1(read_y_then_x);
    write_x_then_y.join();
    t1.join();
    if (z == 1) {
        cout << "z == 1" << endl;
    } else {
        cout << "z != 1" << endl;
    }

    cout << "=============" << z << endl;
    cout << "atomic_thread_fence in front." << endl;
    x = false;
    y = false;
    z = 0;
    std::thread write_x_then_y_1([&x,&y](){
        std::atomic_thread_fence(std::memory_order_release); // store-store, 释放栅栏，同步点， 就是栅栏本身
        x.store(true, std::memory_order_relaxed); // 栅栏已经释放，可能x先于y
        y.store(true, std::memory_order_relaxed); // 顺序不保证
    });
    std::thread t2(read_y_then_x);
    write_x_then_y_1.join();
    t2.join();
    if (z == 1) {
        cout << "z == 1" << endl;
    } else {
        cout << "z != 1" << endl;
    }

    cout << "test_memory_fences test end." << endl;
    cout << endl;
}

void test_atomic_operate_order() {
    cout << "test_atomic_operate_order test start." << endl;
    bool x = false; // non atomic variance!!!!!
    std::atomic<bool> y(false);
    std::atomic<int> z(0);
    std::thread write_x_then_y([&x,&y](){
        x = true;
        std::atomic_thread_fence(std::memory_order_release); // 提供一个序列
        y.store(true, std::memory_order_relaxed);
    });
    std::thread read_y_then_x([&x,&y,&z](){
        while(!y.load(std::memory_order_relaxed)){}
        std::atomic_thread_fence(std::memory_order_acquire); // 此时x已经设置完毕，x不存在条件竞争。前后有序
        if (x) {
            ++z;
        }
    });
    write_x_then_y.join();
    read_y_then_x.join();
    cout << "z=" << z << endl;
    cout << "test_atomic_operate_order test end;" << endl;
}
void test13(){
    // 三种内存模型
    // 排序一致序列(sequentially consistent):
    // 1. memory_order_seq_cst // 顺序一致性，所有线程都能看到相同的操作顺序，所有操作都是原子的，所有操作都不重排
    // 获取-释放序列：
    // 1. memory_order_release // 写入操作，前面的写不能延后
    // 2. memory_order_acquire // 读取操作，后面的读不能提前
    // 3. memory_order_consume // 读取操作，与该原子有关的依赖不能提前，于该原子无关的依赖可以提前 不应该出现在你的代码中。
    // 4. memory_order_acq_rel // qcuire和release的合并
    // 松散序列：
    // 1. memory_order_relaxed // 加不加不影响
    // 分层协议
    // c++ 11 6 种 memory order，本身只是C++给出的约定，至于到底按照哪种方式实现6种内存序，由编译器和硬件决定
    // 编译器
    // cpu硬件 cache的一致性
    // memory barrier, memory fence，对内存序列有约束，影响线程内其他原子操作，与memory_order_relaxed一起使用
    vector<int> data;
    std::atomic<bool> data_ready(false);
    std::thread t1([&data_ready,&data](){
        while(!data_ready.load()){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        cout << "t1 receive: " << data[0] << endl;
        cout << "t1 exit" << endl;
    });
    std::thread t2([&data_ready,&data](){
        data.push_back(42);
        data_ready = true;
        cout << "t2 exit" << endl;
    });
    t1.join();
    t2.join();
    cout << endl;

    // memory_order_seq_cst cost is high
    cout << "memory_order_seq_cst test start." << endl;
    std::atomic<bool> x(false);
    std::atomic<bool> y(false);
    std::atomic<int> z(0);
    std::thread write_x([&x](){
        x.store(true, std::memory_order_seq_cst);
        cout << "write x=" << x << endl;
    });
    std::thread write_y([&y](){
        // this_thread::sleep_for(std::chrono::milliseconds(1)); // 添加后z可能等于1，不等于2
        y.store(true, std::memory_order_seq_cst);
        cout << "write y=" << y << endl;
    });
    std::thread read_x_then_y([&x,&y,&z](){
        while(x.load(std::memory_order_seq_cst) == false) {}
        cout << "111" << endl;
        if (y.load(std::memory_order_seq_cst) == true) {
            ++z;
        }
    });
    std::thread read_y_then_x([&x,&y,&z](){
        while(y.load(std::memory_order_seq_cst) == false) {}
        cout << "222" << endl;
        if (x.load(std::memory_order_seq_cst) == true) {
            ++z;
        }
    });
    write_x.join();
    write_y.join();
    read_x_then_y.join();
    read_y_then_x.join();
    cout << "z=" << z << endl;
    z = 0;
    x = false;
    y = false;
    cout << "memory_order_seq_cst test end." << endl;
    cout << endl;

    // memory_order_relaxed cost is low
    cout << "memory_order_relaxed test start." << endl;
    std::thread read_y_then_x_relaxed([&x,&y,&z](){
        while(y.load(std::memory_order_relaxed) == false) {}
        if (x.load(std::memory_order_relaxed) == true) {
            ++z;
        }
    });
    std::thread write_x_then_y_relaxed([&x,&y](){
        x.store(true, std::memory_order_relaxed);
        y.store(true, std::memory_order_relaxed); // 可能乱序
        // cout << "write x=" << x << endl;
    });
    read_y_then_x_relaxed.join();
    write_x_then_y_relaxed.join();
    cout << "z=" << z << endl; // 可能等于1，可能等于2
    z = 0;
    x = false;
    y = false;
    cout << "memory_order_relaxed test end." << endl;
    cout << endl;

    test_memory_order_relaxed();
    test_memory_order_release();
    test_memory_order_release_1();
    test_memory_order_release_2(); // release relaxed
    test_memory_order_release_2_1(); // release relaxed compare_exchange_strong
    test_memory_order_consume();
    test_memory_order_cnt(); // example count, use memory_order_release memory_order_acquire
    test_memory_fences(); // memory fences
    test_atomic_operate_order(); // atomic operate order
}

void test14() {
    std::atomic<bool> done_flag(false);
    std::thread t1([&done_flag](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout << "t1 exit." << endl;
    });
    std::thread t2([&done_flag](){
        done_flag.store(true);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout << "t2 exit." << endl;
    });
    t1.join(); // 阻塞
    t2.join();
    if (!done_flag.load()) { // 必然在store之后，因为store是原子操作，但是怎么保证到这里时线程都执行完毕了呢
        cout << "done_flag is false." << endl;
    } else {
        cout << "done_flag is true." << endl;
    }
    cout << endl;
    
}

int main() {
    // test1(); // test condition_variable
    // test2(); // test threadsafe queue
    // test3(); // future async, 一个future只能get一次, asynck可能是硬件并发最好的
    // test4(); // packaged_task
    // test5(); // std::promise
    // test6(); // shared_future，多个线程共享一个future，
    // test7(); // test chrono
    // test8(); // quick sort
    // test9(); // CSP Communicating Sequential Processer 通信顺序处理 actor model
    // test10(); // atomic flag
    // test11(); // atomic 要求不能拷贝构造或拷贝赋值，因赋值和拷贝调用了两个对象，这就就破坏了操作的原子性
    // test12(); // atomic 强制排序
    // test13(); // memory order
    test14(); // std::atomic<bool> done_flag(false);
    return 0;
}
