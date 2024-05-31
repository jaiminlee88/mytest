#include <iostream>
#include <set>

using namespace std;

// Write  a  function  that  returns  the  number  of  unique  strings  of  three  characters  in  in-putString.  For example,abcbcbreturns 3.
int unique_Substr_Cnt(string str) {
    if (str.size() < 3) {
        return 0;
    }
    set<string> counter;
    for (int i = 0; i < str.size() - 2; i++) {
        counter.insert(str.substr(i, 3));
    }
    return counter.size();
}

int get_layer_cnt(int n) {
    int layer_cnt = 0;
    int cnt = 1;
    while(true) {
        if (n >= cnt) {
            n -= cnt;
            cnt += 2;
            layer_cnt++;
        } else {
            if (n != 0) {
                layer_cnt++;
            }
            break;
        }
    }
    return layer_cnt;
}
// Write a function that usesinputStringto print to screen a pyramid.
void print_pyramid(string str) {
    if (str.size() < 1) {
        return;
    }
    int n = str.size();
    // get layer count
    int layer_cnt = get_layer_cnt(n);
    // print
    int col_num = 2 * layer_cnt - 1;
    int row_num = 1;
    int pos = 0;
    for (int i = 0; i < layer_cnt; i++) {
        for (int j = 0; j < (col_num - row_num) / 2; j++) {
            cout << " ";
        }
        cout << str.substr(pos, row_num);
        pos += row_num;
        cout << endl;
        row_num += 2;
    }
}
int main ()
{
    string str = "asdfasdfasdfafafasdsdfsadfasdfgfafasdffasdfasdfafasasfedfafafasdfasd";
    set<char> counter;

    for (int i=0; i < str.size(); i++)
    {
        counter.insert(str[i]);
    }
    cout << counter.size() << endl;

    string str1 = "abcbcb";
    cout << unique_Substr_Cnt(str1) << endl;

    print_pyramid(str);

    return 0;
}