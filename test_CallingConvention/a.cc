void __attribute__((__fastcall__)) foo(int, int);

int main()
{
    foo(1, 3);
    return 0;
}


// 【 在 intron (内含子) 的大作中提到: 】

// : stdcall,fastcall一定没有。

// : 因为UNIX下都是32位线性指针，或是新型的64位指针。

// : cdecl也没有。参数传送顺序均C语言顺序，即使是UNIX下的pascal,basic也是如此。

// : 这个大概与当初UNIX是用C建造的历史有关。