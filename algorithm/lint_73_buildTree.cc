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
     * @param preorder: A list of integers that preorder traversal of a tree
     * @param inorder: A list of integers that inorder traversal of a tree
     * @return: Root of a tree
     * 根据前序遍历和中序遍历树构造二叉树.
     * 
     * mlr   mid [...left...][...right...]
     * lmr   [...left...] mid [...right...]
     * lrm   [...left...][...right...] mid
     * 非常相似,找到mid,然后分为左右两边递归即可
     */
    TreeNode* buildTree(vector<int> &preorder, vector<int> &inorder) {
        // write your code here
        return buildTree_0(preorder, inorder);
    }
    TreeNode* buildTree_0(vector<int> &preorder, vector<int> &inorder) {
        if (preorder.size() != inorder.size() || preorder.size() < 1 || inorder.size() < 1) {
            return {};
        }
        return buildTree_0_core(preorder, 0, preorder.size(), inorder, 0, inorder.size());
    }

    TreeNode* buildTree_0_core(vector<int> &preorder, int pstart, int pend, vector<int> &inorder, int istart, int iend) {
        if (pstart >= pend || istart >= iend) {
            return nullptr;
        }
        TreeNode* root = new TreeNode(preorder[pstart]);
        int mpos = istart;
        while (mpos < iend && inorder[mpos] != preorder[pstart]) {
            mpos++;
        }
        int len =  mpos - istart;
        root->left = buildTree_0_core(preorder, pstart + 1, pstart + len + 1, inorder, istart, mpos);
        root->right = buildTree_0_core(preorder, pstart + len + 1, pend, inorder, mpos + 1, iend);
        return root;
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