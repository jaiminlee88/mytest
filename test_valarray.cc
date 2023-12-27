#include<iostream>
#include<valarray>
#include<iterator>
using namespace std;

// 打印valarray
template <typename T> void printValarray(const valarray<T>& va)
{
	for(int i=0;i<va.size();i++)
		cout<<va[i]<<"  ";
	cout<<endl;
}

void show (const std::valarray<int> &v, int cols) {
    int lim = v.size();
    for (int i=0; i<lim; ++i) {
        cout.width(3);
        cout << v[i];
        if ((i % cols) == (cols -1)) {
            cout << endl;
        } else {
            cout << " ";
        }
    }
    if (lim % cols != 0) {
        cout << endl;
    }
}
int main() {
    using std::slice;
    const int SIZE = 12;

    // int input[] = {1,2,3,4,10,11,5,6,7,8,9,12};
    // std::valarray<int> valint(input, sizeof(input)/sizeof(int));
    // std::valarray<int> valint({1,2,3,4,10,11,5,6,7,8,9,12}); // 显示调用构造函数
    std::valarray<int> valint{1,2,3,4,10,11,5,6,7,8,9,12};
    printValarray(valint);

    sort(begin(valint), end(valint));
    printValarray(valint);

    valint = 10*valint;
    printValarray(valint);

    valint = valint + valint; // 可能效率并不那么高，基于循环
    printValarray(valint);

    cout << "origin:" << endl;
    show(valint, 3);

    cout << "col 1:" << endl;
    show(valint[slice(0,4,3)],1);

    cout << "col 3:" <<endl;
    show(valint[slice(2,4,3)],1);

    cout << "valint[slice[1,4,3]] = 10;" << endl;
    valint[slice(1,4,3)] = 10;
    show(valint, 3);
}