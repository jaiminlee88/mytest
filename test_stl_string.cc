#include <iostream>
#include <string>
#include <queue>

using namespace std;


void mystrcpy(char* dst, const char* src) {
    if (dst == nullptr || src == nullptr) {
        return;
    }
    while(*src != '\0') {
        *dst = *src;
        ++dst;
        ++src;
    }
}
void test1() {
    char* dst = new char[1024];
    const char* src = "this is a static string";
    mystrcpy(dst, src);
    cout << "dst: " << dst << endl;
    delete[] dst;
}

void test2() {
#if __cplusplus >= 201703L
    //std::string_view
    // #直接设置字符串的长度0x18，也就是24Bytes，还有字符串的起始指针$0x401e30，没有堆内存分配
    // 对象析构：std::string_view分配在栈上，无需析构
    // std::string_view高效的地方在于，它不管理内存，只保存指针和长度，所以对于只读字符串而言，查找和拷贝是相当简单的
    // C++17中我们可以使用std::string_view来获取一个字符串的视图，字符串视图并不真正的创建或者拷贝字符串，
    // 而只是拥有一个字符串的查看功能。std::string_view比std::string的性能要高很多，因为每个std::string都独自拥有一份字符串的拷贝，
    // 而std::string_view只是记录了自己对应的字符串的指针和偏移位置。当我们在只是查看字符串的函数中可以直接使用std::string_view来代替
    std::string_view sv = "this is a static string";
    cout << "sv.size(): " << sv.size() << endl;
    cout << "sv.data(): " << sv.data() << endl;
    cout << "sv: " << sv << endl;

    std::queue<char> q;  // 存储值
    cout << "origin str: " << str << endl;
    for (const auto &c: str) {
        q.push(c);
    }
    cout << "q.size() before: " << q.size() << endl;
    auto substr = std::string_view(&q.front(), 7);
    cout << "q.size() after: " << q.size() << endl;
    cout << "substr: " << substr << endl;
    cout << "sizeof(substr): " << sizeof(substr) << endl;

    string str2 = "static s.";
    cout << "str2.size(): " << str2.size() << endl;
    int pos = 0;
    for (auto& it:str2) {
        cout << pos << ": " << it << endl;
        ++pos;
    }

    char tmp = 'a';
    std::string_view str3(&tmp);
    cout << "str3: " << str3.substr( 0, 1) << " str3.size()=" << str3.size() << endl;
#endif
}

void test3() {
    class str4 {
    private:
        uint64_t capacity_;
        char* buffer_;
    public:
        str4(uint64_t capacity) : capacity_(capacity), buffer_(nullptr) {
            if (!buffer_) {
                buffer_ = new (nothrow) char[capacity_];
                if (!buffer_) {
                    cout << "str4 init succ..." << endl;
                    throw bad_alloc();
                }
                cout << "buffer_=" << (void*)buffer_ << endl;
            }
        }
        ~str4() {
            if (buffer_) {
                // cout << "delete buffer_" << endl;
                delete[] buffer_;
            }
            buffer_ = nullptr;
            capacity_ = 0;
        }
    };
    str4 s4(1024);

    string str5 = "static";
    cout << "str5[1] =" << str5[1] << endl;
}
void test4 () {
    // insert and erase
    vector<int> nums = {1, 2, 0};
    vector<int> subset{4,5,6,7,8,9};
    nums.insert(nums.end(), subset.begin(), subset.end());
    cout << "before     : ";
    for (auto& it : nums) {
        cout << it << " ";
    }
    cout << endl;
    nums.erase(nums.begin() + 3, nums.begin() + 4);
    cout << "after erase: ";
    for (auto& it : nums) {
        cout << it << " ";
    }
    cout << endl;

    nums.insert(nums.begin(), 18);
    cout << "after insert: ";
    for (auto& it : nums) {
        cout << it << " ";
    }
    cout << endl;

    nums.insert(nums.begin() + nums.size(), 19); // 在末尾插入
    cout << "after insert: ";
    for (auto& it : nums) {
        cout << it << " ";
    }
    cout << endl;
}

void test5() {
    char a = 'a';
    char b = 'b';
    char c = 'c';
    string s;
    s.append(1, a);
    s.append(1, b);
    cout << "s: " << s << endl;
    string s1("111");
    s1 += s;
    cout << "s1: " << s1 << endl;
    cout << "s1.substr(0,3): " << s1.substr(0,3) << endl;
}
int main() {
    // g++ -O0 -o static_str static_str.cc -std=c++17 -g && objdump -S -t -D static_str > static_str.s

    // 指针指向静态字符串,##直接设置字符串指针
    const char* str_ptr = "this is a static string";
    cout << "str_ptr: " << str_ptr << endl;

    //字符串数组
    // 这里使用一个很取巧的办法，不使用循环，而是使用多个mov语句把字符串设置到堆栈
    // 对象析构：字符串数组分配在栈上，无需析构
    char str_array[] = "this is a static string";
    cout << "str_array: " << str_array << endl;

    //std::string
    // esi保存了字符串开始地址$0x401e30,调用std::string的构造函数
    // 对象析构：调用析构函数
    std::string str = "this is a static string";
    cout << "str: " << str << endl;

    test1(); // strcpy
    test2();
    test3();
    test4();
    test5();
}