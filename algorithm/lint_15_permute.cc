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
     * @param nums: A list of integers.
     * @return: A list of permutations.
     *          we will sort your return value in output
     * 
     * 全排列
     * 1. 递归法，
     *      可以用交换法，不断交换元素位置，空间复杂度O(n)，时间复杂度O(n!)
     *      也可以用深度优先搜索，用递归实现，记录每次已经访问过的节点
     * 2. 非递归法 https://blog.csdn.net/weixin_43362002/article/details/104098802
     */
    vector<vector<int>> permute(vector<int> &nums) {
        // write your code here
        // return permute_0(nums);
        return permute_1(nums);

    }

    vector<vector<int>> permute_0(vector<int> &_nums) {
        // 递归法，不断交换元素位置，空间复杂度O(n)，时间复杂度O(n!)
        if (_nums.empty()) {
            return vector<vector<int>>();
        }
        vector<int> nums = _nums;
        vector<vector<int>> res;
        // 不同层次替换
        premute_0_core(nums, 0, res);
        return res;
    }
    void premute_0_core(vector<int> &nums, int start, vector<vector<int>>& res) {
        if (start >= nums.size()) {
            res.push_back(nums);
            return;
        }
        for (int i = start; i < nums.size(); i++) {
            // 尝试交换，确定当前位置
            std::swap(nums[start], nums[i]); // 确定当前位置
            premute_0_core(nums, start + 1, res); // 递归下一层
            std::swap(nums[start], nums[i]);
        }
    }

    vector<vector<int>> permute_1(vector<int> &nums) {
        // 非递归法
        // 逐步生成方法，插位置法
        // abc, [[a]] , [[b, a], [a, b]], [[c, b, a], [b, c, a], [b, a, c], [c, a, b], [a, c, b], [a, b, c]]
        // 找位置插空，尝试在每个位置上插入新值，直至生成想要的长度
        if (nums.empty()) {
            return vector<vector<int>>({vector<int>()});
        }
        if (nums.size() == 1) {
            return vector<vector<int>>({vector<int>({nums[0]})});
        }
        vector<vector<int>> res;
        std::queue<vector<int>> q;
        q.push(vector<int>({nums[0]}));
        for (int i = 1; i < nums.size(); ++i) {
            int new_num = nums[i];
            int size = q.size();
            for (int j = 0; j < size; ++j) {
                vector<int> v = q.front();
                q.pop();
                // 尝试在每个位置上插入新值
                for(int delta = 0; delta <= v.size(); delta++) {
                    v.insert(v.begin() + delta, new_num);
                    q.push(v);
                    v.erase(v.begin() + delta);
                }
            }
        }
        while (q.empty() == false) {
            res.push_back(q.front());
            q.pop();
        }
        return res;
    }

    vector<vector<int>> permute_2(vector<int> &nums) {
        // 非递归法
        // 逐步生成方法，依次添加后缀元素，广度优先搜索
        // abc, [a,b]用unordered_set存储，if a in [a,b]
        // [[a],[b],[c]]
        // [[b],[c],[a,b],[a,c]]
        // [[c],[a,b],[a,c],[b,a],[b,c]]
        // [[a,b],[a,c],[b,a],[b,c],[c,a],[c,b]]，依次类推
        return {{}};
    }

    vector<vector<int>> permute_3(vector<int> &nums) {
        // 递归法
        // 也可以深度优先搜索，用递归实现，记录每次已经访问过的节点
        if (nums.size() == 0) {
            return {{}};
        }
        vector<vector<int>> res;
        vector<int> subset;
        vector<bool> visited(nums.size(), false);
        permute_2_core(nums, res, subset, visited);
        return res;
    }
    void permute_2_core(vector<int>& nums, vector<vector<int>>& res, vector<int>& subset, vector<bool>& visited) {
        if (subset.size() == nums.size()) {
            res.push_back(subset);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (visited[i] == true) {
                continue;
            }
            visited[i] = true;
            subset.push_back(nums[i]);
            permute_2_core(nums, res, subset, visited);
            subset.pop_back();
            visited[i] = false;
        }
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
        auto res = s.permute_3(nums);
        cout << "permutaions: "<< endl;
        for (auto& v : res) {
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

    nums = {1,2,3,4};
    test(nums);
}