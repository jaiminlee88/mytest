#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
using namespace std;


/**
 * Definition for Directed graph.
 * struct DirectedGraphNode {
 *     int label;
 *     vector<DirectedGraphNode *> neighbors;
 *     DirectedGraphNode(int x) : label(x) {};
 * };
 */

struct DirectedGraphNode {
    int label;
    vector<DirectedGraphNode *> neighbors;
    DirectedGraphNode(int x) : label(x) {};
};

class Solution {
public:
    /**
     * @param org: a permutation of the integers from 1 to n
     * @param seqs: a list of sequences
     * @return: true if it can be reconstructed only one or false
     * 判断是否序列 org 能唯一地由 seqs重构得出. org是一个由从1到n的正整数排列而成的序列，1≤n≤10^4。 
     * 重构表示组合成seqs的一个最短的父序列 (意思是，一个最短的序列使得所有 seqs里的序列都是它的子序列).
     * 判断是否有且仅有一个能从 seqs重构出来的序列，并且这个序列是org。
     * 
     * 说的太复杂了，其实就是判断seqs里的序列能否组合成org，而且只有一种组合方式
     * 先用seqs构建图
     */

    bool sequenceReconstruction(vector<int> &_org, vector<vector<int>> &seqs) {
        // write your code here
        vector<int> org = _org;
        if (seqs.size() == 0) {
            return false;
        }
        if (org.size() == 0) {
            org.push_back(-1);
        }
        std::unordered_map<int, std::unordered_set<int>> nodes_map;
        std::unordered_map<int, int> indegree;
        std::queue<int> q;
        for (auto& lk : seqs) {
            if (lk.size() == 0) {
                indegree[-1] = 0;
                nodes_map[-1] = std::unordered_set<int>{};
                continue;
            }
            if (lk.size() == 1) {
                auto degree = indegree.find(lk[0]);
                if (degree == indegree.end()) {
                    indegree[lk[0]] = 0;
                }
                continue;
            }

            for (int pre_pos = 0; pre_pos < lk.size() - 1; ++pre_pos) {
                int pre = lk[pre_pos];
                int post = lk[pre_pos + 1];
                // cout << "pre: " << pre << " post: " << post << endl;
                auto& pre_node = nodes_map[pre];
                pre_node.insert(post);
                auto pre_degree = indegree.find(pre);
                if (pre_degree == indegree.end()) {
                    indegree[pre] = 0;
                }

                auto post_node = nodes_map.find(post);
                if (post_node == nodes_map.end()) {
                    nodes_map[post] = std::unordered_set<int>{};
                }
                // 可能会有重复的，不应该放到这里
                // auto& post_degree = indegree[post];
                // ++post_degree;
            }
        }
        for (auto& node : nodes_map) {
            for (auto& neighbor : node.second) {
                auto& post_degree = indegree[neighbor];
                ++post_degree;
            }
        }
        // for (auto& node : nodes_map) {
        //     cout << node.first << " neighbor[";
        //     for (auto& neighbor : node.second) {
        //         cout << neighbor << " ";
        //     }
        //     cout << "]" << endl;
        // }
        // cout << endl;
        // for (auto& degree : indegree) {
        //     cout << degree.first << " degree: " << degree.second << endl;
        // }


        for (auto& degree : indegree) {
            if (degree.second == 0) {
                q.push(degree.first);
            }
        }
        while (q.empty() == false) {
            if (q.size() != 1) {
                return false;
            }
            int curnode = q.front();
            q.pop();
            if (org.size() == 0) { // org为空,但是q中还有数据
                return false;
            }
            int org_node = org.front();
            org.erase(org.begin());
            if (curnode != org_node) {
                return false;
            }
            auto& curnode_neighbors = nodes_map[curnode];
            for (auto& neighbor : curnode_neighbors) {
                auto& neighbor_degree = indegree[neighbor];
                --neighbor_degree;
                if (neighbor_degree == 0) {
                    q.push(neighbor);
                }
            }
        }
        return org.size() == 0 ? true : false;
    }

};

int main() {
    Solution s;
    vector<vector<int>> seqs;
    vector<int> org;
    auto test = [&s] (vector<vector<int>>& grid, vector<int> org) {
        cout << "============" << endl;
        cout << "grid:---" << endl;
        for (auto row : grid) {
            cout << "{";
            for (auto col : row) {
                cout << "{"<< col << "} ";
            }
            cout << "}" << endl;
        }
        cout << "-----------" << endl;
        bool ret = s.sequenceReconstruction(org, grid);
        cout << "org: [";
        for (auto& o : org) {
            cout << o << " ";
        }
        cout << "] " << endl;
        if (ret) {
            cout << "only one" << endl;
        } else {
            cout << "not only one" << endl;
        }
    };

    org = {1,2,3};
    seqs = {{1,2},{1,3}}; // 1->2, 1->3
    test(seqs, org); // false

    org = {};
    seqs = {{}};
    test(seqs, org); // false

    org = {1};
    seqs = {{1}};
    test(seqs, org); // true

    org = {1};
    seqs = {{}};
    test(seqs, org); // false

    org = {4,1,5,2,6,3};
    seqs = {{5,2,6,3},{4,1,5,2}};
    test(seqs, org); // true

    org = {5,3,2,4,1};
    seqs = {{5,3,2,4},{4,1},{1},{3},{2,4},{1,1000000000}};
    test(seqs, org); // false
    return 0;
}