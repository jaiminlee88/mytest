#include <vector>
#include <iostream>
#include <vector>

using namespace std;
/**
 * Definition of ParentTreeNode:
 * class ParentTreeNode {
 * public:
 *     int val;
 *     ParentTreeNode *parent, *left, *right;
 * }
 */

class ParentTreeNode {
public:
    ParentTreeNode(int _val = -1) : val(_val), parent(nullptr), left(nullptr), right(nullptr) {}
    int val;
    ParentTreeNode *parent, *left, *right;
};

class Solution {
public:
    /*
     * @param root: The root of the tree
     * @param A: node in the tree
     * @param B: node in the tree
     * @return: The lowest common ancestor of A and B
     * 
     * 给一棵二叉树和二叉树中的两个节点，找到这两个节点的最近公共祖先LCA。
        两个节点的最近公共祖先，是指两个节点的所有父亲节点中（包括这两个节点），离这两个节点最近的公共的节点。
        每个节点除了左右儿子指针以外，还包含一个父亲指针parent，指向自己的父亲。
     *  
     * 可以各自找到两个节点的parent，直到找到root，对比两个路径，找到公共的节点
     * 或者，A B如果存在，一定落于某个根的左右子树，如果A B分别在左右子树，那么根就是LCA，相当于到某个root时，遍历左子树可能找到一个，遍历右子树可能找到一个，那么root就是LCA
     */

    ParentTreeNode * lowestCommonAncestorII(ParentTreeNode * root, ParentTreeNode * A, ParentTreeNode * B) {
        return lowestCommonAncestorII_1(root, A, B);
    }
    ParentTreeNode * lowestCommonAncestorII_0(ParentTreeNode * root, ParentTreeNode * A, ParentTreeNode * B) {
        if (root == nullptr || A == nullptr || B == nullptr) {
            return nullptr;
        }
        std::vector<ParentTreeNode*> path_A;
        std::vector<ParentTreeNode*> path_B;
        while(A != root) {
            path_A.push_back(A);
            A = A->parent;
        }

        path_A.push_back(root);
        while(B != root) {
            path_B.push_back(B);
            B = B->parent;
        }
        path_B.push_back(root);
        int A_pos = path_A.size() - 1;
        int B_pos = path_B.size() - 1;
        // cout << "path_A.size()=" << path_A.size() << " path_B.size()=" << path_B.size() << endl;
        // for (int i = 0; i < path_A.size(); ++i) {
        //     std::cout << "path_A[" << i << "]=" << path_A[i]->val << std::endl;
        // }
        // for (int i = 0; i < path_B.size(); ++i) {
        //     std::cout << "path_B[" << i << "]=" << path_B[i]->val << std::endl;
        // }
        while(true) {
            if (A_pos < 0 || B_pos < 0) {
                return nullptr;
            }
            if (path_A[A_pos] != path_B[B_pos]) {
                return path_A[A_pos + 1];
            }
            --A_pos;
            --B_pos;
        }
        return nullptr;
    }

    ParentTreeNode * lowestCommonAncestorII_1(ParentTreeNode * root, ParentTreeNode * A, ParentTreeNode * B) {
        if (root == nullptr || A == nullptr || B == nullptr) {
            return nullptr;
        }
        if (root == A || root == B) {
            return root;
        }

        ParentTreeNode* left = lowestCommonAncestorII_1(root->left, A, B); // 对当前节点，如果从子树找到了A或B，返回A或B
        ParentTreeNode* right = lowestCommonAncestorII_1(root->right, A, B);
        if (left != nullptr && right != nullptr) {
            return root;
        }

        return left != nullptr ? left : right;
    }
};

void connect(ParentTreeNode* parent, ParentTreeNode* lchild, ParentTreeNode* rchild) {
    if (lchild != nullptr) {
        parent->left = lchild;
        lchild->parent = parent;
    }
    if (rchild != nullptr) {
        parent->right = rchild;
        rchild->parent = parent;
    }
}

ParentTreeNode* constructGraph(vector<int> nodes) {
    if (nodes.empty()) {
        return nullptr;
    }
    std::vector<ParentTreeNode*> graph;
    for (auto& it : nodes) {
        graph.push_back(new ParentTreeNode(it));
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

ParentTreeNode* find_node(ParentTreeNode* root, int val) {
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

int main() {
    Solution s;
    ParentTreeNode* root;

    auto test = [&s](ParentTreeNode* root, int _A, int _B){
        ParentTreeNode* A = find_node(root, _A);
        ParentTreeNode* B = find_node(root, _B);
        ParentTreeNode* ret = s.lowestCommonAncestorII(root, A, B);
        if (ret == nullptr) {
            std::cout << "root= not found." << std::endl;
            return;
        }
        std::cout << "root=" << ret << " val=" << ret->val << std::endl;
    };

    root = constructGraph(vector<int>({4,3,7,-1,-1,5,6}));
    test(root, 5, 6);

    root = constructGraph(vector<int>({4,3,7,-1,-1,5,6}));
    test(root, 3, 7);

    root = constructGraph(vector<int>({4,3,7,-1,-1,5,6}));
    test(root, 6, 7);

    root = constructGraph(vector<int>({4,3,7,-1,-1,5,6}));
    test(root, 3, 10);
    return 0;

}