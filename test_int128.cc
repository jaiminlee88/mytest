#include<iostream>

using namespace std;

// // 128位整数实现(工程)
// // 实现一个有符号的128位整数类，满足128位整数的加减乘除和比较需求。要求运算效率高，存储空间小。16 字节，两个long
// // //128位有符号整数，有效的数值范围是-2^127~2^127-1/即
// // -170141183460469231731687303715884105728 170141183460469231731687303715884105727
// class int128 {
// public:
//     int128(uint64_t l = 0, uint64_t u = 0) : lower(l), upper(u) {}
// public:
//     int128 operator +(const int128& r)noexcept {
//         uint64_t sum_lower = lower + other.lower;
//         uint64_t carry = (sum_lower < lower) ? 1 : 0;
//         uint64_t sum_upper = upper + other.upper + carry;
//         return int128(sum_lower, sum_upper);
//     }
// //实现此方法
//     int128 operator -(const int128& r) noexcept {
//         uint64_t diff_lower = lower - other.lower;
//         uint64_t borrow = (diff_lower > lower) ? 1 : 0;
//         uint64_t diff_upper = upper - other.upper - borrow;
//         return int128(diff_lower, diff_upper);
//     }
// //实现此方法
//     int128 operator *(const int128& r) noexcept {
//         uint64_t a0 = lower & 0xFFFFFFFF;
//         uint64_t a1 = (lower >> 32) & 0xFFFFFFFF;
//         uint64_t a2 = upper & 0xFFFFFFFF;
//         uint64_t a3 = (upper >> 32) & 0xFFFFFFFF;

//         uint64_t b0 = other.lower & 0xFFFFFFFF;
//         uint64_t b1 = (other.lower >> 32) & 0xFFFFFFFF;
//         uint64_t b2 = other.upper & 0xFFFFFFFF;
//         uint64_t b3 = (other.upper >> 32) & 0xFFFFFFFF;

//         uint64_t c0 = a0 * b0;
//         uint64_t c1 = a1 * b0 + a0 * b1 + (c0 >> 32);
//         uint64_t c2 = a2 * b0 + a1 * b1 + a0 * b2 + (c1 >> 32);
//         uint64_t c3 = a3 * b0 + a2 * b1 + a1 * b2 + a0 * b3 + (c2 >> 32);
//         uint64_t c4 = a3 * b1 + a2 * b2 + a1 * b3 + (c3 >> 32);
//         uint64_t c5 = a3 * b2 + a2 * b3 + (c4 >> 32);
//         uint64_t c6 = a3 * b3 + (c5 >> 32);

//         uint64_t result_lower = (c1 << 32) | c0;
//         uint64_t result_upper = (c3 << 32) | (c2 & 0xFFFFFFFF);
//         return int128(result_lower, result_upper);
//     }
// //实现此方法
//     int128 operator /(const int128& r) noexcept {
//         if (other == int128(0)) {
//             std::cout << "Error: Division by zero!" << std::endl;
//             return int128();
//         }

//         int128 quotient;
//         int128 remainder = *this;
//         int128 divisor = other;

//         for (; divisor <= remainder; divisor = divisor << 1) ;
//         divisor = divisor >> 1;

//         while (remainder >= other) {
//             if (remainder >= divisor) {
//                 remainder = remainder - divisor;
//                 quotient = quotient + int128(1);
//             }
//             quotient = quotient << 1;
//             divisor = divisor >> 1;
//         }

//         return quotient;
//     }
//     //实现此方法
//     bool operator < (const int128& ) const noexcept {
//         return (upper < other.upper) || ((upper == other.upper) && (lower < other.lower));
//     }
//     bool operator==(const int128& other) const {
//         return (lower == other.lower) && (upper == other.upper);
//     }
// //实现此方法
// private:
//     //允许自行设计数据结构，允许自行添加必要的方法(如默认构造函数)及其他可用于辅助功能实现的方法或函数。
//     uint64_t lower;
//     uint64_t upper;
// };

// // 重载左移运算符
// int128 operator<<(const int128& num, int bits) {
//     uint64_t new_lower = num.lower << bits;
//     uint64_t new_upper = (num.upper << bits) | (num.lower >> (64 - bits));
//     return int128(new_lower, new_upper);
// }

// // 重载右移运算符
// int128 operator>>(const int128& num, int bits) {
//     uint64_t new_upper = num.upper >> bits;
//     uint64_t new_lower = (num.lower >> bits) | (num.upper << (64 - bits));
//     return int128(new_lower, new_upper);
// }
// // 打印整数值
// void printint128(const int128& num) {
//     std::cout << "Upper: " << num.upper << ", Lower: " << num.lower << std::endl;
// }

union Byte {
    unsigned char data;
    struct {
        unsigned char bit0 : 1;
        unsigned char bit1 : 1;
        unsigned char bit2 : 1;
        unsigned char bit3 : 1;
        unsigned char bit4 : 1;
        unsigned char bit5 : 1;
        unsigned char bit6 : 1;
        unsigned char bit7 : 1;
    } bits;
};

int main() {
    cout << "sizeof(unsigned int)=" << sizeof(unsigned int) << endl;
    cout << "sizeof(int)=" << sizeof(int) << endl;
    cout << "sizeof(double)=" << sizeof(double) << endl;
    cout << "sizeof(long)=" << sizeof(long int) << endl;
    cout << "sizeof(long long)=" << sizeof(long long int) << endl;

    Byte b;
    b.data = 0; // 初始化为0
    // 设置位
    b.bits.bit0 = 1;
    b.bits.bit3 = 1;
    b.bits.bit7 = 1;
    // 读取位
    std::cout << "Bit 0: " << static_cast<int>(b.bits.bit0) << std::endl;
    std::cout << "Bit 3: " << static_cast<int>(b.bits.bit3) << std::endl;
    std::cout << "Bit 7: " << static_cast<int>(b.bits.bit7) << std::endl;
    b.data = 0; // 初始化为0
    // 设置位
    b.bits.bit0 = 1;
    b.bits.bit1 = 1;
    b.bits.bit2 = 1;
    std::cout << "b.data=" << (int)b.data << endl;
    std::cout << "sizeof(Byte)=" << sizeof(Byte) << endl;

    int a = 0;
    cout << "~a=" << (~a)/2 << endl;
    // int128 a(1234567890, 9876543210);
    // int128 b(9876543210, 1234567890);

    // int128 sum = a + b;
    // int128 diff = a - b;
    // int128 product = a * b;
    // int128 quotient = a / b;

    // std::cout << "Sum: ";
    // printint128(sum);

    // std::cout << "Difference: ";
    // printint128(diff);

    // std::cout << "Product: ";
    // printint128(product);

    // std::cout << "Quotient: ";
    // printint128(quotient);
}