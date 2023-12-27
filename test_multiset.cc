#include<iostream>
#include<set>
#include<iterator>

using namespace std;

int main(){
    std::multiset<string> a;
    a.insert("apple");
    a.insert("banana");
    a.insert("stem");
    a.insert("hello");
    a.insert("banana");
    copy(a.begin(), a.end(), ostream_iterator<std::string>(cout, ","));
    cout << endl;
    copy(a.rbegin(), a.rend(), ostream_iterator<std::string>(cout, ","));
    cout << endl;
}