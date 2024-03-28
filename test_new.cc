#include <iostream>
#include <new>
using namespace std;

struct chaff {
    char dross[20];
    int slag;
};

char buffer1[50];
char buffer2[500];

void test_new_failed() {
    const int a = 1000000000;
    long long *p = nullptr;
    while(true) {
        p = nullptr;
        // p = new long long[a]{0}; // throw std::bad_alloc
        p = new(std::nothrow)long long[a]{0}; // return nullptr if failed

        if (p == nullptr) {
            cout << "new failed." << endl;
            break;
        }
    }
}


void outOfMem1() {
    std::cerr << "2--Unable to satisfy request for memory\n";
    std::abort();
}

void outOfMem() {
    std::cerr << "1--Unable to satisfy request for memory\n";
    // 方案一，可以尝试释放一定内存，然后再次尝试分配内存

    // 方案二，设置新的处理函数，也许可以释放一定内存，然后再次尝试分配内存
    std::set_new_handler(outOfMem1);

    // 方案三，恢复默认处理函数，抛出bad_alloc异常
    // std::set_new_handler(nullptr);

    // 方案四，直接结束程序
    // std::abort(); // 不会返回，直接结束
}

// 运用到模板类中
template<typename T>
class NewHandlerSupport {
public:
    static std::new_handler set_new_handler(std::new_handler p) noexcept;
    static void* operator new(std::size_t size) noexcept(false);
private:
    static std::new_handler currentHandler;
};
template<typename T>
std::new_handler NewHandlerSupport<T>::set_new_handler(std::new_handler p) noexcept {
    std::new_handler oldHandler = currentHandler;
    currentHandler = p;
    return oldHandler; // static
}
template<typename T>
void* NewHandlerSupport<T>::operator new(std::size_t size) noexcept(false) {
    std::new_handler h(std::set_new_handler(currentHandler));
    return ::operator new(size);
}
template<typename T>
std::new_handler NewHandlerSupport<T>::currentHandler = nullptr;

class Widget: public NewHandlerSupport<Widget> {
public:
    Widget() {
        cout << "Widget()" << endl;
    }
    ~Widget() {
        cout << "~Widget()" << endl;
    }
    static void outOfMem() {
        std::cerr << "Unable to satisfy request for memory\n";
        std::abort();
    }
    static std::new_handler set_new_handler(std::new_handler p) noexcept {
        return NewHandlerSupport<Widget>::set_new_handler(p);
    }
    static void* operator new(std::size_t size) noexcept(false) {
        return NewHandlerSupport<Widget>::operator new(size);
    }
};
void test1() {
    cout << "[test1]==============================" << endl;
    // std::set_new_handler(outOfMem);
    // int *p = new int[1000000000000000000L]; // 抛出错误，不会返回nullptr
    // int *p = new(std::nothrow) int[1000000000000000000L];

    Widget* pwl = new Widget;
    Widget::set_new_handler(Widget::outOfMem);
    // Widget::operator new(1000000000000000000L); // 抛出错误，不会返回nullptr
}

void* operator new(std::size_t size) noexcept(false) { // 不要这么写吧
    cout << "operator new: size=" << size << endl;
    using namespace std;
    if (size == 0) {
        size = 1;
    }
    while(true) {
        void* p = malloc(size);
        if (p) {
            return p;
        }
        std::new_handler globalHandler = set_new_handler(nullptr);
        std::set_new_handler(globalHandler);
        if (globalHandler) {
            (*globalHandler)();
        } else {
            throw std::bad_alloc();
        }
    }
}

class Base{
public:
    Base() {
        cout << "Base()" << endl;
        // throw std::string("abc=======");// 抛出异常，会调用对应的operator placement delete
    }
    virtual ~Base() {
        cout << "virtual ~Base()" << endl;
    }
    static void* operator new(std::size_t size) noexcept(false) {
        cout << "base operator new: size=" << size << endl;
        using namespace std;
        if (size != sizeof(Base)) { // size不是Base的大小，调用全局operator new, new Base时size大概为8
            return ::operator new(size);
        }
        if (size == 0) {
            size = 1;
        }
        while(true) {
            void* p = malloc(size);
            if (p) {
                return p;
            }
            std::new_handler globalHandler = set_new_handler(nullptr);
            std::set_new_handler(globalHandler);
            if (globalHandler) {
                (*globalHandler)();
            } else {
                throw std::bad_alloc();
            }
        }
    }
    static void operator delete(void* pMemory, std::size_t size) noexcept {
        cout << "base operator delete: size=" << size << endl;
        if (pMemory == nullptr) {
            return;
        }
        if (size != sizeof(Base)) {
            cout << "---base operator delete: size=" << size << endl;
            ::operator delete(pMemory);
            return;
        }
        free(pMemory);
    }
    static void operator delete[](void* pMemory, std::size_t size) noexcept {
        cout << "base operator delete[]: size=" << size << endl;
        if (pMemory == nullptr) {
            return;
        }
        if (size != sizeof(Base)) {
            cout << "---base operator delete[]: size=" << size << endl;
            ::operator delete[](pMemory);
            return;
        }
        free(pMemory);
    }

    static void* operator new(std::size_t size, std::string str) noexcept {
        cout << "base operator new(std::size_t size, std::string str) : size=" << size << " str=" << str << endl;
        return ::operator new(size);
    }
    static void operator delete(void* pMemory, std::string str) noexcept {
        cout << "base operator delete(void* pMemory, std::string str) : pMemory=" << pMemory << " str=" << str << endl;
        return ::operator delete(pMemory);
    }

    static void* operator new(std::size_t size, std::ostream& os) noexcept {
        cout << "base operator new(std::size_t size, std::ostream& os) : size=" << size << " os=" << &os << endl;
        os << "error-----" << endl;
        return ::operator new(size);
    }
    static void operator delete(void* pMemory, std::ostream& os) noexcept {
        cout << "base operator delete(void* pMemory, std::ostream& os) : pMemory=" << pMemory << " os=" << &os << endl;
        return ::operator delete(pMemory);
    }

    /*
    除了size参数，还有其他参数的都叫做placement new，它们都是new的重载函数。
    只有再使用new时，new成功，但是对应构造函数抛出异常时，才会调用对应的placement delete。
    有placement new，就有placement delete，placement delete的参数和placement new的参数一致。

    类中的名称会隐藏外部的名称，如果你不定义的话

    placement并不分配内存，只是返回指向已经分配好的某段内存的一个指针。因此不能删除它，但需要调用对象的析构函数。
    如果你想在已经分配的内存中创建一个对象，使用new时行不通的。
    也就是说placement new允许你在一个已经分配好的内存中（栈或者堆中）构造一个新的对象。原型中void* p实际上就是指向一个已经分配好的内存缓冲区的的首地址。
    */
    static void* operator new(std::size_t size, void* pMemory) noexcept { // placement new
        cout << "base operator new(std::size_t size, void* pMemory) : size=" << size << " pMemory=" << pMemory << endl;
        if (pMemory == nullptr) {
            return ::operator new(size);
        }
        return pMemory;
    }
    static void operator delete(void* pMemory, void* p) noexcept {
        cout << "base operator delete(void* pMemory, void* p) pMemory=" << pMemory << " p=" << p << endl;
        // return ::operator delete(pMemory, p);
    }
};

class Derived: public Base {
public:
    // 子类的名称会隐藏父类的名称。如果你不定义的话
    static void* operator new(std::size_t size, std::string str) noexcept {
        cout << "derived operator new(std::size_t size, std::string str) : size=" << size << " str=" << str << endl;
        return ::operator new(size);
    }
    // static void operator delete(void* pMemory, std::string str) noexcept {
    //     cout << "derived operator delete(void* pMemory, std::string str) : pMemory=" << pMemory << " str=" << str << endl;
    //     return ::operator delete(pMemory);
    // }
};
void test2() {
    cout << "[test2]==============================" << endl;
    // new int[1000000000000000000L]; // 抛出错误，不会返回nullptr
    // Base* pb = new Base;// 先调用operator new，再调用构造函数
    // delete pb;

    // cout << "pb2===========================" << endl;
    // Base* pb2 = new Base[10];
    // delete[] pb2;

    cout << "pb3===========================" << endl;
    try {
        Base* pb3 = new("hello") Base; // 先调用operator new，再调用构造函数,需要在构造函数中抛出异常
        delete pb3;
    } catch (std::string e) {
        cout << "catch e: " << e << endl;
    }

    // cout << "pb4===========================" << endl;
    // cout << "&buffer1=" << static_cast<void*>(buffer1) << endl;
    // Base* pb4 = new(buffer1) Base; // placement news 在buffer1中分配空间，从首地址buffer1开始
    // // delete pb4; // 不能删除placement new的对象

    // cout << "pb5===========================" << endl;
    // Base* pb5 = new (std::cerr) Base; // 先调用operator new，再调用构造函数
    // delete pb5;

    cout << "pb6===========================" << endl;
    Derived* pb6 = new("hello") Derived; // 先调用operator new，再调用构造函数
    delete pb6;
    // Derived* pb6_1 = new Derived; // invalid Derived中没有定义operator new，调用Base中的operator new被placement new覆盖覆盖
}
int main() {
	// test_new_failed();
	chaff *p1, *p2;
    int *p3, *p4;
    p1 = new chaff;
    p3 = new int[20];

    // placement new 用法
    p2 = new(buffer1) chaff; // 在buffer1中分配空间，从首地址buffer1开始
    p4 = new(buffer2) int[20]{2}; // 在buffer2中分配空间，从首地址buffer2开始

    for(int i = 0; i < 500; i++) {
        cout << buffer2[i] << ",";
    }
    cout << endl;
    int *p5 = (int*)(&buffer2);
    for(int i = 0; i < 500; i++) {
        cout << p5[i] << ",";
    }
    cout << endl;
    for(int i = 0; i < 500; i++) {
        cout << p4[i] << ",";
    }
    cout << endl;
    cout << "&buffer2=" << (void*)buffer2 << " &p4=" << &p4 << endl;

    // delete p2; // invalid， 因为new此时不是常规运算符，是定位new运算符，除非构造一个解析函数 ~chaff()，并直接调用p2->~chaff()，否则不会调用析构函数

    test1();
    test2();
    return 0;
}