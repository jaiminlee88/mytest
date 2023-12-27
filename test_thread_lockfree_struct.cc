
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <map>

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
using namespace std;

std::atomic<bool> job_done(false);
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

// void threadsafe_print(const std::string& str, log_level level = INFO) {
//     if (g_log_level > level) {
//         std::lock_guard<std::mutex> lock(print_lock);
//         std::cout << str << std::endl;
//     }
// }


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
void test_lockfree_stack() {
    lock_free_stack<int> si;
    si.push(1);
    si.push(2);
    si.push(3);
    int i;
    while(si.pop(i)) {
        cout << i << endl;
    }

    job_done = false;
    std::thread t1([&si](){
        for (int i = 0; i < 10000; ++i) {
            si.push(i);
        }
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        job_done = true;
        threadsafe_print("t1 done");
    });
    auto func_pop = [&si](std::string str){
        while(true) {
            // int i;
            // if (si.pop(i)) { // 函数bool pop(T& result)
            //     threadsafe_print(str + " pop " + std::to_string(i));
            //     continue;
            // } else {
            //     threadsafe_print(str + " pop nullptr");
            // }
            std::shared_ptr<int> p = si.pop(); // 函数std::shared_ptr<T> pop()
            if (p.get()) {
                threadsafe_print(str + " pop " + std::to_string(*p));
                continue;
            } else {
                threadsafe_print(str + " pop nullptr");
            }
            if (job_done) {
                break;
            }
        }
        threadsafe_print(str + " done");
    };
    std::thread t2(func_pop, "t2");
    std::thread t3(func_pop, "t3");
    std::thread t4(func_pop, "t4");
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    if (si.pop(i)) {
        threadsafe_print("main pop " + std::to_string(i));
    }
}

template<typename T>
class lock_free_stack_hazard_pointer {
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
        while(!head.compare_exchange_weak(old_head, old_head->next)){}
        result = *(old_head->data); // 会出错吗，会有异常吗
        delete old_head; //
        return true;
    }

    static unsigned int const max_hazard_pointers = 100;
    struct hazard_pointer {
        std::atomic<std::thread::id> id;
        std::atomic<void*> pointer;
    };
    hazard_pointer hazard_pointers[max_hazard_pointers]; // 风险指针数组,一个线程一个槽，可以2*max_hazard_pointers个槽!!!!!!!!!!
    class hp_owner {
    private:
        hazard_pointer* hp;
    public:
        hp_owner(hazard_pointer* hazard_pointers) : hp(nullptr) {
            for (unsigned int i=0; i < max_hazard_pointers; ++i) {
                std::thread::id old_id; // 空id
                // 尝试声明风险指针所有权
                if (hazard_pointers[i].id.compare_exchange_strong(old_id, std::this_thread::get_id())) {
                    // 从hazard_pointers找到一个空槽，将其设置为当前线程的风险指针，线程数限制max_hazard_pointers
                    // 让线程都分开，不要互相干扰，空间换机会，换时间
                    // 当某个线程获得自己的槽内风险指针，后面就特别快了，不用再去遍历
                    hp = &(hazard_pointers[i]);
                    break;
                }
            }
            if (!hp) { // 获取所有权失败
                throw std::runtime_error("No hazard pointers available");
            }
        }
        hp_owner(hp_owner const&) = delete;
        hp_owner operator=(hp_owner const&) = delete;
        ~hp_owner() {
            hp->pointer.store(nullptr); // 释放风险指针
            hp->id.store(std::thread::id()); // 对线程id进行清除，让其他线程使用
        }
        std::atomic<void*>& get_pointer() {
            return hp->pointer;
        };
    };
    std::atomic<void*>& get_hazard_pointer_for_current_thread() {
        thread_local static hp_owner hazard(hazard_pointers); // 每个线程都有一个hazard对象，只会访问hazard_pointers中自己的槽，获取自己的风险指针
        return hazard.get_pointer();
    }
    bool outstanding_hazard_pointers_for(void* p) {
        for (unsigned int i=0; i < max_hazard_pointers; ++i) { // 删除之前对hp引用的节点进行检查，如果有引用，就不删除
            if (hazard_pointers[i].pointer.load() == p) {
                return true;
            }
        }
        return false;
    }
    // 各种节点操作,可以抽离出去，此处不做过多考虑
    static void do_delete(void* p) {
        threadsafe_print("do_delete");
        delete static_cast<T*>(p);
    }
    struct data_to_reclaim {
        void* data;
        std::function<void(void*)> deleter; // 安全产生一个函数指针
        data_to_reclaim* next;
        data_to_reclaim(node* p) : data(p), deleter(&do_delete), next(nullptr) {}
        ~data_to_reclaim() { deleter(data); }
    };
    std::atomic<data_to_reclaim*> nodes_to_reclaim{nullptr}; // 将要回收的数据结构!!!!!!!!!!!
    void add_to_reclaim_list(data_to_reclaim* node) {
        // 添加节点的方法仍然老办法，巧妙！！！
        node->next = nodes_to_reclaim.load(); // 加到链表头部，注意此时受delete_nodes_with_no_hazards影响nodes_to_reclaim可能为nullptr，
        // 添加节点很巧妙！！！！！！！！！！！！！
        while(!nodes_to_reclaim.compare_exchange_weak(node->next, node)){} // 将node插入到nodes_to_reclaim链表头部;
    }
    void reclaim_later(node* data) { // 可以做成模板
        add_to_reclaim_list(new data_to_reclaim(data)); // 将要回收的数据结构
    }
    void delete_nodes_with_no_hazards() {
        data_to_reclaim* current = nodes_to_reclaim.exchange(nullptr); // 原子交换，很关键，也很耗时，一个线程只会获得一个，将nodes_to_reclaim存入nullptr，返回原来的值，即取出待删除链表头部，nodes_to_reclaim置空
        while(current) { // 逐个检查有没有相同的风险指针
            data_to_reclaim* const next = current->next;
            if (!outstanding_hazard_pointers_for(current->data)) { // 检查线程风险指针槽内是否有该 风险指针的引用
                delete current; // 没有风险指针引用，删除，调用析构函数
            } else {
                add_to_reclaim_list(current); // 有风险指针引用，重新插入到nodes_to_reclaim链表头部
            }
            current = next;
        }
    }
    std::shared_ptr<T> pop() {
        // threadsafe_print("try shared_ptr pop.");
        std::atomic<void*>& hp = get_hazard_pointer_for_current_thread(); // 获得一个空闲槽，返回对应风险指针
        node* old_head = head.load(); // 尝试获取head, 但是可能被其他线程修改
        do {
            // 内部while，先不断循环并设置风险指针，直到old_head==head, 一定要先设置风险指针（old有效），再判断old_head==head
            // 外部while，通过while争取置换出头部节点，确保head最终由一个线程获得，并且设置上风险指针hp，失败则重新再来
            // 成功后，old节点加上了风险指针，其他线程不能删除该节点（old_next->next有效），同时获得该节点
            node* temp;
            do { // 目的是要将风险指针hp设置为head，可能多个线程获得了同一个风险指针
                temp = old_head;
                hp.store(old_head); // 将old_head存入hp，确保当前线程获得head，同时其他线程看到hp，不会删除该节点
                old_head = head.load(); // 重新获取head
            } while (old_head != temp); // 如果old_head被修改，重新获取,可能多个线程同时获取一个old_head
            // 上方while循环能保证node不会在读取旧head指针①时，以及在设置风险指针的时被删除
        } while(old_head && !head.compare_exchange_strong(old_head, old_head->next)); // 成功后head变为next（old_head不一定有效，可能被其他线程改变）
        // 上面不用compare_exchange_weak，当设置成功后还要重置hp，没有必要
        hp.store(nullptr); // 当声明完成，因为已经成功获得了head，其他节点不会再持有该节点，将hp清除，因为当前线程是要弹出节点，不需要风险指针了
        std::shared_ptr<T> res{nullptr};
        if (old_head) {
            res.swap(old_head->data); // 将data置换到res内，old_head变成空节点，但是old_head->next可能被其他线程访问
            if (outstanding_hazard_pointers_for(old_head)) { // 删除之前对hp引用的节点进行检查，如果有引用，就不删除
                reclaim_later(old_head); // 其他线程还在用，缓慢回收
            } else {
                // if (old_head == nullptr) {
                //     threadsafe_print("old_head nullptr");
                // }
                delete old_head; // 没有任何引用
            }
            delete_nodes_with_no_hazards(); // 再做一次检查，删除所有没有风险指针引用的节点，如果不能就只能等待下一个节点
            // delete_nodes_with_no_hazards很耗时，可以等待风险指针超过一定数量再执行，能保证有一个节点可以删除
            // hazard_pointers[2*max_hazard_pointers],可以保证有max_hazard_pointers个节点可以删除，但增加内存了
            // 或者内存盈余，每个线程中的都拥有其自己的回收链表，一共max_hazard_pointers x max_hazard_pointers个节点
        }
        return res;
    }
};
void test_lockfree_stack_hazard_pointer() {
    // 1 风险指针,其他线程要能看到
    // 1. 一个线程访问一个节点时，其他线程不能删除该节点
    lock_free_stack_hazard_pointer<int> si;
    std::vector<int> vec{1,2,3};
    for (auto& i : vec) {
        si.push(i);
        cout << "push " << i << endl;
    }
    int i;
    std::shared_ptr<int> p;
    while(true) {
        p = si.pop();
        if (p.get()) {
            cout << *p << endl;
        } else {
            break;
        }
    }
    si.push(1);
    cout << "push 1 again." << endl;
    while(true) {
        p = si.pop();
        if (p.get()) {
            cout << *p << endl;
        } else {
            break;
        }
    }
    cout << endl;
    cout << "test threads." << endl;
    job_done = false;
    std::thread t1([&si](){
        for (int i = 0; i < 10000; ++i) {
            si.push(i);
        }
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        job_done = true;
        threadsafe_print("t1 done");
    });
    auto func_pop = [&si](std::string str){
        while(true) {
            // int i;
            // if (si.pop(i)) { // 函数bool pop(T& result)
            //     threadsafe_print(str + " pop " + std::to_string(i));
            //     continue;
            // } else {
            //     threadsafe_print(str + " pop nullptr");
            // }
            // threadsafe_print(str + " pop try to pop.");
            std::shared_ptr<int> p = si.pop(); // 函数std::shared_ptr<T> pop()
            if (p.get()) {
                threadsafe_print(str + " pop " + std::to_string(*(p)));
                continue;
            } else {
                threadsafe_print(str + " pop nullptr");
            }
            if (job_done) {
                break;
            }
        }
        threadsafe_print(str + " done");
    };
    std::thread t2(func_pop, "t2");
    std::thread t3(func_pop, "t3");
    std::thread t4(func_pop, "t4");
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    if (si.pop(i)) {
        threadsafe_print("main pop " + std::to_string(i));
    }
}

template<typename T>
class lock_free_stack_ref_count_shareptr {
private:
    struct node {
        std::shared_ptr<T> data;
        std::shared_ptr<node> next;
        node(T const& data_) : data(std::make_shared<T>(data_)), next(nullptr) {}
    };
    std::shared_ptr<node> head{nullptr};
public:
    lock_free_stack_ref_count_shareptr() {}
    ~lock_free_stack_ref_count_shareptr() {
        while(pop());
    }
    void push(T const& data) {
        std::shared_ptr<node> const new_node = std::make_shared<node>(data);
        new_node->next = std::atomic_load(&head);
        // while(!head.compare_exchange_weak(new_node->next, new_node)){}
        while(!std::atomic_compare_exchange_weak(&head, &new_node->next, new_node)){}
    }
    std::shared_ptr<T> pop() {
        // 智能指针本身有引用计数。pop出如果有其他线程访问，不会被删除
        // shared_ptr有一些操作是原子的（实现上可能有锁，也可能无锁），但不一定是无锁的，但如果在无锁情况下，性能开销很大
        std::shared_ptr<node> old_head = std::atomic_load(&head);
        while(old_head && !std::atomic_compare_exchange_weak(&head, &old_head, old_head->next));
        if (old_head) {
            std::atomic_store(&old_head->next, std::shared_ptr<node>());
            return old_head->data;
        }
        return std::shared_ptr<T>();
    }
};
void test_lockfree_stack_ref_count_sharedptr() {
    // 1 风险指针,其他线程要能看到
    // 1. 一个线程访问一个节点时，其他线程不能删除该节点
    lock_free_stack_ref_count_shareptr<int> si;
    std::vector<int> vec{1,2,3};
    for (auto& i : vec) {
        si.push(i);
        cout << "push " << i << endl;
    }
    int i;
    std::shared_ptr<int> p;
    while(true) {
        p = si.pop();
        if (p.get()) {
            cout << *p << endl;
        } else {
            break;
        }
    }
    si.push(1);
    cout << "push 1 again." << endl;
    while(true) {
        p = si.pop();
        if (p.get()) {
            cout << *p << endl;
        } else {
            break;
        }
    }
    cout << endl;
    cout << "test threads." << endl;
    while(true) {
        job_done = false;
        std::thread t1([&si](){
            for (int i = 0; i < 10000; ++i) {
                si.push(i);
            }
            // std::this_thread::sleep_for(std::chrono::seconds(1));
            job_done = true;
            threadsafe_print("t1 done");
        });
        auto func_pop = [&si](std::string str){
            while(true) {
                // int i;
                // if (si.pop(i)) { // 函数bool pop(T& result)
                //     threadsafe_print(str + " pop " + std::to_string(i));
                //     continue;
                // } else {
                //     threadsafe_print(str + " pop nullptr");
                // }
                // threadsafe_print(str + " pop try to pop.");
                std::shared_ptr<int> p = si.pop(); // 函数std::shared_ptr<T> pop()
                if (p.get()) {
                    threadsafe_print(str + " pop " + std::to_string(*(p)));
                    continue;
                } else {
                    threadsafe_print(str + " pop nullptr");
                }
                if (job_done) {
                    break;
                }
            }
            threadsafe_print(str + " done");
        };
        std::thread t2(func_pop, "t2");
        std::thread t3(func_pop, "t3");
        std::thread t4(func_pop, "t4");
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        // std::shared_ptr<int> p1 = si.pop();
        // if (p) {
        //     threadsafe_print("main pop " + std::to_string(*p1));
        // }
    }
}

template<typename T>
class lock_free_stack_ref_count_doublecnt { // 有问题!!!!!!!!!!
private: // counted_node_ptr--->node--->counted_node_ptr--->node...
    struct node; // 前置声明
    struct counted_node_ptr {
        // 注意，外部计数器用的是普通变量，因为外部计数器(或者说这个counted_node_ptr结构体对象)
		// 一般都是拷贝副本，并不会对单一对象执行并发访问
        int external_count{0}; // 外部计数，表示有多少指针指向该节点，即读取时加一
        node* ptr{nullptr}; // pnode
    };
    struct node {
        std::shared_ptr<T> data;
        std::atomic<int> internal_count; // 内部计数，线程结束时则减一，访问结束时减一
        counted_node_ptr next;
        node(T const& data_) : data(std::make_shared<T>(data_)), internal_count(0) {}
    };
    std::atomic<counted_node_ptr> head; // 指针操作也是原子 counted_node_ptr足够小（太大可能无法保证原子性），有些平台支持双子比较和交换操作
public:
    lock_free_stack_ref_count_doublecnt() {}
    ~lock_free_stack_ref_count_doublecnt() {
        while(pop());
    }
    void push(T const& data) {
        counted_node_ptr new_counted_node_ptr;
        new_counted_node_ptr.ptr = new node(data);
        new_counted_node_ptr.external_count = 1; // 由外部head引用，所以为1
        new_counted_node_ptr.ptr->next = head.load();
        while(!head.compare_exchange_weak(new_counted_node_ptr.ptr->next, new_counted_node_ptr));
    }

    void increase_head_count(counted_node_ptr& old_counter, std::string& str) {
        // 进入pop函数的第一件事就是将head的外部计数器递增，表示当前有一个线程打算读取node指针
        // compare_exchange_strong()的循环中完成增加，通过比较和设置整
        // 个结构体来保证，指针不会在同一时间内被其他线程修改。
        counted_node_ptr new_counter;
        do {
            // 此循环是为了能够确保head能被此线程获取并将其外部节点递增
            threadsafe_print_level(log_level::DEBUG, str + " increase_head_count -1- head.external_count=" + std::to_string(head.load().external_count));
            threadsafe_print_level(log_level::DEBUG, str + " increase_head_count -2- old_counter.external_count=" + std::to_string(old_counter.external_count));
            new_counter = old_counter;
            ++new_counter.external_count; // 用到head，访问就要加1，保证解除引用时安全性，
            threadsafe_print_level(log_level::DEBUG, str + " increase_head_count -3- new_counter.external_count=" + std::to_string(new_counter.external_count));
            // 可能同时有多个会访问head,如果当前线程为2，其他线程old_counter为1，就不能重入该head,一直等到cnt变为1
            // 同时访问一个head时，循环什么时候跳出？？？？？？？？？！！！！
            // 注意，线程A取走当前head，head会被重置为new_counter，切换线程B，线程B比较old_counter与head不一致，old_counter会被compare_exchange_strong重置为head，继续比较，直至相等
            // 此时，线程A和线程B可能同时访问了同一个head
        } while(!head.compare_exchange_strong(old_counter, new_counter)); // 注意，这里不再是对比指针，而是通过二进制方式对比结构体对象是否相等 循环直至节点重合，通过比较和设置整个结构体来保证，保证指针不会再同一时间被其他线程更改

        // 此处有可能发生线程切换，导致old_counter与当前真正的head不一致，不过没事，因为已经将之前的head的外部节点递增了，另一个线程可以负责处理那个head
        threadsafe_print_level(log_level::DEBUG, str + " increase_head_count -4- head.external_count=" + std::to_string(head.load().external_count));
        old_counter.external_count = new_counter.external_count;
        threadsafe_print_level(log_level::DEBUG, str + " increase_head_count -5- old_counter.external_count=" + std::to_string(old_counter.external_count));
    }
    std::shared_ptr<T> pop(std::string str="") {
        counted_node_ptr old_counted_head = head.load();  // 先尝试获取当前head，不过，最终获取的以increase_head_count返回的为主 每个线程只能获得当时状态的head,有序获得head
        for(;;) { // 这里是一个大循环，没有退出条件，要么返回值，要么再次循环
            // 真正获取当前head，并将head的外部计数器递增1。注意，此时等此函数退出时，old_head并不一定等于当前stack的head
            threadsafe_print_level(log_level::DEBUG, str + " pop -0- before head.external_count=" + std::to_string(head.load().external_count));
            increase_head_count(old_counted_head, str); // 当加载head必须外部加1，表示该节点正在被引用，此时old_counter与head是同一个
            threadsafe_print_level(log_level::DEBUG, str + " pop -0- after head.external_count=" + std::to_string(head.load().external_count));

            // 假如不在获取指针之前对外部计数器递增，则其他线程可能会释放这个ptr，导致当前线程的ptr变成了悬垂指针
            node* const ptr = old_counted_head.ptr;
            if (!ptr) { // 如果ptr被其他线程释放了，应该直接返回空指针，也可能当前head已经是空指针，head总是存在的，指向的node不一定存在
                // while(!head.compare_exchange_strong(old_counted_head, counted_node_ptr())); // 特意添加，最后一个节点时，head应该是最初的状态
                return std::shared_ptr<T>();
            }

            // ptr真实存在
            // 这里的if才是真正尝试将head从stack中移除，如果发现当前的head与刚才获取的old_head不一致，说明当前线程在
			// 获取old_head并将head的外部计数器递增1后，另一个线程将这个递增后的head给移除了，并没有轮到当前线程来处理。
            threadsafe_print_level(log_level::DEBUG, str + " pop -1- ptr->internal_count=" + std::to_string(ptr->internal_count.load()));
            if (head.compare_exchange_strong(old_counted_head, ptr->next)) { // 完成节点替换，head指向下一个节点，不是自己获得的状态head，直接返回
                // 若比较失败，则表明已经删除该节点
                // 如果发现head与old_head一致，那么就将head移除，然后将head更新为next。此时本线程就可以放心的处理old_head了。
				// 注意，即使当前线程进入到了这里，old_head对应的指针可能也被其他线程的old_head对象所引用，这个就看old_head
				// 里的外部计数器的值了。
                std::shared_ptr<T> res;
                res.swap(ptr->data); // 注意，这里是用的swap，因为以后都肯定不会再访问这个data了，所以直接取出来就行，不用留着

                // 如果其他线程并没有引用old_head中的node指针，则理论上old_head中的外部计数器的值是2，因为刚才在increase_head_count
				// 中对其进行了递增。如果此时其他线程也引用了old_head中对应的node指针，则此时old_head中的外部计数器的值一定大于2，且
				// 减去2之后的值就是其他线程引用的个数(或者你可以这么理解，由于当前线程将不再引用这个节点，因此要把外部计数器减去1。然后
				// 由于这个head节点已经从stack中移除了，所以stack不再引用这个节点了，因此外部引用计数又再次减去1)。然后就需要比较外部
				// 计数器与内部计数器之和是否为0。如果之和为0，则表示现在没有其他线程引用这个节点了，那么就可以安全的删除这个节点了。
				// 注意，如果它们之和为0，则表示internal_count之前的数值一定是-count_increase的值，因为fetch_add返回的是旧值，所以你
				// 会发现下面比较有点怪异，不是比较0，而是比较旧值与-count_increase。
                int const count_increase = old_counted_head.external_count - 2; // 将外部计数加到内部计数中去，其他线程引用的个数
                threadsafe_print_level(log_level::DEBUG, str + " pop -2- old_counted_head.external_count=" + std::to_string(old_counted_head.external_count) + " count_increase=" + std::to_string(count_increase));
                threadsafe_print_level(log_level::DEBUG, str + " pop -3- ptr->internal_count=" + std::to_string(ptr->internal_count.load()));
                if (ptr->internal_count.fetch_add(count_increase) == -count_increase) { // 结束访问加一，当前线程不在引用这个节点，也没有其他线程引用，直接返回
                    threadsafe_print_level(log_level::DEBUG, str + " pop -4- ptr->internal_count=" + std::to_string(ptr->internal_count.load()));
                    delete ptr;
                }
                threadsafe_print_level(log_level::DEBUG, str + " pop -5- ptr->internal_count=" + std::to_string(ptr->internal_count.load()));
                return res;
            } else if (ptr->internal_count.fetch_sub(1) == 1) { // 当前线程是最后一个持有引用，则自己释放ptr， fetch_sub是先减再返回旧值，每次一循环时都会减1
                // 不管是由于其他线程已经把这个节点的内容返回了，还是其他线程新加了节点到stack中，此时都要重新循环，从而重新得到新的head并pop。
				// 但是在重新循环之前，由于上面在获取head时已经将head外部计数器加1了，那么这里需要将内部计数器减去1，表示当前线程不再引用这个节
				// 点了。如果发现内部计数器减去1之后变成了0，则表示内部计数器之前是1，所以肯定有其他线程已经返回这个节点的内容了(只要确定内部
				// 计数器的值大于0，就表示肯定有其他线程已经进入了上面的if分支并且会把节点中的值返回)，且如果正巧发现内部计数器的值为1，则表示
				// 当前已经没有其他线程再引用这个节点了(因为当前线程马上就要将内部计数器减1，则内部计数器就变成了0，就表示没有任何线程拥有这个
				// 节点了)。因此，此时就可以直接删除这个节点了。
                threadsafe_print_level(log_level::DEBUG, str + " pop -6- ptr->internal_count=" + std::to_string(ptr->internal_count.load()));
                delete ptr;
            }
        }
    }
};

void test_lockfree_stack_ref_count_doublecnt() {
    // https://blog.csdn.net/weixin_43376501/article/details/108325765
    // 1 风险指针,其他线程要能看到
    // 1. 一个线程访问一个节点时，其他线程不能删除该节点
    lock_free_stack_ref_count_doublecnt<int> si;
    std::vector<int> vec{1,2,3};
    cout << "----->正常添加并读取" << endl;
    for (auto& i : vec) {
        si.push(i);
        cout << "push " << i << endl;
    }
    int i;
    std::shared_ptr<int> p;
    while(true) {
        p = si.pop();
        if (p.get()) {
            cout << *p << endl;
        } else {
            break;
        }
    }
    cout << "----->栈的复用" << endl;
    si.push(1);
    si.push(2);
    cout << "push 1 again." << endl;
    while(true) {
        p = si.pop();
        if (p.get()) {
            cout << *p << endl;
        } else {
            break;
        }
    }
    cout << "----->空栈的读取" << endl;
    int cnt = 5;
    while(cnt) {
        p = si.pop();
        if (p.get()) {
            cout << *p << endl;
        } else {
            cout << "p == nullptr." << endl;
        }
        --cnt;
    }
    cout << endl;
    // return;
    cout << "test threads." << endl;
    cnt = 1;
    g_log_level = log_level::DEBUG;
    // g_log_level = log_level::DEBUG;
    while(cnt) {
        job_done = false;
        std::thread t1([&si](){
            for (int i = 0; i < 1000; ++i) {
                si.push(i);
            }
            // std::this_thread::sleep_for(std::chrono::seconds(1));
            job_done = true;
            threadsafe_print("t1 done");
        });
        auto func_pop = [&si](std::string str){
            while(true) {
                // int i;
                // if (si.pop(i)) { // 函数bool pop(T& result)
                //     threadsafe_print(str + " pop " + std::to_string(i));
                //     continue;
                // } else {
                //     threadsafe_print(str + " pop nullptr");
                // }
                // threadsafe_print(str + " pop try to pop.");
                std::shared_ptr<int> p = si.pop(str); // 函数std::shared_ptr<T> pop()
                if (p.get()) {
                    threadsafe_print(str + " pop " + std::to_string(*(p)));
                    continue;
                } else {
                    threadsafe_print(str + " pop nullptr");
                }
                if (job_done) {
                    break;
                }
            }
            threadsafe_print(str + " done");
        };
        std::thread t2(func_pop, "t2");
        std::thread t3(func_pop, "t3");
        std::thread t4(func_pop, "t4");
        std::thread t5(func_pop, "t5");
        std::thread t6(func_pop, "t6");
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        std::shared_ptr<int> p1 = si.pop();
        if (p) {
            threadsafe_print("main pop " + std::to_string(*p1));
        }
        --cnt;
    }
}

template<typename T>
class lock_free_stack_ref_count_doublecnt_mmopt { // 有问题!!!!!!!!!!
private: // counted_node_ptr--->node--->counted_node_ptr--->node...
    struct node; // 前置声明
    struct counted_node_ptr {
        // 注意，外部计数器用的是普通变量，因为外部计数器(或者说这个counted_node_ptr结构体对象)
		// 一般都是拷贝副本，并不会对单一对象执行并发访问
        int external_count{0}; // 外部计数，表示有多少指针指向该节点，即读取时加一
        node* ptr{nullptr}; // pnode
    };
    struct node {
        std::shared_ptr<T> data;
        std::atomic<int> internal_count; // 内部计数，线程结束时则减一，访问结束时减一
        counted_node_ptr next;
        node(T const& data_) : data(std::make_shared<T>(data_)), internal_count(0) {}
    };
    std::atomic<counted_node_ptr> head; // 指针操作也是原子 counted_node_ptr足够小（太大可能无法保证原子性），有些平台支持双子比较和交换操作
public:
    lock_free_stack_ref_count_doublecnt_mmopt() {}
    ~lock_free_stack_ref_count_doublecnt_mmopt() {
        while(pop());
    }
    void push(T const& data) {
        counted_node_ptr new_counted_node_ptr;
        new_counted_node_ptr.ptr = new node(data);
        new_counted_node_ptr.external_count = 1; // 由外部head引用，所以为1
        new_counted_node_ptr.ptr->next = head.load(std::memory_order_relaxed); // next必须保证读取安全，所以保证赋值在前
        while(!head.compare_exchange_weak(new_counted_node_ptr.ptr->next, new_counted_node_ptr, std::memory_order_release, std::memory_order_relaxed)); // 释放内存序，保证对next的写入在前
        // 第一个成功序列，要求赋值head完才能访问，所以要求memory_order_release，当失败时并不会更改head，所以第二个失败序列可以是memory_order_relaxed
    }

    void increase_head_count(counted_node_ptr& old_counter, std::string& str) {
        // 进入pop函数的第一件事就是将head的外部计数器递增，表示当前有一个线程打算读取node指针
        // compare_exchange_strong()的循环中完成增加，通过比较和设置整
        // 个结构体来保证，指针不会在同一时间内被其他线程修改。
        counted_node_ptr new_counter;
        do {
            // 此循环是为了能够确保head能被此线程获取并将其外部节点递增
            threadsafe_print_level(log_level::DEBUG, str + " increase_head_count -1- head.external_count=" + std::to_string(head.load().external_count));
            threadsafe_print_level(log_level::DEBUG, str + " increase_head_count -2- old_counter.external_count=" + std::to_string(old_counter.external_count));
            new_counter = old_counter;
            ++new_counter.external_count; // 用到head，访问就要加1，保证解除引用时安全性，
            threadsafe_print_level(log_level::DEBUG, str + " increase_head_count -3- new_counter.external_count=" + std::to_string(new_counter.external_count));
            // 可能同时有多个会访问head,如果当前线程为2，其他线程old_counter为1，就不能重入该head,一直等到cnt变为1
            // 同时访问一个head时，循环什么时候跳出？？？？？？？？？！！！！
            // 注意，线程A取走当前head，head会被重置为new_counter，切换线程B，线程B比较old_counter与head不一致，old_counter会被compare_exchange_strong重置为head，继续比较，直至相等
            // 此时，线程A和线程B可能同时访问了同一个head
        } while(!head.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire, std::memory_order_relaxed)); // 注意，这里不再是对比指针，而是通过二进制方式对比结构体对象是否相等 循环直至节点重合，通过比较和设置整个结构体来保证，保证指针不会再同一时间被其他线程更改
        // 获取next指向的旧值，compare_exchange_strong两个锁，第一个成功内存序，要求读再后，确保next已经加载，读取必须先于赋值memory_order_acquire，
        // 第二个失败锁，失败不会更改head，memory_order_relaxed就足够了

        // 此处有可能发生线程切换，导致old_counter与当前真正的head不一致，不过没事，因为已经将之前的head的外部节点递增了，另一个线程可以负责处理那个head
        threadsafe_print_level(log_level::DEBUG, str + " increase_head_count -4- head.external_count=" + std::to_string(head.load().external_count));
        old_counter.external_count = new_counter.external_count;
        threadsafe_print_level(log_level::DEBUG, str + " increase_head_count -5- old_counter.external_count=" + std::to_string(old_counter.external_count));
    }
    std::shared_ptr<T> pop(std::string str="") {
        counted_node_ptr old_counted_head = head.load(std::memory_order_relaxed);  // 先尝试获取当前head，不过，最终获取的以increase_head_count返回的为主 每个线程只能获得当时状态的head,有序获得head
        for(;;) { // 这里是一个大循环，没有退出条件，要么返回值，要么再次循环
            // 真正获取当前head，并将head的外部计数器递增1。注意，此时等此函数退出时，old_head并不一定等于当前stack的head
            threadsafe_print_level(log_level::DEBUG, str + " pop -0- before head.external_count=" + std::to_string(head.load().external_count));
            increase_head_count(old_counted_head, str); // 当加载head必须外部加1，表示该节点正在被引用，此时old_counter与head是同一个
            threadsafe_print_level(log_level::DEBUG, str + " pop -0- after head.external_count=" + std::to_string(head.load().external_count));

            // 假如不在获取指针之前对外部计数器递增，则其他线程可能会释放这个ptr，导致当前线程的ptr变成了悬垂指针
            node* const ptr = old_counted_head.ptr;
            if (!ptr) { // 如果ptr被其他线程释放了，应该直接返回空指针，也可能当前head已经是空指针，head总是存在的，指向的node不一定存在
                // while(!head.compare_exchange_strong(old_counted_head, counted_node_ptr())); // 特意添加，最后一个节点时，head应该是最初的状态
                return std::shared_ptr<T>();
            }

            // ptr真实存在
            // 这里的if才是真正尝试将head从stack中移除，如果发现当前的head与刚才获取的old_head不一致，说明当前线程在
			// 获取old_head并将head的外部计数器递增1后，另一个线程将这个递增后的head给移除了，并没有轮到当前线程来处理。
            threadsafe_print_level(log_level::DEBUG, str + " pop -1- ptr->internal_count=" + std::to_string(ptr->internal_count.load()));
            if (head.compare_exchange_strong(old_counted_head, ptr->next, std::memory_order_relaxed)) { // 完成节点替换，head指向下一个节点，不是自己获得的状态head，直接返回
                // 若比较失败，则表明已经删除该节点
                // 如果发现head与old_head一致，那么就将head移除，然后将head更新为next。此时本线程就可以放心的处理old_head了。
				// 注意，即使当前线程进入到了这里，old_head对应的指针可能也被其他线程的old_head对象所引用，这个就看old_head
				// 里的外部计数器的值了。
                std::shared_ptr<T> res;
                res.swap(ptr->data); // 注意，这里是用的swap，因为以后都肯定不会再访问这个data了，所以直接取出来就行，不用留着

                // 如果其他线程并没有引用old_head中的node指针，则理论上old_head中的外部计数器的值是2，因为刚才在increase_head_count
				// 中对其进行了递增。如果此时其他线程也引用了old_head中对应的node指针，则此时old_head中的外部计数器的值一定大于2，且
				// 减去2之后的值就是其他线程引用的个数(或者你可以这么理解，由于当前线程将不再引用这个节点，因此要把外部计数器减去1。然后
				// 由于这个head节点已经从stack中移除了，所以stack不再引用这个节点了，因此外部引用计数又再次减去1)。然后就需要比较外部
				// 计数器与内部计数器之和是否为0。如果之和为0，则表示现在没有其他线程引用这个节点了，那么就可以安全的删除这个节点了。
				// 注意，如果它们之和为0，则表示internal_count之前的数值一定是-count_increase的值，因为fetch_add返回的是旧值，所以你
				// 会发现下面比较有点怪异，不是比较0，而是比较旧值与-count_increase。
                int const count_increase = old_counted_head.external_count - 2; // 将外部计数加到内部计数中去，其他线程引用的个数
                threadsafe_print_level(log_level::DEBUG, str + " pop -2- old_counted_head.external_count=" + std::to_string(old_counted_head.external_count) + " count_increase=" + std::to_string(count_increase));
                threadsafe_print_level(log_level::DEBUG, str + " pop -3- ptr->internal_count=" + std::to_string(ptr->internal_count.load()));
                // 下方std::memory_order_release，可以保证swap先于delete操作
                if (ptr->internal_count.fetch_add(count_increase, std::memory_order_release) == -count_increase) { // 结束访问加一，当前线程不在引用这个节点，也没有其他线程引用，直接返回
                    threadsafe_print_level(log_level::DEBUG, str + " pop -4- ptr->internal_count=" + std::to_string(ptr->internal_count.load()));
                    delete ptr;
                }
                threadsafe_print_level(log_level::DEBUG, str + " pop -5- ptr->internal_count=" + std::to_string(ptr->internal_count.load()));
                return res;
            } else if (ptr->internal_count.fetch_sub(1, std::memory_order_relaxed) == 1) { // 当前线程是最后一个持有引用，则自己释放ptr， fetch_sub是先减再返回旧值，每次一循环时都会减1
                // 不管是由于其他线程已经把这个节点的内容返回了，还是其他线程新加了节点到stack中，此时都要重新循环，从而重新得到新的head并pop。
				// 但是在重新循环之前，由于上面在获取head时已经将head外部计数器加1了，那么这里需要将内部计数器减去1，表示当前线程不再引用这个节
				// 点了。如果发现内部计数器减去1之后变成了0，则表示内部计数器之前是1，所以肯定有其他线程已经返回这个节点的内容了(只要确定内部
				// 计数器的值大于0，就表示肯定有其他线程已经进入了上面的if分支并且会把节点中的值返回)，且如果正巧发现内部计数器的值为1，则表示
				// 当前已经没有其他线程再引用这个节点了(因为当前线程马上就要将内部计数器减1，则内部计数器就变成了0，就表示没有任何线程拥有这个
				// 节点了)。因此，此时就可以直接删除这个节点了。
                threadsafe_print_level(log_level::DEBUG, str + " pop -6- ptr->internal_count=" + std::to_string(ptr->internal_count.load()));
                delete ptr;
            }
        }
    }
};

void test_lockfree_stack_ref_count_doublecnt_mmopt() {
    // https://blog.csdn.net/weixin_43376501/article/details/108325765
    // 1 风险指针,其他线程要能看到
    // 1. 一个线程访问一个节点时，其他线程不能删除该节点
    lock_free_stack_ref_count_doublecnt_mmopt<int> si;
    std::vector<int> vec{1,2,3};
    cout << "----->正常添加并读取" << endl;
    for (auto& i : vec) {
        si.push(i);
        cout << "push " << i << endl;
    }
    int i;
    std::shared_ptr<int> p;
    while(true) {
        p = si.pop();
        if (p.get()) {
            cout << *p << endl;
        } else {
            break;
        }
    }
    cout << "----->栈的复用" << endl;
    si.push(1);
    si.push(2);
    cout << "push 1 again." << endl;
    while(true) {
        p = si.pop();
        if (p.get()) {
            cout << *p << endl;
        } else {
            break;
        }
    }
    cout << "----->空栈的读取" << endl;
    int cnt = 5;
    while(cnt) {
        p = si.pop();
        if (p.get()) {
            cout << *p << endl;
        } else {
            cout << "p == nullptr." << endl;
        }
        --cnt;
    }
    cout << endl;
    // return;
    cout << "test threads." << endl;
    cnt = 1;
    g_log_level = log_level::DEBUG;
    // g_log_level = log_level::DEBUG;
    while(cnt) {
        job_done = false;
        std::thread t1([&si](){
            for (int i = 0; i < 1000; ++i) {
                si.push(i);
            }
            // std::this_thread::sleep_for(std::chrono::seconds(1));
            job_done = true;
            threadsafe_print("t1 done");
        });
        auto func_pop = [&si](std::string str){
            while(true) {
                // int i;
                // if (si.pop(i)) { // 函数bool pop(T& result)
                //     threadsafe_print(str + " pop " + std::to_string(i));
                //     continue;
                // } else {
                //     threadsafe_print(str + " pop nullptr");
                // }
                // threadsafe_print(str + " pop try to pop.");
                std::shared_ptr<int> p = si.pop(str); // 函数std::shared_ptr<T> pop()
                if (p.get()) {
                    threadsafe_print(str + " pop " + std::to_string(*(p)));
                    continue;
                } else {
                    threadsafe_print(str + " pop nullptr");
                }
                if (job_done) {
                    break;
                }
            }
            threadsafe_print(str + " done");
        };
        std::thread t2(func_pop, "t2");
        std::thread t3(func_pop, "t3");
        std::thread t4(func_pop, "t4");
        std::thread t5(func_pop, "t5");
        std::thread t6(func_pop, "t6");
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        std::shared_ptr<int> p1 = si.pop();
        if (p) {
            threadsafe_print("main pop " + std::to_string(*p1));
        }
        --cnt;
    }
}

template<typename T>
class lock_free_queue {
    // head->node->node->node...->node.next==tail, tail->next.data==nullptr
private:
    struct node;
    struct counted_node_ptr {
        int external_count{0}; // 记录外部线程通过指针的引用， 外部计数，表示有多少指针指向该节点，即读取时加一
        node* ptr{nullptr};
        // counted_node_ptr(){} // 注意，这里不能设置构造函数，因为这里的构造函数是为了给atomic<counted_node_ptr> head
        // counted_node_ptr() : external_count(0), ptr(nullptr){}
        // counted_node_ptr(int ecnt=0, node* p=nullptr) : external_count(ecnt), ptr(p) {}
    };
    std::atomic<counted_node_ptr> head; // 有自己的引用计数器
    std::atomic<counted_node_ptr> tail; // 有自己的引用计数器
    struct node_counter { // 保证总体大小32bit，保证包在32位或64位机器上都能正常工作
        unsigned internal_count:30; // 内部节点引用， 30位，2^30-1=1073741823 位域定义的标准形式。
        unsigned external_counters:2; // node自己的计数器，表示外部（或虚拟）节点的计数器，小不会超过2，同时最多由上个节点的next，和head或者tail指向。 2位，2^2-1=3 位域定义的标准形式。不支持寻址，最多有两个计数器
    };
    struct node {
        std::atomic<T*> data{nullptr};
        std::atomic<node_counter> count;
        counted_node_ptr next;
        std::atomic<counted_node_ptr> next_optimized; // 1 
        node() {
            node_counter new_count;
            new_count.internal_count = 0; // 初始化为0，表示没有线程引用该节点
            new_count.external_counters = 2; // 外部的计数器个数，当新节点加入时，会被tail和上一个节点的next引用，所以需要2，// next 和 tail 同时指向，因为新创建的节点将会放入到队列中
            count.store(new_count);
            next.ptr = nullptr;
            next.external_count = 0;
        }
        /*
            释放对当前节点的引用，释放节点引用时语言将该节点得到内部计数减一，
            如果减一后节点的外部引用以及内部引用计数为零，则删除该节点。
            释放操作由节点自己完成，所以该函数为node的成员函数。
            所以该函数释放的是该函数的计数器的外部引用节点。
        */
        void release_ref() {
            node_counter old_counter = count.load(std::memory_order_relaxed); // 获取节点count计数器
            node_counter new_counter;
            do {
                new_counter = old_counter;
                --new_counter.internal_count; // 内部引用解除，// 内部计数减一
            } while (!count.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire, std::memory_order_relaxed));
            threadsafe_print_level(log_level::DEBUG, "release_ref -1- old_counter.internal_count=" + std::to_string(old_counter.internal_count));
            if (!new_counter.internal_count && !new_counter.external_counters) { // 计数器个数为0，也没有外部计数器引用，可以删除该节点
                delete this;
            }
        }
    };
    // 静态成员函数，可以操作多个计数器
    /*
        增加对对节点的外部引用计数，
        同时增加两个计数指针的引用，
        增加的是外部节点的引用，而非节点的引用计数器。
    */
    static void increase_external_count(std::atomic<counted_node_ptr>& curr_counter, counted_node_ptr& old_counter) {
        counted_node_ptr new_counter;
        do { // 获取当前tail计数器
            new_counter = old_counter;
            ++new_counter.external_count; // 最初为0，线程引用则加1
            // while并不阻塞，新线程来引用时，external_count++, 然后跳出，可能出现多个线程拥有同一个head
        } while(!curr_counter.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire, std::memory_order_relaxed));
        old_counter.external_count = new_counter.external_count;
    }
    /*
        释放一个节点的引用，同时将外部节点的引用计数加入到内部节点中，
        当节点的引用计数为零时将节点删除。计数器中的外部引用也会减一
    */
    static void free_external_counter(counted_node_ptr& old_node_ptr) { // 对一个节点，只会有一个线程调用
        // 当前节点的next
        node* const ptr = old_node_ptr.ptr;
        int const count_increase = old_node_ptr.external_count - 2; // 还剩几个线程引用该节点，可能时tail的引用计数器
        threadsafe_print_level(log_level::DEBUG, "free_external_counter -1- old_node_ptr.external_count=" + std::to_string(old_node_ptr.external_count) + " count_increase=" + std::to_string(count_increase));
        node_counter old_counter = ptr->count.load(std::memory_order_relaxed); // 获取下一个节点的计数器count，初始化为2，表示head和tail同时引用了
        threadsafe_print_level(log_level::DEBUG, "free_external_counter -2- old_counter.internal_count=" + std::to_string(old_counter.internal_count));
        threadsafe_print_level(log_level::DEBUG, "free_external_counter -3- old_counter.external_counters=" + std::to_string(old_counter.external_counters));
        node_counter new_counter;
        do {
            threadsafe_print_level(log_level::DEBUG, "free_external_counter -4- old_counter.internal_count=" + std::to_string(old_counter.internal_count));
            new_counter = old_counter;
            --new_counter.external_counters; // 1 计数器外部节点计数器引用减一，表示此时tail已经没有指向该节点
            threadsafe_print_level(log_level::DEBUG, "free_external_counter -5- new_counter.external_counters=" + std::to_string(new_counter.external_counters));
            new_counter.internal_count += count_increase; // 2 将外部节点引用的计数加到内部节点引用
            threadsafe_print_level(log_level::DEBUG, "free_external_counter -6- new_counter.internal_count=" + std::to_string(new_counter.internal_count));
        } while(!ptr->count.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire, std::memory_order_relaxed));
        if (!new_counter.internal_count && !new_counter.external_counters) {
            threadsafe_print_level(log_level::DEBUG, "free_external_counter -7- delete ptr, new_counter.internal_count=" + std::to_string(new_counter.internal_count) + " new_counter.external_counters=" + std::to_string(new_counter.external_counters));
            delete ptr;
        }
    }
public:
    lock_free_queue() {
        counted_node_ptr tmp;
        tmp.ptr = new node();
        tmp.external_count = 0;
        head.exchange(tmp);
        tail.exchange(tmp);
    }
    lock_free_queue(const lock_free_queue& other)=delete;
    lock_free_queue& operator=(const lock_free_queue& other)=delete;
    ~lock_free_queue() {
        while(pop()) {}
    }
    void push(T new_value, std::string str="") {
        std::unique_ptr<T> new_data(new T(new_value)); // 为new value分配内存，返回指针
        // 准备一个新next，用来替代tail
        counted_node_ptr new_next; // 声明预备tail，并指向一个空node
        new_next.ptr = new node(); // 提前创建一个空node，指向nullptr
        new_next.external_count = 1; // 即将成为tail的虚拟节点引用提前置1，表示该node引用了改next
        counted_node_ptr old_tail = tail.load(); // 获取当前尾节点,具体的值不重要，只是用来比较
        // 尝试替换tail
        while(true) {
            // 可能多个线程同时拥有同一个head
            increase_external_count(tail, old_tail); // // 增加外部节点的引用（非计数器），线程引用了该节点，所以加一
            // 上方，获得当前tail（指向当前node的next），对当前tail的外部计数加一，表示当前线程引用了这个节点，old_tail也引用了这个节点
            T* old_data = nullptr;
            // 条件失败就会导致一直循环，从而导致cpu一直被占用，多个线程争抢对data赋值，失败的会释放该节点引用，并尝试获取，以下会忙等消耗cpu，直至tail真正改变
            // 忙等就是一个锁，当前只是一个半无锁结构
            if (old_tail.ptr->data.compare_exchange_strong(old_data, new_data.get())) { // 交换指针，不用拷贝，只是交换指针，不会影响其他线程
                // 某线程获得下一个节点的操作权，将其赋值为新的next
                old_tail.ptr->next = new_next; // 旧的tail指向的node->next被覆盖，指向新空node
                old_tail = tail.exchange(new_next); // tail移动到为下一个节点next，old_tail仍为当前节点
                free_external_counter(old_tail); // // 此时内部计数的外部计数将减一,增加节点，外部计数器应该变化， tail已经后移，旧的tail（当前节点的next外部引用释放）需要释放引用
                new_data.release(); // 释放new_data的所有权，不再管理new_data的内存
                break;
            }
            old_tail.ptr->release_ref(); // //指针离开节点则内部计数减一 释放引用，不再管理new_data的内存
        }
    }
    std::shared_ptr<T> pop(std::string str="") {
        counted_node_ptr old_head = head.load(std::memory_order_relaxed);
        while(true) {
            increase_external_count(head, old_head); // 增加节点引用（非计数器引用）
            node* const ptr = old_head.ptr; // 需要pop的节点
            if (ptr == tail.load().ptr) {//条件满足则队列为空
                // head与tail相同，表示队列为空，解除引用
                ptr->release_ref();
                return std::unique_ptr<T>();
            }
            if(head.compare_exchange_strong(old_head, ptr->next)) {// 条件满足则取出数据
                // 某个线程获得了head的所有权，将其赋值为next
                T* const res= ptr->data.exchange(nullptr);
                free_external_counter(old_head);//释放计数器节点，减少节点，外部计数器应该变化
                return std::unique_ptr<T>(res);
            }
            ptr->release_ref();// 指针离开节点，则内部数据减一
        }
    }

public:
    std::unique_ptr<T> pop_optimized() {
		counted_node_ptr old_head=head.load(std::memory_order_relaxed);
		for(;;) {
			increase_external_count(head,old_head);
			node* const ptr=old_head.ptr;
			if(ptr == tail.load().ptr) {
				return std::unique_ptr<T>();
			}
			counted_node_ptr next_optimized = ptr->next_optimized.load(); // 2 默认memory_oreder_seq_cst
			if(head.compare_exchange_strong(old_head, next_optimized)) {
				T* const res = ptr->data.exchange(nullptr); // exchange()返回旧值
				free_external_counter(old_head);
				return std::unique_ptr<T>(res);
			}
			ptr->release_ref();
		}
	}

    /*
        更新尾节点
    */
    void set_new_tail(counted_node_ptr& old_tail, counted_node_ptr const& new_tail) {// 1
        node* const current_tail_ptr = old_tail.ptr;
        while (!tail.compare_exchange_weak(old_tail, new_tail)
                && old_tail.ptr == current_tail_ptr);//如果没有线程修改尾节点或者pop尾节点，那么修改尾节点
        if (old_tail.ptr == current_tail_ptr) // 3 
            free_external_counter(old_tail); // 4 
        else
            current_tail_ptr->release_ref(); // 5 
    }
    //提高性能的push
    void push_optimized(T new_value) {
        std::unique_ptr<T> new_data(new T(new_value));
        counted_node_ptr new_next;
        new_next.ptr = new node;
        new_next.external_count = 1;
        counted_node_ptr old_tail = tail.load();
        for (;;) {
            increase_external_count(tail, old_tail);
            T* old_data = nullptr;
            // next修改工作分配给不同线程，一个线程要么改data，一个线程改next
            if (old_tail.ptr->data.compare_exchange_strong(old_data, new_data.get())) {// 6 将数据放入到原来的节点中
                counted_node_ptr old_next = {0};//old_next 应该为空
                if (!old_tail.ptr->next_optimized.compare_exchange_strong(old_next, new_next)) {// 7 
                    //如果原来的next不空，则说明已经有其他线程已经修改尾指针
                    delete new_next.ptr; // 8 此时数据已经push成功，因此释放new出的节点
                    new_next = old_next; // 9 new_next也应该置空
                }
                set_new_tail(old_tail, new_next);//修改tail指针
                new_data.release();
                break;
            } else {// 10  说明其他线程已经修改数据
                counted_node_ptr old_next = { 0 };
                if (old_tail.ptr->next_optimized.compare_exchange_strong(old_next, new_next)) { // 如果还未修改尾指针，则由该线程修改 
                    //此处是复制操作，非指针操作,但是ptr指针为浅拷贝
                    old_next = new_next; // 12 old_next 指向的node和old_tail指向的是同一个
                    new_next.ptr = new node; // 13 因为该线程new的节点已经称为了tail，因此，需要重新new
                }
                set_new_tail(old_tail, old_next); // 更新节点指针，tail指向下一个节点
            }
        }
    }
};
void test_lockfree_queue() {
    lock_free_queue<int> si;
    std::vector<int> vec{1,2,3};
    cout << "----->正常添加并读取" << endl;
    for (auto& i : vec) {
        si.push(i);
        cout << "push " << i << endl;
    }
    int i;
    std::shared_ptr<int> p;
    while(true) {
        p = si.pop();
        if (p.get()) {
            cout << *p << endl;
        } else {
            break;
        }
    }
    cout << "----->栈的复用" << endl;
    si.push(1);
    cout << "push 1 again." << endl;
    while(true) {
        p = si.pop();
        if (p.get()) {
            cout << "pop " << *p << endl;
        } else {
            break;
        }
    }
    cout << "----->空栈的读取" << endl;
    int cnt = 5;
    while(cnt) {
        p = si.pop();
        if (p.get()) {
            cout << *p << endl;
        } else {
            cout << "p == nullptr." << endl;
        }
        --cnt;
    }
    cout << endl;
    return;
    cout << "test threads." << endl;
    cnt = 1;
    g_log_level = log_level::DEBUG;
    // g_log_level = log_level::DEBUG;
    while(cnt) {
        job_done = false;
        std::thread t1([&si](){
            for (int i = 0; i < 1000; ++i) {
                si.push(i);
                // si.push_optimized(i);
            }
            // std::this_thread::sleep_for(std::chrono::seconds(1));
            job_done = true;
            threadsafe_print_level(log_level::INFO, "t1 done");
        });
        auto func_pop = [&si](std::string str){
            while(true) {
                // int i;
                // if (si.pop(i)) { // 函数bool pop(T& result)
                //     threadsafe_print(str + " pop " + std::to_string(i));
                //     continue;
                // } else {
                //     threadsafe_print(str + " pop nullptr");
                // }
                // threadsafe_print(str + " pop try to pop.");
                std::shared_ptr<int> p = si.pop(str); // 函数std::shared_ptr<T> pop()
                // std::unique_ptr<int> p(std::move(si.pop_optimized()));
                if (p.get()) {
                    threadsafe_print_level(log_level::INFO, str + " pop " + std::to_string(*(p)));
                    continue;
                } else {
                    threadsafe_print_level(log_level::INFO, str + " pop nullptr");
                }
                if (job_done) {
                    break;
                }
            }
            threadsafe_print_level(log_level::INFO, str + " done");
        };
        std::thread t2(func_pop, "t2");
        std::thread t3(func_pop, "t3");
        std::thread t4(func_pop, "t4");
        std::thread t5(func_pop, "t5");
        std::thread t6(func_pop, "t6");
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        std::shared_ptr<int> p1 = si.pop();
        if (p) {
            threadsafe_print_level(log_level::INFO, "main pop " + std::to_string(*p1));
        }
        --cnt;
    }
}

template<typename T>
class test_struct{
public:
    struct node;
    struct counted_node_ptr {
        int external_count{0};
        node* ptr{nullptr};
    };
    std::atomic<counted_node_ptr> head; // 有自己的引用计数器
    std::atomic<counted_node_ptr> tail; // 有自己的引用计数器
    struct node_counter { // 保证总体大小32bit，保证包在32位或64位机器上都能正常工作
        unsigned internal_count:30; // 30位，2^30-1=1073741823 位域定义的标准形式。
        unsigned external_counters:2; // 2位，2^2-1=3 位域定义的标准形式。不支持寻址，最多有两个计数器
    };
    struct node {
        std::atomic<T*> data{nullptr};
        std::atomic<node_counter> count;
        counted_node_ptr next;
        node() {
            node_counter new_count;
            new_count.internal_count = 0; //
            new_count.external_counters = 2; // 当新节点加入时，会被tail和上一个节点的next引用，所以需要2
            count.store(new_count);
            next.ptr = nullptr;
            next.external_count = 0;
        }
    };
    // 静态成员函数，可以操作多个计数器
    static void increase_external_count(std::atomic<counted_node_ptr>& curr_counter, counted_node_ptr& old_counter) {
        counted_node_ptr new_counter;
        do {
            new_counter = old_counter;
            ++new_counter.external_count;
        } while(!curr_counter.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire, std::memory_order_relaxed));
        old_counter.external_count = new_counter.external_count;
    }
    void push() {
        counted_node_ptr old_tail = tail.load();
        increase_external_count(tail, old_tail);
        cout << "push done" << endl;
    }
};
void test() {
    test_struct<int> si;
    si.push();

    struct MyStruct {
        int x;
        int y;
        // MyStruct(){}; // 自定义的默认构造函数，如果是atomic包着，不允许
        // MyStruct() : x(0), y(0) {} // 自定义的默认构造函数，如果是atomic包着，不允许
    };
    std::atomic<MyStruct> mystruct; // use of deleted function ‘std::atomic<_Tp>::atomic() [with _Tp = test()::MyStruct]
}
int main(int argc, char* argv[]) {
    // test();
    // return 0;
    // 复杂性主要在于内存的管理，使用更多的松散操作可以提高并发性
    // 非阻塞数据结构
    // 1. 无阻碍，若所有其他线程都暂停了，任何给定线程都将在一定时间内完成其操作
    // 2. 无锁，多个线程对一个数据结构操作，经过一定时间后，其中一个线程的操作一定会完成
    // 3. 无等待，即使其他线程操作数据结构，每个线程多会在一定时间内完成
    // 4. 可能受活锁影响
    // 无锁数据结构
    // 1. 不受死锁影响
    // 2. 线程不能做相同的事情
    // 3. 当前线程挂起，不影响其他线程
    // 4. 会让一些线程不停的重试
    // 无等待数据结构
    // 1. 保证无锁
    // 2. 既要保证每一个操作一次性完成，又要保证每个线程都能在一定时间内完成
    // 无锁-无等待优势
    // 1. 最大并发性
    //    无锁不意味着不等待，反而要反复访问数据结构，直到成功为止，有些线程循环次数少，有些则多
    //    既要无锁（使得并发最大化，高鲁棒性，），又要避免无限循环（成本），需要一些技巧
    //    虽然无锁，但有可能有活锁（两个线程都试图修改，然后都导致了另一个线程重启，反反复复进入循环），活锁不会存在很久
    // 2. 鲁棒性
    //    有锁结构中可能因获得锁时被中止，数据结构永久性破坏，就需要保证操作的顺序约束
    // 无锁-无等待缺点
    // 1. 无锁-无等待数据结构的实现复杂
    // 2. 无锁-无等待数据结构的性能不如有锁数据结构，反复循环，线程通过一个原子变量的数据进行同步

    // pop核心问题，想要删除还能被其他读者线程访问的节点，该怎么办?
    // test_lockfree_stack(); // 通过记录同时pop的线程数量，无锁栈，注意pop出的节点可能被其他线程访问, 高负荷时效率不高
    // test_lockfree_stack_hazard_pointer(); // 通过风险指针，风险指针是通过将使用中的节点存放到链表中,利用线程风险指针槽辅助判断，确保node删除前没有其他线程访问，无锁栈，注意pop出的节点可能被其他线程访问, 高负荷时效率不高
    // test_lockfree_stack_ref_count_sharedptr(); // 通过引用计数，智能指针实现方式，shared_ptr可能有性能问题，通过对每个节点上访问的线程数量进行统计
    // test_lockfree_stack_ref_count_doublecnt(); // 通过引用计数，外部和内部两种计数实现方式，两个计数之和为总计数
    // test_lockfree_stack_ref_count_doublecnt_mmopt(); // 通过引用计数，外部和内部两种计数实现方式，两个计数之和为总计数，优化，减少内存分配，尝试更多的松散操作
    test_lockfree_queue(); //

    // 一些建议
    // 1.首先使用memory_order_seq_cst内存模型，如果性能不够，再考虑使用其他松散内存模型，但是要保证正确性，最好充分测试
    // 2.内存管理是最大区别，多线程访问时就不能轻易删除节点，但还是希望删除
    //   三种方法：1.风险指针，2.引用计数，3.无线程对数据结构访问时，删除所有等待删除对象
    //            4.循环节点，只有在数据结构被销毁时才将节点完全删除，注意ABA问题
    //            ABA问题
    // 3.识别忙等循环，如果有忙等循环，就要考虑是否有更好的方法，如该线程可以帮助其他线程完成一些工作（需要一些原子变量）
    return 0;
}
