#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <climits>
#include <stack>
using namespace std;


class Solution {
public:
    /**
     * @param nums: A list of integers
     * @return: A list of unique permutations
     *          we will sort your return value in output
     * 给出一个具有重复数字的列表，找出列表所有不同的排列。
     * 
     * 样例
        给出列表 [1,2,2]，不同的排列有：
        [
          [1,2,2],
          [2,1,2],
          [2,2,1]
        ]

        [1,2,2,3]，不同排列有：
        [
          [1,2,2,3],
          [2,1,2,3],
          [2,2,1,3],
          [2,2,3,1],
          [1,2,3,2],
          [2,1,3,2],
          [2,3,1,2],
          [2,3,2,1],
        ]

        思路：
        到达当前节点时，向下找一个与该节点不同的节点交换，遇到重复的就跳过，
        交换重复的节点并不会改变当前节点的排列，所以跳过。
        1. 递归
        2. 非递归
     */
    vector<vector<int>> permuteUnique(vector<int> &nums) {
        // write your code here
        return permuteUnique_0(nums);
    }

    vector<vector<int>> permuteUnique_0(vector<int>& nums) {
        // 分治递归
        if (nums.size() == 0) {
            return {{}};
        }
        sort(nums.begin(), nums.end());
        vector<vector<int>> res;
        permuteUnique_0_core(nums, res, 0);
        return res;
    }
    void permuteUnique_0_core(vector<int>& nums, vector<vector<int>>& res, int start) {
        if (start >= nums.size() - 1) {
            res.push_back(nums);
            return;
        }
        permuteUnique_0_core(nums, res, start + 1); // 不交换
        for (int rpos = start; rpos < nums.size(); rpos++) {// 交换节点
            if (nums[rpos] == nums[rpos - 1] || nums[rpos] == nums[start]) {
                // 相同点跳过
                continue;
            }
            swap(nums[start], nums[rpos]);
            permuteUnique_0_core(nums, res, start + 1);
            swap(nums[start], nums[rpos]);
        }
    }

    vector<vector<int>> permuteUnique_1(vector<int>& nums) {
        // 非递归，逐步增长，暂时找不到解决方案
        if (nums.size() == 0) {
            return {{}};
        }
        sort(nums.begin(), nums.end());
        vector<vector<int>> res;
        
        return res;
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
        auto res = s.permuteUnique(nums);
        cout << "permutaions: "<< endl;
        for (auto& v : res) {
            for (auto& i : v) {
                cout << i << " ";
            }
            cout << endl;
        }
    };

    nums = {1,1};
    test(nums);

    nums = {1,1,2,3};
    test(nums);

    nums = {1,2,2,3};
    test(nums);

    nums = {1,2,3,3};
    test(nums);

    // nums = {};
    // test(nums);

    // nums = {1};
    // test(nums);

    // nums = {1,2};
    // test(nums);

    // nums = {1, 2, 3};
    // test(nums);

    nums = {1,2,3,4};
    test(nums);
}