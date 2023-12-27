#include<iostream>
#include<vector>
#include<algorithm>
#include<iterator>
#include<math.h>
#include<functional>
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

    // copy从vector到vector
    vector<int> vec1{};
    vec1.resize(vec.size()); // 必须主动调整大小，copy本身不会自动调整
    copy(vec.begin(),vec.end(), vec1.begin());
    copy(vec1.begin(), vec1.end(), ostream_iterator<int,char>(cout, ","));
    cout << endl;

    // insert_iterator
    // insert_iterator<vector<int>> insert_iter(vec1, vec1.begin());// 从vec1的头部开始插入
    insert_iterator<vector<int>> insert_iter(vec1, vec1.end());// 从vec1的末尾开始插入
    *insert_iter++ = 42;
    *insert_iter++ = 43;
    copy(vec1.begin(), vec1.end(), ostream_iterator<int,char>(cout, ","));
    cout << endl;
    cout << "reverse print:" << endl;
    copy(vec1.rbegin(), vec1.rend(), ostream_iterator<int,char>(cout, ","));
    cout << endl;

    cout << "预定义函数符 一元函数" << endl;
    copy(vec1.begin(),vec1.end(), ostream_iterator<int,char>(cout,","));
    cout << endl;
    transform(vec1.begin(), vec1.end(), ostream_iterator<int,char>(cout,","), [](int i) {return i*i;});
    cout << endl;
    copy(vec1.begin(),vec1.end(), ostream_iterator<int,char>(cout,","));
    cout << endl;

    cout << "预定义函数符1 二元函数" << endl;
    vector<int> vec3{4,5,6,7};
    cout << "before vec1: ";
    copy(vec1.begin(),vec1.end(), ostream_iterator<int,char>(cout,","));
    cout << endl;
    cout << "before vec3: ";
    copy(vec3.begin(),vec3.end(), ostream_iterator<int,char>(cout,","));
    cout << endl;
    cout << "transform vec1 first: ";
    transform(vec1.begin(),vec1.end(), vec3.begin(), ostream_iterator<int,char>(cout,","), [](int i, int j) {cout << "[" << i << ":" << j << "] ";return i+j;});
    cout << endl;
    cout << "transform1 vec3 first: ";
    transform(vec3.begin(),vec3.end(), vec1.begin(), ostream_iterator<int,char>(cout,","), [](int i, int j) {return i+j;});
    cout << endl;

    cout << "预定义函数符2，二元函数" << endl;
    vector<int> vec4{4,5,6,7};
    plus<int> add3;// 预定义函数符，直接用已有的模板定义
    cout << "add3(3,4)=" << add3(3,4) << endl;
    cout << "before vec1: ";
    copy(vec1.begin(),vec1.end(), ostream_iterator<int,char>(cout,","));
    cout << endl;
    cout << "before vec3: ";
    copy(vec3.begin(),vec3.end(), ostream_iterator<int,char>(cout,","));
    cout << endl;
    transform(vec4.begin(),vec4.end(), vec1.begin(), ostream_iterator<int,char>(cout,","), add3);
    cout << endl;

    cout << "预定义函数符4，自适应" << endl;
    ostream_iterator<int> out_iter(cout, ",");
    vector<int> vec5{4,5,6,7};
    vector<int> vec6{1,2,3,4,5,6};
    cout << "vec5*3:";
    transform(vec5.begin(), vec5.end(), out_iter, bind1st(multiplies<int>(), 3));
    cout << endl;
    cout << "vec5*vec6:";
    transform(vec5.begin(), vec5.end(), vec6.begin(), out_iter, multiplies<int>());
    cout << endl;

    cout << "算法之replace" << endl;
    vector<int> vec7 = {4,5,6,7,6};
    cout << "vec7: ";
    copy(vec7.begin(), vec7.end(), ostream_iterator<int>(cout, ","));
    cout << endl;
    replace(vec7.begin(), vec7.end(), 6, 8); // 把6改成8
    cout << "after vec7: ";
    copy(vec7.begin(), vec7.end(), ostream_iterator<int>(cout, ","));
    cout << endl;

    cout << "算法之replace_copy" << endl;
    vector<int> vec8 = {4,5,6,7,6};
    cout << "vec8: ";
    copy(vec8.begin(), vec8.end(), ostream_iterator<int>(cout, ","));
    cout << endl;
    vector<int> vec9(5);
    replace_copy(vec8.begin(), vec8.end(),vec9.begin(), 6, 8); // 把6改成8
    cout << "after vec9: ";
    copy(vec9.begin(), vec9.end(), ostream_iterator<int>(cout, ","));
    cout << endl;

    cout << "算法之replace_if" << endl;
    vector<int> vec10 = {4,5,6,7,6};
    cout << "vec10: ";
    copy(vec10.begin(), vec10.end(), ostream_iterator<int>(cout, ","));
    cout << endl;
    replace_if(vec10.begin(), vec10.end(), [](int num)->bool { return num == 6;}, 8); // 把6改成8
    cout << "after vec10: ";
    copy(vec10.begin(), vec10.end(), ostream_iterator<int>(cout, ","));
    cout << endl;

    cout << "please input word, end with q: "<<endl; // 唯一组合
    string letters;
    while(cin >> letters && letters != "q") {
        cout << "origin: " << letters << endl;
        sort(letters.begin(), letters.end());
        cout << "after sorted letters: " << letters << endl;
        int i = 0;
        while(next_permutation(letters.begin(), letters.end())) {
            ++i;
            cout << i <<" next_permutaion(letters)=" << letters << endl;
        }
    }

    // valid，copy从屏幕输入到vector中
    cout << "please input numbers, end with ctrl+D:" <<endl;
    vector<int> vec2{};
    vec2.resize(10); // 必须足够大小，否则使用push_back
    copy(istream_iterator<int,char>(cin), istream_iterator<int,char>(), vec2.begin()); // valid
    copy(vec2.begin(), vec2.end(), ostream_iterator<int, char>(cout, ","));
    cout << endl;

    // valid
    // 从屏幕输入到vector中
    // 从标准输入流读取数据，直到遇到文件结束符（EOF）
    cout << "please input numbers, end with ctrl+D:" <<endl;
    vec2.clear();
    std::istream_iterator<int> inputIter(std::cin);
    std::istream_iterator<int> endIter;
    // 将从输入流读取的值添加到vector中
    while (inputIter != endIter) {
        vec2.push_back(*inputIter);
        ++inputIter;
    }
    copy(vec2.begin(),vec2.end(), ostream_iterator<int,char>(cout,","));
    cout << endl;

   
}