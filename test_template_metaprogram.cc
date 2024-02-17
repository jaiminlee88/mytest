#include<iostream>
#include <vector>
#include <set>

using namespace std;

template<typename IterT, typename DistT>
void my_advance(IterT& iter, DistT d) {
    std::cout << "advance" << std::endl;
    // 判断通过结构体tag判断迭代器类型
    if (typeid(typename std::iterator_traits<IterT>::iterator_category) 
      == typeid(std::random_access_iterator_tag)) { // if运行期间才知道，iterator_category编译期间知道，不合理，代码膨胀
        std::cout << "random_access_iterator_tag" << std::endl;
        iter += d;
    } else {
        std::cout << "not random_access_iterator_tag" << std::endl;
        if (d >= 0) {
            while (d--) ++iter;
        } else {
            while (d++) --iter;
        }
    }
}
// 更好迭代器选择实现,overload，将判断放在编译期间，参数可以提前确定改用哪个模板
// 思路很棒，典型traits class用法，根据参数生成相应函数模板，在编译期间可用
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::random_access_iterator_tag) {
    std::cout << "doAdvance(IterT& iter, DistT d, std::random_access_iterator_tag)" << std::endl;
    iter += d;
}
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::bidirectional_iterator_tag) {
    std::cout << "doAdvance(IterT& iter, DistT d, std::bidirectional_iterator_tag)" << std::endl;
    if (d >= 0) {
        while (d--) ++iter;
    } else {
        while (d++) --iter;
    }
}
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::input_iterator_tag) {
    cout << "doAdvance(IterT& iter, DistT d, std::input_iterator_tag)" << endl;
    if (d < 0) {
        throw std::out_of_range("negative distance");
    }
    while(d--) ++iter;
}
template<typename IterT, typename DistT>
void my_advance_1(IterT& iter, DistT d) {
    doAdvance(iter,
               d,
               typename std::iterator_traits<IterT>::iterator_category() // 决定了调用哪个版本，效率比typeid高
            );
}

void test1 () {
    // 五种迭代器
    // 1、输入迭代器：只读，只能单步向前迭代，只能使用一次 istream_iterator
    // 2、输出迭代器：只写，只能单步向前迭代，只能使用一次 ostream_iterator
    // 3、前向迭代器：可读写，只能单步向前迭代，可多次使用 slist
    // 4、双向迭代器：可读写，可双向迭代，可多次使用 set multiset map multimap
    // 5、随机访问迭代器：可读写，可双向迭代，可多次使用，支持算术运算， vector,string deque
    // 对应专属tag struct
    // 1、struct input_iterator_tag{}
    // 2、struct output_iterator_tag{}
    // 3、struct forward_iterator_tag : public input_iterator_tag{}
    // 4、struct bidirectional_iterator_tag : public forward_iterator_tag{}
    // 5、struct random_access_iterator_tag : public bidirectional_iterator_tag{}
    // 本质上，在tempalte内部设置专属tag struct，然后根据tag struct的类型，选择合适的迭代器

    // 有些迭代器可以 iter+=1，有些只能iter++或者iter--
    cout << "[test1]================start===============" << endl;
    // 方法1：typeid
    std::vector<int> v{1,2,3,4,5};
    std::vector<int>::iterator it = v.begin();
    my_advance(it, 2);
    std::cout << "after my_advance(it, 2), *it="<< *it << std::endl;

    // 方法2：traits class,效率更高，编译期间就知道调用哪个函数
    std::vector<int>::iterator it1 = v.begin();
    my_advance_1(it1, 2);
    std::cout << "after my_advance_1(it1, 2), *it1="<< *it1 << std::endl;

    std::set<int> m{1,2,3,4,5,6};
    std::set<int>::iterator it2 = m.begin();
    my_advance_1(it2, 2);
    std::cout << "after my_advance(it2, 2), *it2="<< *it2 << std::endl;
}


template<unsigned n>
struct Factorial {
    enum {
        value = n * Factorial<n-1>::value // 递归，编译期间就知道结果
    };
};
template<>
struct Factorial<0> { // 特化
    enum {
        value = 1
    };
};


template<int n, int m>
struct addInt {
    static const int value = n + m; // 编译期间就知道结果
};

// template<double n, double m>
// struct addDouble {
//     static const double value; // 编译期间就知道结果
// };

// template <double n, double m>
// const addDouble<n,m>::value = n + m;

template<typename T>
void show(const T& t) {
    std::cout << t << std::endl;
}

void test2() {
    cout << "[test2]================start===============" << endl;
    cout << "Factorial<5>::value=" << Factorial<5>::value << endl; // 编译期间就知道结果
    cout << "Factorial<10>::value=" << Factorial<10>::value << endl; // 编译期间就知道结果
    cout << "addInt<10, 20>::value=" << addInt<10, 20>::value << endl; // 编译期间就知道结果

    // cout << "addDouble<10.1, 20.2>::value=" << addDouble<10.1, 20.2>::value << endl; // 编译期间就知道结果

    show<char>('A');
    show<int>(10);
    show<double>(10.1);
    show<string>("hello");
}
int main() {
    test1(); // typeid and traits class
    test2(); // metaprogramming, 编译期间完成计算

}