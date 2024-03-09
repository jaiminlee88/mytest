#include "avltree.hh"
#include "binarytree.hh"
#include <iostream>
#include <vector>
#include <unordered_map>
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

class Solution {
public:
    /**
     * @param root: The root of binary tree.
     * @return: True if this Binary tree is Balanced, or false.
     */
    
    bool isBalanced(TreeNode *root) {
        // write your code here
        if (root == nullptr) {
            return true;
        }
        unordered_map<TreeNode *, int> node_depth;
        return isBalanced(root->left, node_depth)
            && isBalanced(root->right, node_depth)
            && (abs(node_depth[root->left] - node_depth[root->right]) <= 1);
    }

    bool isBalanced(TreeNode* _root, unordered_map<TreeNode*,int>& node_depth) {
        // left right middle
        if (_root == nullptr) {
            return true;
        }
        if (_root->left == nullptr && _root->right == nullptr) {
            node_depth[_root] = 1;
            return true;
        }
        bool l_balanced = isBalanced(_root->left, node_depth);
        bool r_balanced = isBalanced(_root->right, node_depth);
        int left_depth = node_depth[_root->left];
        int right_depth = node_depth[_root->right];
        node_depth[_root] = max(left_depth, right_depth) + 1;
        return l_balanced && r_balanced && (abs(left_depth - right_depth) <= 1);
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
        int height = tree.root == nullptr ? 0 : tree.root->height;
        cout << "DFS_depth:" << height << endl;

        vec.clear();
        tree.get_nodes_dfs_lmr(vec, tree.root);
        cout << "DFS_lmr           : ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;

        vec.clear();
        tree.get_nodes_dfs_lmr_traverse(vec, tree.root);
        cout << "DFS_lmr_traverse  : ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;

        vec.clear();
        tree.get_nodes_dfs_lrm(vec, tree.root);
        cout << "DFS_lrm           : ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;

        vec.clear();
        tree.get_nodes_dfs_lrm_traverse(vec, tree.root);
        cout << "DFS_lrm_traverse  : ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;

        vec.clear();
        tree.get_nodes_dfs_lrm_traverse_1(vec, tree.root);
        cout << "DFS_lrm_traverse_1: ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;

        vec.clear();
        tree.get_nodes_dfs_mlr(vec, tree.root);
        cout << "DFS_mlr           : ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;

        vec.clear();
        tree.get_nodes_dfs_mlr_traverse(vec, tree.root);
        cout << "DFS_mlr_traverse  : ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;
    };

    auto print_BFS = [](AVLTree<int>& tree) {
        cout << "BFS: ";
        // auto ret = tree.get_BFS();
        // for (auto& i : ret) {
        //     cout << i.first << "[" << i.second << "] ";
        // }
        // cout << endl;
        auto ret = tree.get_BFS_1();
        cout << "[";
        for (auto& sub_vec : ret) {
            cout << "[";
            for (auto& i : sub_vec) {
                cout << "("<< i.first << "," << i.second << ") ";
            }
            cout << "] ";
        }
        cout << "]" << endl;
    };

    auto print_path = [](AVLTree<int>& tree) {
        vector<std::string> ret = tree.get_allpath();
        cout << "path: ";
        for (auto& i : ret) {
            cout << i << " ";
        }
        cout << endl;
    };

    auto print_all_subpath = [](AVLTree<int>& tree){
        auto ret = tree.get_all_subpath();
        cout << "subpath: ";
        for (auto& i : ret) {
            cout << i << " ";
        }
        cout << endl;
    };

    AVLTree<int> tree;
    vector<int> vec;

    tree.clear();
    vec = {1,2,3,4,5,6};
    push_data(tree, vec);
    print_data(tree);
    print_BFS(tree);
    print_path(tree);
    print_all_subpath(tree);
}
