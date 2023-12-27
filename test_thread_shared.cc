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

std::mutex mlock;
std::list<int> some_list;

// void add_to_list(int value) {
//     vector<int> tmp;
//     for (int i=1)
//     std::lock_guard<std::mutex> guard(mlock);
//     some_list.push_back(value);
// }
// bool list_contains(int value) {
//     std::lock_guard<std::mutex> guard(mlock);
//     return find(some_list.begin(), some_list.end(), value) != some_list.end();
// }

void test1(){ // access to global or static variable
    std::thread t1([]()->void {
        vector<int> vec;
        for (int i=1; i<10; ++i) {
            vec.push_back(i);
        }
        while(vec.size()){
            {
                int j = vec.front();
                vec.erase(vec.begin());
                std::lock_guard<std::mutex> guard(mlock);
                some_list.push_back(j);
                cout << "push j=" << j << endl;
            }
            sleep(1);
        }
    });

    std::thread t2([](int value)->void {
        while(true) {
            {
                std::lock_guard<std::mutex> guard(mlock);
                cout << "try to find value=" << value << endl;
                if (find(some_list.begin(), some_list.end(), value) != some_list.end()) {
                    cout << "found value=" << value << " , exit thread." << endl;
                    break;
                }
            }
            sleep(1);
        }
    },5);
    t1.join();
    t2.join();
}

std::mutex mlock2;
std::mutex mlock3;
int num1{0};
int num2{0};

void test2() {
    std::thread t1([](){
        cout << "thread t1 start." << endl;
        int cnt = 100;
        while (cnt) {
            cout << "[t1] before t1 num1=" << num1 << ", num2=" << num2 << endl;
            {
                std::lock(mlock2, mlock3);
                std::lock_guard<std::mutex> g2(mlock2, std::adopt_lock);
                std::lock_guard<std::mutex> g3(mlock3, std::adopt_lock); // adopt_lock表示已经lock了，不需要再lock
                num1 = cnt;
                num2 = num1;
            }
            cout << "[t1] after t1 num1=" << num1 << ", num2=" << num2 << endl;
            sleep(1);
            --cnt;
        }
        cout << "exit t1 thread." << endl;
    });

    std::thread t2([](){
        cout << "thread t2 start." << endl;
        while(true) {
            {
                std::lock(mlock2, mlock3);
                std::lock_guard<std::mutex> g2(mlock2, std::adopt_lock);
                std::lock_guard<std::mutex> g3(mlock3, std::adopt_lock); // adopt_lock表示已经lock了，不需要再lock
                // std::scoped_lock<std::mutex, std::mutex> g2(mlock2, mlock3); // 作用同上, c++17以上
                if (num1 != num2) {
                    cout << "[t2] attention!!!  t2 num1=" << num1 << ", num2=" << num2 << endl;
                } else {
                    cout << "[t2] read  t2 num1=" << num1 << ", num2=" << num2 << endl;
                }
            }
            if (num1 == 6) {
                break;
            }
            sleep(1);
        }
        cout << "exit t2 thread." << endl;
    });
    t1.join();
    t2.join();
}

class hierarchical_mutex {
public:
    std::mutex internal_mutex;
    unsigned long const hierarchy_value;
    unsigned long previous_hierarchy_value;
    static thread_local unsigned long this_thread_hierarchy_value;
    void check_for_hierarchy_violation() {
        if (this_thread_hierarchy_value <= hierarchy_value) {
            throw std::logic_error("mutex hierarchy violated.");
        }
    }
    void update_hierarchy_value() {
        previous_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;
    }
public:
    explicit hierarchical_mutex(unsigned long value) : hierarchy_value(value), previous_hierarchy_value(0){}
    void lock() {
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }
    void unlock() {
        if(this_thread_hierarchy_value != hierarchy_value) {
            throw std::logic_error("mutex hierarchy violated when unlock.");
        }
        this_thread_hierarchy_value = previous_hierarchy_value;
        internal_mutex.unlock();
    }
    bool try_lock() {
        check_for_hierarchy_violation();
        if(!internal_mutex.try_lock()) {
            return false;
        }
        update_hierarchy_value();
        return true;
    }
};

thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);
hierarchical_mutex high_lock(10000);
hierarchical_mutex middle_lock(8000);
hierarchical_mutex low_lock(5000);
void test3() {
    std::thread t1([](){
        cout << "thread t1 start." << endl;
        int cnt = 10000000;
        while (cnt) {
            // cout << "[t1] before t1 num1=" << num1 << ", num2=" << num2 << endl;
            {
                low_lock.lock();
                num1 = cnt;
                num2 = num1;
                low_lock.unlock();
            }
            // cout << "[t1] after t1 num1=" << num1 << ", num2=" << num2 << endl;
            // sleep(1);
            --cnt;
        }
        cout << "exit t1 thread." << endl;
    });

    std::thread t2([](){
        cout << "thread t2 start." << endl;
        while(true) {
            {
                // std::scoped_lock<std::mutex, std::mutex> g2(mlock2, mlock3); // 作用同上, c++17以上
                high_lock.lock();
                if (num1 != num2) {
                    cout << "[t2] attention!!!  t2 num1=" << num1 << ", num2=" << num2 << endl;
                } else {
                    // cout << "[t2] read  t2 num1=" << num1 << ", num2=" << num2 << endl;
                }
                high_lock.unlock();
            }
            if (num1 == 6) {
                break;
            }
            // sleep(0.5);
        }
        cout << "exit t2 thread." << endl;
    });
    t1.join();
    t2.join();
}


std::mutex mlock4;
std::mutex mlock5;
void test4() {
    std::thread t1([](){
        cout << "thread t1 start." << endl;
        int cnt = 100000;
        while (cnt) {
            // cout << "[t1] before t1 num1=" << num1 << ", num2=" << num2 << endl;
            {
                std::unique_lock<std::mutex> ulock1(mlock4, std::defer_lock); // 此时不要立即上锁
                std::unique_lock<std::mutex> ulock2(mlock5, std::defer_lock); // 此时不要立即上锁
                std::lock(ulock1, ulock2); // 在这里一并上锁
                num1 = cnt;
                num2 = num1;
            }
            // cout << "[t1] after t1 num1=" << num1 << ", num2=" << num2 << endl;
            // sleep(1);
            --cnt;
        }
        cout << "exit t1 thread." << endl;
    });

    std::thread t2([](){
        cout << "thread t2 start." << endl;
        while(true) {
            {
                // std::scoped_lock<std::mutex, std::mutex> g2(mlock2, mlock3); // 作用同上, c++17以上
                std::unique_lock<std::mutex> ulock1(mlock4, std::defer_lock); // 此时不要立即上锁
                std::unique_lock<std::mutex> ulock2(mlock5, std::defer_lock); // 此时不要立即上锁
                std::lock(ulock1, ulock2); // 在这里一并上锁
                if (num1 != num2) {
                    cout << "[t2] attention!!!  t2 num1=" << num1 << ", num2=" << num2 << endl;
                } else {
                    // cout << "[t2] read  t2 num1=" << num1 << ", num2=" << num2 << endl;
                }
            }
            if (num1 == 6) {
                break;
            }
            // sleep(0.5);
        }
        cout << "exit t2 thread." << endl;
    });
    t1.join();
    t2.join();
}

std::unique_lock<std::mutex> get_lock() {
    extern std::mutex mlock2;
    std::unique_lock<std::mutex> ulock(mlock2);
    return ulock; // implicit move between unique locks
}
void test5() {
    std::thread t1([](){
        cout << "thread t1 start." << endl;
        int cnt = 100000;
        while (cnt) {
            // cout << "[t1] before t1 num1=" << num1 << ", num2=" << num2 << endl;
            {
                std::unique_lock<std::mutex> ulock(get_lock()); // implicit move
                num1 = cnt;
                num2 = num1;
            }
            // cout << "[t1] after t1 num1=" << num1 << ", num2=" << num2 << endl;
            // sleep(1);
            --cnt;
        }
        cout << "exit t1 thread." << endl;
    });

    std::thread t2([](){
        cout << "thread t2 start." << endl;
        while(true) {
            {
                std::unique_lock<std::mutex> ulock(get_lock()); // implicit move
                if (num1 != num2) {
                    cout << "[t2] attention!!!  t2 num1=" << num1 << ", num2=" << num2 << endl;
                } else {
                    // cout << "[t2] read  t2 num1=" << num1 << ", num2=" << num2 << endl;
                }
            }
            if (num1 == 6) {
                break;
            }
            // sleep(0.5);
        }
        cout << "exit t2 thread." << endl;
    });
    t1.join();
    t2.join();
}

std::shared_ptr<int> resource_ptr;
std::once_flag resource_flag;
void foo(int v){
    if (!resource_ptr) {
        std::lock_guard<std::mutex> llock(mlock2);
        if (!resource_ptr) {
            resource_ptr.reset(new int(v)); // 注意，因为编译器可能使得指令乱序，多线程可能会出问题
            // 正常过程如下：
            // 1 分配内存空间
            // 2 初始化int实例并存入空间
            // 3 赋值 resource_ptr 实例引用，把指针指向这片空间

            // 但是被重排序以后可能会出现：
            // 1 分配内存空间
            // 3 赋值 resource_ptr 实例引用，把指针指向这片空间，
            //     注意此时线程挂起，但此时还没有初始化对象，其他线程发现非空，就直接访问该对象，出错
            // 2 初始化int实例并存入空间

            // 其实最为便捷的方法就是在单线程中完成初始化，不要放到多线程中
            // 请不要使用延迟初始化(lazily-initialized)的方式，除非你必须这么做
        }
    }
    std::lock_guard<std::mutex> llock(mlock2);
    cout << "resource_ptr=" << *(resource_ptr.get()) << endl;
}

void foo_callonce(int v){
    std::call_once(resource_flag, [&v](){
        resource_ptr.reset(new int(v));  // 请不要使用延迟初始化(lazily-initialized)的方式，除非你必须这么做
    });
    std::lock_guard<std::mutex> llock(mlock2);
    cout << "resource_ptr=" << *(resource_ptr.get()) << endl;
}

void test6(){
    // std::thread t1([](){
    //     // sleep(3);
    //     foo(1);
    // });
    // std::thread t2([](){
    //     // sleep(2);
    //     foo(2);
    // });
    // std::thread t3([](){
    //     // sleep(1);
    //     foo(3);
    // });
    std::thread t1([](){
        // sleep(3);
        foo_callonce(1);
    });
    std::thread t2([](){
        // sleep(2);
        foo_callonce(2);
    });
    std::thread t3([](){
        // sleep(1);
        foo_callonce(3);
    });
    t1.join();
    t2.join();
    t3.join();
}

class dns_entry;

class dns_entry {
public:
    dns_entry(): domain(""), ipAddress("") {}
    dns_entry(const std::string& domain, const std::string& ipAddress)
        : domain(domain), ipAddress(ipAddress) {}

    const std::string& getDomain() const { return domain; }

    const std::string& getIpAddress() const { return ipAddress; }

    void setIpAddress(const std::string& ipAddress) { this->ipAddress = ipAddress; }
private:
    std::string domain;
    std::string ipAddress;
};


class dns_cache { // c++17
    std::map<std::string, dns_entry> entries;
    mutable std::shared_mutex entry_mutex;
public:
    dns_entry find_entry(std::string const& domain) const {
        std::shared_lock<std::shared_mutex> lk(entry_mutex); // 读取时，shared_mutex获得共享锁
        std::map<std::string, dns_entry>::const_iterator const it = entries.find(domain);
        return (it==entries.end()) ? dns_entry() : it->second;
    }
    void update_or_add_entry(std::string const& domain, dns_entry const& dns_details) {
        std::lock_guard<std::shared_mutex> lk(entry_mutex); // 更改时，shared_mutex获得独占锁
        entries[domain] = dns_details;
    }
};
bool job_done = false;
void test7() {
    map<string, dns_entry> entries = {
        {"www.baidu.com", dns_entry("www.baidu.com","100.0.0.1")},
        {"www.google.com", dns_entry("www.google.com","100.0.0.2")},
        {"www.douban.com", dns_entry("www.douban.com","100.0.0.3")},
        {"www.sina.com", dns_entry("www.sina.com","100.0.0.4")},
        {"www.qq.com", dns_entry("www.qq.com","100.0.0.5")},
        {"www.163.com", dns_entry("www.163.com","100.0.0.6")},
        {"www.126.com", dns_entry("www.126.com","100.0.0.7")},
        {"www.360.com", dns_entry("www.360.com","100.0.0.8")}
    };
    dns_cache dns;
    std::thread t1([&dns, &entries](){
        cout << "thread t1 start." << endl;
        for (auto& entry : entries) {
            dns.update_or_add_entry(entry.first, entry.second);
            sleep(1);
        }
        job_done = true;
    });
    std::thread t2([&dns,&entries](){
        cout << "thread t2 start." << endl;
        while (!job_done) {
            for (auto& it : entries) {
                dns_entry ip_info = dns.find_entry(it.first);
                if (ip_info.getIpAddress() != "") {
                    cout << "domain=" << ip_info.getDomain() << ", ip=" << ip_info.getIpAddress() << endl;
                }
            }
            cout << "---------------------" << endl;
            sleep(1);
        }
    });
    t1.join();
    t2.join();
}

std::recursive_mutex mtx;
void recursiveFunction(int depth) {
    std::lock_guard<std::recursive_mutex> lock(mtx);

    std::cout << "Depth: " << depth << ", Thread ID: " << std::this_thread::get_id() << std::endl;

    // 递归调用
    if (depth > 0) {
        recursiveFunction(depth - 1);
    }
}
void test8(){
    std::thread t1(recursiveFunction, 5);
    std::thread t2(recursiveFunction, 3);

    t1.join();
    t2.join();
}
int main(){
    // test1(); // lock_guard
    // test2(); // lock
    // test3(); // hierarchical_mutex
    // test4(); // unique lock
    // test5(); // 移动unique lock,锁之间的传递
    // test6(); // 双重检查锁模式存在问题
    // test7(); // shared_mutex
    test8(); // recursive_mutex,可以对同一互斥变量上锁多次，嵌套锁，加锁几次，就释放几次
    cout << "exit main." << endl;
}