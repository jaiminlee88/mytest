#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;

/**
 * Definition for Undirected graph.
 * struct UndirectedGraphNode {
 *     int label;
 *     vector<UndirectedGraphNode *> neighbors;
 *     UndirectedGraphNode(int x) : label(x) {};
 * };
 */
 
struct UndirectedGraphNode {
    int label;
    vector<UndirectedGraphNode *> neighbors;
    UndirectedGraphNode(int x) : label(x) {};
};
class Solution {
public:
    /*
     * @param nodes: a array of Undirected graph node
     * @return: a connected set of a Undirected graph
     * 给定一个无向图，求其所有连通分量，每个连通分量里的数要升序排列。
     * idea
     * 依次遍历所有节点，如果节点没有被访问过，那么就进行BFS或者DFS，然后将结果排序
     */
    vector<vector<int>> connectedSet(vector<UndirectedGraphNode*> nodes) {
        if (nodes.empty()) {
            return {};
        }

    }
    vector<int> BFS(UndirectedGraphNode* curnode, std::unordered_map<UndirectedGraphNode*, bool> visited) {
        if (curnode == nullptr) {
            return {};
        }
        if (visited.find(curnode) != visited.end()) {
            return {};
        }
        std::queue<UndirectedGraphNode*> q;
        std::vector<int> result;
        q.push(curnode);
        visited[curnode] = true;
        while (q.empty() == false) {
            auto node = q.front();
            q.pop();
            result.push_back(node->label);
            for (auto& neighbor : node->neighbors) {
                if (visited.find(neighbor) != visited.end()) {
                    continue;
                }
                q.push(neighbor);
                visited[neighbor] = true;
            }
        }
        sort(result.begin(), result.end());
        return result;
    }
};

int main(){

}