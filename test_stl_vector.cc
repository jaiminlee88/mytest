#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
using namespace std;

class P{
public:
    P() {cout << "P construct." << endl; }
    ~P() {cout << "P destruct." << endl; }
    P(int x,int y,int z){
        a[0]=x;a[1]=y;a[2]=z;
        cout << "P x,y,z construct." << endl;
    };
    vector<int> a=vector<int>(3);
};


int main(){
    int test_times = 1;
    cout << "test" << test_times << "======" << endl;
    vector<int> p;
    p.emplace_back(1); // 使用传递给它的任何实参直接在std::vector内部构造一个std::string。没有临时变量会生成：
    // emplace_back使用完美转发，因此只要你没有遇到完美转发的限制，就可以将任何参数传递给emplace_back。
    // p.emplace_back(2);
    test_times++;

    cout << "test" << test_times << "======" << endl;
    vector<P> ps0;
    ps0.push_back(P(1,1,1));
    // ps0.push_back(P(1,2,3));
    test_times++;

    cout << "test" << test_times << "======" << endl;
    vector<P> ps;
    ps.emplace_back(P(1,1,1));
    // ps.emplace_back(P(1,2,3));
    test_times++;

    cout << "test" << test_times << "======" << endl;
    vector<P> ps1;
    ps1.emplace_back(1,1,1);
    // ps1.emplace_back(1,2,3);
    cout<<ps1[0].a[2] << endl;
    test_times++;

    cout << "test" << test_times << "======" << endl;
    vector<string> ps2{"jack","lily","james"};
    copy(ps2.begin(), ps2.end(), ostream_iterator<int>(cout, ", ")); // a: 1, 2, 3, 4, )
    cout << endl;
    ps2.emplace(ps2.begin(), "duck");
    copy(ps2.begin(), ps2.end(), ostream_iterator<int>(cout, ", ")); // a: 4, 1, 2, 3, 4, )
    cout << endl;
    ps2.emplace(ps2.end(), "tom");
    test_times++;
    return 0;
}