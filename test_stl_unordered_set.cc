#include <iostream>
#include <unordered_set>

using namespace std;

struct MyKey {
    int x;
    int y;

    bool operator==(const MyKey& other) const {
        return x == other.x && y == other.y;
    }
};

// 自定义哈希函数
struct MyHash {
    std::size_t operator()(const MyKey& k) const {
        return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1);
    }
};

int main() {
    std::unordered_set<MyKey, MyHash> mySet;

    // 插入元素
    mySet.insert({1, 2});
    mySet.insert({2, 3});

    // 查找元素
    MyKey key = {1, 2};
    if (mySet.find(key) != mySet.end()) {
        std::cout << "Found key {1, 2}" << std::endl;
    } else {
        std::cout << "Key {1, 2} not found" << std::endl;
    }

    return 0;
}
