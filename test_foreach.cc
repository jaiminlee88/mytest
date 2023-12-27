#include<iostream>
#include<vector>
#include<algorithm>
#include<iterator>
using namespace std;

void print(int& i) {
    cout << "i=" << i << endl;
}
int main() {
    vector<int> vec = {4,5,6,1,2,3};
    for_each(vec.begin(),vec.end(), print);

    for (vector<int>::iterator it = vec.begin(); it!= vec.end(); it++) {
        cout << "*it=" << *it << endl;
    }

    for (auto& ref_it : vec) {
        cout << "ref_it=" << ref_it << endl;
    }

    for (int &ref_it : vec) {
        cout << "int_ref_it=" << ref_it << endl;
    }

    sort(vec.begin(), vec.end(), [](int a, int b) ->bool { return a>=b?true:false;});
    for (auto& ref_it : vec) {
        cout << "after ref_it=" << ref_it << endl;
    }

    // copy(vec.begin(), vec.end(), out_iter);
    copy(vec.begin(), vec.end(), ostream_iterator<int>(cout,","));
    cout << endl;

    vector<int> vec1{};
    vec1.resize(vec.size()); // 必须主动调整大小，copy本身不会自动调整
    copy(vec.begin(),vec.end(), vec1.begin());
    copy(vec1.begin(), vec1.end(), ostream_iterator<int,char>(cout, ","));
    cout << endl;

    // valid
    cout << "please input numbers, end with ctrl+D:" <<endl;
    vector<int> vec2{};
    vec2.resize(10); // 必须足够大小，否则使用push_back
    copy(istream_iterator<int,char>(cin), istream_iterator<int,char>(), vec2.begin()); // valid
    copy(vec2.begin(), vec2.end(), ostream_iterator<int, char>(cout, ","));
    cout << endl;

    // valid
    // 从屏幕输入到vector中
    // 从标准输入流读取数据，直到遇到文件结束符（EOF）
    cout << "please input numbers, e
    nd with ctrl+D:" <<endl;
    vec2.clear();
    std::istream_iterator<int> inputIter(std::cin);
    std::istream_iterator<int> endIter;
    // 将从输入流读取的值添加到vector中
    while (inputIter != endIter) {
        vec2.push_back(*inputIter);
        ++inputIter;
    }
    copy(vec2.begin(),vec2.end(), ostream_iterator<int,char>(cout,","));
}