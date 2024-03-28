#include <iostream>
using namespace std;
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void test1() {
    cout << "this is test1" << endl;
}



//========================================
void testfork_core() {
    fork() && fork() && fork() && sleep(3);
    cout << "core" << endl;
}
void testfork() {
    cout << "this is testfork" << endl;
    testfork_core();
    return;
}

//========================================
// make tu test_setbit 第一位direction 第2，3位offset，后面是instrument
#define BITSET_DIRECTION 0x0
#define BITSET_OFFSET 0x1
#define BITSET_INSTRUMENT 0x3
#define BITSET_DIRECTION_MASK 0x1
#define BITSET_OFFSET_MASK 0x3
#define BITSET_INSTRUMENT_MASK 0x1fffffff
int get_direction(int& val) {
    return (val >> BITSET_DIRECTION) & BITSET_DIRECTION_MASK;
}
int get_offset(int& val) {
    return (val >> BITSET_OFFSET) & BITSET_OFFSET_MASK;
}
int get_instrument(int& val) {
    return (val >> BITSET_INSTRUMENT) & BITSET_INSTRUMENT_MASK;
}
int set_direction(int& val, int direct) {
    if (direct < 0 && direct > BITSET_DIRECTION_MASK) {
        return -1;
    }
    val = (val & (~BITSET_DIRECTION_MASK)) | (direct << BITSET_DIRECTION);
    // 注意按位非是~，按位与是&，按位或是|，按位异或是^，！不是按位非，是逻辑非
    return 0;
}
int set_offset(int& val, int offset) {
    if (offset < 0 || offset > BITSET_OFFSET_MASK) {
        return -1;
    }
    val = (val & (~(BITSET_OFFSET_MASK << BITSET_OFFSET))) | (offset << BITSET_OFFSET);
    return 0;
}
int set_instrument(int& val, int inst) {
    if (inst < 0 || inst > BITSET_INSTRUMENT_MASK) {
        return -1;
    }
    val = (val & (~(BITSET_INSTRUMENT_MASK << BITSET_INSTRUMENT))) | (inst << BITSET_INSTRUMENT);
    return 0;
}
void test_setbit() {
    int val = 0;
    int direct = 0;
    int offset = 0;
    int inst = 0;
    int ori_val = val;

    cout << "ori_val=" << ori_val << endl;
    set_direction(val, 1);
    cout << "val=" << val << endl;
    set_offset(val, 1);
    cout << "val=" << val << endl;
    set_instrument(val, 1);
    cout << "val=" << val << endl;
    set_offset(val, 3);
    cout << "val=" << val << endl;
    set_instrument(val, 4);
    cout << "val=" << val << endl;
}

int main() {
    // m tu test_setbit
    cout << "=============start test=============" << endl;
#ifdef func
    func();
#endif
    return 0;
}