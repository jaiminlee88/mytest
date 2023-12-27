#include<sys/mman.h>
#include<iostream>
#include<unistd.h>

int main() {
    long maxbytes = 900000;
    std::cout << "malloc "<< maxbytes <<" bytes:" << std::endl;
    char*p = (char*)malloc(maxbytes);
    if (p == nullptr) {
        std::cout << "malloc failed." << std::endl;
    } else {
        std::cout << "malloc succeed." << std::endl;
        free(p);
        std::cout << "free" << std::endl;
    }

    std::cout << "mmap "<< maxbytes <<" bytes:" << std::endl;
    p = (char*)mmap(0, maxbytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (p == MAP_FAILED) {
        std::cout << "mmap failed." << std::endl;
    } else {
        std::cout << "mmap succeed." << std::endl;
        munmap(p, maxbytes);
        std::cout << "munmap" << std::endl;
    }

    // std::cout << "brk "<< maxbytes <<" bytes:" << std::endl;
    // brk()
}