#include <map>
#include <string>
#include <functional>
#include <iostream>

using namespace std;
// 自定义比较函数
struct CustomCompare {
    bool operator()(const std::string& a, const std::string& b) const {
        return a.length() < b.length();
    }
};

int main() {
    // 使用自定义比较函数的 map
    std::map<std::string, int, CustomCompare> myMap;

    // 插入元素
    myMap["apple"] = 1;
    myMap["banana"] = 2;
    myMap["orange"] = 3;
    myMap["pineapple"] = 4;

    // 遍历输出
    for (const auto& pair : myMap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    cout << myMap.begin()->first << " " << myMap.begin()->second << endl;
    myMap.erase(myMap.begin());
    cout << myMap.begin()->first << " " << myMap.begin()->second << endl;
    return 0;
}