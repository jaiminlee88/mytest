#include <iostream>
#include <memory>
using namespace std;

class RedBlackTree {
public:
    struct node {
        int data;
        std::shared_ptr<node> left;
        std::shared_ptr<node> right;
        std::shared_ptr<node> parent;
        bool color;
    };
    std::shared_ptr<struct node*> root;
public:
    RedBlackTree() {
        cout << "establish a red black tree" << endl;
        root = nullptr;
    }
    void insert(int) {}
};
