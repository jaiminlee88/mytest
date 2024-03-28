#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <climits>
using namespace std;


class Solution {
public:
    /**
     * @param n: an integer,denote the number of cities
     * @param roads: a list of three-tuples,denote the road between cities
     * @return: return the minimum cost to travel all cities
     * 
     * Give n cities(labeled from 1 to n), and the undirected road's cost among the cities as a three-tuple [A, B, c]
     * (i.e there is a road between city A and city B and the cost is c). 
     * We need to find the smallest cost to travel all the cities starting from 1.
     * 
     * 旅行商问题，给定一个有权无向图，顶点1∼n，问从顶点1出发，
     * 走遍所有顶点的花费最小是多少。题目不保证没有平行边。
     * 要求返回起点
     * 
     *  Input: 
        n = 3
        tuple = [[1,2,1],[2,3,2],[1,3,3]]
        Output: 3
        Explanation: The shortest path is 1->2->3

        Input:
        n = 1
        tuple = []
        Output: 0

        暴力深度搜索法
        深度搜索并剪枝法
        状态压缩动态规划法，状态压缩可以用来处理信息，可以处理很多问题
        随机搜索算法，时间复杂度很高，结果不一定对，但是可以处理大数问题，当做近似算法
            可以判断中间路径位置是否可以交换，交换后是否更优
            也可以反转中间路径，反转后是否更优
     */
    int minCost(int n, vector<vector<int>> &roads) {
        // return minCost_DFS_0(n, roads);
        // return minCost_DFS_1(n, roads); // 剪枝法
        // return minCost_DP(n, roads); // 状态压缩法
        return minCost_random(n, roads); // 随机搜索算法
    }

    int minCost_DFS_0(int n, vector<vector<int>> &roads) {
        if (roads.size() == 0) {
            return 0;
        }
        std::unordered_set<int> visited;
        std::unordered_map<int, std::unordered_map<int, int>> graph;
        vector<int> path;
        for (auto& it : roads) {
            // 也许会有重复的边，要选最小值
            if ((graph.find(it[0]) != graph.end())
             && (graph[it[0]].find(it[1]) != graph[it[0]].end())) {
                if (it[2] < graph[it[0]][it[1]]) {
                    graph[it[0]][it[1]] = it[2];
                    graph[it[1]][it[0]] = it[2];
                }
                continue;
            }
            graph[it[0]][it[1]] = it[2];
            graph[it[1]][it[0]] = it[2];
        }
        int mincost = INT32_MAX;
        int tmpcost = 0;
        visited.insert(1);
        path.push_back(1);
        minCost_DFS_0_core(graph, n, visited, 1, mincost, tmpcost, path);
        return mincost;
    }

    void minCost_DFS_0_core(std::unordered_map<int, std::unordered_map<int, int>>& graph,
                     int n, std::unordered_set<int> &visited, int node,
                     int& mincost, int& tmpcost, vector<int>& path) {
        // 相当于每个路径都去算了一遍，然后取最小值，这样的话，时间复杂度是O(n!)，太高了
        if (visited.size() >= n) {
            // cout << "tmpcost: " << tmpcost << " mincost: " << mincost << " | ";
            // for (auto& it : path) {
            //     cout << it << " ";
            // }
            // cout << endl;
            if (tmpcost < mincost) {
                mincost = tmpcost;
            }
            return;
        }
        for (auto& neighbor : graph[node]) {
            if (visited.find(neighbor.first) != visited.end()) {
                continue;
            }
            if (tmpcost + neighbor.second > mincost) {
                // 如果当前的tmpcost已经大于mincost了，就不用再继续了
                continue;
            }
            visited.insert(neighbor.first);
            path.push_back(neighbor.first);
            tmpcost += neighbor.second;
            minCost_DFS_0_core(graph, n, visited, neighbor.first, mincost, tmpcost, path);
            visited.erase(neighbor.first);
            path.pop_back();
            tmpcost -= neighbor.second;
        }
    }

    int minCost_DFS_1(int n, vector<vector<int>> &roads) {
        // 暴力搜索剪枝法
        if (roads.size() == 0) {
            return 0;
        }
        std::unordered_set<int> visited;
        std::unordered_map<int, std::unordered_map<int, int>> graph;
        vector<int> path;
        for (int i=1; i <= n; ++i) {
            for (int j=1; j <= n; ++j) {
                graph[i][j] = (INT32_MAX >> 4);
            }
        }
        for (auto& it : roads) {
            // 也许会有重复的边，要选最小值
            if ((graph.find(it[0]) != graph.end())
             && (graph[it[0]].find(it[1]) != graph[it[0]].end())) {
                if (it[2] < graph[it[0]][it[1]]) {
                    graph[it[0]][it[1]] = it[2];
                    graph[it[1]][it[0]] = it[2];
                }
            }
        }
        int mincost = (INT32_MAX >> 4);
        int tmpcost = 0;
        visited.insert(1);
        path.push_back(1);
        minCost_DFS_1_core(graph, n, visited, 1, mincost, tmpcost, path);
        return mincost;
    }

    void minCost_DFS_1_core(std::unordered_map<int, std::unordered_map<int, int>>& graph,
                     int n, std::unordered_set<int> &visited, int node,
                     int& mincost, int& tmpcost, vector<int>& path) {
        // 相当于每个路径都去算了一遍，然后取最小值，这样的话，时间复杂度是O(n!)，太高了
        if (visited.size() >= n) {
            // cout << "tmpcost: " << tmpcost << " mincost: " << mincost << " path=[";
            // for (auto& it : path) {
            //     cout << it << " ";
            // }
            // cout << "]" << endl;
            if (tmpcost < mincost) {
                mincost = tmpcost;
            }
            return;
        }
        for (auto& neighbor : graph[node]) {
            if (visited.find(neighbor.first) != visited.end()) {
                continue;
            }

            // 剪枝法
            // 当前找到一条路径，--->a->b----->last->new
            for (int i = 1; i < path.size(); ++i) {
                int a = path[i-1];
                int b = path[i];
                int last = path.back();
                if (graph[a][b] + graph[last][neighbor.first] >
                    graph[a][last] + graph[b][neighbor.first]) {
                    // 路径两两比较，反转比较last和之前的节点情况，但不比较比last更前的两两节点，因为已经比较过了
                    // 1---->xxxxx---> last--->new, 1和last确定，之间两两节点比较看有没有更好的
                    // 如果当前的tmpcost已经大于mincost了，就不用再继续了
                    continue;
                }
            }
            visited.insert(neighbor.first);
            path.push_back(neighbor.first);
            tmpcost += neighbor.second;
            minCost_DFS_1_core(graph, n, visited, neighbor.first, mincost, tmpcost, path);
            visited.erase(neighbor.first);
            path.pop_back();
            tmpcost -= neighbor.second;
        }
    }

    int minCost_DP(int n, vector<vector<int>> &roads) {
        // 状态压缩法
        // [1001001] 分别表示是否访问到
        if (roads.size() == 0) {
            return 0;
        }
        std::unordered_set<int> visited;
        std::unordered_map<int, std::unordered_map<int, int>> graph;
        vector<int> path;
        for (int i=1; i <= n; ++i) {
            for (int j=1; j <= n; ++j) {
                graph[i][j] = (INT32_MAX >> 2);
            }
        }
        for (auto& it : roads) {
            // 也许会有重复的边，要选最小值
            if ((graph.find(it[0]) != graph.end())
             && (graph[it[0]].find(it[1]) != graph[it[0]].end())) {
                if (it[2] < graph[it[0]][it[1]]) {
                    graph[it[0]][it[1]] = it[2];
                    graph[it[1]][it[0]] = it[2];
                }
            }
        }
        // cout << "graph:" << endl;
        // for (auto& it : graph) {
        //     auto city = it.first;
        //     for (auto& neighbor : it.second) {
        //         cout << city << "->" << neighbor.first << " : " << neighbor.second << endl;
        //     }
        //     cout << endl;
        // }

        // c(i,j)表示从i出发直接到达j的最小花费
        // d(S',i)表示从i出发经过S集合中的点一次且仅一次到达i的最小花费
        // d(S',i) = min{d(S'-{k},k)+c(k,i)} k∈S
        // d({},i) = 0
        // d({i},i) = INT_MAX
        // 前后出现依赖关系
        // S集合有许多状态，可以用二进制表示
        // 正推距离，逆推路径
        int stateSize = 1 << n; // if n =3, 1<<n -> [1 0 0 0]
        // cout << "stateSize: " << stateSize << endl;
        vector<vector<int>> dp(stateSize, vector<int>(n+1, (INT32_MAX >> 2))); // 不用0, 该方案每行是 上一个状态 对应 下一个节点，不太好
        dp[0] = vector<int>(n+1, 0); // 所有的起点都是0
        // cout << "dp============" << endl;
        // for (int i = 0; i < dp.size(); ++i) {
        //     cout << i << ": ";
        //     for (int j = 0; j <= n; ++j) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        std::vector<std::vector<int>> vecs;
        std::vector<int> subvec;
        for (int num = 1; num <= n; ++num) {
            vecs = {};
            subvec = {};
            gen_full_permutaion(vecs, subvec, n, num); // 生成状态集合
            for (auto s : vecs) {
                int state = 0;
                for (int i = 0; i < s.size(); ++i) { // [1 0 0 1] 从左到右 为 低到高位
                    if (s[i] == 1) {
                        state |= (1 << i);
                    }
                }
                // cout << "state: " << state << endl;
                for (int i = 1; i <= n; ++i) { // 即将到达的点
                    if (s[i-1] == 1) {
                        continue;
                    }
                    for (int k = 0; k < s.size(); ++k) {
                        if (s[k] == 0) { // 只对S中的点进行计算，不存在的点为0
                            continue;
                        }
                        int prev_state = state ^ (1 << k); // 从i出发，经过S集合中的点一次且仅一次到达i的最小花费
                        // cout << "prev_state:" << prev_state << " state:" <<state << endl;
                        dp[state][i] = std::min(dp[prev_state][k+1] + graph[k+1][i], dp[state][i]);
                    }
                }
            }
        }
        vecs = {};
        subvec = {};
        int mincost = INT32_MAX;
        gen_full_permutaion(vecs, subvec, n, n); // 生成状态集合
        for (auto& s : vecs) {
            int state = 0;
            for (int i = 0; i < s.size(); ++i) { // [1 0 0 1] 从左到右 为 低到高位
                if (s[i] == 1) {
                    state |= (1 << i);
                }
            }
            state = state ^ 1;
            // cout << "state: " << state << " dp[state][1]: " << dp[state][1] << endl;
            mincost = std::min(mincost, dp[state][1]);
        }
        // cout << "dp===============" << endl;
        // for (int i = 0; i < stateSize; ++i) {
        //     cout << "(" << i << ") ";
        //     for (int k = n - 1; k >= 0; --k) {
        //         if ((i & (1 << k)) != 0) {
        //             cout << "1 ";
        //         } else {
        //             cout << "0 ";
        //         }
        //     }
        //     cout << ": ";
        //     for (int j = 0; j <= n; ++j) {
        //         if (dp[i][j] == (INT32_MAX >> 2)) {
        //             cout << "INF ";
        //         } else {
        //             cout << dp[i][j] << " ";
        //         }
        //     }
        //     cout << endl;
        // }
        return mincost;
    }

    void gen_full_permutaion(std::vector<std::vector<int>>& vecs, std::vector<int>& subvec, int n, int ones) {
        // 全排列
        if (n < ones) {
            // cout << "n=" << n << " ones=" << ones << endl;
            return;
        }
        if (n == 0) {
            vecs.push_back(subvec);
            return;
        }
        if (ones == 0) {
            subvec.insert(subvec.end(), n, 0);
            vecs.push_back(subvec);
            subvec.erase(subvec.end()-n, subvec.end());
        } else {
            subvec.push_back(1);
            gen_full_permutaion(vecs, subvec, n-1, ones-1);
            subvec.pop_back();
            subvec.push_back(0);
            gen_full_permutaion(vecs, subvec, n-1, ones);
            subvec.pop_back();
        }
    }

    std::vector<std::vector<int>> construct_graph(vector<vector<int>>& roads, int n) {
        std::vector<std::vector<int>> graph(n + 1, vector<int>(n + 1, (INT32_MAX >> 4)));
        for (auto& it : roads) {
            // 也许会有重复的边，要选最小值
            if (it[2] < graph[it[0]][it[1]]) {
                    graph[it[0]][it[1]] = it[2];
                    graph[it[1]][it[0]] = it[2];
            }
        }
        return graph;
    }
    int minCost_DP_1(int n, vector<vector<int>> &roads) {
        // 压缩动态规划时间复杂度 O(n^2*2^n) 
        if (roads.size() == 0) {
            return 0;
        }
        std::vector<std::vector<int>> graph = construct_graph(roads, n);
        cout << "graph:" <<endl;
        for (int i = 0; i <= n; ++i) {
            for (int j = 0; j <= n; ++j) {
                cout << i << "->" << j << " : " << graph[i][j] << endl;
            }
        }
        int stateSize = 1 << n; // if n =3, 1<<n -> [1 0 0 0]
        vector<vector<int>> dp(stateSize, vector<int>(n+1, (INT32_MAX >> 2))); // 不用0
        // dp含义为，从1出发，经过集合S到达i的最小花费
        // 1101 1 2 3 4
        // 1列表示该状态最后一个节点访问的是1
        // 2列表示该状态最后一个节点访问的是2
        cout << "dp before ==========" << endl;
        for (int i = 0; i < dp.size(); ++i) {
            cout << "(" << i << ") ";
            for (int k = n - 1; k >= 0; --k) {
                if ((i & (1 << k)) != 0) {
                    cout << "1 ";
                } else {
                    cout << "0 ";
                }
            }
            for (int j = 1; j <= n; ++j) {
                cout << dp[i][j] << " ";
            }
            cout << endl;
        }
        dp[1][1] = 0; // 从1出发，经过空集合到达1的最小花费
        for (int state = 0; state < stateSize; ++state) {// 枚举V’的所有状态
            for (int i = 2; i <= n; ++i) { // 遍历集合中所有位置的点，因为从1出发，可以不遍历
                if ((state & (1 << (i - 1))) == 0) {
                    // 集合中i点必须已访问过，若未访问过则略过，
                    // i=3  1101&0100 = 0100 访问过3
                    // i=2  1101&0010 = 0000 未访问过2
                    continue;
                }
                // 筛选出已经访问过的点i，然后删除i点
                int preState = state ^ (1 << (i - 1)); // 删除集合中的i点，得以i为结尾的上一个状态，i是集合最后一个节点
                for (int j = 1; j <= n; ++j) { // 下一个到达的点j
                    if ((preState & (1 << (j - 1))) == 0) {
                        continue; // j点必须已经访问过，j为上一个状态结尾
                    }
                    // x->j->i
                    // dp的行表示状态，列表示该状态的最后一个节点访问的是哪个节点
                    dp[state][i] = std::min(dp[state][i], dp[preState][j] + graph[j][i]);
                }
            }
        }
        cout << "dp after ==========" << endl;
        for (int i = 0; i < dp.size(); ++i) {
            cout << "(" << i << ") ";
            for (int k = n - 1; k >= 0; --k) {
                if ((i & (1 << k)) != 0) {
                    cout << "1 ";
                } else {
                    cout << "0 ";
                }
            }
            for (int j = 1; j <= n; ++j) {
                cout << dp[i][j] << " ";
            }
            cout << endl;
        }
        int mincost = INT32_MAX;
        for (int i = 0; i <= n; ++i) {
            mincost = std::min(mincost, dp[stateSize - 1][i]);
        }
        return mincost;
    }

    int minCost_random(int n, vector<vector<int>> &roads) {
        // 随机搜索算法
        // 时间复杂度很高，结果不一定对，但是可以处理大数问题，当做近似算法
        if (roads.size() == 0) {
            return 0;
        }
        int mincost = INT32_MAX;
        int tmpcost = 0;
        vector<vector<int>> graph = construct_graph(roads, n);
        // cout << "graph:" <<endl;
        // for (int i = 0; i <= n; ++i) {
        //     for (int j = 0; j <= n; ++j) {
        //         cout << i << "->" << j << " : " << graph[i][j] << endl;
        //     }
        // }
        int random_times = 1000;
        vector<int> path;
        for (int i = 0; i < random_times; ++i) {
            path = gen_random_path(n); // 获取一个随机路径
            if (path[0] != 1) {
                continue;
            }
            tmpcost = adjust_path(path, graph); // 调整该路径，获得最优排序
            mincost = std::min(mincost, tmpcost);
        }
        // cout << "path: ";
        // for (auto& it : path) {
        //     cout << it << " ";
        // }
        // cout << endl;
        return mincost;
    }
    vector<int> gen_random_path(int n) {
        vector<int> path;
        for (int i = 1; i <= n; ++i) {
            path.push_back(i);
        }
        std::random_shuffle(path.begin(), path.end());
        return path;
    }
    int adjust_path(vector<int> path, vector<vector<int>>& graph) {
        // 可以调整路径，使得路径最优
        // 方法1，1->2->3->4->5->6->7, 1 5不能动，任意两个点交换，改变4条边
        // 方法2，1->2->3->4->5->6->7, 1 5不能动，反转中间任意长度的路径，改变n/2条边
        // cout << "path(" << path.size() << "): ";
        // for  (auto& it : path) {
        //     cout << it << " ";
        // }
        // cout << endl;
        bool adjusted = true;
        while (adjusted) {
            adjusted = false;
            for (int i = 1; i < path.size(); ++i) {
                for (int j = i + 1; j < path.size() - 1; ++j) {
                    // cout << "i: " << i 
                    // << " j: " << j 
                    // << " path[i-1]: " << path[i-1] 
                    // << " path[i]: " << path[i] 
                    // << " path[j]: " << path[j] 
                    // << " path[j+1]: " << path[j+1] << endl;
                    if (graph[path[i-1]][path[i]] + graph[path[i]][path[j]] + graph[path[j]][path[j+1]] >
                        graph[path[i-1]][path[j]] + graph[path[j]][path[i]] + graph[path[i]][path[j+1]]) {
                        std::swap(path[i], path[j]);
                        adjusted = true;
                    }
                }
            }
        }
        int cost = 0;
        for (int i = 1; i < path.size(); ++i) {
            cost += graph[path[i-1]][path[i]];
        }
        return cost;
    }
};



int main () {
    Solution s;
    vector<vector<int>> roads;
    int result = 0;
    auto test = [&s](vector<vector<int>> &roads, int n){
        cout << "======================" << endl;
        cout << "n: " << n << " roads: ";
        for (auto& it : roads) {
            cout << "[";
            for (auto& itt : it) {
                cout << itt << " ";
            }
            cout << "]";
        }
        cout << endl;
        int result0 = s.minCost_DFS_0(n, roads);
        cout << "minCost_DFS_0   : " << result0 << endl;

        int result1 = s.minCost_DFS_1(n, roads); // 剪枝法
        cout << "minCost_DFS_1: " << result1 << endl;

        int result2 = s.minCost_DP(n, roads); // 状态压缩法
        cout << "minCost_DP   : " << result2 << endl;

        int result3 = s.minCost_DP_1(n, roads); // 状态压缩法，官方写法
        cout << "minCost_DP_1 : " << result3 << endl;

        int result4 = s.minCost_random(n, roads); // 随机搜索算法
        cout << "minCost_random  : " << result4 << endl;
        return 0;
    };

    roads = {{1,2,1},{2,3,2},{1,3,3}};
    test(roads, 3);

    roads = {};
    test(roads, 1);

    roads = {{4,1,10}, {4,2,15}, {4,3,20}, {1,3,25}, {1,2,35}, {2,3,30}};
    test(roads, 4);

    roads = {{4,1,10}, {4,1,5}, {4,2,15}, {4,3,20}, {1,3,25}, {1,2,35}, {2,3,30}};
    test(roads, 4);

    roads = {{4,1,10}, {4,1,5}, {4,2,15}, {1,3,25}, {1,2,35}, {2,3,30}};
    test(roads, 4);
    return 0;
}