
#include <stdio.h>

void __attribute__((__cdecl__)) foo(int a, int b)
{
    printf("a=%d,b=%d", a, b);
}