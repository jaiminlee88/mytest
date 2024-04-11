#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <climits>
#include <list>
#include <stack>
#include <set>
#include <queue>
using namespace std;


struct Node {
    int x;
    int y;
    Node(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

class Solution {
public:
    /**
     * @param grid: a chessboard included 0 and 1
     * @return: the shortest path
     * 
     * Given a knight in a chessboard n * m (a binary matrix with 0 as empty and 1 as barrier). 
     * the knight initialze position is (0, 0) and he wants to reach position (n - 1, m - 1),
     * Knight can only be from left to right. Find the shortest path to the destination position, 
     * return the length of the route. Return -1 if knight can not reached.

        Example
        Example 1:

        Input:
        [[0,0,0,0],
         [0,0,0,0],
         [0,0,0,0]]
        Output:
        3
        Explanation:
        [0,0]->[2,1]->[0,2]->[2,3]
        Example 2:

        Input:
        [[0,1,0],
         [0,0,1],
         [0,0,0]]
        Output:
        -1
        Clarification
        If the knight is at (x, y), he can get to the following positions in one step:

        (x + 1, y + 2)
        (x - 1, y + 2)
        (x + 2, y + 1)
        (x - 2, y + 1)

        DFS遍历所有路径，并记录步数，找到最短路径，但是要遍历所有路径
        BFS遍历所有路径，找到最短路径，可能比较好
        动态规划，因为只能向右移动，所以只能从上面或者左边移动过来，所以可以用动态规划
     */
    int shortestPath2(vector<vector<bool>> &grid) {
        // return shortestPath2_0(grid);
        return shortestPath2_1(grid);
    }
    vector<vector<int>> delta = {{ 1, -2},
                                 {-1, -2},
                                 { 2, -1},
                                 {-2, -1}};
    vector<vector<int>> delta1 = {{ 1, 2},
                                 {-1, 2},
                                 { 2, 1},
                                 {-2, 1}};
    int shortestPath2_1(vector<vector<bool>> &grid) {
        // 双向BFS
        if (grid.size() < 1 || grid[0].size() < 1 || grid[0][0] == true) {
            return 0;
        }
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<bool>> svisited(grid.size(), vector<bool>(grid[0].size(), false));
        vector<vector<bool>> dvisited(grid.size(), vector<bool>(grid[0].size(), false));
        queue<Node> sq;
        queue<Node> dq;
        sq.push(Node(0, 0));
        dq.push(Node(m - 1, n - 1));
        svisited[0][0] = true;
        dvisited[m - 1][n - 1] = true;
        int step = 0;
        while (sq.size() > 0 && dq.size() > 0) {
            ++step;
            if (extend_que(sq, grid, m, n, svisited, dvisited, 1)) {
                return step;
            }
            ++step;
            if (extend_que(dq, grid, m, n, dvisited, svisited, -1)) {
                return step;
            }
        }
        return -1;
    }

    bool valid_node(vector<vector<bool>> &grid, vector<vector<bool>> &visited, Node &node, int m, int n) {
        if (node.x >= 0 && node.x < m && node.y >= 0 && node.y < n
         && grid[node.x][node.y] == false
         && visited[node.x][node.y] == false) {
            return true;
        }
        return false;
    }
    bool extend_que(queue<Node> &q, vector<vector<bool>> &grid, int m, int n,
                    vector<vector<bool>> &visited, vector<vector<bool>> & other_visited, int direc) {
        int size = q.size();
        for (int i = 0; i < size; ++i) {
            auto cur = q.front();
            q.pop();
            for (auto& d : delta1) {
                auto tmp = cur;
                tmp.x += d[0];
                tmp.y += d[1] * direc;
                if (!valid_node(grid, visited, tmp, m, n)) {
                    continue;
                }
                q.push(tmp);
                visited[tmp.x][tmp.y] = true;
                if (other_visited[tmp.x][tmp.y]) {
                    return true;
                }
            }
        }
        return false;
    }

    int shortestPath2_0(vector<vector<bool>> &grid) {
        // 动态规划法
        // dp记录到达当前节点最短步数
        // 注意y是往右移的，依赖于左侧，x上下移动，所以要先遍历列，再遍历行
        /*
        * 有序的遍历，从左到右，从上到下，求最小值，可能用动态规划
          (1) dp[i][j]到达坐标[i,j]的最短步数
          (2) function: dp[i][j] = min(dp[i-delta.x][j-delta.y] + 1) for delta in deltas
          (3) init: dp[0][0] = 0
                    dp[i][i] = INT_MAX
          (4) result: dp[m-1][n-1]
          dp[]
        */
        if (grid.size() < 1 || grid[0].size() < 1 || grid[0][0] == 1) {
            return 0;
        }
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<int>> dp(m, vector<int>(n, INT_MAX));
        dp[0][0] = 0;
        for (int c = 0; c < n; ++c) { // 从列开始，因为需要用到左侧的值
            for (int r = 0; r < m; ++r) {
                if (r == 0 && c == 0) {
                    continue;
                }
                if (grid[r][c] == 1) {
                    continue;
                }
                int min_step = INT_MAX;
                for (auto& d : delta) {
                    int x = r + d[0];
                    int y = c + d[1];
                    if (!validpos(x,y,grid)) {
                        continue;
                    }
                    min_step = min(min_step, dp[x][y]);
                }
                if (min_step != INT_MAX && min_step + 1 < dp[r][c]) {
                    dp[r][c] = min_step + 1;
                }
            }
        }
        cout << "dp:" << endl;
        for (auto& it : dp) {
            for (auto& i : it) {
                cout << i << " ";
            }
            cout << endl;
        }
        return dp[m-1][n-1] == INT_MAX ? -1 : dp[m-1][n-1];
    }

    bool validpos(int& x, int& y, vector<vector<bool>>& grid) {
        if (x < 0 || x >= grid.size() || y < 0 || y >= grid[0].size()) {
            return false;
        }
        if (grid[x][y] == 1) {
            return false;
        }
        return true;
    }
};


int main() {
    Solution s;
    vector<vector<bool>> nums;
    auto test = [&s](vector<vector<bool>> nums) {
        cout << "nums: " << endl;
        for (auto i : nums) {
            for (auto j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
        // cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.shortestPath2(nums);
        cout << " res=" << res << endl;
        // for (auto& v : res) {
        //     for (auto& i : v) {
        //         cout << i << " ";
        //     }
        //     cout << endl;
        // }
    };

    nums = {{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    test(nums);

    nums = {{0,1,0},{0,0,1},{0,0,0}};
    test(nums);
}