#include <chrono>
#include <iostream>
#include <thread>
using namespace std;

int main() {
    // 使用system_clock获取当前时间
    std::chrono::system_clock::time_point system_now = std::chrono::system_clock::now();

    // 使用steady_clock获取开始时间，精度更高更稳定
    std::chrono::steady_clock::time_point steady_start = std::chrono::steady_clock::now();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 使用steady_clock获取结束时间
    std::chrono::steady_clock::time_point steady_end = std::chrono::steady_clock::now();

    // 使用high_resolution_clock获取开始时间
    std::chrono::high_resolution_clock::time_point high_res_start = std::chrono::high_resolution_clock::now();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 使用high_resolution_clock获取结束时间
    std::chrono::high_resolution_clock::time_point high_res_end = std::chrono::high_resolution_clock::now();

    // 输出时间间隔
    std::chrono::duration<double> steady_duration = std::chrono::duration_cast<std::chrono::duration<double>>(steady_end - steady_start);
    std::chrono::duration<double> high_res_duration = std::chrono::duration_cast<std::chrono::duration<double>>(high_res_end - high_res_start);


    cout << " steady_duration=" << steady_duration.count() << endl;
    cout << " high_res_duration=" << high_res_duration.count() << endl;
}