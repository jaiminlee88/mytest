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
     * @param candidates: A list of integers
     * @param target: An integer
     * @return: A list of lists of integers
     *          we will sort your return value in output
     *
     * 给定一个候选数字的集合 candidates 和一个目标值 target。
     * 找到 candidates 中所有的和为 target 的组合。
     * 在同一个组合中, candidates 中的某个数字出现次数不限
     *    所有数值 (包括 target ) 都是正整数.
          返回的每一个组合内的数字必须是非降序的.
          返回的所有组合之间可以是任意顺序.
          解集不能包含重复的组合.
     *  输入: candidates = [2, 3, 6, 7], target = 7
        输出: [[7], [2, 2, 3]]

        输入: candidates = [1], target = 3
        输出: [[1, 1, 1]]
     */
    vector<vector<int>> combinationSum(vector<int> &candidates, int target) {
        // write your code here
        return combinationSum_0(candidates, target);
    }

    vector<vector<int>> combinationSum_0(vector<int> &candidates, int target) {
        if (candidates.empty()) {
            return {};
        }
        std::unordered_set<int> set(candidates.begin(), candidates.end());
        candidates.assign(set.begin(), set.end());
        sort(candidates.begin(), candidates.end());
        vector<vector<int>> res;
        vector<int> subset;
        combinationSum_0_core(candidates, res, subset, target, 0);
        return res;
    }
    void combinationSum_0_core(vector<int> &candidates,
                               vector<vector<int>> &res,
                               vector<int> &subset,
                               int target,
                               int start) {
        if (target == 0) {
            res.push_back(subset);
            return;
        }
        if (start >= candidates.size() || target < candidates[start]) {
            return;
        }

        // 下面代码有问题，问题在于，如果当前元素使用次数不限，那么就不需要考虑当前元素的使用次数
        // int max_cnt = target / candidates[start]; // 最多使用次数
        // for (int cnt = 0; cnt <= max_cnt; cnt++) { // 重复使用当前元素
        //     if (cnt == 0) { // 不使用当前元素
        //         combinationSum_0_core(candidates, res, subset, target, start + 1);
        //         continue;
        //     }
        //     for (int j = 1; j <= cnt; j++) { // 使用当前元素
        //         subset.push_back(candidates[start]);
        //         combinationSum_0_core(candidates, res, subset, target - j * candidates[start], start + 1);
        //     }
        //     for (int j = 1; j <= cnt; j++) {
        //         subset.pop_back();
        //     }
        // }
        for(int i = start; i < candidates.size(); i++) {
            if (candidates[i] > target) {
                break;
            }
            subset.push_back(candidates[i]);
            combinationSum_0_core(candidates, res, subset, target - candidates[i], i);
            subset.pop_back();
        }
    }
};


int main() {
    Solution s;
    vector<int> nums;
    auto test = [&s](vector<int>& nums, int target) {
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << "target: " << target << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.combinationSum(nums, target);
        cout << "ret: "<< endl;
        for (auto& v : res) {
            for (auto& i : v) {
                cout << i << " ";
            }
            cout << endl;
        }
    };

    nums = {2, 3, 6, 7};
    test(nums, 7);

    nums = {2, 3};
    test(nums, 7);

    nums = {1};
    test(nums, 3);
}