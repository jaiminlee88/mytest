// #include "b.cc"

extern void swap(int *a, int *b);
extern int shared;

int main(void) {
    int a = 100;
    swap(&a,&shared);
}