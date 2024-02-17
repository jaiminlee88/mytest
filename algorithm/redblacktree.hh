#include <iostream>
#include <memory>
using namespace std;

template<typename T>
class RedBlackTree {
/*
1 节点是红色或黑色
2 根是黑色
3 叶子节点（外部节点，空节点）都是黑色，
    这里的叶子节点指的是最底层的空节点（外部节点），
    下图中的那些null节点才是叶子节点，null节点的父节点在红黑树里不将其看作叶子节点
4 红色节点的子节点都是黑色
5 红色节点的父节点都是黑色
6 从根节点到叶子节点的所有路径上不能有 2 个连续的红色节点
7 从任一节点到叶子节点的所有路径都包含相同数目的黑色节点
*/
public:
    enum Color {RED, BLACK};
    struct node {
        T data;
        std::shared_ptr<node> left;
        std::shared_ptr<node> right;
        std::shared_ptr<node> parent;
        Color color{BLACK};
    };
    std::shared_ptr<struct node*> root;
public:
    RedBlackTree() {
        cout << "establish a red black tree" << endl;
        root = nullptr;
    }
    void insert(T data) {}
};
