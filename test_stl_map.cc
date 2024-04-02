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

void test1() {
    map<int,string> m;
    m[1] = "hello";
    m[7] = "banana";
    m[4] = "world";
    m[13] = "orange";
    m[10] = "apple";
    for (auto& it : m) {
        cout << it.first << " " << it.second << endl;
        it.second += "+abc"; // 可以修改
    }
    auto up_iter = m.upper_bound(5);
    auto low_iter = m.lower_bound(5); // 指向 std::set 中第一个不小于（大于或等于）指定键的元素。
    --low_iter; // 此时指向小于5的最大值
    cout << up_iter->first << " : " << up_iter->second << endl;
    cout << low_iter->first << " : " << low_iter->second << endl;
    --low_iter;
    cout << "low_iter: " << low_iter->first << " : " << low_iter->second << endl;
    if (low_iter == m.begin()) {
        cout << "low_iter == m.begin()" << endl;
    }
    --low_iter;
    // if (low_iter < m.begin()) { 不能这么写
    // if (low_iter + 1 == m.begin()) { 也不能这么写
    if (low_iter == m.begin()) {
        cout << "2->low_iter == m.begin()" << endl;
    }
    low_iter = m.lower_bound(0);
    if (low_iter == m.begin()) {
        cout << "3->low_iter == m.begin() " << low_iter->first << " : " << low_iter->second << endl;
    }

    map<int,string> m1;
    if (m1.begin() == m1.end()) {
        cout << "m1 is empty" << endl;
    }
}

void test2(const string& str) {
    string tmp = std::move(str);
    cout << "str: " << str << endl;
    cout << "tmp: " << tmp << endl;
}

void test3() {
    string A = "Hello";
    string B = "world!";
    A.insert(1, B, 1, 3);  // 在A的开头插入B的前三个字符
    cout << A << endl;  // 输出: world!Hello
    cout << B << endl;
    A.insert(A.size() - 1, B, 1, 3);  // 在A的开头插入B的前三个字符
    cout << A << endl;  // 输出: Horlellorlo
}

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

    test1();
    string str = "hello";
    test2(str);
    test3();
    return 0;
}