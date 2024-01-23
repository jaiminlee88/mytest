#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

void test1 () {
    cout << __func__ << " start..." << endl;
    // const 对模板 iterator的使用
    std::vector<int> vec1{1,2,3,4};
    copy(vec1.begin(), vec1.end(), ostream_iterator<int>(cout, ", ")); // a: 1, 2, 3, 4, )
    cout << endl;
    const std::vector<int>::iterator it = vec1.begin(); // const针对的是it本身，而不是it指向的对象, 类似 T* const it，而非 const T* it
    *it = 5;
    // ++it; // error: increment of read-only variable ‘it’
    copy(vec1.begin(), vec1.end(), ostream_iterator<int>(cout, ", ")); // a: 1, 2, 3, 4, )
    cout << endl;

    std::vector<int>::const_iterator c_it = vec1.begin(); // const_iterator针对的是it指向的对象，而不是it本身
    // *c_it = 5; // error: assignment of read-only location ‘c_it.std::vector<_Tp, _Alloc>::const_iterator::operator*<int, std::allocator<int> >()’
    ++c_it;
    copy(vec1.begin(), vec1.end(), ostream_iterator<int>(cout, ", ")); // a: 1, 2, 3, 4, )
    cout << endl;
    cout << __func__ << " end..." << endl;
}

class textbook {
public:
    textbook() {}
    textbook(std::string str) : text(str) {}
    ~textbook() {}

    void set(std::size_t pos, char c) {
        text[pos] = c;
    }
    const char& operator[](std::size_t pos) const {
        cout << "[textbook] ---const char& operator[](std::size_t pos) const--- " << endl;
        return text[pos];
    }
    char& operator[](std::size_t pos) {
        cout << "[textbook] ---char& operator[](std::size_t pos)--- " << endl;
        return text[pos];
    }
private:
    std::string text;
};

class testbook_1{ // 反调用const版本的operator[]，使得代码维护更简单
public:
    testbook_1() {}
    testbook_1(std::string str) : text(str) {}
    ~testbook_1() {}

    void set(std::size_t pos, char c) {
        text[pos] = c;
    }
    const char& operator[](std::size_t pos) const {
        cout << "[testbook_1] ---const char& operator[](std::size_t pos) const--- " << endl;
        return text[pos];
    }
    char& operator[](std::size_t pos) { // 减少代码维护
        cout << "[testbook_1] ---char& operator[](std::size_t pos)--- " << endl;
        return const_cast<char&>(
                    static_cast<const testbook_1&>(*this)[pos] // 先转static_cast<const char&>(*this)， 调用const版本的operator[]
                );
    }
private:
    std::string text;
};
void print(const textbook& t) {
    cout << "---void print(const textbook& t)--- " << endl;
    cout << "t[0]=" << t[0] << endl;
}

void test2() {
    cout << __func__ << " start..." << endl;

    cout << "t0--------------------------------" << endl;
    textbook t("hello");
    cout << "t[0]=" << t[0] << endl;
    t.set(0, 'X');
    cout << "t[0]=" << t[0] << endl;
    const textbook ct("hello");
    cout << "t[0]=" << ct[0] << endl;
    // char* pc = &ct[0]; // error: invalid conversion from ‘const char*’ to ‘char*’ [-fpermissive]

    cout << "t1-----------------------------------" << endl;
    testbook_1 t1("hello"); 
    cout << "t1[0]=" << t1[0] << endl;
    t1.set(0, 'X');
    cout << "t1[0]=" << t1[0] << endl;
    const testbook_1 ct1("hello");
    cout << "t1[0]=" << ct1[0] << endl;


    print(t);
    print(ct);
    cout << __func__ << " end..." << endl;
}

int main() {
    test1();
    test2();
    return 0;
}