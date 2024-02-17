#include <iostream>
#include <queue>
#include <vector>
#include <memory>
#include "avltree.hh"
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
class TreeNode;
class Solution {
public:
    /**
     * @param root: A Tree
     * @return: Level order a list of lists of integer
     * 给出一棵二叉树，返回其节点值的层次遍历（逐层从左往右访问）
     * 单队列法
     * 双队列法
     * dummy node法
     */
    vector<vector<int>> levelOrder(TreeNode *root) {
        // 单队列法，记住每次读取的次数
        if (root == nullptr) {
            return {};
        }
        vector<vector<int>> ret;
        vector<int> tmp;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            tmp.clear();
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                TreeNode* node = q.front();
                tmp.push_back(node->val);
                q.pop();
                if (node->left != nullptr) {
                    q.push(node->left);
                }
                if (node->right != nullptr) {
                    q.push(node->right);
                }
            }
            ret.push_back(tmp);
        }
        return ret;
    }
    vector<vector<int>> levelOrder(TreeNode *root) {
        // 双队列法，一个队列暂存下一个层次的节点
        if (root == nullptr) {
            return {};
        }
        vector<vector<int>> ret;
        vector<int> tmp;
        queue<TreeNode*> q1;
        queue<TreeNode*> q2;
        q1.push(root);
        bool use_q1 = true;
        queue<TreeNode*>* pque1 = nullptr;
        queue<TreeNode*>* pque2 = nullptr;
        while (q1.size() || q2.size()) {
            if (use_q1) {
                pque1 = &q1;
                pque2 = &q2;
            } else {
                pque1 = &q2;
                pque2 = &q1;
            }
            use_q1 = !use_q1;
            int size = pque1->size();
            tmp.clear();
            for (int i = 0; i < size; ++i) {
                TreeNode* node = pque1->front();
                pque1->pop();
                tmp.push_back(node->val);
                if (node->left != nullptr) {
                    pque2->push(node->left);
                }
                if (node->right != nullptr) {
                    pque2->push(node->right);
                }
            }
            ret.push_back(tmp);
        }
        return ret;
    }
    vector<vector<int>> levelOrder(TreeNode *root) {
        // dummy node法
        // write your code here
        if (root == nullptr) {
            return {};
        }
        queue<TreeNode*> q;
        vector<vector<int>> ret;
        vector<int> tmp;
        q.push(root);
        q.push(nullptr);
        while(!q.empty()) {
            TreeNode* node = q.front();
            q.pop();
            if (node != nullptr) {
                if (node->left != nullptr) {
                    q.push(node->left);
                }
                if (node->right != nullptr) {
                    q.push(node->right);
                }
                tmp.push_back(node->val);
                continue;
            }
            ret.push_back(tmp);
            tmp.clear();
            if (q.empty()) { // 最后一个nullptr
                break;
            }
            q.push(nullptr);
        }
        return ret;
    }
};


int main() {
    auto push_data = [](AVLTree<int>& tree, vector<int>& v) {
        if (v.size() == 0) {
            cout << "===push---> empty" << endl;
            return;
        }
        cout << "===push---: ";
        for (auto& i : v) {
            cout << i << " ";
        }
        cout << endl;
        for (auto i : v) {
            tree.insert(i);
        }
    };
    auto print_data = [](AVLTree<int>& tree) {
        std::vector<std::pair<int, int>> vec;
        tree.get_nodes_dfs_lmr(vec, tree.root);
        cout << "inorder: ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        int height = tree.root == nullptr ? 0 : tree.root->height;
        cout << " depth:" << height << endl;
    };
    auto print_BFS = [](AVLTree<int>& tree) {
        cout << "BFS: ";
        auto ret = tree.get_BFS();
        for (auto& i : ret) {
            cout << i.first << "[" << i.second << "] ";
        }
        cout << endl;
    };

    Solution s;
    AVLTree<int> tree;
    vector<int> vec;

    tree.clear();
    vec = {1,2,3,4,5,6,7};
    push_data(tree, vec);
    print_data(tree);
    print_BFS(tree);


}