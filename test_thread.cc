#include <iostream>
#include <string>
#include <vector>
#include <iterator>

#include <chrono>
#include <assert.h>

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <unistd.h>
#include <numeric>

using namespace std;

void hello(string name) {
    // cout << "thread " << name <<" starts to run. pid=" << getpid() <<",tid=" << std::this_thread::get_id() << endl;
    cout << "thread " << name <<" starts to run. pid=" << getpid() <<",tid=" << std::this_thread::get_id() << endl;
    for (int i = 0; i<10000; ++i) {
        // cout << name <<": hello world " << i << endl;
    }
}

class background_task {
public:
    string name;
    background_task(string _name) : name(_name) {}
    void operator()()const{ //重载（）
        hello(name);
    }
};

void test0(){
    cout << "test0" << endl;
    background_task task1("task1");
    background_task task2("task2");
    thread t1(task1);
    thread t2(task2);
    // thread t3(hello()); // invalid, return object
    // thread t3{hello()}; // invalid
    thread t3(hello,"task3"); // 函数传参
    thread t4([]()->void {
        cout << "thread task4 starts to run." << endl;
        for (int i=0; i<1000; ++i) {
            // cout << "lambda " << i <<": kkkk" << endl;
        }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    if (t4.joinable()) {
        cout << "true" << endl;
    } else {
        cout << "false" << endl;
    }
    // t4.detach(); // main thread exit, t4 will be terminated
}

class thread_guard {
private:
    thread& t;
public:
    thread_guard(thread& _t) : t(_t) {
        cout << "thread_guard constructor" << endl;
    }
    ~thread_guard() {
        cout << "thread_guard destructor" << endl;
        if (t.joinable()) {
            t.join();
        }
    }
    thread_guard(const thread_guard&) = delete;
    thread_guard& operator=(const thread_guard&) = delete;
};

void test1(){
    background_task task1("task1");
    thread t1(task1);
    thread_guard g1(t1); // 包装之后，线程结束后自动join

    background_task task2("task2");
    thread t2(task2);
    thread_guard g2(t2); // 包装之后，线程结束后自动join
    // t1= t2; // 线程对象不可复制，编译报错, delete copy constructor
}

void test2() {
    background_task task1("task1");
    thread t1(task1);
    t1.detach(); // main thread exit, t will be terminated
    assert(!t1.joinable());
}

void func(int i,  string  & str) {
    cout << "i=" << i << " str=" << str << " &str=" << &str << endl;
    i = 10; // 无用
    str = "abc";
    for (int i=0; i<10000;i++){

    }
}

void test3() {
    int i = 3;
    string str = "hello world";
    cout << "test3 &str=" << &str << endl;
    // thread t(func, i, str); // thread构造函数会复制提供的变量，拷贝引用,除非使用str::ref
    thread t(func, i, std::ref(str)); // 线程会改变外部变量str
    thread_guard g(t); // 也可把参数放到thread_gurad里
    sleep(1);
    cout << "str=" <<str<< endl;
}

class X{
public:
    X(){}
    ~X(){}
    void func(int& j, string str){
        cout << "this is func() of X, str=" << str << " j=" << j << endl;
        j = 2;
        for(int i=0; i<10000; i++) {}
    }
};

void test4(){
    X x;
    int i = 1;
    thread t(&X::func, &x, std::ref(i), "hello"); // useful, 2是实例的地址
    thread_guard g(t);
    sleep(1);
    cout << "j=" << i << endl;
}

void func5(std::unique_ptr<string> msg){
    cout << "func5 msg=" << *msg << " msg.get()=" << msg.get() << endl;
    for(int i=0; i<10000; i++) {}
}

void test5() {
    std::unique_ptr<string> str(new string("hello world."));
    // cout << "before str:" << *str << endl;
    cout << "before str:" << str.get() << endl;
    thread t(func5, std::move(str));
    thread_guard g(t);
    // sleep(1);
    cout << "after str:" << str.get() << endl;
}

void func61(int i = 0){
    cout << "this is " << __FUNCTION__ << " i=" << i << endl;
    for(int i=0; i<10000; i++) {}
    sleep(1);
}

void func62(){
    cout << "this is " << __FUNCTION__ << endl;
    for(int i=0; i<10000; i++) {}
    sleep(1);
}

class scoped_thread{
public:
    thread t;
public:
    scoped_thread(thread _t) : t(move(_t)) {
        if (!t.joinable()) {
            throw std::logic_error("no thread");
        }
    }
    ~scoped_thread(){
        t.join();
    }
    scoped_thread(scoped_thread const&) = delete;
    scoped_thread& operator=(scoped_thread const&) = delete;
};
void test6(){
    // thread t1(func61);
    // thread t2 = move(t1); // t1归属权转移到t2
    // t1 = thread(func62); // 不用move
    // thread t3;
    // t3 = move(t2);
    // t1 = move(t3); // 终止整个程序，t1正在管理一个线程，简单赋值操作导致该线程被遗弃

    int i =10;
    scoped_thread t{thread(func61,i)}; // 转移所有权到t
    sleep(1);
}

class joining_thread{
public:
    thread t;
public:
    joining_thread() noexcept = default;
    template<typename Callable, typename... Args>
    explicit joining_thread(Callable&& func, Args&&... args)
        : t(forward<Callable>(func), forward<Args>(args)...) {
    }
    explicit joining_thread(thread t_) noexcept : t(move(t_)) {
    }
    joining_thread(joining_thread&& other) noexcept : t(move(other.t)) {}

    joining_thread& operator=(joining_thread&& other) noexcept {
        if (t.joinable()) {
            t.join();
        }
        t = move(other.t);
        return *this;
    }
    joining_thread& operator=(thread other) noexcept {
        if (t.joinable()) {
            t.join();
        }
        t = move(other);
        return *this;
    }
    ~joining_thread(){
        if(t.joinable()) {
            t.join();
        }
    }
    void swap(joining_thread& other) noexcept {
        t.swap(other.t);
    }
    thread::id get_id() const noexcept { return t.get_id(); }
    void detach() { t.detach(); }
    void join() {
        if(t.joinable()) {
            t.join();
        }
    }
    thread& as_thread() noexcept { return t; }
    const thread& as_thread() const noexcept { return t;}
};
void hello1(unsigned int id) {
    cout << "thread " << id <<" starts to run." << endl;
    for (int i = 0; i<10000; ++i) {
        // cout << name <<": hello world " << i << endl;
    }
}
void test7(){
    // vector<thread> threads;
    // for (unsigned int i=0; i < 20; ++i) {
    //     threads.emplace_back(hello1, i);
    // }
    // for (auto& t : threads) {
    //     t.join();
    // }

    // joining_thread(hello1, 1);
    vector<joining_thread> joining_threads(20);
    for (unsigned int i=0; i < 20; ++i) {
        joining_threads.emplace_back(hello1, i); // valid
        // joining_threads.emplace_back(joining_thread(hello1, i)); // valid
    }
    cout << "exit test7" << endl;
}

template<typename Iterator, typename T>
struct accumulate_block{
    void operator()(Iterator first, Iterator last, T& result){
        thread::id _tid = this_thread::get_id();
        cout << "thread_id:" << _tid << endl;
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T& init) {
    unsigned long const length = std::distance(first, last);
    if(!length) {
        return init;
    }
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread)/min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency(); // 真正并发数
    cout << "hardware_threads=" << hardware_threads << endl;
    unsigned long const num_threads = std::min(hardware_threads!=0?hardware_threads:2,max_threads);
    unsigned long const block_size = length / num_threads;
    cout << "num_threads=" << num_threads
         << " block_size=" << block_size << endl;
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads-1);
    Iterator block_start = first;
    for (unsigned long i=0; i < (num_threads - 1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size); // 前移block_size步
        threads[i] = std::thread(accumulate_block<Iterator,T>(),
                                block_start,
                                block_end,
                                std::ref(results[i]));
        block_start = block_end;
    }
    accumulate_block<Iterator,T>()(block_start, last, results[num_threads - 1]);
    cout << "results:" << endl;
    copy(results.begin(), results.end(), ostream_iterator<T>(cout ,","));
    cout << endl;
    for(auto& entry:threads) {
        entry.join();
    }
    init = std::accumulate(results.begin(), results.end(), init);
    return init;
}
void test8() {
    // vector<int> a = {1,2,3,4,5,6,7,8,9};
    // int sum_a = 0;
    // accumulate_block<vector<int>::iterator,int>()(a.begin(), a.end(), sum_a);
    // cout << "sum(a)=" << sum_a << endl;

    // vector<int> b = {1,2,3,4,5,6,7,8,9};
    vector<int> b = [](int count)->vector<int> {
        vector<int> v;
        v.resize(count);
        for (int i=0; i < count; ++i) {
            v[i] = i + 1;
        }
        return v;
    }(100);
    int sum = 0;
    parallel_accumulate(b.begin(), b.end(), sum);
    cout << "sum=" << sum << endl;
}
int main() {
    // test0();
    // test1();
    // test2();
    // test3(); // 传入函数指针
    // test4(); // 通过对象传入函数
    // test5(); // 移动参数，而不是复制
    // test6(); // 转移线程权限
    // test7(); // 使用joining_thread来管理线程权限
    test8();
    sleep(1);
    cout << "main thread exit" << endl;
}