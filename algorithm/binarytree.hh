#include <iostream>
#include <memory>
#include <vector>
using namespace std;

template<typename T>
class BinaryTree {
/*
普通二叉树
可能会过于深
*/
public:
    struct node {
        T data;
        std::shared_ptr<node> left;
        std::shared_ptr<node> right;
        std::shared_ptr<node> parent;
    };
    std::shared_ptr<node> root;
public:
    BinaryTree() {
        cout << "establish a binary tree" << endl;
        root = nullptr;
    }

    void clear() {
        if (root == nullptr) {
            return;
        }
        vector<std::shared_ptr<node>> tmp;
        tmp.push_back(root);
        while (!tmp.empty()) {
            auto curnode = tmp.front();
            tmp.erase(tmp.begin());
            if (curnode->left != nullptr) {
                tmp.push_back(curnode->left);
            }
            if (curnode->right != nullptr) {
                tmp.push_back(curnode->right);
            }
            curnode->left = nullptr;
            curnode->right = nullptr;
            curnode->parent = nullptr;
        }
        root = nullptr;
    }
    void insert(T data) {
        if (root == nullptr) {
            root = std::make_shared<node>(node{data, nullptr, nullptr, nullptr});
            return;
        }
        std::shared_ptr<node> curnode = root;
        while (true) {
            if (data <= curnode->data) {
                if (curnode->left == nullptr) {
                    curnode->left = std::make_shared<node>(node{data, nullptr, nullptr, curnode});
                    return;
                }
                curnode = curnode->left;
            } else {
                if (curnode->right == nullptr) {
                    curnode->right = std::make_shared<node>(node{data, nullptr, nullptr, curnode});
                    return;
                }
                curnode = curnode->right;
            }
        }
    }

    void print_BFS() {
        vector<std::shared_ptr<node>> tmp;
        tmp.push_back(root);
        cout << "print_BFS: ";
        while (!tmp.empty()) {
            auto curnode = tmp.front();
            tmp.erase(tmp.begin());
            if (curnode->left != nullptr) {
                tmp.push_back(curnode->left);
            }
            if (curnode->right != nullptr) {
                tmp.push_back(curnode->right);
            }
            cout << curnode->data << " ";
        }
        cout << endl;
    }

    void get_nodes_dfs_lmr(std::vector<T>& vec, std::shared_ptr<node> curnode) {
        if (curnode == nullptr) {
            return;
        }
        get_nodes_dfs_lmr(vec, curnode.get()->left);
        vec.push_back(curnode.get()->data);
        get_nodes_dfs_lmr(vec, curnode.get()->right);
    }
};
