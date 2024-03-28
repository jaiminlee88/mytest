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
     * @param nums: An integer array
     * @return: The length of LIS (longest increasing subsequence)
     * 给定一个整数序列，找到最长上升子序列（LIS），返回LIS的长度。
     * 最长上升子序列的定义：
        最长上升子序列问题是在一个无序的给定序列中找到一个尽可能长的由低到高排列的子序列，
        这种子序列不一定是连续的或者唯一的。

        nums = [5,4,1,2,3] 3 LIS 是 [1,2,3]
        nums = [4,2,4,5,3,7] 4 LIS 是 [2,4,5,7]

        DPS方法，每次确定一位，找下一个比它大的数，然后递归，找不到最大的数，就记录当前的长度，找到最长的
        动态规划法

        求极值，数组有序，可能用动态规划
        类似一维度接龙问题
        status   dp[i] 表示以i结尾的最长上升子序列的长度
        function dp[i] = max(dp[j] + 1), for j in [1, i - 1] && nums[j] < nums[i]
        init     dp[0] = 0; dp[1] = 1
        result   max(dp[i]), for i in [1,n]
     */
    int longestIncreasingSubsequence(vector<int> &nums) {
        // write your code here
        return longestIncreasingSubsequence_0(nums);
    }
    int longestIncreasingSubsequence_0(vector<int> &nums) {
        // 动态规划法
        if (nums.size() <= 1) {
            return nums.size();
        }
        vector<int> dp(nums.size(), 1);
        int max_len = 1;
        int max_index = 0;
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] >= nums[i]) {
                    continue;
                }
                dp[i] = max(dp[i], dp[j] + 1);
                if (dp[i] > max_len) {
                    max_len = dp[i];
                    max_index = i;
                }
            }
        }
        cout << "dp: ";
        for (auto& i : dp) {
            cout << i << " ";
        }
        cout << endl;
        vector<int> path;
        path.push_back(nums[max_index]);
        // 也可以用数组记录前一个节点
        for (int i = max_index - 1; i >= 0; --i) {
            if (dp[max_index] - 1 == 0) {
                break;
            }
            if (dp[i] == dp[max_index] - 1) {
                path.push_back(nums[i]);
                max_index = i;
            }
        }
        cout << "path: ";
        for (auto& i : path) {
            cout << i << " ";
        }
        cout << endl;
        return max_len;
    }
};

int main() {
    Solution s;
    vector<int> nums;
    auto test = [&s](vector<int>& nums) {
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.longestIncreasingSubsequence(nums);
        cout << "permutaions: "<< res << endl;
    };

    nums = {};
    test(nums);

    nums = {1};
    test(nums);

    nums = {1,2};
    test(nums);

    nums = {5,4,1,2,3};
    test(nums);

    nums = {4,2,4,5,3,7};
    test(nums);
}