#include <iostream>

extern "C" {
    void print(int a){
        std::cout << "[print] This is myfunc.cc a=" << a << std::endl;
    }
}

void print1(int a){
    std::cout << "[print1] This is myfunc.cc a=" << a << std::endl;
}