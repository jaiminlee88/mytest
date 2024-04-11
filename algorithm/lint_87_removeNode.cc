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
     * @param root: The root of the binary search tree.
     * @param value: Remove the node with given value.
     * @return: The root of the binary search tree after removal.
     * 
     * 给定一棵具有不同节点值的二叉查找树，删除树中与给定值相同的节点。
     * 如果树中没有相同值的节点，就不做任何处理。你应该保证处理之后的树仍是二叉查找树。
     * 
     Tree = {5,3,6,2,4}
        value = 3
        {5,2,6,#,4} 或 {5,4,6,2}

     Tree = {5,3,6,2,4}
        value = 4
        {5,3,6,2}
    
     * 删除节点分为几种情况：
        1.删除的节点为叶子节点：直接删除。
        2.删除的节点只存在左子树或右子树：删除节点的父节点直接指向子树节点。
        3.删除的节点同时存在左子树和右子树：将删除节点的左子树的最右节点或右子树的最左节点替换删除节点，同时删除替换节点，再将删除节点指向子树节点。
     */
    TreeNode* removeNode(TreeNode *root, int value) {
        // write your code here
        if (root == nullptr) {
            return nullptr;
        }
        TreeNode * parent = nullptr;
        TreeNode * curnode  = root;
        while (curnode != nullptr) {
            if (curnode->val == value) {
                break;
            }
            if (curnode->val > value) {
                parent = curnode;
                curnode = curnode->left;
                continue;
            }
            if (curnode->val < value) {
                parent = curnode;
                curnode = curnode->right;
                continue;
            }
        }
        if (curnode == nullptr) {
            return root;
        }
        // curnode may be the root, or the left or right child of parent
        if (curnode->left == nullptr && curnode->right == nullptr) {
            if (parent != nullptr) {
                if (parent->left == curnode) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
            }
            if (root == curnode) {
                root = nullptr;
            }
            delete curnode;
        } else if (curnode->left != nullptr && curnode->right == nullptr) {
            if (parent != nullptr) {
                if (parent->left == curnode) {
                    parent->left = curnode->left;
                } else {
                    parent->right = curnode->left;
                }
            }
            if (root == curnode) {
                root = curnode->left;
            }
            delete curnode;
        } else if (curnode->left == nullptr && curnode->right != nullptr) {
            if (parent != nullptr) {
                if (parent->left == curnode) {
                    parent->left = curnode->right;
                } else {
                    parent->right = curnode->right;
                }
            }
            if (root == curnode) {
                root = curnode->right;
            }
            delete curnode;
        } else { // both left and right child are not null
            TreeNode* mostright_parent = curnode;
            TreeNode* mostright = curnode->right;
            while (mostright != nullptr) {
                if (mostright->right != nullptr) {
                    mostright_parent = mostright;
                    mostright = mostright->right;
                } else if (mostright->left != nullptr) {
                    mostright_parent = mostright;
                    mostright = mostright->left;
                } else {
                    break;
                }
            }
            if (mostright_parent->left == mostright) {
                mostright_parent->left = nullptr;
            } else {
                mostright_parent->right = nullptr;
            }
            curnode->val = mostright->val;
            delete mostright;
            while (true) {
                if (curnode == nullptr) {
                    break;
                } else if (curnode->left != nullptr && curnode->left->val > curnode->val) {
                    int tmp = curnode->val;
                    curnode->val = curnode->left->val;
                    curnode->left->val = tmp;
                    curnode = curnode->left;
                } else if (curnode->right != nullptr && curnode->right->val < curnode->val) {
                    int tmp = curnode->val;
                    curnode->val = curnode->right->val;
                    curnode->right->val = tmp;
                    curnode = curnode->right;
                } else {
                    break;
                }
            }
            return root;
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
        cout << "], remove: " << val << " ";
        TreeNode * ret = s.removeNode(root, val);
        BFS_print(ret);
    };

    test(vector<int>({5,3,6,2,4}), 3);
    test(vector<int>({5,3,6,2,4}), 4);
    // test(vector<int>({1}), 5);
    // test(vector<int>({2,1}), 0);
    // test(vector<int>({2,1,4,-1,-1,3}), 6);
    // test(vector<int>({10,5,15,3,6,12,18,-1,4,-1,8}), 10);
    return 0;
}