#include "binarytree.hh"
#include <iostream>
#include <vector>
using namespace std;


int main() {
    auto push_data = [](BinaryTree<int>& tree, vector<int>& v) {
        if (v.size() == 0) {
            cout << "push---> empty" << endl;
            return;
        }
        cout << "push---: ";
        for (auto i : v) {
            tree.insert(i);
            cout << i << " ";
        }
        cout << endl;
    };
    auto print_data = [](BinaryTree<int>& tree) {
        vector<int> vec;
        tree.get_nodes_dfs_lmr(vec, tree.root);
        cout << "inorder: ";
        for (auto i : vec) {
            cout << i << " ";
        }
        cout << endl;
    };

    BinaryTree<int> tree;
    vector<int> vec = { 4, 5, 6, 1, 8, 9};
    push_data(tree, vec);
    print_data(tree);

    tree.clear();
    vec = {10, 4, 5, 6, 1, 8, 9};
    push_data(tree, vec);
    print_data(tree);
    // tree.print_BFS();

    tree.clear();
    vec = { 5, 4, 6, 1, 8, 9, 2, 3, 7};
    push_data(tree, vec);
    print_data(tree);

    tree.clear();
    vec = {};
    push_data(tree, vec);
    print_data(tree);

    tree.clear();
    vec = {1};
    push_data(tree, vec);
    print_data(tree);

    tree.clear();
    vec = {1, 2};
    push_data(tree, vec);
    print_data(tree);

    tree.clear();
    vec = {2, 1, 2};
    push_data(tree, vec);
    print_data(tree);

    tree.clear();
    vec = {2, 1, 3,2,2,2};
    push_data(tree, vec);
    print_data(tree);

}
