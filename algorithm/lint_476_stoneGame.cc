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
     * @param a: An integer array
     * @return: An integer
     * 
     * 有一个石子归并的游戏。最开始的时候，有n堆石子排成一列，目标是要将所有的石子合并成一堆。合并规则如下：
        每一次可以合并相邻位置的两堆石子
        每次合并的代价为所合并的两堆石子的重量之和
        求出最小的合并代价。

        输入: [3, 4, 3]
        输出: 17

        输入: [4, 1, 1, 4]
        输出: 18
        解释:
        1. 合并第二堆和第三堆 => [4, 2, 4], score = 2
        2. 合并前两堆 => [6, 4]，score = 8
        3. 合并剩余的两堆 => [10], score = 18

        求极值，数字有序，可能用动态规划
        典型区间问题，大区间依赖小区间
        大区间成本 = 小区间成本 + 两个小区间的和
        状态d[i][j]表示合并第i堆到第j堆石子的最小成本
        方程d[i][j] = min(d[i][k] + d[k+1][j] + sum[i][j]) for i <= k < j
                    = min(d[i][k] + d[k+1][j] + sum[i][j]) for i <= k < j
            k是i和j之间的分割点，可以根据长度来划分，先确定长度，再确定起点
        初始化d[i][i] = sum[i][i] = a[i], sum[i][j]表示第i堆到第j堆石子的重量和
        返回d[0][n-1]
     */
    int stoneGame(vector<int> &a) {
        // write your code here
        return stoneGame_0(a);
    }

    int stoneGame_0(vector<int> &a) {
        if (a.size() < 1) {
            return 0;
        }
        vector<vector<int>> dp(a.size(), vector<int>(a.size(), INT_MAX));
        vector<vector<int>> sum(a.size(), vector<int>(a.size(),0));
        for (int i = 0; i < a.size(); i++) {
            sum[i][i] = a[i];
            dp[i][i] = 0;
            if (i + 1 < a.size()) {
                sum[i][i+1] = a[i] + a[i+1];
                dp[i][i+1] = a[i] + a[i+1];
            }
        }
        for (int i = a.size() - 1; i >= 0; i--) {
            for (int j = i + 2; j < a.size(); j++) {
                sum[i][j] = sum[i][j-1] + a[j]; // at least 3 chars
                for (int k = i; k < j; k++) {
                    dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j] + sum[i][k] + sum[k+1][j]);
                }
                // cout << "i: " << i << " j: " << j << " sum: " << sum[i][j] << " dp: " << dp[i][j] << endl;
            }
        }
        // cout << "dp: " << endl;
        // for (auto i : dp) {
        //     for (auto j : i) {
        //         cout << j << " ";
        //     }
        //     cout << endl;
        // }
        // cout << "sum: " << endl;
        // for (auto i : sum) {
        //     for (auto j : i) {
        //         cout << j << " ";
        //     }
        //     cout << endl;
        // }
        return dp[0][a.size() - 1];
    }
};


int main() {
    Solution s;
    vector<int> nums;
    auto test = [&s](vector<int>& nums) {
        cout << "nums: ";
        for (auto i : nums) {
            cout << i << " ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.stoneGame(nums);
        cout << "permutaions: " << res << endl;
    };

    nums = {};
    test(nums);

    nums = {1};
    test(nums);

    nums = {1,2};
    test(nums);

    nums = {3, 4, 3};
    test(nums);

    nums = {4, 1, 1, 4};
    test(nums);
}