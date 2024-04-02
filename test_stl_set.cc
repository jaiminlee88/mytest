#include<iostream>
#include<set>
#include<algorithm>
#include<iterator>
#include <vector>
using namespace std;


void test1() {
    cout << "test1============" << endl;
    struct student {
        int id;
        int rank;
        int* p;
        bool operator<(const student& s) const {
            return id < s.id;
            // return (*p) < (*s.p); // 不要改，会报错
        }

        // find用
        bool operator() (const student &a, const student &b) const {
            /*先比较名字；若名字相同，则比较年龄。小的返回true*/
            if(a.id == b.id) {
                return true;
            }
            return false;
        }
    };
    set<student> s;
    vector<int> v = {2, 2, 1, 4, 5, 2, 3};
    for(auto& i : v) {
        student y = {i,0};
        y.p = new int(i);
        s.insert(y);
    }

    auto iter = s.find(student{2});
    if (iter != s.end()) {
        cout << "found id=" << iter->id << " rank=" << iter->rank << " iter->p: " << *iter->p << endl;
        // iter->rank = 100; // 不能直接修改 readonly 注意注意注意，除非里面放了一个指针，指针指向的内容可以修改
        *(iter->p) = 200;
    } else {
        cout << "not found" << endl;
    }

    cout << "set s: ";
    while (s.empty() == false) {
        cout << "[" << s.begin()->id << "," << s.begin()->rank << "," << *s.begin()->p << "] ";
        s.erase(s.begin());
    }
    cout << endl;
}

void test2(){
    struct node {
        int id;
        string str;
        // bool operator<(node& a, node* b) {
        //     return a.id < b.id;
        // }
        bool operator<(const node& a) const {
            return id < a.id;
        }
    };
    set<node> s;
    s.insert({1, "hello"});
    s.insert({4, "world"});
    s.insert({7, "apple"});
    s.insert({10, "banana"});
    for (auto& it : s) {
        cout << it.id << " " << it.str << endl;
        // it.str = "new"; // 都是const的，不能修改，除非是地址，或者入队之前修改
    }
    auto up_iter = s.upper_bound({5, ""});
    auto low_iter = s.lower_bound({5, ""}); // 指向 std::set 中第一个不小于（大于或等于）指定键的元素。
    --low_iter; // 此时指向小于5的最大值
    // 注意需要判断是否存在，否则会报错
    cout << up_iter->id << ":" << up_iter->str << endl;
    cout << low_iter->id << ":" << low_iter->str << endl;
}
int main(){
    /*
    begin()     　　 ,返回set容器的第一个元素
    end() 　　　　 ,返回set容器的最后一个元素
    clear()   　　     ,删除set容器中的所有的元素
    empty() 　　　,判断set容器是否为空
    max_size() 　 ,返回set容器可能包含的元素最大个数
    size() 　　　　 ,返回当前set容器中的元素个数
    rbegin　　　　 ,返回的值和end()相同
    rend()　　　　 ,返回的值和rbegin()相同
    */
    set<string> a;
    a.insert("hello");
    a.insert("world");
    a.insert("apple");
    a.insert("banana");
    cout << "set a: ";
    copy(a.begin(), a.end(), ostream_iterator<string>(cout, ", ")); // apple,banana,hello,world,
    cout << endl;

    set<string> b;
    b.insert("lambda");
    b.insert("function");
    b.insert("hello");
    b.insert("world");
    cout << "set b: ";
    copy(b.begin(), b.end(), ostream_iterator<string>(cout, ", ")); // function,hello,lambda,world,
    cout << endl;

    // merge
    cout << "merge a and b: ";
    set_union(a.begin(),a.end(), b.begin(), b.end(), ostream_iterator<string>(cout, ", ")); // apple,banana,function,hello,lambda,world,
    cout << endl;

    // intersection
    cout << "intersection a and b: ";
    set_intersection(a.begin(),a.end(), b.begin(), b.end(), ostream_iterator<string>(cout, ", ")); // hello,world,
    cout << endl;

    // difference
    cout << "difference a-(a&b): ";
    set_difference(a.begin(),a.end(), b.begin(), b.end(), ostream_iterator<string>(cout, ", ")); // apple,banana,
    cout << endl;
    cout << "difference b-(a&b): ";
    set_difference(b.begin(),b.end(), a.begin(), a.end(), ostream_iterator<string>(cout, ", ")); // function,lambda,
    cout << endl;

    // symmetric difference
    cout << "symmetric difference a^b: ";
    set_symmetric_difference(a.begin(),a.end(), b.begin(), b.end(), ostream_iterator<string>(cout, ", ")); // apple,banana,function,lambda,
    cout << endl;

    // subset
    cout << "a is subset of b: " << includes(a.begin(),a.end(), b.begin(), b.end()) << endl; // 0
    cout << "b is subset of a: " << includes(b.begin(),b.end(), a.begin(), a.end()) << endl; // 0
    cout << "a is subset of a: " << includes(a.begin(),a.end(), a.begin(), a.end()) << endl; // 1

    // insert b to a
    cout << "insert b to a: ";
    a.insert(b.begin(), b.end());
    copy(a.begin(), a.end(), ostream_iterator<string>(cout, ", ")); // apple,banana,function,hello,lambda,world,
    cout << endl;

    test1();

    test2();
}