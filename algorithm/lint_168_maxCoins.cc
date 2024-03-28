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
     * @param nums: A list of integer
     * @return: An integer, maximum coins
     * 
     * 有n个气球，编号为0到n-1，每个气球都有一个分数，
     * 存在nums数组中。每次吹气球i可以得到的分数为 nums[left] * nums[i] * nums[right]，
     * left和right分别表示i气球相邻的两个气球。当i气球被吹爆后，其左右两气球即为相邻。
     * 要求吹爆所有气球，得到最多的分数。
     * 你可以假设nums[-1] = nums[n] = 1。-1和n位置上的气球不真实存在，因此不能吹爆它们。
        0 ≤ n ≤ 500, 0 ≤ nums[i] ≤ 100

     * 
     *  输入：[4, 1, 5, 10]
        输出：270
        解释：
        nums = [4, 1, 5, 10] 吹爆 1, 得分 4 * 1 * 5 = 20
        nums = [4, 5, 10]   吹爆 5, 得分 4 * 5 * 10 = 200
        nums = [4, 10]   吹爆 4, 得分 1 * 4 * 10 = 40
        nums = [10]   吹爆 10, 得分 1 * 10 * 1 = 10
        总得分 20 + 200 + 40 + 10 = 270

        输入：[3,1,5]
        输出：35
        解释：
        nums = [3, 1, 5] 吹爆 1, 得分 3 * 1 * 5 = 15
        nums = [3, 5] 吹爆 3, 得分 1 * 3 * 5 = 15
        nums = [5] 吹爆 5, 得分 1 * 5 * 1 = 5
        总得分 15 + 15 + 5  = 35

        求极值问题，数据有序，可能用动态规划
        有区间，典型区间问题，大区间依赖小区间
        为啥是区间问题，想想最后戳爆那个，如果最后戳爆的是k，那么左右两边的气球一定先被戳爆，而左右两两边的气球又包含了更小的区间

        status   dp[i][j]表示爆破i到j所有的气球，得到的最大分数，包括i和j
        function dp[i][j] = max(dp[i][k-1] + dp[k+1][j] + a[i-1]*a[k]*a[j+1]) for i <= k <= j
                ---i---k-1,k,k+1---j---
                ---i(k)---k+1---j------
                ---i------k+1---j(k)---
        init     dp[i][i] = a[i-1]*a[i]*a[i+1] for i = 1 to n
                 dp[i][i+1] = max(dp[i][i] + a[i-1]*a[i+1]*a[i+2], dp[i+1][i+1] + a[i-1]*a[i]*a[i+2])
        result   dp[1][n]

        status  dp[i][j] 也可以表示为戳爆i到j之间的气球，此时只与a[i]和a[j]有关，更直观
        funtion dp[i][j] = max(dp[i][k] + dp[k][j] + a[i] * a[k] * a[j]) for i < k < j
        init    dp[i][i] = 0, 因为中间没有气球
        result  dp[0][n+1]
       */
    int maxCoins(vector<int> &nums) {
        // write your code here
        return maxCoins_0(nums);
    }

    int maxCoins_0(vector<int> &nums) {
        if (nums.size() < 1) {
            return 0;
        }
        if (nums.size() == 1) {
            return nums[0];
        }
        if (nums.size() == 2) {
            return nums[0] + nums[1];
        }
        vector<int> a(nums.size() + 2, 1);
        for (int i = 0; i < nums.size(); i++) {
            a[i + 1] = nums[i];
        }
        vector<vector<int>> dp(nums.size() + 2, vector<int>(nums.size() + 2, 0));
        for (int i = 1; i <= nums.size(); i++) {
            dp[i][i] = a[i-1] * a[i] * a[i+1];
        }
        for (int i = 1; i < nums.size(); i++) {
            dp[i][i+1] = max(dp[i][i] + a[i-1] * a[i+1] * a[i+2], dp[i+1][i+1] + a[i-1] * a[i] * a[i+2]);
        }
        cout << "a: " << endl;
        for (auto i : a) {
            cout << i << " ";
        }
        cout << endl;
        cout << "before dp:" << endl;
        for (auto i : dp) {
            for (auto j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
        for (int i = nums.size(); i >= 1; i--) {
            for (int j = i + 2; j <= nums.size(); j++) {
                for (int k = i; k <= j; k++) {
                    dp[i][j] = max(dp[i][j], dp[i][k-1] + dp[k+1][j] + a[i-1] * a[k] * a[j+1]);
                }
            }
        }
        cout << endl;
        cout << "dp:" << endl;
        for (auto i : dp) {
            for (auto j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
        return dp[1][nums.size()];
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
        auto res = s.maxCoins(nums);
        cout << "permutaions: " << res << endl;
    };

    // nums = {};
    // test(nums);

    // nums = {1};
    // test(nums);

    // nums = {1,2};
    // test(nums);

    nums = {3, 1, 5};
    test(nums);

    nums = {4, 1, 5, 10};
    test(nums);
}