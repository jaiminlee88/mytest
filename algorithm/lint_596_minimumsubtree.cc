#include <iostream>
#include <vector>

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
        this->left = this->right = nullptr;
    }
};

class Solution {
public:
    /**
     * @param root: the root of binary tree
     * @return: the root of the minimum subtree
     * 给一棵二叉树, 找到和为最小的子树, 返回其根节点。输入输出数据范围都在int内
     * 
     * 可以用DFS法，找到叶子节点就能知道子树大小，注意可能有正负数情况，所以不能提前返回
     * 返回的不一定是根节点，而是子树的根节点
     */
    //递归的定义
    TreeNode * findSubtree(TreeNode * root) {
        if (root == nullptr) {
            return nullptr;
        }
        int minSum = INT_MAX;
        TreeNode* minSubtree = nullptr;
        //递归的调用
        findminSum(root, minSubtree, minSum);
        return minSubtree;
    }

    int findminSum(TreeNode * root, TreeNode*& minSubtree, int& minSum) {
        if (root == nullptr) {
            return 0;
        }
        int lvalue = findminSum(root->left, minSubtree, minSum);
        int rvalue = findminSum(root->right, minSubtree, minSum);
        int sum = lvalue + rvalue + root->val;
        if (sum < minSum) {
            minSum = sum;
            minSubtree = root;
        }
        return sum;
    }

};

int main(){

}