#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;

class Solution {
public:
    /**
     * @param nums: A set of numbers.
     * @return: A list of lists. All valid subsets.
     *          we will sort your return value in output
     *
     * 给定一个可能具有重复数字的列表，返回其所有可能的子集。
     * 
     * 搜子集合
     * 
     * 
     * 子集中的每个元素都是非降序的
        两个子集间的顺序是无关紧要的
        解集中不能包含重复子集
     */
    vector<vector<int>> subsetsWithDup(vector<int> &nums) {
        // write your code here
        if (nums.empty()) {
            return {{}};
        }
        sort(nums.begin(), nums.end());
        vector<vector<int>> result;
        vector<int> subset;
        // subsetsWithDup_DFS(nums, 0, subset, result);
        subsetsWithDup_DFS_1(nums, 0, subset, result);
        // subsetsWithDup_BFS(nums, result);
        return result;
    }
    void subsetsWithDup_DFS(vector<int> &nums, int start, vector<int> &subset, vector<vector<int>> &result) {
        if (start >= nums.size()) {
            result.push_back(subset);
            return;
        }
        int lpos = start;
        int end = start;
        while (end < nums.size() && nums[end] == nums[lpos]) {
            end++;
        }
        int rpos = lpos - 1;
        while (rpos < end) {
            if (rpos < lpos) {
                subsetsWithDup_DFS(nums, end, subset, result);
            } else {
                subset.insert(subset.end(), nums.begin() + lpos, nums.begin() + rpos + 1);
                subsetsWithDup_DFS(nums, end, subset, result);
                subset.erase(subset.end() - (rpos - lpos + 1), subset.end());
            }
            ++rpos;
        }
    }
    void subsetsWithDup_DFS_1(vector<int> &nums, int start, vector<int> &subset, vector<vector<int>> &result) {
        // 只能处理不重复的情况
        if (start > nums.size()) {
            return;
        }
        result.push_back(subset);
        for (int pos = start; pos < nums.size(); ++pos) {
            subset.push_back(nums[pos]);
            subsetsWithDup_DFS_1(nums, pos + 1, subset, result);
            subset.pop_back();
        }
    }

    void subsetsWithDup_BFS(vector<int> &nums, vector<vector<int>> &result) {
        // 从空集合开始，依次遍历即可，
        // 只能处理不重复的情况
        std::queue<std::vector<int>> q;
        q.push({});
        for (auto& it : nums) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                if (q.empty()) {
                    break;
                }
                vector<int> subset = q.front();
                q.pop();
                q.push(subset);
                subset.push_back(it);
                q.push(subset);
            }
        }
        while (!q.empty()) {
            result.push_back(q.front());
            q.pop();
        }
    }
};


int main() {
    Solution s;
    vector<int> nums;

    // nums = {1, 2, 0};
    // vector<int> subset{4,5,6,7,8,9};
    // nums.insert(nums.end(), subset.begin(), subset.end());
    // for (auto& it : nums) {
    //     cout << it << " ";
    // }
    // cout << endl;
    // nums.erase(nums.begin() + 3, nums.begin() + 4);
    // for (auto& it : nums) {
    //     cout << it << " ";
    // }
    // cout << endl;
    // return 0;
    auto test = [&s](vector<int>& nums){
        vector<vector<int>> result = s.subsetsWithDup(nums);
        cout << "nums: ";
        for (auto& it : nums) {
            cout << it << " ";
        }
        cout << " result(" << result.size() << "):" << endl;
        for (auto& it : result) {
            cout << "[";
            for (auto& itt : it) {
                cout << itt << " ";
            }
            cout << "]" << endl;
        }
        return 0;
    };

    nums = {};
    test(nums);

    nums = {1, 2, 2};
    test(nums);

    nums = {1, 2, 3};
    test(nums);

    nums = {1, 2, 2, 3};
    test(nums);
}