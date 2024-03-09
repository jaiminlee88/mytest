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
     * @param a: an integer array
     * @param k: a postive integer <= length(A)
     * @param target: an integer
     * @return: A list of lists of integer
     *          we will sort your return value in output
     * 
     * 给定 n 个不同的正整数，整数 k（1<=k<=n）以及一个目标数字。
        在这 n 个数里面找出 k 个不同的数，使得这 k 个数的和等于目标数字，
        你需要找出所有满足要求的方案（方案顺序不作要求）

        数组 = [1,2,3,4]
        k = 2
        target = 5
        [[1,4],[2,3]]

        数组 = [1,3,4,6]
        k = 3
        target = 8
        [[1,3,4]]

        求方案问题，可以用DFS方式暴力搜索
        回忆n=2时
            也可以用降维法，先确定一个数字，然后在剩下的数字中找k-1个数字
            也可用hashmap
    */
    vector<vector<int>> kSumII(vector<int> &a, int k, int target) {
        // write your code here
        return kSumII_0(a, k, target);
    }
    vector<vector<int>> kSumII_0(vector<int> &a, int k, int target) {
        // 递归方式
        if (a.empty() || k == 0) {
            return {};
        }
        sort(a.begin(), a.end());
        vector<vector<int>> res;
        vector<int> path;
        int         path_value = 0;
        kSumII_0_core(a, res, path, path_value, k, target, 0);
        return res;
    }
    void kSumII_0_core(vector<int> &a,
                        vector<vector<int>> &res,
                        vector<int> &path,
                        int &path_value,
                        int k,
                        int target,
                        int start) {
        if (path.size() == k && path_value == target) {
            res.push_back(path);
            return;
        }
        if (start >= a.size()) {
            return;
        }
        for (int i = start; i < a.size(); i++) {
            if (path_value + a[i] > target) {
                return;
            }
            path.push_back(a[i]);
            path_value += a[i];
            kSumII_0_core(a, res, path, path_value, k, target, i + 1);
            path_value -= a[i];
            path.pop_back();
        }
    }
};


int main() {
    Solution s;
    vector<int> nums;
    auto test = [&s](vector<int>& nums, int k, int target) {
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.kSumII(nums, k, target);
        cout << "permutaions: "<< endl;
        for (auto& v : res) {
            for (auto& i : v) {
                cout << i << " ";
            }
            cout << endl;
        }
    };

    nums = {1,2,3,4};
    test(nums, 2, 5);

    nums = {1,3,4,6};
    test(nums, 3, 8);
    
    nums = {1, 1, 3, 3, 4,6}; // 有重复数字，方案会重复
    test(nums, 3, 7);
}