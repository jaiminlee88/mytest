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
// #include <experimental/future>

#include "myutils.hh"

using namespace std;

std::mutex mlock;
std::queue<int> data_queue;
std::condition_variable data_cond;
bool job_done = false;
utils_log_level g_utils_log_level = utils_log_level::DEBUG;

template<typename T>
class threadsafe_queue {
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue() {}
    void push(T new_value) {
        // // 方案1
        // std::lock_guard<std::mutex> lk(mut);
        // data_queue.push(std::move(new_value));
        // data_cond.notify_one(); // 若是有一个线程抛出异常，那么其他线程就会永远等待
        // 方案2
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(std::move(new_value));
        data_cond.notify_all(); // 通知所有线程，能解决抛出异常问题，但是性能会差一些
        // // 方案3 使用shared_ptr
        // std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value))); // 内存不够会在这报错
        // std::lock_guard<std::mutex> lk(mut); // 一定要写在这里，否则可能会死锁
        // data_queue.push(data);
        // data_cond.notify_one();
    }
    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this](){ return !data_queue.empty(); }); // wait挂起，直到lambda返回true
        value = std::move(data_queue.front());
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this](){ return !data_queue.empty(); }); // 阻塞，比empty好一些
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
        // try { // make_shared可能因内存不足抛出异常, 可以试着通知其他线程
        //     std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        //     data_queue.pop();
        //     return res;
        // } catch(...) {
        //     data_cond.notify_one();
        //     return std::shared_ptr<T>();
        // }
    }
    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty()) return false;
        value = std::move(data_queue.front());
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop(){
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty()) {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> res = std::make_shared<T>(std::move(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool empty() const {
        std::lock_guard<std::mutex> lk(mut); // 连续调用加锁解锁，性能不好
        return data_queue.empty();
    }
};


class thread_pool {
private:
    std::atomic_bool done; // std::atomic<bool>
    threadsafe_queue<std::function<void(void)>> work_queue; // 任务队列,不需要任何返回值，不用等待任务完成
    std::vector<std::thread> threads;
    utils_join_threads joiner;
    void worker_thread() {
        while(!done) {
            std::function<void(void)> task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield(); // 线程主动放弃时间片
            }
        }
    }
public:
    thread_pool() : done(false), joiner(threads) {
        unsigned const thread_count = std::thread::hardware_concurrency(); // 硬件支持的线程数量
        try {
            for (unsigned i=0; i< thread_count; ++i) {
                threads.push_back(std::thread(&thread_pool::worker_thread, this));
            }
        } catch (...) {
            done = true;
            throw;
        }
    }
    ~thread_pool() {
        done = true;
    }
    template<typename FunctionType>
    void submit(FunctionType f) {
        work_queue.push(std::function<void(void)>(f)); // 把f对象包装成一个std::function<void(void)>对象
    }
};
void test1() {
    thread_pool work_pool;
    work_pool.submit([](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "f1, hello world");
    }
    );
    work_pool.submit([](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "f2, hello world");
    }
    );
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

class function_wrapper {
private:
    struct impl_base {
        virtual void call()=0;
        virtual ~impl_base() {}
    };
    std::unique_ptr<impl_base> impl;

    template<typename F>
    struct impl_type : impl_base {
        F f; // member
        impl_type(F &&f_) : f(std::move(f_)) {} // move constructor
        void call() { f(); }
    };

public:
    template<typename F>
    function_wrapper(F &&f) : impl(new impl_type<F>(std::move(f))) {} // 构建一个impl_type对象
    function_wrapper(function_wrapper &&other) : impl(std::move(other.impl)) {} // move constructor
    function_wrapper() = default; // 默认构造函数 不能覆盖
    function_wrapper(const function_wrapper &other) = delete; // 禁止拷贝构造函数
    function_wrapper(function_wrapper &other) = delete; // 禁止拷贝构造函数
    function_wrapper& operator=(function_wrapper &&other) {
        impl = std::move(other.impl);
        return *this;
    }
    function_wrapper& operator=(const function_wrapper &other) = delete; // 禁止拷贝赋值函数
    void operator()() { impl->call(); } // 外部调用impl_type对象的call方法
};
class thread_pool_funcwrapper {
private:
    std::atomic_bool done;
    threadsafe_queue<function_wrapper> work_queue; // not use std::function
    std::vector<std::thread> threads;
    utils_join_threads joiner; // 析构后自动join
    void worker_thread() {
        while(!done) {
            function_wrapper task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }
    void worker_thread_pending() {
        while(!done) {
            function_wrapper task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }
public:
    thread_pool_funcwrapper()=delete;
    thread_pool_funcwrapper(std::string worker="") : done(false), joiner(threads) {
        unsigned const thread_count = std::thread::hardware_concurrency(); // 硬件支持的线程数量
        try {
            for (unsigned i=0; i< thread_count; ++i) {
                threads.push_back(std::thread(&thread_pool_funcwrapper::worker_thread, this));
            }
        } catch (...) {
            done = true;
            throw;
        }
    }
    ~thread_pool_funcwrapper() {
        done = true;
    }
    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f) {
        typedef typename std::result_of<FunctionType()>::type result_type; // f没有参数
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future()); // 使用使用这种方式,需要任务不能太小,否则效率不高
        work_queue.push(std::move(task));
        return res; // 返回future，等待任务完全计数
    }
};

void test2 (){
    thread_pool_funcwrapper work_pool(""); // 可以直接调用
    std::function<void(void)> f1 = []() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "f1, hello world");
        return;
    };
    std::function<void(void)> f2 = []() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "f2, hello world");
        return;
    };
    std::future<void> ret1 = work_pool.submit(f1);
    std::future<void> ret2 = work_pool.submit(f2);
    cout << "wait..." << endl;
    ret1.get(); // 等待任务完成, block
    ret2.get(); // 等待任务完成, block
    cout << "done..." << endl;
}

class thread_pool_funcwrapper_pending {
private:
    std::atomic_bool done;
    threadsafe_queue<function_wrapper> work_queue; // not use std::function
    std::vector<std::thread> threads;
    utils_join_threads joiner; // 析构后自动join
    // void worker_thread() {
    //     while(!done) {
    //         function_wrapper task;
    //         if (work_queue.try_pop(task)) {
    //             task();
    //         } else {
    //             std::this_thread::yield();
    //         }
    //     }
    // }
public:
    void run_pending_task() {
        while(!done) {
            function_wrapper task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                if (work_queue.empty())
                    return;
                else
                    std::this_thread::yield();
            }
        }
    }
    thread_pool_funcwrapper_pending()=delete;
    thread_pool_funcwrapper_pending(std::string worker="") : done(false), joiner(threads) {
        // unsigned const thread_count = std::thread::hardware_concurrency(); // 硬件支持的线程数量
        // try {
        //     for (unsigned i=0; i< thread_count; ++i) {
        //         threads.push_back(std::thread(&thread_pool_funcwrapper_pending::worker_thread, this));
        //     }
        // } catch (...) {
        //     done = true;
        //     throw;
        // }
    }
    ~thread_pool_funcwrapper_pending() {
        done = true;
    }
    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f) {
        typedef typename std::result_of<FunctionType()>::type result_type; // f没有参数
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future()); // 使用使用这种方式,需要任务不能太小,否则效率不高
        work_queue.push(std::move(task));
        return res; // 返回future，等待任务完全计数
    }
};
template<typename T>
struct sorter {
    /*
    // 需要注意的是，对于某些自定义类型（比如thread_pool_funcwrapper），
    // 如果该类型没有提供默认构造函数或默认的拷贝/移动构造函数，
    // 或者需要进行更复杂的初始化逻辑，那么只能通过构造函数的成员初始化列表来对其进行初始化
    // thread_pool_funcwrapper pool("pending"); // invalid 特别注意,此处不能直接初始化
    */
    thread_pool_funcwrapper_pending pool; // 特别注意,此处不能直接初始化
    sorter(std::string str="") : pool(str) {}
    std::list<T> do_sort(std::list<T>& chunk_data) {
        if (chunk_data.empty()) {
            return chunk_data;
        }
        std::list<T> result;
        result.splice(result.begin(), chunk_data, chunk_data.begin()); // 把第一个元素移动到result
        T const& partial_val = *result.begin(); // 取出该元素
        typename std::list<T>::iterator divide_point = std::partition( // 以partial_val为中心，把chunk_data分成两部分
                                                        chunk_data.begin(),
                                                        chunk_data.end(),
                                                        [&](T const& val){
                                                            return val < partial_val;});
        std::list<T> new_lower_chunk;
        new_lower_chunk.splice(new_lower_chunk.end(), chunk_data, chunk_data.begin(), divide_point); // 把小于partial_val的元素移动到new_lower_chunk
        // 不断拆分后,放入新任务,新任务不断执行,外部get等待任务完成
        std::future<std::list<T>> new_lower = pool.submit(std::bind(&sorter::do_sort, this, std::move(new_lower_chunk)));
        std::list<T> new_higer(do_sort(chunk_data));
        result.splice(result.end(), new_higer);

        while (new_lower.wait_for(std::chrono::seconds(0)) == std::future_status::timeout) {
            pool.run_pending_task();
        }
        result.splice(result.begin(), new_lower.get());
        // for (auto& it : result) {cout << it << " ";} cout << endl;
        return result;
    }
};

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
    if (input.empty()) {
        return input;
    }
    sorter<T> s;
    return s.do_sort(input);
}

void test3() {
    // std::list<int> input = { 7, 6, 9, 8, 1, 3, 2, 5, 4};
    std::list<int> input;
    int max_num = 100;
    for (int i=max_num; i>0; --i) {
        input.push_back(i);
    }
    cout << "origin: ";
    for (auto& it : input) {cout << it << " ";} cout << endl;

    std::list<int> output = parallel_quick_sort(input);
    utils_print_copy_decorate(output);
}

class thread_pool_funcwrapper_localqueue { // 解决pingpong问题
private:
    threadsafe_queue<function_wrapper> work_queue; // not use std::function
    typedef std::queue<function_wrapper> local_queue_type;
    static thread_local std::unique_ptr<local_queue_type> local_work_queue; // 每个线程都有一个local_work_queue

    std::atomic_bool done;
    std::vector<std::thread> threads;
    utils_join_threads joiner; // 析构后自动join
    void worker_thread() {
        local_work_queue.reset(new local_queue_type); // 每个线程都有一个local_work_queue
        if (local_work_queue) {
            cout << "111111" << endl;
        }
        while(!done) {
            run_pending_task();
        }
    }
public:
    thread_pool_funcwrapper_localqueue()=delete;
    thread_pool_funcwrapper_localqueue(std::string worker="", unsigned thread_count=0) : done(false), joiner(threads) {
        if (!thread_count) {
            thread_count = std::thread::hardware_concurrency(); // 硬件支持的线程数量
        }
        try {
            for (unsigned i=0; i< thread_count; ++i) {
                threads.push_back(std::thread(&thread_pool_funcwrapper_localqueue::worker_thread, this));
            }
        } catch (...) {
            done = true;
            throw;
        }
    }
    ~thread_pool_funcwrapper_localqueue() {
        done = true;
    }
    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f) {
        typedef typename std::result_of<FunctionType()>::type result_type; // f没有参数
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future()); // 使用使用这种方式,需要任务不能太小,否则效率不高
        if (local_work_queue) {
            cout << "aaaa" << endl;
            local_work_queue->push(std::move(task)); // 本地任务队列
        }else {
            work_queue.push(std::move(task)); // 全局任务队列
        }
        return res; // 返回future，等待任务完全计数
    }
    void run_pending_task() {
        function_wrapper task;
        if (local_work_queue && !local_work_queue->empty()) { // 如果本地任务队列不为空
            task = std::move(local_work_queue->front());
            local_work_queue->pop();
            task();
        } else if (work_queue.try_pop(task)) {
            task();
        } else {
            std::this_thread::yield();
        }
    }
};
thread_local std::unique_ptr<thread_pool_funcwrapper_localqueue::local_queue_type> thread_pool_funcwrapper_localqueue::local_work_queue;

void test4() {
    thread_pool_funcwrapper_localqueue work_pool("",3); // 可以直接调用
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::vector<std::future<void>> rets;
    int t_cnt = 100;
    for (unsigned i=0; i < t_cnt; ++i) {
        std::function<void(void)> f = [=]() { // 不要引用
            std::string str = "f" + std::to_string(i);
            // std::this_thread::sleep_for(std::chrono::seconds(1));
            std::thread::id id = std::this_thread::get_id();
            utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "id:" << id << " " << str << ", hello world");
            return;
        };
        std::future<void> ret = work_pool.submit(f);
        // std::future<void> ret = work_pool.submit([i]() { // 不要引用
        //     std::this_thread::sleep_for(std::chrono::seconds(1));
        //     utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "id:" << std::thread::get_id() + str +", hello world");
        // });
        rets.push_back(std::move(ret));
    }
    cout << "wait..." << endl;
    for (auto& it : rets) {
        it.get();
    }
    cout << "done..." << endl;
}

class work_strealing_queue {
private:
    typedef function_wrapper data_type;
    std::deque<data_type> the_queue;
    mutable std::mutex the_mutex;
public:
    work_strealing_queue() {}
    work_strealing_queue(const work_strealing_queue& other)=delete;
    work_strealing_queue& operator=(const work_strealing_queue& other)=delete;

    void push(data_type data) {
        std::lock_guard<std::mutex> lock(the_mutex);
        the_queue.push_front(std::move(data));
    }
    bool empty() {
        std::lock_guard<std::mutex> lock(the_mutex);
        return the_queue.empty();
    }
    bool try_pop(data_type& res) {
        std::lock_guard<std::mutex> lock(the_mutex);
        if(the_queue.empty()) {
            return false;
        }
        res = std::move(the_queue.front());
        the_queue.pop_front();
        return true;
    }
    bool try_steal(data_type& res) {
        utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "9999");
        std::lock_guard<std::mutex> lock(the_mutex);
        utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "11111");
        if(the_queue.empty()) {
            utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "00000");
            return false;
        }
        utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "2222");
        res = std::move(the_queue.back());
        utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "33333");
        the_queue.pop_back();
        utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "44444");
        return true;
    }
};
class thread_pool_funcwrapper_workstealing {
public:
    typedef function_wrapper task_type;
private:
    std::atomic_bool done{false};
    threadsafe_queue<task_type> pool_work_queue;
    std::vector<std::unique_ptr<work_strealing_queue>> queues;
    std::vector<std::thread> threads;
    utils_join_threads joiner;
    static thread_local work_strealing_queue* local_work_queue; // 每个线程都有一个local_work_queue
    static thread_local unsigned my_index;

    void worker_thread(unsigned my_index_) {
        my_index = my_index_;
        local_work_queue = queues[my_index].get();
        while(!done) {
            run_pending_task();
        }
    }
    bool pop_task_from_local_queue(task_type& task) {
        return local_work_queue && local_work_queue->try_pop(task);
    }
    bool pop_task_from_pool_queue(task_type& task) {
        return pool_work_queue.try_pop(task);
    }
    bool pop_task_from_other_thread_queue(task_type& task) {
        for (unsigned i=0; i<queues.size(); ++i) {
            unsigned const index = (my_index + i + 1) % queues.size();
            utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "aaaa->" + std::to_string(index));
            // if (queues[my_index] == nullptr) {
            //     utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "!!!!" +std::to_string(index) + "null");
            // }
            if (queues[my_index]->try_steal(task)) {
                utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "bbbb")
                return true;
            }
        }
        utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "cccc");
        return false;
    }
public:
    thread_pool_funcwrapper_workstealing(unsigned thread_count=0) : done(false), joiner(threads) {
        if (!thread_count) {
            thread_count = std::thread::hardware_concurrency(); // 硬件支持的线程数量
        }
        try {
            for (unsigned i=0; i< thread_count; ++i) {
                queues.push_back(std::unique_ptr<work_strealing_queue>(new work_strealing_queue));
                threads.push_back(std::thread(&thread_pool_funcwrapper_workstealing::worker_thread, this, i));
            }
        } catch (...) {
            done = true;
            throw;
        }
    };
    ~thread_pool_funcwrapper_workstealing() {
        done = true;
    }

    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f) {
        typedef typename std::result_of<FunctionType()>::type result_type; // f没有参数
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future()); // 使用使用这种方式,需要任务不能太小,否则效率不高
        if (local_work_queue) {
            local_work_queue->push(std::move(task)); // 本地任务队列
        }else {
            pool_work_queue.push(std::move(task)); // 全局任务队列
        }
        return res; // 返回future，等待任务完全计数
    }

    void run_pending_task() {
        task_type task;
        if (pop_task_from_local_queue(task) || pop_task_from_pool_queue(task) || pop_task_from_other_thread_queue(task)) {
            task();
        } else {
            std::this_thread::yield();
        }
    }
};
thread_local work_strealing_queue* thread_pool_funcwrapper_workstealing::local_work_queue;
thread_local unsigned thread_pool_funcwrapper_workstealing::my_index;

void test5() {
    cout << __FUNCTION__ << " start..." << endl;
    thread_pool_funcwrapper_workstealing work_pool(0); // 可以直接调用,注意不要主动设置
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::vector<std::future<void>> rets;
    int t_cnt = 100;
    for (unsigned i=0; i < t_cnt; ++i) {
        std::function<void(void)> f = [=]() { // 不要引用
            std::string str = "f" + std::to_string(i);
            // std::this_thread::sleep_for(std::chrono::seconds(1));
            std::thread::id id = std::this_thread::get_id();
            utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "id:" << id << " " << str << ", hello world");
            return;
        };
        std::future<void> ret = work_pool.submit(f);
        // std::future<void> ret = work_pool.submit([i]() { // 不要引用
        //     std::this_thread::sleep_for(std::chrono::seconds(1));
        //     utils_threadsafe_print_level(utils_log_level::INFO, g_utils_log_level, mlock, "id:" << std::thread::get_id() + str +", hello world");
        // });
        rets.push_back(std::move(ret));
    }
    cout << __FUNCTION__ << "wait..." << endl;
    for (auto& it : rets) {
        it.get();
    }
    cout << __FUNCTION__ << "done..." << endl;

}


class interrupt_flag {
private:
    std::atomic_bool flag;
    std::condition_variable* thread_cond;
    std::condition_variable_any* thread_cond_any;
    std::mutex set_clear_mutex;
public:
    interrupt_flag() : flag(false), thread_cond(nullptr), thread_cond_any(nullptr) {}
    void set() {
        flag.store(true, std::memory_order_relaxed);
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        if (thread_cond) {
            thread_cond->notify_all();
        } else if (thread_cond_any) {
            thread_cond_any->notify_all();
        }
    }
    bool is_set() const {
        return flag.load(std::memory_order_relaxed);
    }
    void set_condition_variable(std::condition_variable& cv) {
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        thread_cond = &cv;
    }
    void clear_condition_variable() {
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        thread_cond = nullptr;
    }
    struct clear_cv_on_destruct {
        ~clear_cv_on_destruct() { // 退出时会执行
            this_thread_interrupt_flag.clear_condition_variable();
        }
    };
};
thread_local interrupt_flag this_thread_interrupt_flag; // 每个线程都有一个interrupt_flag对象

// 判断是否已经被中断
void interruption_point() {
    if (this_thread_interrupt_flag.is_set()) {
        throw std::runtime_error("interrupted"); // throw thread_interrupted()
    }
}
template<typename Predicate>
void interruptible_wait(std::condition_variable& cv, std::unique_lock<std::mutex>& lk, Predicate pred) {
    interruption_point();
    this_thread_interrupt_flag.set_condition_variable(cv); // 传入外部cv
    interrupt_flag::clear_cv_on_destruct guard; // 退出时执行析构函数
    while(!this_thread_interrupt_flag.is_set() && !pred()){ // 过多检查
        cv.wait_for(lk,std::chrono::milliseconds(1));
    }
    interruption_point();
}
template<typename Lockable>
void interruptible_wait(std::condition_variable_any& cv, Lockable& lk) {
    this_thread_interrupt_flag.wait(cv,lk);
}
template<typename Lockable>
void interruptible_wait_any(std::condition_variable_any& cv, Lockable& lk) {
    struct custom_lock {
        interrupt_flag* self;
        Lockable& lk;
        custom_lock(interrupt_flag* self_, std::condition_variable_any& cond, Lockable& lk_) : self(self_), lk(lk_) {
            self->set_clear_mutex.lock();
            self->thread_cond_any = &cond;
        }
        void unlock() {
            lk.unlock();
            self->set_clear_mutex.unlock();
        }
        void lock() {
            std::lock(self->set_clear_mutex,lk);
        }
        ~custom_lock() {
            self->thread_cond_any = nullptr;
            self->set_clear_mutex.unlock();
        }
    };
    custom_lock cl(this, cv, lk);
    interruption_point();
    cv.wait(cl);
    interruption_point();
}


class interruptible_thread {
private:
    std::thread internal_thread;
    interrupt_flag* flag; // 中断结构
public:
    template<tupeanme FunctionType>
    interruptible_thread(FunctionType f) {
        std::promise<interrupt_flag*> p;
        internal_thread = std::thread([f,&p](){
            p.set_value(&this_thread_interrupt_flag); // 子线程独有的interrupt_flag
            f(); // 子线程执行的函数
        });
        flag = p.get_future().get();
    }
    void join() { internal_thread.join(); }
    void detach() { internal_thread.detach(); }
    bool joinable() const {return internal_thread.joinable();}
    void interrupt() {
        if (flag) {
            flag->set(); // 设置中断标志，当前中断被使用
        }
    }
};

class thread_pool_funcwrapper_interrupt {
private:
    std::atomic_bool done;
    threadsafe_queue<function_wrapper> work_queue; // not use std::function
    std::vector<interruptible_thread> threads;
    utils_join_threads joiner; // 析构后自动join
    void worker_thread() {
        while(!done) {
            function_wrapper task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }
    void worker_thread_pending() {
        while(!done) {
            function_wrapper task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }
public:
    thread_pool_funcwrapper_interrupt()=delete;
    thread_pool_funcwrapper_interrupt(std::string worker="") : done(false), joiner(threads) {
        unsigned const thread_count = std::thread::hardware_concurrency(); // 硬件支持的线程数量
        try {
            for (unsigned i=0; i< thread_count; ++i) {
                threads.push_back(interruptible_thread(worker_thread));
            }
        } catch (...) {
            done = true;
            throw;
        }
    }
    ~thread_pool_funcwrapper_interrupt() {
        done = true;
    }
    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f) {
        typedef typename std::result_of<FunctionType()>::type result_type; // f没有参数
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future()); // 使用使用这种方式,需要任务不能太小,否则效率不高
        work_queue.push(std::move(task));
        return res; // 返回future，等待任务完全计数
    }
};

int main() {
    g_utils_log_level = utils_log_level::DEBUG;
    // 线程池考虑场景：
    // 硬件资源，可使用的线程数量，
    // 任务复杂度，高效的任务
    // 任务依赖关系，分配方式，以及是否需要等待一个任务完成
    // test1(); // 任务无依赖关系，不需要等待任务完成
    // test2(); // 任务无依赖关系，需要等待任务完成
    // test3(); // 任务有依赖关系，需要等待任务完成
    // test4(); // 解决pingpong问题，但该代码不工作！！！！！！！！！！！！！！
    // test5(); // 窃取任务，但代码内存泄露！！！！！！！！！！！！！！！！！

}