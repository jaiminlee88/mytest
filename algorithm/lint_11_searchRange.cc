#include <iostream>
#include <climits>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
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
     * @param root: param root: The root of the binary search tree
     * @param k1: An integer
     * @param k2: An integer
     * @return: return: Return all keys that k1<=key<=k2 in ascending order
     * 
     * 给定一个二叉查找树和范围[k1, k2]。按照升序返回给定范围内的节点值。
     * 
     * 中序遍历
     */
    vector<int> searchRange(TreeNode *root, int k1, int k2) {
        // write your code here
        if (root == nullptr) {
            return {};
        }
        vector<int> ret;
        TreeNode* curnode = root;
        stack<TreeNode*> s;
        while(true) {
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
            if (curnode->val >= k1 && curnode->val <= k2) {
                ret.push_back(curnode->val);
            }
            curnode = curnode->right;
        }
        return ret;
    }
};

int main() {
    Solution s;

    auto test = [&s](vector<int> vec, double k1, int k2){
        TreeNode * root = constructGraph(vec);
        cout << "orig: [";
        for (auto& it : vec) {
            cout << it << " ";
        }
        vector<int> ret = s.searchRange(root, k1, k2);
        cout << "] range=[" << k1 << "," << k2 << "] ret: ";
        for (auto& it : ret) {
            cout << it << " ";
        }
        cout << endl;
    };

    test(vector<int>({10,5,15,3,6,12,18,-1,4,-1,8}), 10, 15);
    test(vector<int>({1}), 2, 5);
    test(vector<int>({2,1}), 0, 4);
    return 0;
}