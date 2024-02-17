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

    //std::string_view
    // #直接设置字符串的长度0x18，也就是24Bytes，还有字符串的起始指针$0x401e30，没有堆内存分配
    // 对象析构：std::string_view分配在栈上，无需析构
    // std::string_view高效的地方在于，它不管理内存，只保存指针和长度，所以对于只读字符串而言，查找和拷贝是相当简单的
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

    test1(); // strcpy
}