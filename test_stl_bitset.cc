#include <iostream>
#include <bitset>

using namespace std;

int main () {
    // std::bitset 是一个固定大小的位集合，可以有效地表示一组布尔值的集合，并支持位操作。
    // std::bitset 使用一个 unsigned long 或 unsigned long long 数组来存储位，每个 unsigned long 或 unsigned long long 可以存储多个位。这样设计的目的是为了提高位操作的效率和性能。

    std::bitset<5> bits0; // 创建一个包含5位的bitset，初始值为0
    std::cout << "Initial bits0: " << bits0 << std::endl;

    // 创建一个包含5位的bitset，初始值为10101
    std::bitset<5> bits("10101");

    // 输出当前bitset的值
    std::cout << "Initial bits: " << bits << std::endl;

    // 使用set方法设置第3位为1
    bits.set(3);
    std::cout << "After set(3): " << bits << std::endl;

    // 使用reset方法重置第2位为0
    bits.reset(2);
    std::cout << "After reset(2): " << bits << std::endl;

    // 使用flip方法翻转第4位
    bits.flip(4);
    std::cout << "After flip(4): " << bits << std::endl;

    // 使用test方法测试第1位是否为1
    if (bits.test(1)) {
        std::cout << "Bit 1 is set." << std::endl;
    } else {
        std::cout << "Bit 1 is not set." << std::endl;
    }

    // 使用any方法检查是否有任何位被设置为1
    if (bits.any()) {
        std::cout << "At least one bit is set." << std::endl;
    } else {
        std::cout << "No bit is set." << std::endl;
    }

    // 使用none方法检查是否没有任何位被设置为1
    if (bits.none()) {
        std::cout << "No bit is set." << std::endl;
    } else {
        std::cout << "At least one bit is set." << std::endl;
    }

    // 使用count方法计算设置为1的位的数量
    std::cout << "Number of set bits: " << bits.count() << std::endl;

    // 使用to_string方法将bitset转换为字符串
    std::string bitString = bits.to_string();
    std::cout << "Bitset as string: " << bitString << std::endl;

    return 0;

}