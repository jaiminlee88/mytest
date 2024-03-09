
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>

using namespace std;

class Solution {
public:
    /**
     * @param n: An integer
     * @param edges: a list of undirected edges
     * @return: true if it's a valid tree, or false
     * 给出 n 个节点，标号分别从 0 到 n - 1 并且给出一个 
     * 无向 边的列表 (给出每条边的两个顶点), 写一个函数去判断这张｀无向｀图是否是一棵树
     * 
     * 无序图，那么就是DFS或者BFS或者类似拓扑排序法
     * 有序图，那么就是拓扑排序法，判断是否存在拓扑排序，如果存在，那么就是树
        有向无环图（DAG）才有拓扑排序，非DAG图没有拓扑排序一说。做法：
        1.从 DAG 图中选择一个 没有前驱（即入度为0）的顶点并输出。
        2.从图中删除该顶点和所有以它为起点的有向边。
        3.重复 1 和 2 直到当前的 DAG 图为空或当前图中不存在无前驱的顶点为止。后一种情况说明有向图中必然存在环。
     */
    bool validTree(int n, vector<vector<int>> &edges) {
        // write your code here
        return validTree_1(n, edges);
    }
    bool validTree_1(int n, vector<vector<int>> &edges) {
        // 通过类似拓扑排序方法来判断是否是树
        // 独立节点不能成树，多个独立节点就是多个树
        // 拓扑排序可以排除那些只有一个入度的节点，然后删除这些节点，再继续排除
        // 环本身的入度是大于1的，所以环不会被排除
        if (n == 0) {
            return true;
        }
        if (edges.size() > n) {
            return false;
        }
        if (edges.size() < n - 1) {
            return false;
        }
        std::unordered_map<int, std::unordered_set<int>> graph;
        std::unordered_map<int, int> indegree;

        for (auto& it : edges) {
            int node_0 = it[0];
            int node_1 = it[1];
            graph[node_0].insert(node_1);
            graph[node_1].insert(node_0);
            indegree[node_0]++;
            indegree[node_1]++;
        }
        // for (auto& it : graph) {
        //     std::cout << it.first << "->";
        //     for (auto& it_1 : it.second) {
        //         std::cout << it_1 << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // for (auto& it : indegree) {
        //     std::cout << it.first << " degree=" << it.second << std::endl;
        // }

        std::queue<int> q;
        for (auto& it : indegree) {
            if (it.second == 1) {
                q.push(it.first);
            }
        }

        while (q.empty() == false) {
            int curnode = q.front();
            q.pop();
            for (auto& neighbor : graph[curnode]) {
                graph[neighbor].erase(curnode);
                indegree[neighbor]--;
                if (indegree[neighbor] == 1) {
                    q.push(neighbor);
                }
            }
            indegree.erase(curnode);
            graph.erase(curnode);
        }

        // cout << "-----" << endl;
        // for (auto& it : graph) {
        //     std::cout << it.first << "->";
        //     for (auto& it_1 : it.second) {
        //         std::cout << it_1 << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // for (auto& it : indegree) {
        //     std::cout << it.first << " degree=" << it.second << std::endl;
        // }

        return indegree.size() == 0;
    }

    bool validTree_2(int n, vector<vector<int>> &edges) {
        // DFS方法，遍历所有路径，如果有个路径走过两次，那么就是环
        // 优先左中右遍历，中序遍历
        // DFS，判断有没有后向边，关于图边的类型看这里
        // 图中的一个节点，根据其C[N]的值，有三种状态：
        //     0，此节点没有被访问过
        //     -1，被访问过至少1次，其后代节点正在被访问中
        //     1，其后代节点都被访问过。
        // 按照这样的假设，当按照DFS进行搜索时，碰到一个节点时有三种可能：
        //     1、如果C[V]=0，这是一个新的节点，不做处理
        //     2、如果C[V]=-1，说明是在访问该节点的后代的过程中访问到该节点本身，则图中有环。
        //     3、如果C[V]=1，类似于2的推导，没有环。    在程序中加上一些特殊的处理，即可以找出图中有几个环，并记录每个环的路径
    
        // 使用 DFS 可以判断一个无向图和有向中是否存在环。深度优先遍历图，如果在遍历的过程中，发现某个结点有一条边指向已访问过的结点，并且这个已访问过的结点不是上一步访问的结点，则表示存在环。

        // 我们不能仅仅使用一个 bool 数组来表示结点是否访问过。规定每个结点都拥有三种状态，白、灰、黑。开始时所有结点都是白色，当访问过某个结点后，该结点变为灰色，当该结点的所有邻接点都访问完，该节点变为黑色。

        // 那么我们的算法可以表示为：如果在遍历的过程中，发现某个结点有一条边指向灰色节点，并且这个灰色结点不是上一步访问的结点，那么存在环。

        if (n == 0) {
            return true;
        }
        if (edges.size() > n) {
            return false;
        }
        if (edges.size() < n - 1) {
            return false;
        }
        enum node_stat {
            stat_new = 0,
            stat_one,
            stat_two
        };
        std::unordered_map<int, node_stat> color;
        for (int i = 0; i < n; ++i) {
            color[i] = stat_new;
        }


        return false;
    }

    bool validTree_3(int n, vector<vector<int>> &edges) {
        // BFS方法
        // BFS，首先通过广度遍历（BFS）访问图的所有点，对于每个点，
        // 都检测和已访问过的点是否有边（除了和它连接的上层节点），
        // 如果有边，说明有回路（有环）。如果对于每个点，都没有和已访问过的点有边，
        // 说明从该点出发的当前图没有回路（无环）。如果从任意点开始的BFS，
        // 以上操作均说明无回路，才能说明没有回路。
    }

    int find(std::unordered_map<int, int> &parent, int node) {
        if (parent[node] == -1) {
            return node;
        }
        return find(parent, parent[node]); // 串行查找，最后返回的是X，代表该节点属于X集合
    };
    bool validTree_4(int n, vector<vector<int>> &edges) {
        // 并查集方法
        // 并查集，对于每个边，如果两个节点的根节点相同，说明有环
        // 如果两个节点的根节点不同，那么就合并两个节点
        // 如果最后只有一个根节点，那么就是树
        // 我们可以用并查集，检测给定的无向图是否存在环。注意，这个方法假定无向图中不包含自环边。
        // 原理是：图的全部顶点都各自属于一个集合；然后依次检查图中的所有边，
        //        如果边关联的两个顶点不属于同一个集合，就合并两个顶点所在的集合。
        //        若图中存在环，则一定会在检测到某条边的时候，发现它关联的两个顶点早已属于同一个集合。
        // 0  1  2  3
        // 1  2 -1 -1
        // 0属于的集合是2
        if (n == 0) {
            return true;
        }
        if (edges.size() > n) {
            return false;
        }
        if (edges.size() < n - 1) {
            return false;
        }
        std::unordered_map<int, int> parent;
        for (int i = 0; i < n; ++i) {
            parent[i] = -1;
        }
        for (auto& edge : edges) {
            int src = edge[0];
            int dst = edge[1];

            int src_root = find(parent, src);
            int dst_root = find(parent, dst);
            if (src_root == dst_root) {
                return false;
            }
            // merge,最终都会修改到根节点的，确保合并
            parent[src_root] = dst_root;
        }

        return true;
    }
};

int main() {
    Solution s;
    vector<vector<int>> edges;
    int n;

    auto test = [&s](int n, vector<vector<int>>& edge) {
        cout << "edge: ";
        for (auto& it : edge) {
            cout << it[0] << "-" << it[1] << " ";
        }
        cout << " n: " << n << endl;
        bool ret = s.validTree_4(5, edge);
        if (ret) {
            cout << "valid tree" << endl;
        } else {
            cout << "invalid tree" << endl;
        }
    };


    edges = {{0, 1}, {0, 2}, {0, 3}, {1, 4}};
    n = 5;
    test(n, edges);

    edges = {{0, 1}, {1, 2}, {2, 3}, {1, 3}, {1, 4}};
    n = 5;
    test(n, edges);

    edges = {};
    n = 2;
    test(n, edges);

    return 0;
}