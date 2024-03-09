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
     * @param nums: the given array
     * @return: the minimum difference between their sums 
     * 
     * 给出一个正整数数组，写一个程序把这个整数数组分成S1跟S2两部分，
     * 使S1中的和跟S2中的和的差的绝对值最小。
     * 换句话讲，如果有一个一个整数数组 S 有 n 个数，如果Subset1有 m 个数，
     * Subset2必须有 n-m 个数并且 abs(sum(Subset1) – sum(Subset2)) 应该最小
     * 
     * 
     * 输入: nums = [1, 6, 11, 5]
        输出: 1
        解释: 
        Subset1 = [1, 5, 6]，和是12
        Subset2 = [11]，和是11
        abs(11 - 12) = 1

      输入: nums = [1, 2, 3, 4]
        输出: 0
        解释: 
        Subset1 = [1, 4], 和是 5
        Subset2 = [2, 3], 和是5   
        abs(5 - 5) = 0     

        暴力解法，先装满s2，再逐步往s1装，然后比较，1个时，2ge时，3个时，4个时，...n-1个时
        sum/2，找到最接近sum/2的值
     */
    int findMin(vector<int> &nums) {
        // write your code here
        // return findMin0(nums);
        return findMin1(nums);
    }
    int findMin0(vector<int> &nums) {
        if (nums.size() < 1) {
            return 0;
        }
        if (nums.size() == 1) {
            return nums[0];
        }
        sort(nums.begin(), nums.end());
        int total = 0;
        for (auto& i : nums) {
            total += i;
        }
        int target = total / 2;
        int minsum = INT_MAX;
        findMin0_core(nums, 0, target, minsum, 0); // 这种找法有点慢
        return abs(total - 2 * minsum);
    }
    void findMin0_core(vector<int>& nums, int start, int& target, int& minsum, int sum) {
        if (start >= nums.size()) {
            return;
        }
        if (abs(sum + nums[start] - target) < abs(minsum - target)) {
            minsum = sum + nums[start];
        }
        findMin0_core(nums, start + 1, target, minsum, sum);
        findMin0_core(nums, start + 1, target, minsum, sum + nums[start]);
    }

    int findMin1(vector<int> &nums) {
        if (nums.size() < 1) {
            return 0;
        }
        if (nums.size() == 1) {
            return nums[0];
        }
        int total = 0;
        for (auto& i : nums) {
            total += i;
        }
        int target = total / 2;
        // 尝试使用背包法，即从一堆数中选择一些数，使得这些数的和最接近target
        // 背包大小为target，物品大小为nums[i]，假设物品价值对应物品大小num[i]
        // 0 - 1背包问题
        // 0 ~ nums.size()个物品
        vector<vector<int>> dp(nums.size() + 1, vector<int>(target + 1, 0));
        // dp[0][j] 不装物品，都为0
        // dp[i][0] 背包大小为0，都为0
        for (int i = 1; i <= nums.size(); i++) {
            int w_i = nums[i-1];
            int v_i = nums[i-1]; // 是要求最小差值，所以价值等于物品大小
            for (int j = 1; j <= target; j++) {
                if (j < w_i) {
                    dp[i][j] = dp[i-1][j];
                } else {
                    dp[i][j] = max(dp[i-1][j], dp[i-1][j-w_i] + v_i);
                }
            }
        }
        return abs(total - 2 * dp[nums.size()][target]);
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
        auto res = s.findMin(nums);
        cout << "permutaions: "<< res <<  endl;
    };

    nums = {1,5,6,11};
    test(nums);

    nums = {1, 2, 3, 4};
    test(nums);
}