// 测试main之前运行的全局构造函数

#include <iostream>

#include <stdio.h>
void my_init(void) {
       printf("my_init ");
}
void my_init1(void) {
       printf("my_init_1 ");
}

typedef void (*ctor_t)(void);
//在.ctors段里添加一个函数指针
ctor_t __attribute__((section(".ctors"))) my_init_p = &my_init;

void my_init1(void) __attribute__ ((constructor));

int main() {
    printf("main-> hello World!\n");
    fread
    return 0;
}


