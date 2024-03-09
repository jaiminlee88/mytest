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
    /*
     * @param root: The root of the binary search tree.
     * @param node: insert this node into the binary search tree
     * @return: The root of the new binary search tree.
     * 
     * 给定一棵二叉查找树和一个新的树节点，将节点插入到树中。
       你需要保证该树仍然是一棵二叉查找树。
     */
    TreeNode * insertNode(TreeNode * root, TreeNode * node) {
        // write your code here
        if (root == nullptr) {
            return node;
        }
        TreeNode * curnode = root;
        while (curnode) {
            if (node->val < curnode->val) {
                if (curnode->left == nullptr) {
                    curnode->left = node;
                    break;
                }
                curnode = curnode->left;
                continue;
            }
            if (node->val >= curnode->val) {
                if (curnode->right == nullptr) {
                    curnode->right = node;
                    break;
                } else {
                    curnode = curnode->right;
                }
            }
        }
        return root;
    }
};

void BFS_print(TreeNode * root) {
    // BFS print
    cout << "BFS_print: ";
    if (root == nullptr) {
        cout << "empty tree" << endl;
    }
    queue<TreeNode*> q;
    TreeNode* curnode;
    bool found_new = true;
    q.push(root);
    while (!q.empty()) {
        int size = q.size();
        if (!found_new) {
            break;
        }
        found_new = false;
        for (int i = 0; i < size; i++) {
            curnode = q.front();
            q.pop();
            cout << curnode->val << " ";
            if (curnode->left != nullptr) {
                q.push(curnode->left);
                found_new = true;
            } else {
                q.push(new TreeNode(-1));
            }
            if (curnode->right != nullptr) {
                q.push(curnode->right);
                found_new = true;
            } else {
                q.push(new TreeNode(-1));
            }
        }
    }
    cout << endl;
}
int main() {
    Solution s;

    auto test = [&s](vector<int> vec, int val){
        TreeNode * root = constructGraph(vec);
        cout << "orig: [";
        for (auto& it : vec) {
            cout << it << " ";
        }
        cout << "], insert: " << val << " ";
        TreeNode * node = new TreeNode(val);
        TreeNode * ret = s.insertNode(root, node);
        BFS_print(ret);
    };

    test(vector<int>({1}), 5);
    test(vector<int>({2,1}), 0);
    test(vector<int>({2,1,4,-1,-1,3}), 6);
    test(vector<int>({10,5,15,3,6,12,18,-1,4,-1,8}), 10);
    return 0;
}