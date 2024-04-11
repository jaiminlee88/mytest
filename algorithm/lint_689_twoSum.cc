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


/**
 * Definition of TreeNode:
 * class TreeNode {
 * public:
 *     int val;
 *     TreeNode *left, *right;
 *     TreeNode(int val) {
 *         this->val = val;
 *         this->left = this->right = NULL;
 *     }
 * }
 */

class TreeNode {
public:
    int val;
    TreeNode *left, *right;
    TreeNode(int val) {
        this->val = val;
        this->left = this->right = NULL;
    }
};

class Solution {
public:
    /*
     * @param : the root of tree
     * @param : the target sum
     * @return: two number from tree witch sum is n
     * 给一棵二叉搜索树以及一个整数 n, 在树中找到和为 n 的两个数字。无法找到解时返回 null。
     * 
     * 1、可以先转成数组，然后用two sum的方法来做，但空间复杂度是O(n)
     * 2、或者一边用lmr的方法，一边用rml的方法遍历，使用 O(1) 的额外空间，用two sum的方法来试错
     */
    vector<int> twoSum(TreeNode * root, int n) {
        // write your code here
        return twoSum_0(root, n);
    }

    vector<int> twoSum_0(TreeNode * root, int n) {
        if (root == nullptr) {
            return {};
        }
        stack<TreeNode*> lstack;
        stack<TreeNode*> rstack;
        TreeNode* lnode = get_lmr(lstack, root);
        TreeNode* rnode = get_rml(rstack, root);
        while (lnode != nullptr && rnode != nullptr && lnode != rnode && lnode->val < rnode->val) {
            if (lnode->val + rnode->val == n) {
                return {lnode->val, rnode->val};
            }
            if (lnode->val + rnode->val < n) {
                lnode = get_lmr(lstack, nullptr);
                continue;
            }
            if (lnode->val + rnode->val > n) {
                rnode = get_rml(rstack, nullptr);
                continue;
            }
        }
        return {};
    }
    TreeNode* get_lmr(stack<TreeNode*>& s, TreeNode* curnode) {
        TreeNode* res = nullptr;
        while (true) {
            while (curnode != nullptr) {
                s.push(curnode);
                curnode = curnode->left;
            }
            if (res != nullptr) {
                break;
            }
            if (s.empty()) {
                return nullptr;
            }
            res = s.top();
            s.pop();
            if (res->right != nullptr) {
                curnode = res->right;
            }
        }
        return res;
    }
    TreeNode* get_rml(stack<TreeNode*>& s, TreeNode* curnode) {
        TreeNode* res = nullptr;
        while (true) {
            while (curnode != nullptr) {
                s.push(curnode);
                curnode = curnode->right;
            }
            if (res != nullptr) {
                break;
            }
            if (s.empty()) {
                return nullptr;
            }
            res = s.top();
            s.pop();
            if (res->left != nullptr) {
                curnode = res->left;
            }
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