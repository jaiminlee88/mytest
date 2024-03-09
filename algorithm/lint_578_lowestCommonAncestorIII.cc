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

class Solution {
public:
    /*
     * @param root: The root of the binary tree.
     * @param A: A TreeNode
     * @param B: A TreeNode
     * @return: Return the LCA of the two nodes.
     * 
     * 给一棵二叉树和二叉树中的两个节点，找到这两个节点的最近公共祖先LCA。
        两个节点的最近公共祖先，是指两个节点的所有父亲节点中（包括这两个节点），离这两个节点最近的公共的节点。
        返回 null 如果两个节点在这棵树上不存在最近公共祖先的话。
        这两个节点未必都在这棵树上出现。
        每个节点的值都不同

     */

    TreeNode * lowestCommonAncestor3(TreeNode * root, TreeNode * A, TreeNode * B) {
        if (root == nullptr || A == nullptr || B == nullptr) {
            return nullptr;
        }

        TreeNode * _A{nullptr};
        TreeNode * _B{nullptr};
        TreeNode * _root = lowestCommonAncestor3_core(root, A, B, &_A, &_B);
        // cout << "hahaha A=" << _A << " B=" << _B << endl;
        if (_A && _B) {
            return _root;
        }
        return nullptr;
    }

    TreeNode * lowestCommonAncestor3_core(TreeNode * root, TreeNode * A, TreeNode * B, TreeNode ** _A, TreeNode ** _B) {
        if (root == nullptr || A == nullptr || B == nullptr) {
            return nullptr;
        }

        TreeNode * pleft = lowestCommonAncestor3_core(root->left, A, B, _A, _B);
        TreeNode * pright = lowestCommonAncestor3_core(root->right, A, B, _A, _B);

        if (root == A || root == B) { // 注意，B可能是A的子节点
            if (root == A) {
                (*_A) = A;
            } else if (root == B) {
                (*_B) = B;
            }
            // cout << "--->root=" << root << " r->val=" << root->val << " A=" << A << " B=" << B << endl;
            return root;
        }

        if (pleft != nullptr && pright != nullptr) {
            // A and B are in different subtrees
            return root;
        }

        if (pleft != nullptr) {
            return pleft;
        }
        if (pright != nullptr) {
            return pright;
        }
        return nullptr;
    }
};

int main() {
    Solution s;
    TreeNode* root;
    TreeNode* _A;
    TreeNode* _B;

    auto test = [&s](TreeNode* root, TreeNode* A, TreeNode* B){
        TreeNode* ret = s.lowestCommonAncestor3(root, A, B);
        if (A == nullptr) {
            std::cout << "A==NULL  ";
            return;
        } else {
            std::cout << "A->val=" << A->val << " ";
        }
        if (B == nullptr) {
            std::cout << "B==NULL  ";
        } else {
            std::cout << "B->val=" << B->val << " ";
        }
        if (ret == nullptr) {
            std::cout << "root= not found." << std::endl;
            return;
        }
        std::cout << "root=" << ret << " val=" << ret->val << std::endl;
    };

    root = constructGraph(vector<int>({4,3,7,-1,-1,5,6}));
    _A = find_node(root, 5);
    _B = find_node(root, 6);
    test(root, _A, _B);

    root = constructGraph(vector<int>({4,3,7,-1,-1,5,6}));
    _A = find_node(root, 3);
    _B = find_node(root, 7);
    test(root, _A, _B);

    root = constructGraph(vector<int>({4,3,7,-1,-1,5,6}));
    _A = find_node(root, 6);
    _B = find_node(root, 7);
    test(root, _A, _B);

    root = constructGraph(vector<int>({4,3,7}));
    _A = find_node(root, 4);
    _B = find_node(root, 3);
    test(root, _A, _B);

    root = constructGraph(vector<int>({4,3,2}));
    _A = find_node(root, 3);
    _B = find_node(root, 2);
    test(root, _A, _B);

    root = constructGraph(vector<int>({4,3,7,-1,-1,5,6}));
    _A = find_node(root, 3);
    _B = find_node(root, 10);
    test(root, _A, _B);

    root = constructGraph(vector<int>({4,3,7,-1,-1,5,6}));
    _A = new TreeNode(10);
    _B = new TreeNode(20);
    test(root, _A, _B);

    root = constructGraph(vector<int>({4,3,7,-1,-1,5,6}));
    _A = find_node(root, 5);
    _B = new TreeNode(10);
    test(root, _A, _B);

   

    return 0;

}