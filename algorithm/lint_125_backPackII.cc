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
     * @param m: An integer m denotes the size of a backpack
     * @param a: Given n items with size A[i]
     * @param v: Given n items with value V[i]
     * @return: The maximum value
     * 
     * 有 n 个物品和一个大小为 m 的背包. 给定数组 A 表示每个物品的大小和数组 V 表示每个物品的价值.
        问最多能装入背包的总价值是多大?
        A[i], V[i], n, m 均为整数
        你不能将物品进行切分
        你所挑选的要装入背包的物品的总大小不能超过 m
        每个物品只能取一次
        m <= 1000m<=1000

        典型背包问题，动态规划
     */
    int backPackII(int m, vector<int> &a, vector<int> &v) {
        // write your code here
        return backPackII_0(m, a, v);
    }

    int backPackII_0(int m, vector<int> &a, vector<int> &v) {
        if (m <= 0 || a.empty() || v.empty() || a.size() != v.size()) {
            return 0;
        }
        vector<vector<int>> dp(a.size() + 1, vector<int>(m + 1, 0)); // dp表示最大价值
        // dp[0][j] = 0 没有物品时，不管包多大，价值都是0
        // dp[i][0] = 0 背包大小为0时，不管有多少物品，价值都是0
        for (int i = 1; i <= a.size(); ++i) {
            // 遍历所有前i个物品选择情况
            for (int j = 1; j <= m; ++j) {
                // 遍历所有包大小
                if (j < a[i-1]) {
                    // j < a[i - 1] 说明当前物品i不能放入背包
                    dp[i][j] = dp[i-1][j];
                } else {
                    // 当前物品可以放入背包
                    dp[i][j] = max(dp[i-1][j], dp[i-1][j-a[i-1]] + v[i-1]);
                }
            }
        }
        // 记录那些物品被装入背包，展示最佳选择
        vector<int> res;
        int i = a.size();
        int j = m;
        while (i > 0 || j > 0) {
            if (dp[i][j] == dp[i-1][j]) {
                // 没有用到第i个物品
                i -= 1;
                continue;
            }
            // 用到了第i个物品
            res.push_back(i-1);
            i -= 1;
            j -= a[i];
        }
        cout << "res: ";
        for (auto& i : res) {
            cout << i << " ";
        }
        cout << endl;
        return dp[a.size()][m];
    }
};


int main() {
    Solution s;
    vector<int> nums;
    vector<int> value;
    int m;
    auto test = [&s](vector<int>& nums, vector<int>& values, int m) {
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
        cout << "value: ";
        for (auto& i : values) {
            cout << i << " ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.backPackII(m, nums, values);
        cout << "permutaions: "<< res << endl;
    };

    nums = {2, 3, 5, 7};
    value = {1, 5, 2, 4};
    m = 10;
    test(nums, value, m);

    nums = {2, 3, 8};
    value = {2, 5, 8};
    m = 10;
    test(nums, value, m);

}