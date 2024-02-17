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
        std::vector<std::pair<int, int>> vec;
        int height = tree.root == nullptr ? 0 : tree.root->height;
        cout << "DFS_depth:" << height << endl;

        vec.clear();
        tree.get_nodes_dfs_lmr(vec, tree.root);
        cout << "DFS_lmr           : ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;

        vec.clear();
        tree.get_nodes_dfs_lmr_traverse(vec, tree.root);
        cout << "DFS_lmr_traverse  : ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;

        vec.clear();
        tree.get_nodes_dfs_lrm(vec, tree.root);
        cout << "DFS_lrm           : ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;

        vec.clear();
        tree.get_nodes_dfs_lrm_traverse(vec, tree.root);
        cout << "DFS_lrm_traverse  : ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;

        vec.clear();
        tree.get_nodes_dfs_lrm_traverse_1(vec, tree.root);
        cout << "DFS_lrm_traverse_1: ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;

        vec.clear();
        tree.get_nodes_dfs_mlr(vec, tree.root);
        cout << "DFS_mlr           : ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;

        vec.clear();
        tree.get_nodes_dfs_mlr_traverse(vec, tree.root);
        cout << "DFS_mlr_traverse  : ";
        for (auto i : vec) {
            cout << i.first << "(" << i.second << ") ";
        }
        cout << endl;
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

    auto print_path = [](AVLTree<int>& tree) {
        vector<std::string> ret = tree.get_allpath();
        cout << "path: ";
        for (auto& i : ret) {
            cout << i << " ";
        }
        cout << endl;
    };
    AVLTree<int> tree;
    vector<int> vec;

    tree.clear();
    vec = {1,2,3,4,5,6};
    push_data(tree, vec);
    print_data(tree);
    print_BFS(tree);
    print_path(tree);

}
