
#include <iostream>
#include "func.hh"

#include <dlfcn.h>

#include <cxxabi.h>

int main() {

    void* handler1 = dlopen("libfunc.so", RTLD_LAZY);
    std::cout << "handler1 : " << handler1 << std::endl;
    using func_type = void(*)();

    func_type f1 = (func_type)dlsym(handler1, "func"); // 可能会找不到，因为函数名被编译器改了

    std::cout << "f1: " << (void*)f1 << std::endl;

    f1();
    dlclose(handler1);

    return 0;
}
