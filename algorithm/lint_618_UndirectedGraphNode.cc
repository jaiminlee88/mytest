
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>

using namespace std;


/**
 * Definition for Undirected graph.
 * struct UndirectedGraphNode {
 *     int label;
 *     vector<UndirectedGraphNode *> neighbors;
 *     UndirectedGraphNode(int x) : label(x) {};
 * };
 */
class Solution {
public:
    /**
     * @param graph a list of Undirected graph node
     * @param values a hash mapping, <UndirectedGraphNode, (int)value>
     * @param node an Undirected graph node
     * @param target an integer
     * @return the a node
     * 给定一个无向图，再给定一个顶点和一个数字target，求距离该顶点最近的且值为target的顶点。
     * 可以用BFS来做，一旦遍历到值为target的顶点就可以返回了。
     * BFS依次往外扩
     */
    UndirectedGraphNode* searchNode(vector<UndirectedGraphNode*>& graph,
                                    map<UndirectedGraphNode*, int>& values,
                                    UndirectedGraphNode* node,
                                    int target) {
        // Write your code here
        if (node == nullptr) {
            return nullptr;
        }
        std::queue<UndirectedGraphNode*> q;
        std::unordered_set<UndirectedGraphNode*> visited;
        q.push(node);
        visited.insert(node);
        int level = 0;
        while (q.empty() == false) {
            int size = q.size();
            ++level;
            for (int i = 0; i < size; ++i) {
                auto node = q.front();
                q.pop();
                if (node->label == target) {
                    return node;
                }
                for (auto& neighbor : node->neighbors) {
                    if (visited.find(neighbor) != visited.end()) {
                        continue;
                    }
                    q.push(neighbor);
                    visited.insert(neighbor);
                }
            }
        }
        return nullptr;
    }
};

int main() {
    Solution s;
    vector<vector<int>> edges;
    int n;

    

    return 0;
}