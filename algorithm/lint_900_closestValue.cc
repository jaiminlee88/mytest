#include <iostream>
#include <climits>
#include <algorithm>
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

class Solution {
public:
    /**
     * @param root: the given BST
     * @param target: the given target
     * @return: the value in the BST that is closest to the target
     * 给一棵非空二叉搜索树以及一个target值，找到在BST中最接近给定值的节点值
     */
    int closestValue(TreeNode *root, double target) {
        return closestValue_1(root, target);
    }
    int closestValue_0(TreeNode *root, double target) {
        // write your code here
        if (root == nullptr) {
            return -1;
        }
        TreeNode * curnode = root;
        double diff = INT_MAX;
        int closest = -1;
        while (curnode != nullptr) {
            if (abs(curnode->val - target) < diff) {
                diff = abs(curnode->val - target);
                closest = curnode->val;
            }
            // cout << "curnode: " << curnode->val << " closest: " << closest << " diff: " << diff << endl;
            if (curnode->val == target) {
                return curnode->val;
            } else if (curnode->val < target) {
                curnode = curnode->right;
            } else {
                curnode = curnode->left;
            }
        }
        return closest;
    }

    int closestValue_1(TreeNode *root, double target) {
        // write your code here
        // 寻找上下限
        if (root == nullptr) {
            return -1;
        }
        TreeNode * lower = root;
        TreeNode * upper = root;
        TreeNode * curnode = root;
        while (curnode != nullptr) {
            if (target > root->val) {
                upper = root;
                root = root->right;
            }
            if (target < root->val) {
                lower = root;
                root = root->left;
            } else {
                return root->val;
            }
        }
        return abs(target - lower->val) < abs(target - upper->val) ? lower->val : upper->val;
    }
};

int main() {
    Solution s;

    auto test = [&s](vector<int> vec, double target){
        TreeNode * root = constructGraph(vec);
        cout << "orig: ";
        for (auto& it : vec) {
            cout << it << " ";
        }
        int ret = s.closestValue(root, target);
        cout << "target: " << target << " ret: " << ret << endl;
    };

    test(vector<int>({10,5,15,3,6,12,18,-1,4,-1,8}), 4.12);

    return 0;
}