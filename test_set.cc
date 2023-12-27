#include<iostream>
#include<set>
#include<algorithm>
#include<iterator>
using namespace std;

int main(){
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
}