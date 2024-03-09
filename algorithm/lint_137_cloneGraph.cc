#include <queue>
#include <unordered_map>
using namespace std;


/**
 * Definition for undirected graph.
 * struct UndirectedGraphNode {
 *     int label;
 *     vector<UndirectedGraphNode *> neighbors;
 *     UndirectedGraphNode(int x) : label(x) {};
 * };
 */

class Solution {
public:
    /**
     * @param node: A undirected graph node
     * @return: A undirected graph node
     * 克隆一张无向图. 无向图的每个节点包含一个 label 和一个列表 neighbors. 
     * 保证每个节点的 label 互不相同.
     * 你的程序需要返回一个经过深度拷贝的新图. 新图和原图具有同样的结构, 
     * 并且对新图的任何改动不会对原图造成任何影响.
     */

    // 代码过于耦合，不够简洁
    UndirectedGraphNode* cloneGraph(UndirectedGraphNode *node) {
        // write your code here
        if (node == nullptr) {
            return nullptr;
        }
        std::queue<UndirectedGraphNode*> q;
        std::unordered_map<int, UndirectedGraphNode*> newnode_map;
        UndirectedGraphNode* new_curnode = nullptr;
        UndirectedGraphNode* new_neighbor = nullptr;
        q.push(node);
        while(q.empty() == false) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                auto curnode = q.front();
                q.pop();

                if (newnode_map.find(curnode->label) == newnode_map.end()) {
                    new_curnode = new UndirectedGraphNode(curnode->label);
                    newnode_map[curnode->label] = new_curnode;
                } else {
                    new_curnode = newnode_map[curnode->label];
                }

                for (auto& neighbor : curnode->neighbors) {
                    auto it = newnode_map.find(neighbor->label);
                    if (it != newnode_map.end()) {
                        new_neighbor = it->second;
                    } else {
                        new_neighbor = new UndirectedGraphNode(neighbor->label);
                        newnode_map[neighbor->label] = new_neighbor;
                        q.push(neighbor);
                    }
                    new_curnode->neighbors.push_back(new_neighbor);
                }
            }
        }
        return newnode_map[node->label];
    }


    UndirectedGraphNode* clone_Node_Edge(UndirectedGraphNode* node, std::unordered_map<int, UndirectedGraphNode*>& newnode_map) {
        if (node == nullptr) {
            return nullptr;
        }
        if (newnode_map.find(node->label) != newnode_map.end()) {
            return newnode_map[node->label]; // 已经创建过
        }
        auto newnode = new UndirectedGraphNode(node->label);
        newnode_map[node->label] = newnode;
        for (auto& neighbor : node->neighbors) {
            newnode->neighbors.push_back(clone_Node_Edge(neighbor, newnode_map));
        }
        return newnode;
    }
    UndirectedGraphNode* cloneGraph_1(UndirectedGraphNode *node) {
        // write your code here
        if (node == nullptr) {
            return nullptr;
        }
        std::queue<UndirectedGraphNode*> q;
        std::unordered_map<int, UndirectedGraphNode*> newnode_map;
        UndirectedGraphNode* new_curnode = nullptr;
        q.push(node);
        while(q.empty() == false) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                auto curnode = q.front();
                q.pop();
                new_curnode = clone_Node_Edge(curnode, newnode_map);
                for (auto& neighbor : curnode->neighbors) {
                    auto it = newnode_map.find(neighbor->label);
                    if (it != newnode_map.end()) {
                        continue;
                    }
                    q.push(neighbor); // 入队前必检查，单纯遍历所有源节点
                }
            }
        }
        return newnode_map[node->label];
    }

    /*
     * 代码很有层次性
    */
    std::vector<UndirectedGraphNode*> find_all_nodes(UndirectedGraphNode * node) {
        if (node == nullptr) {
            return {};
        }
        std::queue<UndirectedGraphNode*> q;
        std::vector<UndirectedGraphNode*> nodes;
        std::unordered_map<UndirectedGraphNode*, bool> visited; // 有点多余
        q.push(node);
        visited[node] = true; // 入队时就得标记
        while(q.empty() == false) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                auto curnode = q.front();
                q.pop();
                if (visited.find(curnode) != visited.end()) {
                    continue;
                }
                nodes.push_back(curnode);
                for (auto& neighbor : curnode->neighbors) {
                    q.push(neighbor);
                    visited[neighbor] = true; // 确保不再进队列
                }
            }
        }
        return nodes;
    }

    std::unordered_map<UndirectedGraphNode*, UndirectedGraphNode*> copy_nodes(std::vector<UndirectedGraphNode*> nodes) {
        if (nodes.empty()) {
            return {};
        }
        std::unordered_map<UndirectedGraphNode*, UndirectedGraphNode*> node_map;
        for (auto& node : nodes) {
            node_map[node] = new UndirectedGraphNode(node->label);
        }
        return node_map;
    }

    void copy_edges(std::unordered_map<UndirectedGraphNode*, UndirectedGraphNode*>& node_map) {
        if (node_map.empty()) {
            return;
        }
        for (auto& it : node_map) {
            auto ori_node = it.first;
            auto cpy_node = it.second;
            for (auto& neighbor : ori_node->neighbors) {
                auto cpy_neighbor = node_map.find(neighbor);
                if (cpy_neighbor != node_map.end()) {
                    cpy_node->neighbors.push_back(cpy_neighbor->second);
                }
            }
        }
    }
    // 代码更加简洁
    UndirectedGraphNode* cloneGraph_3(UndirectedGraphNode *node) {
        if (node == nullptr) {
            return nullptr;
        }
        std::vector<UndirectedGraphNode*> nodes = find_all_nodes(node);
        std::unordered_map<UndirectedGraphNode*, UndirectedGraphNode*> node_map = copy_nodes(nodes);
        copy_edges(node_map);
        return node_map[node];
    }
};