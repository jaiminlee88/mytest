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
     * @param a: an integer array
     * @param v: an integer array
     * @param m: An integer
     * @return: an array
     * 
     * 给定 n 种物品, 每种物品都有无限个. 第 i 个物品的体积为 A[i], 价值为 V[i].
        再给定一个容量为 m 的背包. 问可以装入背包的最大价值是多少?


        输入: A = [2, 3, 5, 7], V = [1, 5, 2, 4], m = 10
        输出: 15
        解释: 装入三个物品 1 (A[1] = 3, V[1] = 5), 总价值 15.

        输入: A = [1, 2, 3], V = [1, 2, 3], m = 5
        输出: 5
        解释: 策略不唯一. 比如, 装入五个物品 0 (A[0] = 1, V[0] = 1).

        本质上还是01背包问题，只是每次多次尝试放入重复物品，选择最大那个
        dp每一层代表着加或者不加的情况，多加几个的情况
     */
    int backPackIII(vector<int> &a, vector<int> &v, int m) {
        // write your code here
        return backPackIII_0(a, v, m);
    }

    int backPackIII_0(vector<int> &a, vector<int> &v, int m) {
        if (m < 1 || a.empty() || v.empty() || a.size() != v.size()) {
            return 0;
        }
        vector<vector<int>> dp(a.size() + 1, vector<int>(m + 1, 0));
        for (int i = 1; i <= a.size(); ++i) {
            for (int j = 1; j <= m; ++j) {
                if (j < a[i-1]) {
                    dp[i][j] = dp[i-1][j];
                } else {
                    int max_cnt = j / a[i-1]; // 重复物品的个数，最多
                    dp[i][j] = dp[i-1][j];
                    // 以下两个式子等价
                    dp[i][j] = max(dp[i][j], dp[i][j-a[i-1]] + v[i-1]); // 相当于必有i物品
                    // for (int k = 1; k <= max_cnt; ++k) {
                    //     dp[i][j] = max(dp[i][j], dp[i-1][j-k*a[i-1]] + k * v[i - 1]);
                    // }
                }
            }
        }
        cout << "dp:" << endl;
        for(auto& i : dp) {
            for (auto& j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
        // 输出最佳方案
        int i = a.size();
        int j = m;
        vector<vector<int>> res;
        vector<int> tmp;
        generate_res(dp, i, j, a, v, res, tmp);
        cout << "res: " << endl;
        for (auto& i : res) {
            for (auto& j : i) {
                cout << a[j] << "(" << v[j] << ") ";
            }
            cout << endl;
        }
        return dp[a.size()][m];
    }
    void generate_res(vector<vector<int>>& dp,
                        int i,
                        int j,
                        vector<int>& a,
                        vector<int>& v,
                        vector<vector<int>>& res, vector<int>& tmp) {
        if (i == 0 || j == 0) {
            res.push_back(tmp);
            return;
        }
        // 用到了第i个物品
        int max_cnt = j / a[i-1]; // 重复物品的个数，最多
        for (int k = 0; k <= max_cnt; ++k) {
            // 0表示没有用到第i个物品
            if (dp[i][j] == (dp[i-1][j-k*a[i-1]] + k * v[i-1])) {
                for (int l = 0; l < k; ++l) {
                    tmp.push_back(i-1);
                }
                generate_res(dp, i-1, j-k*a[i-1], a, v, res, tmp);
                for (int l = 0; l < k; ++l) {
                    tmp.pop_back();
                }
            }
        }
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
        auto res = s.backPackIII(nums, values, m);
        cout << "permutaions: "<< res << endl;
    };

    nums = {2, 3, 5, 7};
    value = {1, 5, 2, 4};
    m = 10;
    test(nums, value, m);

    nums = {1, 2, 3};
    value = {1, 2, 3};
    m = 5;
    test(nums, value, m);

}