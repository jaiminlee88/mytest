
#include <stdio.h>

void b1();
void b2();

int main()
{
    b1();
    b2(); // 共享对象全局符号介入（Global Symbol Interpose）,加载a2.so时发现a已经存在了，则不加载
    return 0;
}
