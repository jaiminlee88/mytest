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
using namespace std;


class Solution {
public:
    /**
     * @param nums: a list of integers.
     * @param k: length of window.
     * @return: the sum of the element inside the window at each moving.
     * 
     * Given an array of n integers, and a moving window(size k), 
     * move the window at each iteration from the start of the array, 
     * find the sum of the element inside the window at each moving.

        Example
        Example 1

        Input：array = [1,2,7,8,5], k = 3
        Output：[10,17,20]
        Explanation：
        1 + 2 + 7 = 10
        2 + 7 + 8 = 17
        7 + 8 + 5 = 20
     */
    vector<int> winSum(vector<int> &nums, int k) {
        // 双指针法
        if (nums.size() == 0 || k == 0) {
            return {};
        }
        int lpos = 0;
        int rpos = 0;
        int sum = 0;
        int tmp_k = k;
        vector<int> res;
        while (tmp_k > 0 && rpos < nums.size()) {
            sum += nums[rpos];
            ++rpos;
            --tmp_k;
        } // 先得到一个窗口
        res.push_back(sum);
        if (rpos >= nums.size()) {
            return res;
        }
        rpos--;
        for (int lpos = 0; lpos < nums.size(); ++lpos) {
            ++rpos;
            if (rpos >= nums.size()) {
                break;
            }
            sum = sum - nums[lpos] + nums[rpos];
            res.push_back(sum);
        }
        return res;
    }
};

int main() {
    Solution s;
    vector<int> nums;
    vector<int> nums0;
    vector<int> nums1;
    vector<vector<int>> nums3;
    string str;
    string str0;
    string str1;
    int k = 0;
    auto test = [&s](vector<int>& nums, int k) {
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.winSum(nums, k);
        cout << "nums: ";
        for (auto i : nums) {
            cout << i << " ";
        }
        cout << " k: " << k << " res: ";
        for (auto i : res) {
            cout << i << " ";
        }
        cout << endl;
    };

    nums = {};
    k = 3;
    test(nums, k);

    nums = {1,2};
    k = 3;
    test(nums, k);

    nums = {1,2,7};
    k = 3;
    test(nums, k);

    nums = {1,2,7,8,5};
    k = 3;
    test(nums, k);
}