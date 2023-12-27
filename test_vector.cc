#include <vector>
#include <iostream>

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
    p.emplace_back(1);
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

    return 0;
}