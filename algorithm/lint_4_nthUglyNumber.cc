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
using namespace std;


class Solution {
public:
 /**
     * @param n: An integer
     * @return: return a  integer as description.
     * 
     * 如果一个数只有质数因子2，3，5 ，那么这个数是一个丑数。
        前10个丑数分别为 1, 2, 3, 4, 5, 6, 8, 9, 10, 12...设计一个算法，找出第N个丑数。
        我们可以认为 1 也是一个丑数。
        X = 2^x * 3^y * 5^z, 可以是[x,y,z]可以是2 3 5 的任何组合
        n = 9
        return 10
        解释：
            [1,2,3,4,5,6,8,9,10,....]，第9个丑数为10。

        n = 1
        return 1

        直接暴力解法，从1开始遍历，判断是否是丑数，直到找到第n个丑数
        但是这样的时间复杂度是O(n^2)

        如何逐步生成丑数，然后计数，直到第n个丑数, 任意丑数x，x*2, x*3, x*5都是丑数，且都是由x生成的，都大于x，确保了顺序，时间复杂度O(nlogn)


     */
    int nthUglyNumber(int n) {
        // write your code here
        // return nthUglyNumber1(n);
        return nthUglyNumber2(n);
    }
    int nthUglyNumber1(int n) {
        // 直接生成法
        if (n <= 0) {
            return 0;
        }
        int N = 0;
        priority_queue<long long, vector<long long>, greater<long long>> s;
        std::set<long long> set;
        s.push(1);
        long long val = 0;
        vector<int> num = {2,3,5};
        while (N < n) {
            val = s.top();
            for (auto& v : num) {
                long long tmp = val*v;
                if (set.find(tmp) == set.end()) {
                    set.insert(tmp);
                    s.push(tmp);
                }
            }
            s.pop();
            N++;
        }
        // cout << endl;
        return val;
    }

    int nthUglyNumber2(int n) {
        // 动态规划法
        // 解题思路：动态规划
        // 使用数组 dp[n] 存储第k个丑数
        // 写出状态转移方程 —— 这一步需要我们稍微分析一下
        // dp[k] = ?
        // （1）丑数如何产生 —— 由前面的丑数乘以质因子2， 3， 5
        // （2） 第 k 大个丑数产生条件 —— 由第a个丑数乘2产生，由第b个丑数乘3产生，由第c个丑数乘5产生 —— 第k大的丑数就是它们中的最小值
        // （3） 在循环过程中，我们需要使用a, b, c记录下乘2的次数，乘3的次数以及乘5的次数

        if (n <= 0) {
            return 0;
        }
        vector<int> dp(n, 0);
        dp[0] = 1;
        int p2 = 0, p3 = 0, p5 = 0;
        for (int i = 1; i < n; ++i) {
            dp[i] = std::min({dp[p2]*2, dp[p3]*3, dp[p5]*5});
            // 这样才能去重
            if (dp[i] == dp[p2] * 2) {
                p2++;
            }
            if (dp[i] == dp[p3] * 3) {
                p3++;
            }
            if (dp[i] == dp[p5] * 5) {
                p5++;
            }
        }
        return dp[n - 1];
    }
};


int main() {
    Solution s;
    vector<int> nums;
    auto test = [&s](int n) {
        int ret  = s.nthUglyNumber(n);
        cout << "n=" << n << " ret=" << ret << endl;
    };
    // long long a = INT64_MAX;
    // cout << INT64_MAX << " " << a<< endl;;

    // test(0);
    // test(1);
    // test(2);
    // test(3);
    // test(4);
    // test(5);
    // test(6);
    // test(7);
    // test(8);
    // test(9);
    // test(10);
    test(1665);
}