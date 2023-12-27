#include <iostream>

extern "C" bool login(int a, int b, std::string user, std::string pwd) {
    std::cout << "user " << user << "; pwd " << pwd << std::endl;
    return true;
}