#include<iostream>
#include <vector>
#include <set>

using namespace std;

template <class Type>
class Stack {
private:
    enum {MAX=10};// constant specific to class
    Type items[MAX];//holds stack items
    int top;// index for top stack item
public:
    Stack();
    bool isempty();
    bool isfull();
    bool push(const Type & item); // add item to stack
    bool pop(Type & item);// pop top into item
};

template <class Type>
Stack<Type>::Stack() {
    std::cout << "stack init, Type=" << typeid(Type).name() << std::endl;
    top=0;
}

template <class Type>
bool Stack<Type>::isempty() {
    return top == 0;
}

template <class Type>
bool Stack<Type>::isfull() {
    return top == MAX;
}

template<class Type>
bool Stack<Type>::push(const Type & item) {
    if (top < MAX) {
        items[top++] = item;
        return true;
    } else {
        return false;
    }
}

template <class Type>
bool Stack<Type>::pop(Type &item) {
    if (top > 0) {
        item = items[--top];
        return true;
    } else {
        return false;
    }
}


template <class T, int n>
class ArrayTP {
private:
    T ar[n];
public:
    ArrayTP() { std::cout << "this is ArrayTP" << std::endl; }
    explicit ArrayTP(const T & v);
    virtual T & operator[](int i);
    virtual T operator[](int i) const;
};

template <class T,int n>
ArrayTP<T,n>::ArrayTP(const T & v) {
    for (int i=0;i<n;i++)
        ar[i] = v;
}

template <class T,int n>
T & ArrayTP<T,n>::operator[](int i) {
    if(i<0 || i>=n) {
        std::cerr <<"Error in array limits: u << i<<uis out of range n";
        std::exit(EXIT_FAILURE);
    }
    return ar[i];
}

template <class T,int n>
T ArrayTP<T,n>::operator[](int i) const {
    if (i<0 || i>=n) {
        std::cerr << "Error in array limits:" << i<< "is out of range n";
        std::exit(EXIT_FAILURE);
    }
    return ar[i];
}

// template <typename n> class ArrayTP<std::string, typename n> {
// private:
//     T ar[n];
// public:
//     ArrayTP() { std::cout << "this is string ArrayTP" << std::endl; }
//     explicit ArrayTP(const T & v) {};
//     virtual T & operator[](int i) {};
//     virtual T operator[](int i) const {};
// };


template <typename T>
class beta {
public:
    beta(T t,int i) :q(t),n(i) {}

    template<typename U>// template method
    U blab(U u,T t) { return (n.Value() + q.Value()) * u /t;}
    void Show() const {
        q.show();
        n.show();
    }

private:
    template <typename V> // nested template class member
    class hold{
    private:
        V val;
    public:
        hold(V v =0) :val(v) {}
        void show() const { cout << val << endl;}
        V Value() const { return val; }
    };

    hold<T> q;// template object
    hold<int> n;// template object
};

template <template <typename T> class Thing> // template <[template <typename T> class] Thing>
class Crab {
private:
    Thing<int> sl;
    Thing<double> s2;
public:
    Crab(){};
    // assumes the thing class has push() and pop() members
    bool push(int a,double x) { return sl.push(a) && s2.push(x);}
    bool pop(int &a,double & x) { return sl.pop(a) && s2.pop(x);}
};


// template prototypes
template <typename T> void counts();
template <typename T> void report(T &);
// template class
template <typename TT>
class HasFriendT {
private:
    TT item;
    static int ct;
public:
    HasFriendT(const TT & i) : item(i) { ct++; }
    ~HasFriendT() { ct--; }
    friend void counts<TT>();
    friend void report<>(HasFriendT<TT> &);
};

template <typename T>
int HasFriendT<T>::ct = 0;

// template friend functions definitions
template <typename T>
void counts() {
    cout << "template size: " << sizeof(HasFriendT<T>) <<";";
    cout << "template counts(): " << HasFriendT<T>::ct << endl;
}
template <typename T>
void report(T & hf) {
    cout << hf.item << endl;
}

template <typename T>
class ManyFriend {
private:
    T item;
public:
    ManyFriend() { cout << "ManyFriend initialize..." << endl; }
    ManyFriend(const T & i) : item(i) {}
    template <typename C,typename D> friend void show2(C &,D &);
};

template <typename C,typename D>
void show2(C & c,D &d) {
    cout << c.item <<", "<< d.item << endl;
}

template<typename T>
using MFtype = ManyFriend<T>;

class companyA {
public:
    void sendClearText(const std::string & msg) {
        std::cout << "companyA sendClearText: " << msg << std::endl;
    }
    void sendEncrypted(const std::string & msg) {
        std::cout << "companyA sendEncrypted: " << msg << std::endl;
    }
};
class companyB {
public:
    void sendClearText(const std::string & msg) {
        std::cout << "companyB sendClearText: " << msg << std::endl;
    }
    void sendEncrypted(const std::string & msg) {
        std::cout << "companyB sendEncrypted: " << msg << std::endl;
    }
};
class companyZ {
public:
    void sendEncrypted(const std::string & msg) {
        std::cout << "companyZ sendEncrypted: " << msg << std::endl;
    }
};
template<typename T>
class MsgSender {
public:
    void sendClear(const std::string & msg) {
        T t;
        t.sendClearText(msg);
    }
};

template<>
class MsgSender<companyZ> {
public:
    // 特例化，但是没有 sendClear这个通用模板的函数，所以会报错
    void sendSecret(const std::string & msg) {
        companyZ z;
        z.sendEncrypted(msg);
    }
};

template<typename T>
class loggingMsgSender : public MsgSender<T> {
public:
    // 不能直接使用sendClear
    // 方案一，
    void sendClearMsg(const std::string & msg) {
        std::cout << "loggingMsgSender sendClearMsg: " << msg << std::endl;
        // sendClear(msg); // invalid 因为此时并不知道 MsgSender<T> 的具体类型，无法实例化，所以要用 this->sendClear(msg)
        this->sendClear(msg); // valid, 假设sendClear可以被继承
    }
    // 方案二，using MsgSender<T>::sendClear; // valid
    using MsgSender<T>::sendClear; // 高速编译器，假设sendClear在base class内，编译器不进入base内寻找，而是直接通过using告诉它
    void sendClearMsg2(const std::string & msg) {
        std::cout << "loggingMsgSender sendClearMsg2: " << msg << std::endl;
        sendClear(msg); // valid
    }
    // 方案三，using MsgSender<T>::sendClear; // valid
    void sendClearMsg3(const std::string & msg) {
        std::cout << "loggingMsgSender sendClearMsg3: " << msg << std::endl;
        MsgSender<T>::sendClear(msg); // valid假设sendClear将被继承下来
    }
};


void test1() {
    cout << "[test1]================start===============" << endl;
    loggingMsgSender<companyA> lms;
    lms.sendClearMsg("hello");
    lms.sendClearMsg2("hello");
    lms.sendClearMsg3("hello");

    // loggingMsgSender<companyZ> lms2; // 报错
}


template<typename T, std::size_t n> // 代码膨胀
class SquareMatrix {
public:
    void invert() {
        std::cout << "SquareMatrix invert" << std::endl;
    }
};

template<typename T> // 只有一个base了
class SquareMatrixBase {
protected:
    void invert(std::size_t matrixSize) {
        std::cout << "SquareMatrixBase invert matrixSize=" << matrixSize << std::endl;
    }
};

template<typename T, std::size_t n>
class SquareMatrix_1 : private SquareMatrixBase<T> {
private:
    using SquareMatrixBase<T>::invert;
public:
    void invert() {
        this->invert(n);
    }
};
void test2() {
    cout << "[test2]================start===============" << endl;
    SquareMatrix<double, 5> sm; // 生成两个模板，代码膨胀
    sm.invert();
    SquareMatrix<double, 10> sm1;
    sm1.invert();

    SquareMatrix_1<double, 15> sm2; // 生成一个模板，代码不膨胀
    sm2.invert();
    SquareMatrix_1<double, 20> sm3;
    sm3.invert();
}


template <typename T>
class SmartPtr {
public:
    SmartPtr(SmartPtr & other) : heldPtr(other.get()) { // copy构造函数
        std::cout << "SmartPtr copy constructor" << std::endl;
    }
    template<typename U>
    SmartPtr(const SmartPtr<U> & other) : heldPtr(other.get()) { // 泛化copy构造函数
        std::cout << "SmartPtr general copy constructor" << std::endl;
    }
    T* get() const { return heldPtr; }
private:
    T * heldPtr;
};

template<typename T>
class Rational {
public:
    Rational(const T & numerator = 0, const T & denominator = 1) : n(numerator), d(denominator) {
        std::cout << "Rational constructor" << std::endl;
    }
    template<typename U>
    Rational(const Rational<U> & other) : n(other.n), d(other.d) {
        std::cout << "Rational general copy constructor" << std::endl;
    }
    friend const Rational operator*(const Rational & lhs, const Rational & rhs){
        std::cout << "const Rational<T> operator*" << std::endl;
        return Rational<T>(lhs.n * rhs.n, lhs.d * rhs.d);
    }
    // 1、operator*的参数const Rational可以不写 const Rational<T>，简略表达，可加可不加
    // 2、friend const Rational operator* 混合式调用
    //    不加friend，oneHalf * 2的参数2不会被隐式转换为Rational<int>，
    //    friend声明方式可以指涉某个特定函数，而不是通过template推导出来，在声明Rational<int> oneHalf(1,2)时，operator*函数被声明出来
    //    Rationa<int>时，operator*顺带被自动声明出来，然后调用非模板函数
    // 3、operator*函数的实现必须在模板内部，否则链接不通过
    //    目前只是声明，没有定义，链接不通过，
    //    template要求声明，就一定要定义，写在外边链接无法通过
private:
    T n, d;
};
// template<typename T> // 写在外部无效
// const Rational<T> operator*(const Rational<T> & lhs, const Rational<T> & rhs) {
//     std::cout << "const Rational<T> operator*" << std::endl;
//     return Rational<T>(lhs.n * rhs.n, lhs.d * rhs.d);
// }

void test3(){
    cout << "[test3]================start===============" << endl;
    // template不做隐式类型转换
    Rational<int> oneHalf(1,2);
    Rational<int> result = oneHalf * 2; // template本身不能隐式转换，通过声明为friend函数，告诉template不要用推导，可以实现隐式转换
}

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

void test4 () {
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
    cout << "[test4]================start===============" << endl;
    std::vector<int> v{1,2,3,4,5};
    std::vector<int>::iterator it = v.begin();
    my_advance(it, 2);
    std::cout << "after my_advance(it, 2), *it="<< *it << std::endl;

    std::vector<int>::iterator it1 = v.begin();
    my_advance_1(it1, 2);
    std::cout << "after my_advance_1(it1, 2), *it1="<< *it1 << std::endl;

    std::set<int> m{1,2,3,4,5,6};
    std::set<int>::iterator it2 = m.begin();
    my_advance_1(it2, 2);
    std::cout << "after my_advance(it2, 2), *it2="<< *it2 << std::endl;
}
// 在C++中，模板是一种用于生成通用代码的机制，可以根据模板参数的不同来生成特定类型或值的代码。模板的实例化（Instantiation）和具体化（Specialization）是模板的两个重要概念。

// 实例化（Instantiation）：模板实例化是指将模板用具体的类型或值来替换模板参数，从而生成针对特定类型或值的代码。当程序中使用模板时，编译器会在需要的时候进行模板实例化。实例化是根据模板定义和使用的上下文，在编译时自动生成特定的代码。例如，当我们使用 std::vector<int> 实例化模板 std::vector<T> 时，编译器会根据模板定义，在编译时生成一个针对 int 类型的 vector 类。实例化是编译时的过程，和编译期上下文相关。

// 具体化（Specialization）：模板具体化是指为模板的特定参数提供显式的定义。当模板无法满足特定类型或值的需求时，我们可以通过显式提供模板具体化来定义特定类型或值的特殊行为。可以用具体化来对模板进行特殊处理，以覆盖默认的模板定义。具体化通常发生在模板实例化无法满足要求的情况下。例如，当我们需要为某个特定类型提供一个特殊的实现时，我们可以为该类型提供一个具体化版本。具体化是在编译期对模板定义进行的附加定义。

// 总结起来，模板的实例化是根据模板定义和使用的上下文，在编译期自动生成特定类型或值的代码。而模板的具体化是为特定的模板参数提供显式的定义，以覆盖默认的模板行为。实例化是编译时的过程，而具体化是在编译期对模板定义的附加定义。
int main() {

    Stack<double> *kernel_double; // 不会实例化，因为没有用它
    Stack<int> kernel;
    Stack<std::string> colonels;
    const char *st = "abc";
    std::cout << "st=" << st << " *st=" << *st << std::endl;

    ArrayTP<int, 12> arr0;

    beta<double> guy(3.5,3);
    cout << "T was set to doublel\n";
    guy.Show();
    cout << "V was set to T, which is double, then V was set to int\n";
    cout << guy.blab(10, 2.3) << endl;
    cout << "U was set to int\n";
    cout << guy.blab(10.0,2.3) << endl;
    cout << "U was set to doublel\n";
    cout << "Donel\n";

    Crab<Stack> nebula; //Stack must match template <typename T> class thing
    int ni;
    double nb;
    cout << "Enter int double pairs, such as 4 3.5 (0 0 to end): \n";
    // cin >> ni >> nb;
    ni = 4;
    nb = 3.5;
    while (ni > 0 && nb > 0) {
        if (!nebula.push(ni,nb))
            break;
    }
    while (nebula.pop(ni,nb)) {
        cout << "ni=" << ni << " nb=" << nb << endl;
    }
    cout <<"Done.\n";

    counts<int>();
    HasFriendT<int> hfil(10);
    HasFriendT<int> hfi2(20);
    HasFriendT<double> hfdb(10.5);
    report(hfil);// generate report (HasFriendT<int> &)
    report(hfi2);// generate report(HasFriendT<int> &)// generate report (HasFriendT<double> &)
    report(hfdb);
    cout <<"counts<int>() output: \n";
    counts<int>();
    cout << "counts<double>() output: \n";
    counts<double>();

    cout << endl;
    ManyFriend<int> hfil_1(10);
    ManyFriend<int> hfi2_1(20);
    ManyFriend<double> hfdb_1(10.5);
    cout <<"hfil_1,hfi2_1:";
    show2(hfil_1,hfi2_1);
    cout <<"hfdb_1,hfi2_1:";
    show2(hfdb_1,hfi2_1);

    cout << endl;
    MFtype<int> MFtype_int;
    MFtype<double> MFtype_double;

    test1();
    test2();
    test3(); // 隐式转换
    // 五种迭代器
    // 1、输入迭代器：只读，只能单步向前迭代，只能使用一次 istream_iterator
    // 2、输出迭代器：只写，只能单步向前迭代，只能使用一次 ostream_iterator
    // 3、前向迭代器：可读写，只能单步向前迭代，可多次使用 slist
    // 4、双向迭代器：可读写，可双向迭代，可多次使用 set multiset map multimap
    // 5、随机访问迭代器：可读写，可双向迭代，可多次使用，支持算术运算， vector,string deque
    test4(); // 根据traits class表现类型信息选择迭代器
    return 0;
}