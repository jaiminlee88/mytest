#include <iostream>
#include <iomanip>

#define utils_consume_time_decorate(func) { \
    auto now = std::chrono::steady_clock::now();\
    func;\
    auto end = std::chrono::steady_clock::now();\
    auto steady_duration = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - now);\
    std::cout << fixed << setprecision(10) << #func << "--->consume=" << steady_duration.count() << std::endl;\
}

// #是字符串化的意思，出现在宏定义中的#是把跟在后面的参数转成一个字符串；
#define CONCAT(str) #str "("
#define utils_print_copy_decorate(data) { \
    std::cout << fixed << setprecision(10) << #data << "(" << data.size() <<") ";\
    copy(data.begin(), data.end(), ostream_iterator<decltype(data)::value_type>(std::cout, " "));\
    std::cout << std::endl;\
}

#define utils_print_copy_iter_decorate(start, end) { \
    std::cout << #start << "(" << end - start <<") ";\
    for (auto& it = start; it != end; ++it) {\
        std::cout << *it << " ";\
    }\
    std::cout << std::endl;}

enum class utils_log_level {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
};

#define utils_threadsafe_print_level(utils_level, g_utils_log_leve, print_lock, str) \
{if (g_utils_log_leve <= utils_level) { \
    std::lock_guard<std::mutex> lock(print_lock); \
    std::cout << str << std::endl; }}

// 异常安全的代码
class utils_join_threads {
private:
    std::vector<std::thread>& threads;
public:
    explicit utils_join_threads(std::vector<std::thread>& threads_) : threads(threads_) {}
    ~utils_join_threads() {
        for (unsigned long i = 0; i < threads.size(); ++i) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }
    }
};