#include<iostream>
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
    return 0;
}