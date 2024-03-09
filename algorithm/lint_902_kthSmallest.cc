#include <iostream>
#include <stack>
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
     * @param root: the given BST
     * @param k: the given k
     * @return: the kth smallest element in BST
     * 给一棵二叉搜索树，写一个 KthSmallest 函数来找到其中第 K 小的元素。
     *
     * 中序遍历查找第k小的元素
     */
    int kthSmallest(TreeNode *root, int k) {
        // write your code here
        kthSmallest_0(root, k);
    }
    int kthSmallest_0(TreeNode *root, int k) {
        if (root == nullptr || k < 1) {
            return -1;
        }

        TreeNode * curnode = root;
        std::stack<TreeNode *> s;
        while (true) {
            if (curnode != nullptr) {
                s.push(curnode);
                curnode = curnode->left;
                continue;
            }
            if (s.empty()) {
                break;
            }
            curnode = s.top();
            s.pop();
            --k;
            if (k == 0) {
                return curnode->val;
            }
            curnode = curnode->right;
        }
        return -1;
    }
};

int main() {

}