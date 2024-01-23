#include<iostream>
#include<list>
#include<iterator>
#include<algorithm>


using namespace std;

int main() {
    list<int> a;
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    cout << "a: ";
    copy(a.begin(), a.end(), ostream_iterator<int>(cout, ", ")); // a: 1, 2, 3, 4, 
    cout << endl;

    list<int> b;
    b.push_back(5);
    b.push_back(6);
    b.push_back(7);
    b.push_back(8);
    cout << "b: ";
    copy(b.begin(), b.end(), ostream_iterator<int>(cout, ", ")); // b: 5, 6, 7, 8, 
    cout << endl;

    // merge
    cout << "merge a and b: ";
    a.merge(b);
    copy(a.begin(), a.end(), ostream_iterator<int>(cout, ", ")); // merge a and b: 1, 2, 3, 4, 5, 6, 7, 8, 
    cout << endl;

    // sort
    cout << "sort a: ";
    a.sort();
    copy(a.begin(), a.end(), ostream_iterator<int>(cout, ", ")); // sort a: 1, 2, 3, 4, 5, 6, 7, 8, 
    cout << endl;

    // reverse
    cout << "reverse a: ";
    a.reverse();
    copy(a.begin(), a.end(), ostream_iterator<int>(cout, ", ")); // reverse a: 8, 7, 6, 5, 4, 3, 2, 1, 
    cout << endl;

    // unique
    cout << "unique a: ";
    a.unique();
    copy(a.begin(), a.end(), ostream_iterator<int>(cout, ", ")); // unique a: 8, 7, 6, 5, 4, 3, 2, 1, 
    cout << endl;

    // splice
    // list1.splice(position, list2): 将list2中的所有元素剪贴到list1的position位置；
    // list1.splice(position, list2, iter): 将list2中某个位置的迭代器iter指向的元素剪贴到list1中的position位置；
    // list1.splice(position, list2, iter1, iter2): 将list2中的某一段位置iter1 ~ iter2的元素剪贴到list1中的position位置
    b.push_back(5);
    b.push_back(6);
    b.push_back(7);
    b.push_back(8);
    cout << "a before splice: ";
    copy(a.begin(), a.end(), ostream_iterator<int>(cout, ", ")); // a: 8, 7, 6, 5, 4, 3, 2, 1,
    cout << endl;
    cout << "b before splice: ";
    copy(b.begin(), b.end(), ostream_iterator<int>(cout, ", ")); // b: 5, 6, 7, 8,
    cout << endl;
    cout << "splice a and b(把b复制到a的begin): ";
    cout << endl;
    a.splice(a.begin(), b);
    cout << "a after splice: ";
    copy(a.begin(), a.end(), ostream_iterator<int>(cout, ", ")); // splice a and b: 5, 6, 7, 8, 8, 7, 6, 5
    cout << endl;
    cout << "b after splice: ";
    copy(b.begin(), b.end(), ostream_iterator<int>(cout, ", ")); // 此时B中为空，都移动到a去了
    cout << endl;

    int vals[10] = {1,2,3,4,5,6,7,8,9,10};
    list<int> c(vals, vals+10);
    auto f = [](int i) { cout << i << ",";};
    cout << "origin list: ";
    for_each(c.begin(), c.end(), [](int i) { cout << i << ",";});
    cout << endl;
    c.remove_if([](int i) { return i==6;});
    cout << "after remove_if: ";
    for_each(c.begin(), c.end(), [](int i) { cout << i << ",";});
    cout << endl;
    c.remove(8);
    cout << "after remove 8: ";
    for_each(c.begin(), c.end(), [](int i) { cout << i << ",";});
    cout << endl;
    class toobig {
        private:
            int cutoff;
        public:
            toobig(int c) : cutoff(c) {}
            bool operator()(int v) { return v > cutoff; }
    };
    c.remove_if(toobig(5));
    cout << "after remove_if toobig(5): ";
    for_each(c.begin(), c.end(), [](int i) { cout << i << ",";});
    cout << endl;

    list<int> d{5,9,1,3,5,1,9,7,4,2};
    cout << "origin d: ";
    copy(d.begin(), d.end(), ostream_iterator<int>(cout, ", ")); // origin d: 5, 9, 1, 3, 5, 1, 9, 7, 4, 2,
    cout << endl;
    // partition会导致原数据结构改变
    std::list<int>::iterator div_iter = std::partition(d.begin(), d.end(), [](int i) {return i <= 4; });
    cout << "after partition: ";
    copy(d.begin(), d.end(), ostream_iterator<int>(cout, ", ")); // after partition: 3, 1, 1, 2, 5, 5, 9, 7, 4, 9,
    cout << endl;
    for(auto iter = d.begin(); iter != div_iter; ++iter) {
        cout << *iter << ", "; // 3, 1, 1, 2,
    }
    cout << endl;
    for(auto iter = div_iter; iter != d.end(); ++iter) {
        cout << *iter << ", "; // 5, 5, 9, 7, 4, 9,
    }
    cout << endl;
}