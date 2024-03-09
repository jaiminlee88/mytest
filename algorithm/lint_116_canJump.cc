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
     * @param a: A list of integers
     * @return: A boolean
     * 
     * 给出一个非负整数数组，你最初定位在数组的第一个位置。
        数组中的每个元素代表你在那个位置可以跳跃的最大长度。
        判断你是否能到达数组的最后一个位置。

        A = [2,3,1,1,4]
        返回 true.

        A = [3,2,1,0,4]
        返回 false.

        可以用动态规划
        可以用DFS探索路径，复杂度也很高，不确定每次多少步数
        贪心法
     */
    bool canJump(vector<int> &a) {
        // write your code here
        return canJump0(a);
    }

    bool canJump0(vector<int> &a) {
        // write your code here
        // 动态规划，但效率不是很高 时间复杂度O(n^2)
        if (a.size() < 1) {
            return false;
        }
        if (a.size() == 1) {
            return true;
        }
        vector<bool> dp(a.size(), false); // 表示节点i能够到达0节点，如果i能到达j且dp[j]为true，则dp[i]为true
        dp[0] = true;
        for (int i = 1; i < a.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if ((i - j <= a[j]) && (dp[j] == true)) {
                    dp[i] = true;
                    break;
                }
            }
        }
        // for (auto it : dp) {
        //     cout << it << " ";
        // }
        cout << endl;
        return dp[a.size() - 1];
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
        auto res = s.canJump(nums);
        if (res) {
            cout << "can jump" << endl;
        } else {
            cout << "can not jump" << endl;
        }

    };

    nums = {2,3,1,1,4};
    test(nums);

    nums = {3,2,1,0,4};
    test(nums);
}