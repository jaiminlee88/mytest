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
     * Description
        Given an sorted array of integers, find two numbers that their difference equals to a target value.
        Return a list with two number like [num1, num2] that the difference of num1 and num2 equals to target value, and num1 is less than num2.

        It’s guaranteed there is only one available solution. 注意不要重复输出
        Note: Requires O(1) space complexity to comple

        Example
        Example 1:

        Input: nums = [2, 7, 15, 24], target = 5
        Output: [2, 7]
        Explanation:
        (7 - 2 = 5)
        Example 2:

        Input: nums = [1, 1], target = 0
        Output: [1, 1]
        Explanation:
        (1 - 1 = 0)

        暴力解法，找两两一对
    */
    vector<vector<int>> twoSum7(vector<int>& nums, int target) {
        // return twoSum7_0(nums, target);
        // return twoSum7_1(nums, target);
        return twoSum7_2(nums, target);
    }
    vector<vector<int>> twoSum7_0(vector<int>& nums, int target) {
        if (nums.size() < 2) {
            return {};
        }
        // 双指针  同向指针
        if (target < 0) {
            target = -target;
        }
        vector<vector<int>> res;
        for (int lpos = 0; lpos < nums.size(); ++lpos) {
            while (lpos != 0 && lpos < nums.size() && nums[lpos] == nums[lpos - 1]) {
                ++lpos;
            }
            if (lpos >= nums.size()) {
                break;
            }
            for (int rpos = lpos + 1; rpos < nums.size(); ++rpos) {
                while (rpos + 1 < nums.size() && nums[rpos] == nums[rpos + 1]) {
                    ++rpos;
                }
                if (nums[rpos] - nums[lpos] == target) {
                    res.push_back({nums[lpos], nums[rpos]});
                }
            }
        }
        return res;
    }
    vector<vector<int>> twoSum7_1(vector<int>& nums, int target) {
        // 用哈希表，记录所有的数
        // 遍历所有的数x， target - x 是否在哈希表中, 如果在就加入表
        // 二分法可以代替哈希表，固定一个值，用二分法从有序数组中找出另一个值，
        if (nums.size() < 2) {
            return {};
        }
        target = target > 0 ? target : -target;
        unordered_map<int, int> h;
        for (int i = 0; i < nums.size(); ++i) {
            auto iter = h.find(nums[i]);
            if (iter != h.end()) {
                iter->second++;
                continue;
            }
            h[nums[i]] = 1;
        }
        vector<vector<int>> res;
        unordered_set<int> visited;
        for (int i = 0; i < nums.size(); ++i) {
            if (visited.find(nums[i]) != visited.end()) {
                continue;
            }
            int tmp = target - nums[i];
            auto iter = h.find(tmp);
            if (iter != h.end()) {
                if (tmp < nums[i]) {
                    res.push_back({tmp, nums[i]});
                } else {
                    res.push_back({nums[i], tmp});
                }
                visited.insert(tmp);
                visited.insert(nums[i]);
            }
            tmp = -1 * tmp;
            iter = h.find(tmp);
            if (iter != h.end()) {
                if (tmp < nums[i]) {
                    res.push_back({tmp, nums[i]});
                } else {
                    res.push_back({nums[i], tmp});
                }
                visited.insert(tmp);
                visited.insert(nums[i]);
            }
        }
        return res;
    }

    vector<vector<int>> twoSum7_2(vector<int>& nums, int target) {
        // 二分法可以代替哈希表，固定一个值，用二分法从有序数组中找出另一个值
        if (nums.size() < 2) {
            return {};
        }
        target = target > 0 ? target : -target;
        vector<vector<int>> res;
        unordered_set<int> visited;
        for (int i = nums.size() - 1; i >= 0; --i) {
            int tmp = nums[i] - target;
            if (visited.find(nums[i]) != visited.end() && visited.find(tmp) != visited.end()) {
                continue;
            }
            // 二分法查找
            int lpos = 0;
            int rpos = nums.size() - 1;
            while (lpos + 1 < rpos) { // 二分法
                int mpos = lpos + (rpos - lpos) / 2;
                if (tmp >= nums[mpos]) {
                    lpos = mpos;
                    continue;
                }
                if (tmp < nums[mpos]) {
                    rpos = mpos;
                }
            }
            // cout << "nums[i]=" << nums[i] << " tmp=" << tmp << " lpos: " << lpos << " rpos: " << rpos << " nums[lpos]=" << nums[lpos] << " nums[rpos]=" << nums[rpos] << endl;
            if (nums[lpos] == tmp) {
                if (tmp < nums[i]) {
                    res.push_back({tmp, nums[i]});
                } else {
                    res.push_back({nums[i], tmp});
                }
                visited.insert(tmp);
                visited.insert(nums[i]);
            } else if (nums[rpos] == tmp) {
                if (tmp < nums[i]) {
                    res.push_back({tmp, nums[i]});
                } else {
                    res.push_back({nums[i], tmp});
                }
                visited.insert(tmp);
                visited.insert(nums[i]);
            }
        }
        return res;
    }
};


int main() {
    Solution s;
    vector<int> nums;
    int k;
    auto test = [&s](vector<int>& nums, int target) {
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << " target : " << target << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.twoSum7(nums, target);
        cout << "permutaions: "<< endl;
        for (auto& i : res) {
            cout << "[" << i[0] << "," << i[1] << "] ";
            cout << endl;
        }
    };

    nums = {2, 7, 15, 24};
    k = 5;
    test(nums, k);

    nums = {2, 7, 7, 12, 15, 24};
    k = 5;
    test(nums, k);

    nums = {2, 7, 7, 12, 12, 15, 24};
    k = 5;
    test(nums, k);

    nums = {1, 1};
    k = 0;
    test(nums, k);

    nums = {1, 1, 1};
    k = 0;
    test(nums, k);

    nums = {1, 1, 1, 1};
    k = 0;
    test(nums, k);

    nums = {-7, -2, -2, 0, 3};
    k = 5;
    test(nums, k);

    nums = {2, 7, 7, 12, 12, 15, 24};
    k = -5;
    test(nums, k);
    return 0;
}