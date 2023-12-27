#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <map>
#include <queue>
#include <stack>

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

std::atomic<bool> job_done(false);
std::mutex print_lock;
void threadsafe_print(const std::string& str) {
    std::lock_guard<std::mutex> lock(print_lock);
    std::cout << str << std::endl;
}

// stack
struct empty_stack: std::exception {
    const char* what() const throw();
};

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

void test_threadsafe_stack() {
    for (int i=0; i<100; ++i) { // 确保每次都能把stack中的元素pop完
        job_done = false;
        threadsafe_stack<int> si;
        std::thread t1([&si](){
            for (int i=0; i<1000; ++i) {
                si.push(i);
            }
            job_done = true;
            // cout << "t1 exit" << endl;
        });
        auto pop_stack =[&si](std::string str){
            while(true) {
                std::shared_ptr<int> p = si.pop();
                if (p) {
                    threadsafe_print(str + " " + std::to_string(*p));
                } else if (si.empty() && job_done) {
                    break;
                }
            }
            // threadsafe_print(str + " exit");
        };
        std::thread t2(pop_stack, "t2");
        std::thread t3(pop_stack, "t3");
        std::thread t4(pop_stack, "t4");
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        if (si.empty()) {
            // cout << "si is empty" << endl;
        } else {
            cout << "si is not empty" << endl;
            break;
        }
    }
}

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

void test_threadsafe_queue() {
    threadsafe_queue<int> si;
    job_done = false;
    std::thread t1([&si](){
        for (int i=0; i<1000; ++i) {
            si.push(i);
        }
        job_done = true;
        cout << "t1 exit" << endl;
    });
    auto pop_queue =[&si](std::string str){
        while(true) {
            std::shared_ptr<int> p = si.try_pop();
            if (p) {
                threadsafe_print(str + " " + std::to_string(*p));
            } else if (si.empty() && job_done) {
                break;
            }
        }
        threadsafe_print(str + " exit");
    };
    std::thread t2(pop_queue, "t2");
    std::thread t3(pop_queue, "t3");
    std::thread t4(pop_queue, "t4");
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

template<typename T>
class threadsafe_queue_popsafe { // 使用shared_ptr，内存不足时再push抛出
private:
    mutable std::mutex mut; // 互斥量会限制对并发的支持
    std::queue<std::shared_ptr<T>> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue_popsafe() {}
    // 第一点，内存分配本身会抛出异常，不应放到读操作中
    // 第二点，make_shared本身耗时，不应放到读操作中
    // 第三点，分配内存放到push写操作中
    void push(T new_value) {
        // // 方案3 使用shared_ptr
        std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value))); // 内存不够会在这报错
        std::lock_guard<std::mutex> lk(mut); // 一定要写在这里，分配成功了再加锁，否则可能会死锁
        data_queue.push(data);
        data_cond.notify_one();
    }
    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this](){ return !data_queue.empty(); }); // wait挂起，直到lambda返回true
        value = std::move(*data_queue.front());
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this](){ return !data_queue.empty(); }); // 阻塞，比empty好一些
        std::shared_ptr<T> res = data_queue.front(); // 不需要make_shared，性能低，因为push时已经使用make_shared
        data_queue.pop();
        return res;
        // try { // push内已经使用make_shared，那边会抛出异常，此处不会抛出异常，一下代码无必要
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
        value = std::move(*data_queue.front());
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop(){
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty()) {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> res = data_queue.front(); // 不需要使用make_shared
        data_queue.pop();
        return res;
    }
    bool empty() const {
        std::lock_guard<std::mutex> lk(mut); // 连续调用加锁解锁，性能不好
        return data_queue.empty();
    }
};
void test_threadsafe_queue_popsafe() {
    threadsafe_queue_popsafe<int> si;
    job_done = false;
    std::thread t1([&si](){
        for (int i=0; i<1000; ++i) {
            si.push(i);
        }
        job_done = true;
        cout << "t1 exit" << endl;
    });
    auto pop_queue =[&si](std::string str){
        while(true) {
            // 写法1
            // std::shared_ptr<int> p = si.try_pop();
            // if (p) {
            //     threadsafe_print(str + " " + std::to_string(*p));
            // } else if (si.empty() && job_done) {
            //     break;
            // }
            // 写法2
            if (job_done && si.empty()) break; // 一定要先判断job_done，否则可能会死锁
            std::shared_ptr<int> p = si.wait_and_pop();
            if (p) {
                threadsafe_print(str + " " + std::to_string(*p));
            }
        }
        threadsafe_print(str + " exit");
    };
    std::thread t2(pop_queue, "t2");
    std::thread t3(pop_queue, "t3");
    std::thread t4(pop_queue, "t4");
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

template<typename T>
class single_thread_queue{
private:
    struct node {
        T data;
        std::unique_ptr<node> next; // 不需要显式删除
        node(T const& data_) : data(std::move(data_)) {}
    };
    std::unique_ptr<node> head{nullptr};
    node* tail{nullptr};
public:
    single_thread_queue(){}
    single_thread_queue& operator=(single_thread_queue& other) = delete;
    // try_pop和push会对同一节点操作，用两个锁也没有用
    std::shared_ptr<T> try_pop(){
        if (head) {
            std::shared_ptr<T> const res(std::make_shared<T>(std::move(head->data))); // 使用head数据
            std::unique_ptr<node> const old_head = std::move(head); // head指针放入临时变量
            head = std::move(old_head->next); // head指向下一个节点
            return res;
        }
        tail = nullptr; // head为空，说明队列为空，tail也为空
        return std::shared_ptr<T>();
    }
    void push(T new_value) {
        std::unique_ptr<node> p(new node(std::move(new_value)));
        node* const new_tail = p.get(); // 把它当做最后一个节点，获取原始指针
        if (tail) {
            tail->next = std::move(p);
        } else { // tail为空，说明队列为空
            head = std::move(p);
        }
        tail = new_tail; // 头尾最开始都指向同一个节点时，此时try_pop会锁住next，未被访问时就被同一个锁锁住，有问题
    }
};
void test_single_thread_queue() {
    single_thread_queue<int> si;
    for (int i=0; i<10; ++i) {
        si.push(i);
    }
    while(true) {
        std::shared_ptr<int> p = si.try_pop();
        if (p) {
            threadsafe_print(std::to_string(*p));
        } else {
            break;
        }
    }
    for (int i=0; i<10; ++i) {
        si.push(i);
    }
    cout << "2rd:" << endl;
    while(true) {
        std::shared_ptr<int> p = si.try_pop();
        if (p) {
            threadsafe_print(std::to_string(*p));
        } else {
            break;
        }
    }
}

template<typename T>
class rw_isolated_queue {
private:
    struct node {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;
    std::mutex tail_mutex;
    std::unique_ptr<node> head;
    node* tail;
public:
    rw_isolated_queue() : head(new node), tail(head.get()) {} // 确保虚拟节点一开始就在队列中
    rw_isolated_queue(const rw_isolated_queue& other) = delete;
    rw_isolated_queue& operator=(const rw_isolated_queue& other) = delete;
    inline node* get_tail() {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }
    // 采用读写分离的技术
    // 确保了try_pop和push不会对同一节点进行操作，所以不需要互斥了，只需要一个互斥量就足够了
    std::shared_ptr<T> try_pop() {
        if(head.get() == tail) { // 队列为空，比空指针判断更好，能保证head都不是空指针
            //attention!!! 两者相等时返回空，他们不会同时操作同一个节点，如果指向不同节点，就不存在冲突，也不用加锁
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(head->data); // 使用head数据，直接检索，不需要构造T的新实例
        std::unique_ptr<node> old_head = std::move(head); // head指针放入临时变量，head为空
        head = std::move(old_head->next); // head指向下一个节点
        return res;
    }
    void push(T new_value) { // 只访问tail，不访问head，不需要加锁，线程安全
        // / 创建新实例, make_shared可以表面内存二次分配，避免增加引用次数
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node); // 新空虚拟节点，不需要为new_data提供构造函数
        tail->data = new_data; // new_value副本赋值给tail
        node* const new_tail = p.get(); // 把空虚拟节点当做最后一个节点
        tail->next = std::move(p);
        tail = new_tail; // attention!!!以上步骤，都是在对tail进行修改，到此tail和head进行分离了，tail指向空虚拟节点
    }
};
void test_rw_isolated_queue(){
    rw_isolated_queue<int> si;
    for (int i=0; i<10; ++i) {
        si.push(i);
    }
    while(true) {
        std::shared_ptr<int> p = si.try_pop();
        if (p) {
            threadsafe_print(std::to_string(*p));
        } else {
            break;
        }
    }
    for (int i=0; i<10; ++i) {
        si.push(i);
    }
    cout << "2rd:" << endl;
    while(true) {
        std::shared_ptr<int> p = si.try_pop();
        if (p) {
            threadsafe_print(std::to_string(*p));
        } else {
            break;
        }
    }

    job_done = false;
    std::thread t1([&si](){
        for (int i=0; i<10000; ++i) {
            si.push(i);
        }
        job_done = true;
        threadsafe_print("t1 exit");
    });
    std::thread t2([&si](){
        while(true) {
            std::shared_ptr<int> p = si.try_pop();
            if (p) {
                threadsafe_print("t2 " + std::to_string(*p));
            } else if (job_done) {
                break;
            }
        }
        threadsafe_print("t2 exit");
    });
    std::thread t3([&si](){
        while(true) {
            std::shared_ptr<int> p = si.try_pop();
            if (p) {
                threadsafe_print("t3 " + std::to_string(*p));
            } else if (job_done) {
                break;
            }
        }
        threadsafe_print("t3 exit");
    });
    t1.join();
    t2.join();
    t3.join();
}

template<typename T>
class threadsafe_rw_isolated_queue {
private:
    struct node {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;
    std::mutex tail_mutex;
    std::unique_ptr<node> head;
    node* tail;
    inline node* get_tail() {
        std::lock_guard<std::mutex> tail_lock(tail_mutex); // push调用时，两者之间可以保持有序
        // get_tail比push先调用时，get_tail可以获得旧结尾点
        // push先于get_tail调用时，get_tail可以获得最新结尾点和真正tail值
        return tail;
    }
    std::unique_ptr<node> pop_head() {
        std::lock_guard<std::mutex> head_lock(head_mutex); // 防止多个线程同时访问head，加锁
        // 注意head_lock和tail_lock的顺序，保证head_lock在前，tail_lock在后，保证head block时，tail朝着正确防线走，如果反过来，可能出错
        // 即先获得一个旧的tail，再去获得一个head，但此时head可能被其他线程修改了，导致错误
        // 队列为空，比空指针判断更好，能保证head都不是空指针
        // 同时，tail可能被push修改，所以需要再次判断，加锁，保证此次访问的tail不会被修改
        // tail永远往后移动，试图取到一个有效的tail（旧节点的tail值）
        if (head.get() == get_tail()) { return nullptr; }
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }
public:
    threadsafe_rw_isolated_queue() : head(new node), tail(head.get()) {} // 确保虚拟节点一开始就在队列中
    threadsafe_rw_isolated_queue(const threadsafe_rw_isolated_queue& other) = delete;
    threadsafe_rw_isolated_queue& operator=(const threadsafe_rw_isolated_queue& other) = delete;

    // 采用读写分离的技术
    // 确保了try_pop和push不会对同一节点进行操作，所以不需要互斥了，只需要一个互斥量就足够了
    std::shared_ptr<T> try_pop() {
        std::unique_ptr<node> old_head = pop_head(); // 从队列中取出一个节点
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
    void push(T new_value) {
        // 仍然只访问tail，不访问head，不需要加锁，线程安全
        // 创建新实例, make_shared可以表面内存二次分配，避免增加引用次数
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node); // 新虚拟节点，不需要为new_data提供构造函数
        node* const new_tail = p.get(); // 把空节点当做最后一个节点
        // 上面构造完毕，下面小加入队列中，需要加锁
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = new_data; // new_value副本赋值给tail
        tail->next = std::move(p); // 指向一个空数据的节点
        tail = new_tail; // attention!!!以上步骤，都是在对tail进行修改，到此tail和head进行分离了
        // tail永远指向一个空虚拟节点，与head分离
    }
};

void test_threadsafe_rw_isolated_queue() {
    threadsafe_rw_isolated_queue<int> si;
    job_done = false;
    std::thread t1([&si](){
        for (int i=0; i<1000; ++i) {
            si.push(i);
        }
        job_done = true;
        threadsafe_print("t1 exit");
    });
    std::thread t2([&si](){
        while(true) {
            std::shared_ptr<int> p = si.try_pop();
            if (p) {
                threadsafe_print("t2 " + std::to_string(*p));
            } else if (job_done) {
                break;
            }
        }
        threadsafe_print("t2 exit");
    });
    std::thread t3([&si](){
        while(true) {
            std::shared_ptr<int> p = si.try_pop();
            if (p) {
                threadsafe_print("t3 " + std::to_string(*p));
            } else if (job_done) {
                break;
            }
        }
        threadsafe_print("t3 exit");
    });
    std::thread t4([&si](){
        for (int i=0; i<1000; ++i) {
            si.push(i);
        }
        job_done = true;
        threadsafe_print("t4 exit");
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

template<typename T>
class threadsafe_rw_isolated_notify_queue {
// 无界队列，可以持续增加新数据项，线程可能从较远地方访问，有开销
//（有些队列是指容量相对固定，满仓后就不能再加入数据，对线程开销稍小）
private:
    struct node {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;
    std::mutex tail_mutex;
    std::unique_ptr<node> head;
    node* tail;
    std::condition_variable data_cond;
    node* get_tail(){
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail; // return old tail
    }
    std::unique_ptr<node> pop_head() {
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head; // 返回旧头部指针
    }
    ////////////////wait/////////////////////////
    std::unique_lock<std::mutex> wait_for_data() {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        data_cond.wait(head_lock, [&]{ return head.get() != get_tail(); }); // 不等时，表示队列不为空，返回true
        return std::move(head_lock); // 将此锁传出去
    }
    std::unique_ptr<node> wait_pop_head() {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        return pop_head();
    }
    std::unique_ptr<node> wait_pop_head(T& value) {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        value = std::move(*head->data);
        return pop_head();
    }
    ////////////////try pop/////////////////////////
    std::unique_ptr<node> try_pop_head() {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if (head.get() == get_tail()) {
            return std::unique_ptr<node>();
        }
        return pop_head();
    }
    std::unique_ptr<node> try_pop_head(T& value) {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if (head.get() == get_tail()) {
            return std::shared_ptr<T>();
        }
        value = std::move(*head->data);
        return pop_head();
    }
public:
    threadsafe_rw_isolated_notify_queue() : head(new node), tail(head.get()) {}
    threadsafe_rw_isolated_notify_queue(const threadsafe_rw_isolated_notify_queue& other) = delete;
    threadsafe_rw_isolated_notify_queue& operator=(const threadsafe_rw_isolated_notify_queue& other) = delete;
    ///////////////////////try pop///////////////////
    std::shared_ptr<T> try_pop(){
        std::unique_ptr<node> old_head = try_pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
    bool try_pop(T& value) {
        std::unique_ptr<node> old_head =try_pop_head(value);
        return old_head;
    }
    ///////////////////////wait/////////////////////
    std::shared_ptr<T> wait_and_pop(){
        std::unique_ptr<node> const old_head = wait_pop_head(); // 从队列中取出一个节点
        return old_head->data;
    }
    void wait_and_pop(T& value){
        std::unique_ptr<node> const old_head = wait_pop_head(value); // 从队列中取出一个节点
    }
    ///////////////////////push/////////////////////
    void push(T new_value){
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);
        {
            std::lock_guard<std::mutex> tail_lock(tail_mutex);
            tail->data = new_data;
            node* const new_tail = p.get();
            tail->next = std::move(p); // now p is null
            tail = new_tail;
        }
        data_cond.notify_one(); // 已经增加原始完毕，通知其他线程
    }
    bool empty() {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        return (head.get() == get_tail());
    }
};

void test_threadsafe_rw_isolated_notify_queue() {
    threadsafe_rw_isolated_notify_queue<int> si;
    job_done = false;
    std::thread t1([&si](){
        for (int i=0; i<1000; ++i) {
            si.push(i);
        }
        job_done = true;
        threadsafe_print("t1 exit");
    });
    std::thread t2([&si](){
        while(true) {
            std::shared_ptr<int> p = si.try_pop();
            if (p) {
                threadsafe_print("t2 " + std::to_string(*p));
            } else if (job_done) {
                break;
            }
        }
        threadsafe_print("t2 exit");
    });
    std::thread t3([&si](){
        while(true) {
            std::shared_ptr<int> p = si.try_pop();
            if (p) {
                threadsafe_print("t3 " + std::to_string(*p));
            } else if (job_done) {
                break;
            }
        }
        threadsafe_print("t3 exit");
    });
    t1.join();
    t2.join();
    t3.join();
    if (si.empty()) {
        threadsafe_print("si is empty");
    } else {
        threadsafe_print("si is not empty");
    }
    cout << "try pop done." << endl;
    job_done = false;
    std::thread t4([&si](){
        for (int i=0; i<1000; ++i) {
            si.push(i);
        }
        job_done = true;
        threadsafe_print("t4 exit");
    });
    std::thread t5([&si](){
        while(true) {
            if (job_done && si.empty()) break; // 一定要先判断job_done，否则可能会死锁
            std::shared_ptr<int> p = si.wait_and_pop();
            if (p) {
                threadsafe_print("t5 " + std::to_string(*p));
            }
        }
        threadsafe_print("t5 exit");
    });
    std::thread t6([&si](){
        while(true) {
            if (job_done && si.empty()) break; // 一定要先判断job_done，否则可能会死锁
            std::shared_ptr<int> p = si.wait_and_pop();
            if (p) {
                threadsafe_print("t6 " + std::to_string(*p));
            }
        }
        threadsafe_print("t6 exit");
    });
    t4.join();
    t5.join();
    t6.join();
}

/*
vector<list<pair<Key, Value>>> buckets;
1| 1->2->3
2| 4->5
3| 6
*/
template<typename Key, typename Value, typename Hash=std::hash<Key>>
class threadsafe_hash_lookup_table {
private:
    class bucket_type {
    public:
        typedef std::pair<Key, Value> bucket_value;
        typedef std::list<bucket_value> bucket_data;
        typedef typename bucket_data::iterator bucket_iterator;
        // typename告知编译器bucket_iterator是bucket_data的一个依赖类型，而不是一个变量或常量，typename不能缺少
    private:
        bucket_data data;
        mutable std::shared_mutex mutex; // 读写锁 c++17

        bucket_iterator find_entry_for(Key const& key) { // !!!!!!!!!!!!不能const
            return std::find_if(data.begin(), data.end(), [&](bucket_value const& item){
                return item.first == key;
            });
        }
    public:
        Value value_for(Key const& key, Value const& default_value) { // 在桶中查找 !!!!!!!!!不能const
            std::shared_lock<std::shared_mutex> lock(mutex);
            bucket_iterator const found_entry = find_entry_for(key);
            return (found_entry == data.end()) ? default_value : found_entry->second;
        }
        void add_or_update_mapping(Key const& key, Value const& value) {
            // cout << "key=" << key << " value=" << value << endl;
            std::unique_lock<std::shared_mutex> lock(mutex);  //
            bucket_iterator const found_entry = find_entry_for(key);
            if (found_entry == data.end()) {
                data.push_back(bucket_value(key, value));
            } else {
                found_entry->second = value;
            }
        }
        void remove_mapping(Key const& key) {
            std::unique_lock<std::shared_mutex> lock(mutex);
            bucket_iterator const found_entry = find_entry_for(key);
            if (found_entry != data.end()) {
                data.erase(found_entry);
            }
        }
        void clear() {
            std::unique_lock<std::shared_mutex> lock(mutex);
            data.clear();
        }
    };

    Hash                                      hasher;  // hash函数对象
    std::vector<std::unique_ptr<bucket_type>> buckets; // 桶的集合
    bucket_type& get_bucket(Key const& key) const {
        std::size_t const bucket_index = hasher(key) % buckets.size(); // 算出该hash应该在哪个桶中
        return *buckets[bucket_index];
    }

public:
    typedef Key key_type;
    typedef Value mapped_type;
    typedef Hash hash_type;
    threadsafe_hash_lookup_table(unsigned num_buckets=19, Hash const& hasher_=Hash())
                                : buckets(num_buckets), hasher(hasher_) {
        for (unsigned i=0; i<num_buckets; ++i) {
            buckets[i].reset(new bucket_type); // 先填满空间,19是质数，可以减少hash冲突
        }
    }
    threadsafe_hash_lookup_table(threadsafe_hash_lookup_table const& other) = delete;
    threadsafe_hash_lookup_table& operator=(threadsafe_hash_lookup_table const& other) = delete;
    Value value_for(Key const& key, Value const& default_value=Value()) const {
        return get_bucket(key).value_for(key, default_value);
    }
    void add_or_update_mapping(Key const& key, Value const& value) {
        get_bucket(key).add_or_update_mapping(key, value);
    }
    void remove_mapping(Key const& key) {
        get_bucket(key).remove_mapping(key);
    }
    void clear() {
        std::lock_guard<std::mutex> lock(mutex);
        std::for_each(buckets.begin(), buckets.end(), [](std::unique_ptr<bucket_type>& bucket){
            bucket->clear();
        });
    }
    std::map<Key,Value> get_map() const {
        std::vector<std::unique_lock<std::shared_mutex> > locks;
        for(unsigned i=0;i<buckets.size();++i) {
            locks.push_back(std::unique_lock<std::shared_mutex>(buckets[i].mutex));
        }
        std::map<Key,Value> res;
        for(unsigned i=0;i<buckets.size();++i) {
            for(auto it=buckets[i].data.begin(); it!=buckets[i].data.end(); ++it) {
                res.insert(*it);
            }
        }
        return res;
    }
};
void test_threadsafe_hash_lookup_table() {
    std::hash<int> hash_int; // hash函数对象
    cout << "hash_int(1):" << hash_int(1) << endl;
    std::hash<std::string> hash_str;
    cout << "hash_str(\"1\"):" << hash_str("1") << endl;
    cout << "hash_str(\"hello\"):" << hash_str("hello") << endl;

    std::list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    auto it = std::find_if(l.begin(), l.end(), [](int i){ return i==2; });
    if (it != l.end()) {
        cout << "find 2" << endl;
    } else {
        cout << "not find 2" << endl;
    }
    threadsafe_hash_lookup_table<int,std::string> si;
    int i = 1;
    std::string str = "1";
    si.add_or_update_mapping(1, "1");
    si.add_or_update_mapping(2, "2");
    si.add_or_update_mapping(3, "3");
    std::string ret = si.value_for(1, "None");
    cout << "find 1, ret=" << ret << endl;
    ret = si.value_for(4, "None");
    cout << "find 4, ret=" << ret << endl;

    si.clear();
    std::thread t1([&si](){
        for (int i=0; i<100; ++i) {
            si.add_or_update_mapping(i, std::to_string(i));
        }
    });
    std::thread t2([&si](){
        while(true) {
            std::string ret = si.value_for(50, "None");
            if (ret != "None") {
                threadsafe_print("t2 find 50 ret=" + ret);
                // cout << "t2 find 50 ret=" << ret << endl;
                break;
            }
        }
    });
    std::thread t3([&si](){
        while(true) {
            std::string ret = si.value_for(80, "None");
            if (ret != "None") {
                threadsafe_print("t3 find 80 ret=" + ret);
                // cout << "t3 find 80 ret=" << ret << endl;
                break;
            }
        }
    });
    std::thread t4([&si](){
        while(true) {
            std::string ret = si.value_for(90, "None");
            if (ret != "None") {
                threadsafe_print("t4 find 90 ret=" + ret);
                // cout << "t4 find 90 ret=" << ret << endl;
                break;
            }
        }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

template<typename T>
class threadsafe_list {
    struct node {
        std::mutex m; // 每个节点都有自己的锁，实现真正高并发，
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
        node() : next(nullptr) {}
        node(T const& value) : data(std::make_shared<T>(value)) {}
    };
    node head; // 虚拟节点，next指向第一个节点
public:
    threadsafe_list() {}
    threadsafe_list(threadsafe_list const& other) = delete;
    ~threadsafe_list() {
        remove_if([](node const&){
            return true;
        });
    }
    threadsafe_list& operator=(threadsafe_list const& other) = delete;
    void push_front(T const& value) {
        std::unique_ptr<node> new_node(new node(value));
        std::lock_guard<std::mutex> lk(head.m); // 保护head
        new_node->next = std::move(head.next);
        head.next = std::move(new_node);
    }

    template<typename Function>
    void for_each(Function f) {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m); // 从头开始
        while(node* const next = current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->m); // 先锁住下一个节点
            lk.unlock(); // 解锁当前节点，即使其他线程删除了当前节点，也不会出错，因为next_lk锁住了下一个节点
            f(*next->data); // 访问下一个节点数据
            current = next; // 当前节点移动下一个节点
            lk = std::move(next_lk); // lk指向下一个节点的锁
        }
    }
    template<typename Predicate>
    std::shared_ptr<T> find_first_if(Predicate p) {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m); // 从头开始
        while(node* const next = current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->m); // 先锁住下一个节点
            lk.unlock(); // 解锁当前节点，即使其他线程删除了当前节点，也不会出错，因为next_lk锁住了下一个节点
            if(p(*next->data)) {
                return next->data;
            }
            current = next;
            lk = std::move(next_lk); // 方便下次循环解锁
        }
    }
    template<typename Predicate>
    void remove_if(Predicate p) {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m); // 从头开始
        while(node* const next = current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->m); // 先锁住下一个节点
            if(p(*next->data)) {
                std::unique_ptr<node> old_next = std::move(current->next);
                current->next = std::move(next->next); // 当前节点指向下下个节点
                next_lk.unlock(); // 对即将删除的节点解锁
            } else {
                lk.unlock(); // 解锁当前节点
                current = next; // 当前节点移动下一个节点
                lk = std::move(next_lk); // lk指向下一个节点的锁
            }
        }
    }
};
void test_threadsafe_list() {
    threadsafe_list<int> si;

}
int main(int argc, char* argv[]) {
    // 基于锁的数据结构
    // 1. 访问安全
    // 2. 序列化访问最小化（同一个时间只有一个线程获得锁不是并发），真实并发最大化（尽量从数据结构上分割成各个独立小块，线程影响最小）
    //    map不太适合高并发，细粒度锁比单一锁更能增加高并发
    test_threadsafe_stack();
    // test_threadsafe_queue();
    // test_threadsafe_queue_popsafe();
    // test_single_thread_queue();
    // test_rw_isolated_queue();
    // test_threadsafe_rw_isolated_queue();
    // test_threadsafe_rw_isolated_notify_queue();
    // test_threadsafe_hash_lookup_table(); // vector<list<pair<Key, Value>>> buckets;
    // test_threadsafe_list(); // 每个list成员都有自己的锁
    return 0;
}
