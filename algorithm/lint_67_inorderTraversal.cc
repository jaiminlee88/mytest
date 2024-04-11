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
    /**
     * @param root: A Tree
     * @return: Inorder in ArrayList which contains node values.
     * 中序遍历 左中右 lmr
     * 递归法
     * 迭代法
     * morris法
     */
    vector<int> inorderTraversal(TreeNode *root) {
        // write your code here
        // return inorderTraversal_0(root);
        // return inorderTraversal_1(root);
        return inorderTraversal_2(root);
    }

    // morris法,遍历可以将非递归遍历中的空间复杂度降为O(1)。从而实现时间复杂度为O(N)，而空间复杂度为O(1)的精妙算法。
    // morris遍历利用的是树的叶节点左右孩子为空（树的大量空闲指针），实现空间开销的极限缩减。
    // 实质上 建立一种机制，对于没有左子树的节点只到达一次，对于有左子树的节点会到达两次
    // morris遍历的实现原则
    // 记作当前节点为cur。
    // 1 如果cur无左孩子，cur向右移动（cur=cur.right）
    // 2 如果cur有左孩子，找到cur左子树上最右的节点，记为mostright
    //     2.1 如果mostright的right指针指向空，让其指向cur，cur向左移动（cur=cur.left）
    //     2.2 如果mostright的right指针指向cur，让其指向空，cur向右移动（cur=cur.right）
    vector<int> inorderTraversal_2(TreeNode *root) {
        if (root == nullptr) {
            return {};
        }
        TreeNode* cur = root;
        TreeNode* mostright = nullptr;
        vector<int> ret;
        while (true) {
            // find most right
            if (cur == nullptr) { // 有可能碰到右侧节点为空,所以要判断
                break;
            }
            if (cur->left == nullptr && cur->right == nullptr) { // 没有左子树和右子树,到头了,读取根节点,弹出
                ret.push_back(cur->val);
                break;
            }
            if (cur->left == nullptr) { // 如果没有左子树,就从右边开始
                ret.push_back(cur->val); // 根节点,读取,然后向右移动
                cur = cur->right;
                continue;
            }
            mostright = cur->left; // 有左子树,找到左子树的最右节点
            while (mostright != nullptr && mostright->right != nullptr) {
                if (mostright->right == cur) { // 之前已经链接过了,说明已经访问过了
                    break;
                }
                mostright = mostright->right; // 之前没有访问过
            }
            if (mostright != nullptr) {
                if (mostright->right == nullptr) { // 之前没有访问过
                    mostright->right = cur;
                    cur = cur->left;
                } else if (mostright->right == cur) { // 之前已经访问过,
                    mostright->right = nullptr; // 解关联
                    ret.push_back(cur->val); // 读取根节点
                    cur = cur->right; // 右移
                }
            }
        }
        return ret;
    }

    // 递归方式
    vector<int> inorderTraversal_0(TreeNode *root) {
        if (root == nullptr) {
            return {};
        }
        vector<int> ret;
        inorderTraversal_0_core(root, ret);
        return ret;
    }

    void inorderTraversal_0_core(TreeNode *root, vector<int>& ret) {
        if (root == nullptr) {
            return;
        }
        inorderTraversal_0_core(root->left, ret);
        ret.push_back(root->val);
        inorderTraversal_0_core(root->right, ret);
    }

    // // 迭代法, 下面是 中左右 写错了
    // vector<int> inorderTraversal_1(TreeNode *root) {
    //     if (root == nullptr) {
    //         return {};
    //     }
    //     vector<int> ret;
    //     TreeNode* cur = root;
    //     std::stack<TreeNode*> s;
    //     while (true) {
    //         if (cur != nullptr) {
    //             ret.push_back(cur->val);
    //             s.push(cur);
    //             cur = cur->left;
    //             continue;
    //         }
    //         if (s.empty()) {
    //             break;
    //         }
    //         cur = s.top()->right;
    //         s.pop();
    //     }
    //     return ret;
    // }

    // 迭代法, 左中右
    vector<int> inorderTraversal_1(TreeNode *root) {
        if (root == nullptr) {
            return {};
        }
        vector<int> ret;
        TreeNode* cur = root;
        std::stack<TreeNode*> s;
        while (true) {
            if (cur != nullptr) {
                s.push(cur);
                cur = cur->left;
                continue;
            }
            if (s.empty()) {
                break;
            }
            cur = s.top();
            s.pop();
            ret.push_back(cur->val);
            cur = cur->right;
        }
        return ret;
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