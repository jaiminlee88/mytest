#include <vector>
#include <iostream>
#include <vector>

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
        this->left = this->right = nullptr;
    }
};



void connect(TreeNode* parent, TreeNode* lchild, TreeNode* rchild) {
    if (lchild != nullptr) {
        parent->left = lchild;
    }
    if (rchild != nullptr) {
        parent->right = rchild;
    }
}

TreeNode* constructGraph(vector<int> nodes) {
    if (nodes.empty()) {
        return nullptr;
    }
    std::vector<TreeNode*> graph;
    for (auto& it : nodes) {
        graph.push_back(new TreeNode(it));
    }
    for (int i = 0; i < nodes.size(); ++i) {
        if (nodes[i] == -1) {
            continue;
        }
        int lchild = 2 * i + 1;
        int rchild = 2 * i + 2;
        if (lchild < nodes.size() && nodes[lchild] != -1) {
            connect(graph[i], graph[lchild], nullptr);
        }
        if (rchild < nodes.size() && nodes[rchild] != -1) {
            connect(graph[i], nullptr, graph[rchild]);
        }
    }
    return graph[0];
}

TreeNode* find_node(TreeNode* root, int val) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->val == val) {
        return root;
    }
    auto left = find_node(root->left, val);
    if (left != nullptr) {
        return left;
    }
    auto right = find_node(root->right, val);
    if (right != nullptr) {
        return right;
    }
    return nullptr;
}

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
     * @param root: a TreeNode, the root of the binary tree
     * @return: nothing
     * 将一棵二叉树按照前序遍历拆解成为一个 假链表。所谓的假链表是说，用二叉树的 right 指针，来表示链表中的 next 指针。
     * 前序遍历，mlr
     */
    void flatten(TreeNode *root) {
        // write your code here
        flatten_core(root);
    }

    TreeNode * flatten_core(TreeNode *root) {
        if (root == nullptr) {
            return nullptr;
        }

        if (root->left == nullptr && root->right == nullptr) {
            return root;
        }

        TreeNode * tmp_left = root->left;
        TreeNode * tmp_right = root->right;
        TreeNode * left_last = flatten_core(root->left);
        TreeNode * right_last = flatten_core(root->right);
        if (tmp_left != nullptr) {
            root->right = tmp_left;
            left_last->right = tmp_right;
        }
        root->left = nullptr;
        return right_last == nullptr ? left_last : right_last; // 要返回最后一个节点
    }
};

int main() {
    Solution s;
    TreeNode* root;
    TreeNode* _A;
    TreeNode* _B;

    auto test = [&s](vector<int> vec){
        TreeNode * root = constructGraph(vec);
        cout << "orig: ";
        for (auto& it : vec) {
            cout << it << " ";
        }
        s.flatten(root);
        cout << "flat: ";
        while (root != nullptr) {
            cout << root->val << " ";
            root = root->left;
        }
        cout << endl;
    };

    test(vector<int>({1,2,5,3,4,-1,6}));
    test(vector<int>({1,2,3}));
    test(vector<int>({1,-1,3}));
    test(vector<int>({1,2}));
    test(vector<int>({1}));
    test(vector<int>({}));


    return 0;

}