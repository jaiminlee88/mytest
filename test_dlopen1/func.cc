
#include <iostream>
// #include <stdio.h>

extern "C" void func() { // extern "C" 不能少, 否则会被编译器改名
    std::cout << "i am func" << std::endl;
    // printf("i am func\n");
}
