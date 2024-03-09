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

class Solution {
public:
    /**
     * @param root: the given BST
     * @param target: the given target
     * @param k: the given k
     * @return: k values in the BST that are closest to the target
     *          we will sort your return value in output
     *
     * 给定一棵非空二叉搜索树以及一个target值，找到 BST 中最接近给定值的 k 个数。
     * 给出的target值为浮点数
        你可以假设 k 总是合理的，即 k ≤ 总节点数
        我们可以保证给出的 BST 中只有唯一一个最接近给定值的 k 个值的集合
     *
     *
     * 先中序遍历生成有序数组，然后用双指针法找到最接近的k个数。O(N)
     * 
     */
    vector<int> closestKValues(TreeNode *root, double target, int k) {
        // write your code here
        return closestKValues_2(root, target, k);
    }
    vector<int> closestKValues_1(TreeNode *root, double target, int k) {
        if (root == nullptr) {
            return {};
        }

        vector<TreeNode*> lmr;
        TreeNode * curnode = root;
        stack<TreeNode*> s;
        int pos =0;
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
            lmr.push_back(curnode);
            curnode = curnode->right;
        }
        int left = 0;
        int right = lmr.size() - 1;
        if (lmr.size() < k) {
            return {};
        }
        while (right - left + 1 > k) {
            if (abs(lmr[left]->val - target) < abs(lmr[right]->val - target)) {
                right--;
            } else {
                left++;
            }
        }
        vector<int> ret;
        for (auto it = lmr.begin() + left; it < lmr.begin() + right + 1; ++it) {
            ret.push_back((*it)->val);
        }
        return ret;
    }

    vector<int> closestKValues_2(TreeNode *root, double target, int k) {
        // 用队列存储中序遍历的结果
        if (root == nullptr) {
            return {};
        }
        std::queue<int> q;
        std::stack<TreeNode*> s;
        TreeNode * curnode = root;
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
            if (q.size() < k) {
                q.push(curnode->val);
            } else if (q.size() == k) {
                if (abs(q.front() - target) > abs(curnode->val - target)) {
                    q.pop();
                    q.push(curnode->val);
                } else {
                    break;
                }
            }
            curnode = curnode->right;
        }
        vector<int> ret;
        while (!q.empty()) {
            ret.push_back(q.front());
            q.pop();
        }
        return ret;
    }
};

int main() {
    Solution s;

    auto test = [&s](vector<int> vec, double target, int k){
        TreeNode * root = constructGraph(vec);
        cout << "orig: [";
        for (auto& it : vec) {
            cout << it << " ";
        }
        vector<int> ret = s.closestKValues(root, target, k);
        cout << "] target=" << target << " k=" << k << " ret: ";
        for (auto& it : ret) {
            cout << it << " ";
        }
        cout << endl;
    };

    test(vector<int>({10,5,15,3,6,12,18,-1,4,-1,8}), 4.12, 3);

    test(vector<int>({1}), 0.000000, 1);
    return 0;
}