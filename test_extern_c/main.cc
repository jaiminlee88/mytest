#include <iostream>

// 注意声明，是两个参数，与库内多个参数不同，
// 但库内用extern "C"声明，所以不会被编译器改名，但也不会检查参数个数
extern "C" bool login(std::string user, std::string pwd);

int main() {
    std::string user = "pxcai";
    std::string pwd = "hello world";
    login(user, pwd);
    return 0;
}