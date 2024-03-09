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
     * @param graph: A list of Directed graph node
     * @return: Any topological order for the given graph.
     * 给定一个有向图，图节点的拓扑排序定义如下:
    对于图中的每一条有向边 A -> B , 在拓扑排序中A一定在B之前.
    拓扑排序中的第一个节点可以是图中的任何一个没有其他节点指向它的节点.
    针对给定的有向图找到任意一种拓扑排序的顺序.
     */
    vector<DirectedGraphNode*> topSort(vector<DirectedGraphNode*> graph) {
        // write your code here
        if (graph.size() == 0) {
            return {};
        }
        if (graph.size() == 1) {
            return graph;
        }
        std::unordered_map<DirectedGraphNode*, int> indegree;
        std::queue<DirectedGraphNode*> q;
        for (auto& pnode : graph) {
            auto curnode = indegree.find(pnode);
            if (curnode == indegree.end()) {
                indegree[pnode] = 0;
            }
            for (auto& pneighbor : pnode->neighbors) {
                auto& degree = indegree[pneighbor];
                ++degree;
            }
        }
        for (auto& degree : indegree) {
            if (degree.second == 0) {
                q.push(degree.first);
            }
        }
        vector<DirectedGraphNode*> order;
        while (q.empty() == false) {
            auto pcurnode = q.front();
            q.pop();
            order.push_back(pcurnode);
            for (auto& pneighbor : pcurnode->neighbors) {
                auto& degree = indegree[pneighbor];
                --degree;
                if (degree == 0) {
                    q.push(pneighbor);
                }
            }
        }
        return order.size() == indegree.size() ? order : vector<DirectedGraphNode*>{};
    }

};

int main() {
    Solution s;
    vector<vector<int>> grid;
    int required_course;
    // auto test = [&s] (vector<vector<int>>& grid, int n) {
    //     cout << "============" << endl;
    //     cout << "grid:" << endl;
    //     for (auto row : grid) {
    //         for (auto col : row) {
    //             cout << col << " ";
    //         }
    //         cout << endl;
    //     }
    //     vector<int> order = s.findOrder(n, grid);
    //     cout << "n: " << n << " order: ";
    //     if (order.size() == 0) {
    //         cout << " not found" << endl;
    //         return;
    //     }
    //     for (int i = 0; i < order.size(); ++i) {
    //         cout << order[i];
    //         if (i != order.size() - 1) {
    //             cout << "->";
    //         }
    //     }
    //     cout << endl;
    // };

    // grid = {};
    // required_course = 1;
    // test(grid, required_course); // 2

    // grid = {{1,0}};
    // required_course = 2;
    // test(grid, required_course); // 2

    // grid = {{1,0},
    //         {2,0},
    //         {3,1},
    //         {3,2}};
    // required_course = 4;
    // test(grid, required_course); //  0 1 3 2

    // grid = {{1,2},
    //         {4,3}};
    // required_course = 5;
    // test(grid, required_course); // 3 4 2 1 0

    // grid = {{2,1},
    //         {3,2},
    //         {1,3}};
    // required_course = 2;
    // test(grid, required_course); // 3 4 2 1 0
    return 0;
}