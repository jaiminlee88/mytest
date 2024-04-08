#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <climits>
#include <list>
#include <stack>
#include <set>
#include <queue>
#include <numeric>
#include <bitset>
using namespace std;


class Solution {
public:
    /**
     * @param a: A 32bit integer
     * @param b: A 32bit integer
     * @param n: A 32bit integer
     * @return: An integer
     * 
     * 计算a^n%b,其中a，b和n都是32位的非负整数。
     * a = 3
        b = 7
        n = 5
        5

        a = 3
        b = 1
        n = 0
        0
     */
    int fastPower(int a, int b, int n) {
        // write your code here
        // return fastPower_0(a, b, n);
        return fastPower_1(a, b, n);
    }
    int fastPower_0(int a, int b, int n) {
        if (b == 0 || a == 0) {
            return 0;
        }
        if (n == 0) {
            return 1 % b;
        }
        if (n == 1) {
            return a % b;
        }

        long tmp = fastPower_0(a, b, n / 2) % b;
        tmp = (tmp * tmp) % b;
        if (n % 2 == 1) { // 奇数
            tmp = (tmp * a) % b;
        }
        return tmp;
    }
    int fastPower_1(int a, int b, int n) {
        // 二进制快速幂
        int ans = 1;
        int tmp = a;
        while (n != 0) {
            if (n % 2 == 1) {
                ans = (ans * tmp) % b;
            }
            tmp = (tmp * tmp) % b;
            n = (int)(n / 2);
        }
        return (int)(ans % b);
    }
};

int main() {
    Solution s;
    vector<int> nums;
    vector<int> nums0;
    vector<int> nums1;
    vector<vector<int>> nums3;
    vector<vector<char>> nums4;
    vector<string> nums5;
    string str;
    string str0;
    string str1;
    int k = 0;
    auto test = [&s](int a, int b, int n) {
        auto res = s.fastPower(a,b,n);
        cout << "a: " << a << ", b: " << b << ", n: " << n << ", res: " << res << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
    };

    test(3, 7, 5);
    test(3, 1, 0);
    test(2,3,31);
    test(100, 1000, 1000);
    test(2147483647, 2147483645, 214748364);
}