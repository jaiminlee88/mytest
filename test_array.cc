#include<iostream>
#include <array>
using namespace std;

const int Season = 4;
int main () {
    int a = 4;
    // std::array<std::string, a> abc; // invalid
    std::array<std::string, Season> season_name{"spring", "summer", "autumn", "winter"}; // 第二个参数是初始化个数这种
    for (auto& it : season_name) {
        cout << it << endl;
    }

    int b[3][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
    for (int i=0; i<3; ++i) {
        for (int j=0; j<4; ++j) {
            cout<< b[i][j] << ",";
        }
        cout << endl;
    }
    return 0;
}