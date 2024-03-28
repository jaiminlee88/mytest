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
     *  Knight can only be from left to right. Find the shortest path to the destination position, 
     * return the length of the route. Return -1 if knight can not reached.

        Example
        Example 1:

        Input:
        [[0,0,0,0],[0,0,0,0],[0,0,0,0]]
        Output:
        3
        Explanation:
        [0,0]->[2,1]->[0,2]->[2,3]
        Example 2:

        Input:
        [[0,1,0],[0,0,1],[0,0,0]]
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
        return shortestPath2_0(grid);
    }

    int shortestPath2_0(vector<vector<bool>> &grid) {
        // 动态规划法
        // dp记录到达当前节点最短步数
        // 注意y是往右移的，依赖于左侧，x上下移动，所以要先遍历列，再遍历行
        if (grid.size() < 1 || grid[0].size() < 1 || grid[0][0] == 1) {
            return 0;
        }
        vector<vector<int>> delta = {{-1, -2}, {1, -2}, {-2, -1}, {2, -1}};
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