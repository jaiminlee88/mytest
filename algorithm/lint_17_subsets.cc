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
#include <bitset>
#include <deque>
using namespace std;


class Solution {
public:
    public:
    /**
     * @param nums: A set of numbers
     * @return: A list of lists
     *          we will sort your return value in output
     * 给定一个含不同整数的集合，返回其所有的子集（任意顺序）。
     * 
     */
    vector<vector<int>> subsets(vector<int> &nums) {
        // return subsets_0(nums);
        // return subsets_1(nums);
        return subsets_2(nums);
    }

    vector<vector<int>> subsets_2(vector<int> &nums) {
        /*
        *
        []
        [1]             [2]     [3]
        [1, 2] [1, 3]   [2, 3]
        [1, 2, 3]
        all subsets
        */
        if (nums.size() < 1) {
            return {{}};
        }
        sort(nums.begin(), nums.end());
        vector<vector<int>> res;
        res.push_back({});
        int index = 0;
        while (index < res.size()) {
            auto subset = res[index++];
            for (int i = 0; i < nums.size(); ++i) {
                if (subset.size() != 0 && nums[i] <= subset.back()) {
                    continue;
                }
                auto tmp  = subset;
                tmp.push_back(nums[i]);
                res.push_back(tmp);
            }
        }
        return res;
    }
    vector<vector<int>> subsets_1(vector<int> &nums) {
        /*
        *
        []
        [] [1] -> 1
        [] [1] [2] [1, 2] -> 2
        [] [1] [2] [1, 2] [3] [1, 3] [2, 3] [1, 2, 3] -> 3
        */
        if (nums.size() < 1) {
            return {{}};
        }
        sort(nums.begin(), nums.end());
        vector<vector<int>> res;
        queue<vector<int>> q;
        q.push({});
        int size = 0;
        for (int i = 0; i < nums.size(); ++i) {
            size = q.size();
            for (int j = 0;  j < size; ++j) {
                auto item = q.front();
                q.pop();
                q.push(item);
                item.push_back(nums[i]);
                q.push(item);
            }
        }
        while (!q.empty()) {
            res.push_back(q.front());
            q.pop();
        }
        return res;
    }

    vector<vector<int>> subsets_0(vector<int> &nums) {
        if (nums.size() < 1) {
            return {{}};
        }
        sort(nums.begin(), nums.end());
        vector<vector<int>> res;
        vector<int> subset;
        subsets_0_core(nums, 0, res, subset);
        return res;
    }

    void subsets_0_core(vector<int> &nums, int start, vector<vector<int>> &res, vector<int> &subset) {
        if (start >= nums.size()) {
            vector<int> tmp(subset);
            // sort(tmp.begin(), tmp.end());
            res.push_back(subset);
            return;
        }
        subset.push_back(nums[start]);
        subsets_0_core(nums, start + 1, res, subset);
        subset.pop_back();
        subsets_0_core(nums, start + 1, res, subset);
    }
};


int main() {
    Solution s;
    vector<int> nums;
    vector<int> nums0;
    vector<int> nums1;
    vector<vector<int>> nums3;
    vector<vector<char>> nums4;
    vector<string> nums5;
    string str;
    string str0;
    string str1;
    int k = 0;
    auto test = [&s](vector<int>& nums) {
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.subsets(nums);
        cout << "permutaions: " << endl;
        int cnt = 0;
        for (auto& v : res) {
            cout << ++cnt << " : ";
            for (auto& i : v) {
                cout << i << " ";
            }
            cout << endl;
        }
    };

    nums = {};
    test(nums);

    nums = {1};
    test(nums);

    nums = {1,2};
    test(nums);

    nums = {1, 2, 3};
    test(nums);

    nums = {4,1,0};
    test(nums);

    nums = {1,2,3,4};
    test(nums);
}