#include <iostream>
#include <vector>

class Bitmap {
private:
    std::vector<uint8_t> data;
    size_t size;

public:
    Bitmap(size_t size) : size(size) {
        // 计算需要的字节数，并初始化为全 0
        size_t bytes = (size + 7) / 8;
        data.resize(bytes, 0);
    }

    void set(size_t index, bool value) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }

        size_t byteIndex = index / 8;
        size_t bitIndex = index % 8;
        // size_t bitIndex = index & 7; // 与 7 等价于取模 8

        if (value) {
            data[byteIndex] |= (1 << bitIndex);  // 将对应位设置为 1
        } else {
            data[byteIndex] &= ~(1 << bitIndex); // 将对应位设置为 0
        }
    }

    bool get(size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }

        size_t byteIndex = index / 8;
        size_t bitIndex = index % 8;

        return (data[byteIndex] >> bitIndex) & 1; // 获取对应位的值
    }
};

int main() {
    Bitmap bm(10);

    bm.set(1, true);
    bm.set(3, true);
    bm.set(5, true);

    for (size_t i = 0; i < 10; i++) {
        std::cout << "Bit " << i << ": " << bm.get(i) << std::endl;
    }

    return 0;
}
