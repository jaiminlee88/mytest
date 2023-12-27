int shared = 1;

void swap(int *a, int *b) {
    *a ^= *b ^= *a ^= *b;
}

// objdump -d a.o
// 00000000 <main>:
//    0: 8d 4c 24 04           lea    0x4(%esp),%ecx
//    4: 83 e4 f0              and    $0xfffffff0,%esp
//    7: ff 71 fc              pushl  0xfffffffc(%ecx)
//    a: 55                    push   %ebp
//    b: 89 e5                 mov    %esp,%ebp
//    d: 51                    push   %ecx
//    e: 83 ec 24              sub    $0x24,%esp
//   11: c7 45 f8 64 00 00 00  movl   $0x64,0xfffffff8(%ebp)
//   18: c7 44 24 04 00 00 00  movl   $0x0,0x4(%esp)
//   1f: 00 
//   20: 8d 45 f8              lea 0xfffffff8(%ebp),%eax
//   23: 89 04 24              mov    %eax,(%esp)
//   26: e8 fc ff ff ff        call   27 <main+0x27>
//   2b: 83 c4 24              add    $0x24,%esp
//   2e: 59                    pop    %ecx
//   2f: 5d                    pop    %ebp
//   30: 8d 61 fc              lea 0xfffffffc(%ecx),%esp
//   33: c3                    ret  


// objdump -r a.o
// a.o:     file format elf32-i386
// RELOCATION RECORDS FOR [.text]:
// OFFSET   TYPE              VALUE 
// 0000001c R_386_32          shared
// 00000027 R_386_PC32        swap