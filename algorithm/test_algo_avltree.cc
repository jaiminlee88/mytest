#include "avltree.hh"
#include <iostream>
#include <vector>
using namespace std;


int main() {
    auto push_data = [](AVLTree<int>& tree, vector<int>& v) {
        if (v.size() == 0) {
            cout << "===push---> empty" << endl;
            return;
        }
        cout << "===push---: ";
        for (auto& i : v) {
            cout << i << " ";
        }
        cout << endl;
        for (auto i : v) {
            tree.insert(i);
        }
    };
    auto print_data = [](AVLTree<int>& tree) {
        cout << "print_data---> ";
        std::vector<std::pair<int, int>> vec;
        // tree.get_nodes_dfs_lmr(vec, tree.root);
        tree.get_nodes_dfs_morris(vec, tree.root);
        cout << "inorder: ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        int height = tree.root == nullptr ? 0 : tree.root->height;
        cout << " depth:" << height << endl;
    };

    auto print_BFS = [](AVLTree<int>& tree) {
        cout << "BFS: ";
        // auto ret = tree.get_BFS();
        // for (auto& i : ret) {
        //     cout << i.first << "[" << i.second << "] ";
        // }
        // cout << endl;
        auto ret = tree.get_BFS_1();
        cout << "[";
        for (auto& sub_vec : ret) {
            cout << "[";
            for (auto& i : sub_vec) {
                cout << "("<< i.first << "," << i.second << ") ";
            }
            cout << "] ";
        }
        cout << "]" << endl;
    };

    AVLTree<int> tree;
    vector<int> vec;

    // tree.clear();
    // vec = {};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {1};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {1, 2};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {1, 2, 3};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {2, 1, 2};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {1,2,3};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {3,2,1};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {2,2,2,2};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {2, 1, 3};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {2, 1, 5, 3, 6, 4}; // 先右旋再左旋
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {6,2,7,1,4,3}; // 先左旋再右旋
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {6,3,7,1,4,2}; // 先左旋再右旋
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {2,1,4,3,6,5}; // 先左旋再右旋
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {2, 1, 3, 2};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {2, 1, 3, 2, 2};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {2, 1, 3, 2, 2, 2};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = { 4, 5, 6, 1, 8, 9};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = {10, 4, 5, 6, 1, 8, 9};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    // tree.clear();
    // vec = { 5, 4, 6, 1, 8, 9, 2, 3, 7};
    // push_data(tree, vec);
    // print_data(tree);
    // print_BFS(tree);

    tree.clear();
    vec = {1,2,3};
    push_data(tree, vec);
    print_data(tree);
    print_BFS(tree);

    tree.clear();
    vec = {1,2,3,4};
    push_data(tree, vec);
    print_data(tree);
    print_BFS(tree);

    tree.clear();
    vec = {1,2,3,4,5};
    push_data(tree, vec);
    print_data(tree);
    print_BFS(tree);

    tree.clear();
    vec = {1,2,3,4,5,6};
    push_data(tree, vec);
    print_data(tree);
    print_BFS(tree);

    tree.clear();
    vec = {1,2,3,4,5,6,7};
    push_data(tree, vec);
    print_data(tree);
    print_BFS(tree);
}
