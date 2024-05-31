#include <iostream>
#include <deque>
#include <memory>

using namespace std;

template <typename T>
class quick_queue {
public:
    /*
    - 这个数据结构在构造的时候需要传入参数num_bins用来表示队列中元素的最大数量，如果使用过程中元素数量超出这个值，则为未定义行为(UB)
    - 提供.push_back(item)方法，向队列尾部插入元素
    - 提供.pop_front()方法，从队列头部弹出元素
    - 提供.get(idx)方法，获取从头部开始的第idx个元素
    - 以上三个方法的时间复杂度均为O(1)  hash_map
    - 注重代码简洁、规范、高效、可维护性
    - 不需要考虑多线程读写
    如果你用C++实现，可以考虑以下加分问题：
    - 作为通用容器，应该实现为一个模板类，容器元素的类型为模板参数
    - 如果元素类型是自定义类，可能没有默认构造函数，可能拷贝很重，怎么兼容且保证效率
    - 你的queue如何处理拷贝、移动等操作
    */
    quick_queue(unsigned int num_bins) : _size(num_bins) {}

    inline unsigned int size() { return _q.size(); }
    inline bool         empty() { return _q.empty(); }

    void push_back(T &item) {
        if (_q.size() >= _size) {
            return;
        }
        _q.push_back(std::move(item));
    }

    void push_back(T &&item) {
        if (_q.size() >= _size) {
            return;
        }
        _q.push_back(std::move(item));
    }

    void push_back(std::shared_ptr<T> p) {
        cout << "push_back(std::shared_ptr<T> p)" << endl;
        if (_q.size() >= _size) {
            return;
        }
        _q.push_back(std::move(*p));
    }

    T pop_front() {
        if (_q.size() < 1) {
            return {};
        }
        T ret = _q.front();
        _q.pop_front();
        return ret;
    }

    T get(unsigned int idx) {
        if (idx >= _q.size() || idx < 0) {
            return {};
        }
        return _q[idx];
    }

private:
    unsigned int _size;
    std::deque<T> _q;
};

void test1() {
    cout << "test1---------------" << endl;
    quick_queue<int> q(3);
    q.push_back(1);
    q.push_back(2);
    int a1 = 3;
    q.push_back(a1);
    cout << "a1=" << a1 << endl;
    q.push_back(4);
    cout << "q.get(0)=" << q.get(0) << endl;
    cout << "q.get(1)=" << q.get(1) << endl;
    cout << "q.get(2)=" << q.get(2) << endl;
    cout << "q.get(3)=" << q.get(3) << endl;
    cout << "q.pop_front()=" << q.pop_front() << endl;
    cout << "q.pop_front()=" << q.pop_front() << endl;
    cout << "q.pop_front()=" << q.pop_front() << endl;
    cout << "q.pop_front()=" << q.pop_front() << endl;
    cout << "q.pop_front()=" << q.pop_front() << endl;
}

void test2() {
    cout << "test2---------------" << endl;
    quick_queue<string> q2(3);
    q2.push_back("1");
    string str1 = "abc";
    q2.push_back(str1);
    string str2 = "def";
    q2.push_back(std::move(str2));
    cout << "str1=" << str1 << endl;
    cout << "str2=" << str2 << endl;
    cout << "q2.get(0)=" << q2.get(0) << endl;
    cout << "q2.get(1)=" << q2.get(1) << endl;
    cout << "q2.get(2)=" << q2.get(2) << endl;
    cout << "q2.get(3)=" << q2.get(3) << endl;
}

void test3() {
    cout << "test3---------------" << endl;
    class diy_1 {
    public:
        int a{0};
        int b{0};
    };
    struct diy_2 {
        int c{0};
        int d{0};
    };
    diy_1 d1;
    d1.a = 1;
    d1.b = 2;
    diy_2 d2;
    d2.c = 3;
    d2.d = 4;
    quick_queue<diy_1> q3(3);
    q3.push_back(d1);
    q3.push_back(std::move(d1));
    q3.push_back(std::make_shared<diy_1>(d1));
    cout << "d1.a=" << d1.a << endl;
    cout << "d1.b=" << d1.b << endl;
    cout << "q3.get(0).a=" << q3.get(0).a << endl;
    cout << "q3.get(0).b=" << q3.get(0).b << endl;
    cout << "q3.get(1).a=" << q3.get(1).a << endl;
    cout << "q3.get(1).b=" << q3.get(1).b << endl;
    cout << "q3.get(2).a=" << q3.get(2).a << endl;
    cout << "q3.get(2).b=" << q3.get(2).b << endl;
    cout << "q3.get(4).a=" << q3.get(4).a << endl;
    cout << "q3.get(4).b=" << q3.get(4).b << endl;
}

void test4() {
    cout << "test4---------------" << endl;
    quick_queue<shared_ptr<int>> q4(3);
    shared_ptr<int> p1 = make_shared<int>(1);
    shared_ptr<int> p2 = make_shared<int>(2);
    shared_ptr<int> p3 = make_shared<int>(3);
    q4.push_back(p1);
    q4.push_back(p2);
    q4.push_back(p3);
    cout << "p1 is " << (p1 == nullptr ? "nullptr" : "not nullptr") << endl;
    cout << "p2 is " << (p2 == nullptr ? "nullptr" : "not nullptr") << endl;
    cout << "p3 is " << (p3 == nullptr ? "nullptr" : "not nullptr") << endl;
    cout << "q4.get(0)=" << *q4.get(0) << endl;
    cout << "q4.get(1)=" << *q4.get(1) << endl;
    cout << "q4.get(2)=" << *q4.get(2) << endl;
    cout << "q4.get(3)=" << (q4.get(3) == nullptr ? "nullptr" : to_string(*q4.get(3))) << endl; // invalid read
}

int main() {
    test1();
    test2();
    test3();
    test4();
    return 0;
}