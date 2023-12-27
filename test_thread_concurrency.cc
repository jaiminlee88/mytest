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

std::mutex print_lock;
enum log_level {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
};
log_level g_log_level = DEBUG; // current log level
#define threadsafe_print(str) { \
std::lock_guard<std::mutex> lock(print_lock); \
std::cout << str << std::endl; \
}
#define threadsafe_print_level(level, str) \
if (g_log_level <= level) { \
    std::lock_guard<std::mutex> lock(print_lock); \
    std::cout << str << std::endl; \
}

template<typename T>
class threadsafe_stack{
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack(){}
    threadsafe_stack(const threadsafe_stack& other) {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }
    threadsafe_stack& operator=(const threadsafe_stack&)=delete;
    void push(T new_value) {
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value)); // 内存不足可能抛出异常
    }
    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lock(m);
        // if (data.empty()) throw empty_stack();
        if (data.empty()) return std::shared_ptr<T>(nullptr);
        std::shared_ptr<T> const res(std::make_shared<T>(std::move(data.top()))); //make_shared可能因内存不足抛出异常
        data.pop();
        return res;
    }
    void pop(T& value) {
        std::lock_guard<std::mutex> lock(m);
        // if (data.empty()) throw empty_stack;
        if (data.empty()) return;
        value = std::move(data.top());
        data.pop();
    }
    bool empty() const {
        std::lock_guard<std::mutex> lock(m); //不断等待锁，直到获得锁，序列化线程访问，性能不好
        return data.empty();
    }
};

template<typename T>
class lock_free_stack {
private:
    struct node {
        std::shared_ptr<T> data;
        node* next;
        node(T const& data_) : data(std::make_shared<T>(data_)) {}
    };
    std::atomic<node*> head{nullptr};
    std::atomic<unsigned> threads_in_pop{0}; // 原子操作计数器，记录有几个线程在pop
    std::atomic<node*> to_be_deleted{nullptr};

    static void delete_nodes(node* nodes) {
        while(nodes) {
            node* next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }
    void chain_pending_nodes(node* nodes) { // 从头到尾
        node* last = nodes;
        while(node* const next = last->next) {
            last = next;
        } // 找到最后一个节点
        chain_pending_nodes(nodes, last); // 重新链接回去，to_be_deleted指向链表头
    }
    void chain_pending_nodes(node* first, node* last) {
        last->next = to_be_deleted;
        while(!to_be_deleted.compare_exchange_weak(last->next, first)){} // 不断尝试把last->next占用to_be_deleted，然后把to_be_deleted赋值为first
    }
    void chain_pending_node(node* n) { ///////
        chain_pending_nodes(n,n);
    }
    void try_reclaim(node* old_head) {
        if (old_head == nullptr) return;
        if (threads_in_pop == 1) { // 等于1，说明只有一个线程在pop，此时是删除的最好时机
            node* nodes_to_delete = to_be_deleted.exchange(nullptr); // 交换，将to_be_deleted存入nullptr，返回原来的值，即取出待删除链表头部，to_be_deleted置空
            if (!--threads_in_pop) { // 检查是否只有一个线程调用pop， 因为threads_in_pop可能被其他线程更改，所以要检查
                delete_nodes(nodes_to_delete); // 确实只有一个线程操作，删除所有之前弹出的head，回收资源，delete_nodes(node* nodes)
            } else if (nodes_to_delete) { // 此时有其他线程在pop，但是有待删除的节点，不着急删除，将待删除的节点链入to_be_deleted
                chain_pending_nodes(nodes_to_delete); /////// 注意是chain_pending_nodes(node* nodes)
            }
            delete old_head;
        } else {
            chain_pending_node(old_head); /////// 注意是chain_pending_node(node* n)
            --threads_in_pop;
        }
    }
public:
    void push(T const& data) {
        node* const new_node = new node(data); // prepare data, might throw exception!!! but safe
        new_node->next = head.load(); // prepare next
        // amazing，很巧妙的用法~！！！！
        // ！！！注意，当compare_exchange_weak失败时，new_node->next会被修改成head当前值，下一次比较时会继续比较new_node->next是否等于head，直至弹出
        while(!head.compare_exchange_weak(new_node->next, new_node)){} // try to replace head with new_node, 好于 compare_exchange_strong
    }
    bool pop(T& result) { // 不要用引用返回
        node* old_head = head.load();
        if (old_head == nullptr) {
            return false;
        }
        while(!head.compare_exchange_weak(old_head, old_head->next)){} // 如何保证old_head->next有效，不会被其他线程删除
        result = *(old_head->data); // 会出错吗，会有异常吗
        delete old_head; //
        return true;
    }
    std::shared_ptr<T> pop() { // 注意pop出的节点可能被其他线程访问, 高负荷时效率不高
        ++threads_in_pop; // 做事先加计数器，表示有一个线程正在pop
        node* old_head = head.load();// 两个线程A B可能同时获得old_head,但A先执行到try_reclaim，B执行到while
        // 注意，如果A线程先释放了old_head,而B此时在while中访问old_head->next,就会出错,节点泄露，未定义行为
        // 必须保证此时old_head->next不为空
        while(old_head && !head.compare_exchange_weak(old_head, old_head->next)){} // 重复等待，确保此时弹出的head是唯一线程所有
        std::shared_ptr<T> res(nullptr);
        if (old_head) {
            res.swap(old_head->data); // 回收删除的节点， old_head变成空节点，但是old_head->next可能被其他线程访问
        }
        try_reclaim(old_head); // 从节点中直接提取数据，而不是从节点中拷贝数据
        return res;
    }
};

template<typename T>
struct sorter {
private:
    struct chunk_to_sort {
        std::list<T> data;
        std::promise<std::list<T>> promise; // 内部存入数据
        void print(std::mutex& m){
            std::lock_guard<std::mutex> lock(m);
            for (auto i : data) {
                std::cout << i << " ";
            }
            cout << endl;
        }
    };
    threadsafe_stack<chunk_to_sort> chunks;
    // lock_free_stack<chunk_to_sort> chunks;
    std::vector<std::thread> threads;
    unsigned const max_thread_count;
    std::atomic<bool> end_of_data;
public:
    sorter() : max_thread_count(std::thread::hardware_concurrency()-1), end_of_data(false) {
        cout << "max_thread_count: " << max_thread_count << endl;
    }
    ~sorter() {
        end_of_data = true;
        for(unsigned i=0; i < threads.size(); ++i) {
            threads[i].join();
        }
    }
    void try_sort_chunk() {
        std::shared_ptr<chunk_to_sort> chunk = chunks.pop();
        if (chunk) {
            sort_chunk(chunk);
        }
    }
    void sort_chunk(std::shared_ptr<chunk_to_sort> const& chunk) {
        chunk->promise.set_value(do_sort(chunk->data));
    }
    std::list<T> do_sort(std::list<T>& chunk_data) {
        if(chunk_data.empty()) {
            return chunk_data;
        }
        {
            lock_guard<mutex> lock(mlock);
            cout << "[do_sort start.] ";
            cout << "input (" << chunk_data.size() << "): ";
            for(auto& it:chunk_data) {
                cout << it << " ";
            }
            cout << endl;
        }
        std::list<T> result;
        result.splice(result.begin(), chunk_data, chunk_data.begin()); // chunk_data从begin开始，复制到result
        T const& partition_val = *result.begin(); // result的第一个元素
        typename std::list<T>::iterator divide_point = // chunk_data以partition_val为中心，划分为两部分，返回partition_val位置
                std::partition(chunk_data.begin(), chunk_data.end(), \
                [&](T const& val){return val < partition_val;});

        // 前半部分
        chunk_to_sort new_lower_chunk;
        new_lower_chunk.data.splice(new_lower_chunk.data.end(), // 把前半部分加入到new_lower_chunk， chunk_data剩下后半部分
                chunk_data, chunk_data.begin(), divide_point);
        std::future<std::list<T>> new_lower = new_lower_chunk.promise.get_future();
        chunks.push(std::move(new_lower_chunk)); // 把该任务加入到任务池
        if (threads.size() < max_thread_count) {
            threads.push_back(std::thread(&sorter<T>::sort_thread, this)); // 创建新线程，不断用线程取任务并排序
        }

        // 后半部分
        std::list<T> new_higher_chunk(do_sort(chunk_data)); // 对后半部分做同样操作，结束时后半部分已经排好序
        result.splice(result.end(), new_higher_chunk); // 把排好序的后半部分加入到result
        while(new_lower.wait_for(std::chrono::seconds(0)) != std::future_status::ready) { // 等待前半部分排序结束
            try_sort_chunk();
        }
        result.splice(result.begin(), new_lower.get()); // 把前半部分加入到result
        {
            lock_guard<mutex> lock(mlock);
            cout << "[do_sort done. ] ";
            cout << "result(" << result.size() << "): ";
            for(auto& it:result) {
                cout << it << " ";
            }
            cout << endl;
        }
        return result;
    }
    void sort_thread() {
        while(!end_of_data) {
            try_sort_chunk(); // 尝试从栈上弹出一个数据块
            std::this_thread::yield();
        }
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

void test1(){
    std::vector<list<int>> data = {
        // {2,1},
        { 6, 2, 8, 1, 5, 9, 4, 7, 3, 2, 9, 6, 1, 9, 5, 2, 7, 1, 8, 4 },
        // { 4, 7, 3, 2, 9, 6, 1, 6, 2, 8, 1, 5, 9, 4, 9, 5, 2, 7, 1, 8, 4},
        // { 9, 5, 2, 7, 1, 8, 4, 4, 7, 3, 2, 9, 6, 1, 6, 2, 8, 1, 5, 9, 4}
        };
    for (auto arr : data) {
        cout << "original array(" << arr.size() << "): ";
        copy(arr.begin(), arr.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
        std::list<int> ret = parallel_quick_sort(arr);
        cout << endl;
        cout << "sorted array(" << ret.size() << ")  : ";
        copy(ret.begin(), ret.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
    }
}

// 非异常安全的代码
template<typename Iterator, typename T>
struct accumulate_block {
    void operator()(Iterator first, Iterator last, T& result) {
        result = std::accumulate(first, last, result); // 可能会抛出异常，调用terminate
    }
};
template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
    unsigned long const length = std::distance(first, last);
    if (!length) {
        return init;
    }
    unsigned long const min_per_thread=25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    std::vector<T> result(num_threads);
    std::vector<std::thread> threads(num_threads -1);
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_block<Iterator,T>(), block_start, block_end, std::ref(result[i])); // 可能会创建失败，抛出异常，调用terminate
        block_start = block_end;
    }
    accumulate_block<Iterator,T>()(block_start, last, result[num_threads - 1]);
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return std::accumulate(result.begin(), result.end(), init); // 可能会抛出异常，调用terminate
}

// 异常安全的代码
class join_threads {
private:
    std::vector<std::thread>& threads;
public:
    explicit join_threads(std::vector<std::thread>& threads_) : threads(threads_) {}
    ~join_threads() {
        for (unsigned long i = 0; i < threads.size(); ++i) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }
    }
};
template<typename Iterator, typename T>
struct accumulate_block_throwsafe {
    T operator()(Iterator first, Iterator last) {
        return std::accumulate(first, last, T());
    }
};
template<typename Iterator, typename T>
T parallel_accumulate_throwsafe(Iterator first, Iterator last, T init) {
    unsigned long const length = std::distance(first, last);
    if (!length) {
        return init;
    }
    unsigned long const min_per_thread=25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    std::vector<std::future<T>> futures(num_threads - 1); // 使用future接收返回，可以是正常，可以是报错
    std::vector<std::thread> threads(num_threads - 1);
    join_threads joiner(threads); // 用析构函数去管理线程，避免主线程异常时，子线程没有join，不要用try这种方式
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        // Create a packaged_task<> that encapsulated a Function Object, 必须用move
        std::packaged_task<T(Iterator,Iterator)> task(std::move(accumulate_block_throwsafe<Iterator,T>()));
        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task), block_start, block_end); // 可能会创建失败，抛出异常，调用terminate
        block_start = block_end;
    }
    T last_result = accumulate_block_throwsafe<Iterator,T>()(block_start, last);
    // std::for_each(threads.begin(),threads.end(), std::mem_fn(&std::thread::join)); // 不能解决主线程异常安全问题，用join_threads替代
    T result = init;
    for(unsigned long i=0; i < (num_threads -1); ++i) {
        result += futures[i].get(); // 等待返回,可能返回正常值，可能返回线程中的异常（所有异常在此汇聚）
        // 线程异常，汇入主线程，joinable join
    }
    result += last_result;
    return result;
}
// 更简单，还能处理异常抛出
template<typename Iterator, typename T>
T parallel_accumulate_async(Iterator first, Iterator last, T init) {
    unsigned long const length = std::distance(first, last);
    unsigned long const max_chunk_size = 100000; // 注意每块的大小，太小的话，线程太多，会影响性能
    if (length < max_chunk_size) {
        return std::accumulate(first, last, init);
    } else {
        Iterator mid_point = first;
        std::advance(mid_point, length / 2);
        std::future<T> first_half_result = std::async(parallel_accumulate_async<Iterator,T>, first, mid_point, init);
        T second_half_result = parallel_accumulate_async(mid_point, last, T());
        return first_half_result.get() + second_half_result;
    }
}
void test2() {
    std::list<long> input_int;
    for (unsigned i = 0; i < 10000000; ++i) {
        input_int.push_back(i);
    }
    auto nowtime = std::chrono::steady_clock::now();
    long sum = 0;
    for (auto& it : input_int) {
        sum += it;
    }
    auto endtime = std::chrono::steady_clock::now();
    std::chrono::duration<double> steady_duration = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - nowtime);
    cout << "origin: sum=" << sum << " consume=" << steady_duration.count() << endl;

    nowtime = std::chrono::steady_clock::now();
    // sum = parallel_accumulate(input_int.begin(), input_int.end(), long(0));
    sum = parallel_accumulate_throwsafe(input_int.begin(), input_int.end(), long(0)); // 异常安全的
    endtime = std::chrono::steady_clock::now();
    steady_duration = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - nowtime);
    cout << "after : sum=" << sum << " consume=" << steady_duration.count() << endl;

    // 为什么这么耗时，线程太多了吗
    nowtime = std::chrono::steady_clock::now();
    // sum = parallel_accumulate(input_int.begin(), input_int.end(), long(0));
    sum = parallel_accumulate_async(input_int.begin(), input_int.end(), long(0)); // 异常安全的
    endtime = std::chrono::steady_clock::now();
    steady_duration = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - nowtime);
    cout << "after : sum=" << sum << " consume=" << steady_duration.count() << endl;
}

template<typename Iterator, typename Func>
void parallel_for_each(Iterator first, Iterator last, Func f) {
    unsigned long const length = std::distance(first, last);
    if (!length) {
        return;
    }
    unsigned long const min_per_thread = 25; // 每个线程处理的最小数据量
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread; // 算出当前任务最大线程数
    unsigned long const hardware_threads = std::thread::hardware_concurrency(); // 硬件支持的最大线程数
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads); // 线程数取最小值
    unsigned long const block_size = length / num_threads; // 每个线程处理的数据量
    std::vector<std::future<void>> futures(num_threads - 1); // 保存线程返回值, 返回为空
    std::vector<std::thread> threads(num_threads - 1); // 保存线程
    join_threads joiner(threads); // 线程管理

    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads -1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size); // 每个线程处理的数据量
        std::packaged_task<void(void)> task([&](){
            std::for_each(block_start, block_end, f); // 每个线程处理的数据量
        });
        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task));
        block_start = block_end;
    }
    std::for_each(block_start, last, f); // 主线程处理剩余的数据
    for (unsigned long i = 0; i < (num_threads - 1); ++i) {
        futures[i].get(); // 等待线程返回
    }
}

template<typename Iterator, typename Func>
void parallel_for_each_async(Iterator first, Iterator last, Func f) {
    unsigned long const length = std::distance(first, last);
    if (!length) {
        return;
    }
    // unsigned long hardware_threads = std::thread::hardware_concurrency(); // 硬件支持的最大线程数
    // hardware_threads = hardware_threads != 0 ? hardware_threads : 2;
    // unsigned long min_per_thread = length / hardware_threads; // 每个线程处理的最小数据量
    // min_per_thread = min_per_thread != 0 ? min_per_thread : 25;
    unsigned long min_per_thread = 25;
    if (length < (2*min_per_thread)) {
        std::for_each(first, last, f);
    } else {
        Iterator mid_point = first;
        std::advance(mid_point, length / 2); // 每个线程处理的数据量
        std::future<void> first_half = std::async(parallel_for_each_async<Iterator,Func>, first, mid_point, f);
        parallel_for_each_async(mid_point, last, f);
        first_half.get();
    }
}
void test3(){
    std::list<long> input_int;
    for (unsigned i = 0; i < 50000; ++i) {
        input_int.push_back(i);
    }
    std::function<void(long)> f = [](long i){
        std::thread::id this_id = std::this_thread::get_id();
        // threadsafe_print_level(INFO, "id:" << this_id << " i=" << i);
    };

    auto nowtime = std::chrono::steady_clock::now();
    parallel_for_each(input_int.begin(), input_int.end(), f);
    auto endtime = std::chrono::steady_clock::now();
    auto steady_duration = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - nowtime);
    cout << "parallel_for_each consume=" << steady_duration.count() << endl;

    nowtime = std::chrono::steady_clock::now();
    parallel_for_each_async(input_int.begin(), input_int.end(), f);
    endtime = std::chrono::steady_clock::now();
    steady_duration = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - nowtime);
    cout << "parallel_for_each_async consume=" << steady_duration.count() << endl;
}

template<typename Iterator, typename MatchType>
Iterator parallel_find(Iterator first, Iterator last, MatchType match) {
    struct find_element {
        void operator()(Iterator begin, Iterator end, MatchType match, std::promise<Iterator>* result, std::atomic<bool>* done_flag) {
            try {
                // std::this_thread::sleep_for(std::chrono::seconds(1));
                for (; (begin != end) && !done_flag->load(); ++begin) {
                    if (*begin == match) {
                        result->set_value(begin);
                        done_flag->store(true);
                        threadsafe_print_level(DEBUG, "id:" << std::this_thread::get_id() << " found " << match);
                        return;
                    }
                }
                if (begin == end) {
                    threadsafe_print_level(DEBUG, "id:" << std::this_thread::get_id() << " found nothing.");
                } else {
                    threadsafe_print_level(DEBUG, "id:" << std::this_thread::get_id() << " exit for done_flag=" << done_flag->load());
                }
            } catch (...) {
                try {
                    result->set_exception(std::current_exception());
                    done_flag->store(true);
                } catch (...) {

                }
            }
        }
    };
    unsigned long const length = std::distance(first, last);
    if (!length) {
        return last;
    }
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread -1) / min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    std::promise<Iterator> result;
    std::atomic<bool> done_flag(false);
    std::vector<std::thread> threads(num_threads - 1);
    {
        join_threads joiner(threads);
        Iterator block_start = first;
        for (unsigned long i=0; i < (num_threads - 1); ++i) {
            Iterator block_end = block_start;
            std::advance(block_end, block_size);
            threads[i] = std::thread(find_element(), block_start, block_end, match, &result, &done_flag);
            block_start = block_end;
        }
        find_element()(block_start, last, match, &result, &done_flag);
    }
    if (!done_flag.load()) { // 必然在store之后，因为store是原子操作，join_threads中的join阻塞，确保了store先于此处load
        return last;
    }
    return result.get_future().get();
}

template<typename Iterator, typename MatchType>
Iterator parallel_find_async_impl(Iterator first, Iterator last, MatchType match, std::atomic<bool>& done) {
    try {
        unsigned long const length = std::distance(first, last);
        unsigned long const min_per_thread = 25;
        if (length < (2*min_per_thread)) {
            for(; (first != last) && !done.load(); ++first) {
                if (*first == match) {
                    done = true;
                    return first;
                }
            }
            return last; // maybe end()
        } else {
            Iterator mid_point = first;
            std::advance(mid_point, length / 2);
            std::future<Iterator> async_result(std::async(&parallel_find_async_impl<Iterator,MatchType>, mid_point, last, match, std::ref(done)));
            Iterator const direct_result = parallel_find_async_impl(first, mid_point, match, done);
            return (direct_result == mid_point) ? async_result.get() : direct_result; // get可以延迟调用异步
        }
    } catch (...) {
        done = true; // 捕捉此处异常
        throw;
    }
}
template<typename Iterator, typename MatchType>
Iterator parallel_find_async(Iterator first, Iterator last, MatchType match) {
    std::atomic<bool> done(false);
    return parallel_find_async_impl(first, last, match, done);
}

void test4(){
    std::list<long> input_int;
    for (unsigned i = 0; i <= 500; ++i) {
        input_int.push_back(i);
    }
    long target_num = 500;
    auto now = std::chrono::steady_clock::now();
    auto ret = parallel_find(input_int.begin(), input_int.end(), target_num);
    auto end = std::chrono::steady_clock::now();
    auto steady_duration = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - now);
    if (ret == input_int.end()) {
        cout << "parallel_find       " << target_num << " not found consume=" << steady_duration.count() << endl;
    } else {
        cout << "parallel_find       " << target_num << " found consume=" << steady_duration.count() << endl;
    }

    now = std::chrono::steady_clock::now();
    //  std::async  可以用来提供“异常-安全”和“异常-传播”特性
    ret = parallel_find_async(input_int.begin(), input_int.end(), target_num);
    end = std::chrono::steady_clock::now();
    steady_duration = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - now);
    if (ret == input_int.end()) {
        cout << "parallel_find_async " << target_num << " not found consume=" << steady_duration.count() << endl;
    } else {
        cout << "parallel_find_async " << target_num << " found consume=" << steady_duration.count() << endl;
    }
}


template<typename Iterator>
void parallel_partial_sum(Iterator first, Iterator last) {
    typedef typename Iterator::value_type value_type;
    struct process_chunk {
        void operator()(Iterator begin, Iterator last, std::future<value_type>* previous_end_value, std::promise<value_type>* end_value) {
            try {
                Iterator end = last;
                ++end;
                std::partial_sum(begin, end, begin); // 逐个累加，结果从begin放起
                if (previous_end_value) {
                    value_type addend = previous_end_value->get();
                    *last += addend; // 因为此时该block已经累加完毕，先把最后一个元素叠加上一组最后一个元素
                    if (end_value) {
                        end_value->set_value(*last); // 可以往后一组通知了
                    }
                    std::for_each(begin, last, [&addend](value_type& item){ // 注意，last是不需要加的
                        item += addend; // 为什么要这么做，因为partial_sum是累加，所以每个元素都要加上前面的和
                    });
                } else if (end_value) { // 如果没有前一组，就只需要赋值end_value
                    end_value->set_value(*last);
                }
            } catch (...) {
                if (end_value) {
                    end_value->set_exception(std::current_exception());
                } else {
                    throw;
                }
            }
        }
    };
    unsigned long const length = std::distance(first, last);
    if (!length) {
        return;
    }
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    typedef typename Iterator::value_type value_type;
    std::vector<std::thread> threads(num_threads - 1);
    std::vector<std::promise<value_type>> end_values(num_threads - 1);
    std::vector<std::future<value_type>> previous_end_values;
    previous_end_values.reserve(num_threads - 1);
    join_threads joiner(threads);
    Iterator block_start = first;
    for (unsigned long i=0; i < (num_threads - 1); ++i) {
        Iterator block_last = block_start;
        std::advance(block_last, block_size - 1); // 确保都是有效的
        threads[i] = std::thread(process_chunk(), block_start, block_last, (i!=0)? &previous_end_values[i-1] : nullptr, &end_values[i]); // 第一个block不需要加上前面的和
        block_start = block_last;
        ++block_start;  // 每个block第一个不加，从第二个开始加起
        previous_end_values.push_back(end_values[i].get_future()); // 压入future
    }
    Iterator final_element = block_start;
    std::advance(final_element, std::distance(block_start, last) - 1); // 最后一个block的最后一个元素,last是end没有数据
    process_chunk()(block_start, final_element, (num_threads>1)? &previous_end_values.back() : nullptr, nullptr); // back()表示最后以一个元素
}

class barrier {
private:
    std::atomic<unsigned> count; // 可以支持的线程数，超过count会失效
    std::atomic<unsigned> spaces; // 一个线程调用一次，则占用一个space
    std::atomic<unsigned> generation;
public:
    explicit barrier(unsigned count_) : count(count_), spaces(count_), generation(0) {}
    ~barrier() {
        cout << "barrier destruct. count=" << count << " spaces=" << spaces << " generation=" << generation << endl;
    }
    void wait() {
        unsigned const my_generation = generation.load();
        if (!--spaces) { // 每调用一次sapces减1，直到为0，表示所有线程都到达
            spaces.store(count.load()); // 所有线程到达
            generation.fetch_add(1); // 通知其他线程，新一轮开始
            // ++generation; // 通知其他线程，新一轮开始
        } else {
            while(generation.load() == my_generation) { // 都处于一个generation，等待其他线程到达，不相等时所有线程到达，跳出循环
                std::this_thread::yield();
            }
        }
    }
    void done_waiting() { // 因为当一个线程完成其工作，并在等待的时候，才能对其进行调用它
        --count; // 可以支持的线程数减1 下一次spaces将会被重置为一个较小的数
        if (!--spaces) {
            spaces = count.load(); // 所有线程到达
            ++generation; // 通知其他线程，新一轮开始
        }
    }
};

template<typename Iterator>
void parallel_partial_sum_power(Iterator first, Iterator last) { // 线程很廉价时可以用
    typedef typename Iterator::value_type value_type;
    struct process_element{
        void operator()(Iterator first, Iterator last, std::vector<value_type>& buffer, unsigned i, barrier& b) {
            // 管理每一个节点
            value_type& ith_element = *(first + i); // 找到新起点
            bool update_source = false;
            for(unsigned step = 0, stride = 1; stride <= i; ++step, stride*=2) {
                value_type const& source = (step%2)? buffer[i] : ith_element; // 奇数时用buffer，偶数时用ith_element
                value_type &dest = (step%2)? ith_element : buffer[i];
                value_type const& addend = (step%2)? buffer[i - stride] : *(first + i - stride); // 奇数时取
                dest = source + addend;
                update_source = !(step%2);
                b.wait();
            }
            if (update_source) {
                ith_element = buffer[i];
            } else {
                buffer[i] = ith_element; // 正确数据要更新回去，很重要
            }
            b.done_waiting();
        }
    };
    unsigned long const length = std::distance(first, last);
    if (length <= 1) {
        return;
    }
    // std::vector<value_type> buffer(length); // 保存每个节点的状态值，最终赋值到first和last上
    std::vector<value_type> buffer;
    buffer.insert(buffer.end(), first, last); // 最初状态要有
    barrier b(length);
    std::vector<std::thread> threads(length - 1); // 最后一个线程就是当前线程，所以减一
    join_threads joiner(threads);
    Iterator block_start = first;
    for (unsigned long i = 0; i < (length - 1); ++i) {
        threads[i] = std::thread(process_element(), first, last, std::ref(buffer), i, std::ref(b));
    }
    process_element()(first, last, buffer, length - 1, b);
}

void test5() {
    std::vector<long> input_int;
    for (unsigned i = 1; i < 100; ++i) {
        input_int.push_back(i);
    }
    auto first = input_int.begin();
    auto last = input_int.end();
    unsigned long const length = std::distance(first, last); // 包括首尾元素
    cout << "first=" << *first << " last=" << *(last-1) << " end=" << *last<< " length=" << length << endl;

    std::vector<long> tmp(input_int);
    // utils_print_copy_decorate(tmp);
    std::vector<long> target(tmp.size());
    utils_consume_time_decorate(std::partial_sum(tmp.begin(), tmp.end(), target.begin());)
    // utils_print_copy_decorate(target);
    utils_consume_time_decorate(parallel_partial_sum(tmp.begin(), tmp.end());)
    // utils_print_copy_decorate(tmp);
    if (tmp != target) {
        cout << "parallel_partial_sum [error]." << endl;
    } else {
        cout << "parallel_partial_sum [ok]." << endl;
    }

    cout << "=======================" << endl;
    tmp.clear();
    tmp = input_int;
    // utils_print_copy_decorate(tmp);
    // utils_print_copy_decorate(target);
    utils_consume_time_decorate(parallel_partial_sum_power(tmp.begin(), tmp.end());)
    cout << "after==================" << endl;
    // utils_print_copy_decorate(tmp);
    if (tmp != target) {
        cout << "parallel_partial_sum_power [error]." << endl;
    } else {
        cout << "parallel_partial_sum_power [ok]." << endl;
    }
    // for (unsigned i = 0; i < tmp.size(); ++i) {
    //     if (tmp[i] != target[i]) {
    //         cout << "(" << input_int[i] << "," << target[i] << "," << tmp[i] << ")!!!" << endl;
    //     } else {
    //         cout << "(" << input_int[i] << "," << target[i] << "," << tmp[i] << ")" << endl;
    //     }
    // }
}

int main() {
    // 线程空间划分
    // 1. 处理前对数据划分，thread_thread.cc中，accumulate_block，把任务划分多个空间（难点），对每个空间分配一个线程
    // 2. 递归处理过程中划分（递归划分），快速排序（数据块划分过细线程会爆炸增长）
    // 3. 通过任务类型划分，每个线程做专门的工作（只关心自己的）
    //    分离关注，线程间共享太多数据，需要考虑为什么需要这么多交互，是否可以分离关注，或者合并线程，减少交互
    //    划分任务序列，适合操作开始前，输入数据长度不清楚的情况，pipeline，分成多个阶段，每个阶段一个线程，每个线程只关注自己的任务，不关注其他线程的任务
    // test1(); // 使用栈的并行快速排序，等待数据块排序！！！！！！该结构尚存在内存泄露问题

    // 影响并发性能因素
    // 1. 处理器个数，可并发数量oversubscription问题，要么自己搞线程池，要么std::thread::hardware_concurrency()有多少线程用多少, std::async根据实际情况扩展线程数
    // 2. 数据共享争用
    //    1. 线程数据同步的损耗，对于同一个变量读取，每个线程缓存中会有一个该变量副本，当变量更改时（同步耗时），需要保持缓存一致性MESI（线程自己的核芯缓存失效，被更新，但linux并没有完全实现该机制，所以需要内存序）
    //    2. 内存序的损耗，内存序某个时间会让个别处理器停下来等待
    //    3. 多线程数据竞争修改，
    //       高竞争high contention（处理器常等待）
    //       低竞争low contention（处理器少等待），
    //       乒乓竞争，cache ping-pong（变量在处理器缓存中因更改而频繁传递）注意，互斥量的竞争是系统级别的竞争，而不是处理器级别的，原子竞争是处理器级别的
    // 3. 伪共享False Sharing，处理器处理缓存单位为缓存行cache line，缓存布局会影响性能-（缓存行是共享的，大小由机器确定）
    //    https://blog.csdn.net/rong_toa/article/details/109274448
    //    1. 同一个线程的相关数数据都在一个缓存行，好
    //    2. 不同线程的无关变量在同一个缓存行，过于接近或过于分散，不好，一个线程修改其中一个变量，其他线程修改其他变量，会导致缓存行失效，其他线程需要重新加载缓存行，导致性能下降
    //    3. 解决方法，对齐，对齐后，同一线程的变量在临近内存（如同同一缓存行），不同线程的无关变量在不同缓存行（较远地方），不会相互影响
    //        <new> hardware_destructive_interference_size 避免假共享的最小偏移 (C++17)
    //        <new> hardware_constructive_interference_size 促使真共享的最大偏移 (C++17)
    //        机器也会让一个线程在不同核上运行，如果线程迁移，就意味着缓存行也要迁移到另一个核上
    // 4. 超额认购 oversubscription（线程数超过核数，频繁等待）
    #if __cplusplus >= 201703L
    {
        #ifdef __cpp_lib_hardware_interference_size
        {
            #pragma message("__cpp_lib_hardware_interference_size defined")
            std::cout << "hardware_constructive_interference_size: " << std::hardware_constructive_interference_size << endl;
            std::cout << "hardware_destructive_interference_size: " << std::hardware_destructive_interference_size << endl;
        }
        #else
        {
            #pragma message("__cpp_lib_hardware_interference_size not defined")
        }
        #endif
    }
    #endif

    // 专门服务多线程的数据结构
    // 1. 考量 竞争contention，伪共享false sharing，数据距离 data proximity
    // 2. 数组结构，划分数组元素法，
    //    同一线程的数据紧密联系
    //    不同线程的数据分散开，避免伪共享
    //    数组最好连续访问，大的数据块改变数据划分方式，大变小，就能够提高性能
    // 3. 二叉树，很难
    // 4. 互斥量与数据，如果在同一个cache line，很好，但是线程多了会竞争锁，性能会下降

    // 并发代码注意事项
    // 1. 异常安全
    // 2. 可扩展性，会根据机器的配置自动扩展 Amdahl定律
    // 3. 使用多线程隐藏延迟
    // 4. 注意并发同时，提高响应能力
    //    响应线程与业务线程分离
    // test2(); // 多线程在大数据量时的优势，异常安全和非异常安全
    // test3(); // 并行实现for_each
    // test4(); // 并行实现find
    test5(); // 并行实现partial_sum
    return 0;
}