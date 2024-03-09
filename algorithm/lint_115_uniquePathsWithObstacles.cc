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


class Solution {
public:
    /**
     * @param obstacleGrid: A list of lists of integers
     * @return: An integer
     * 
     * 有一个机器人的位于一个 
        m×n 个网格左上角。
        机器人每一时刻只能向下或者向右移动一步。机器人试图达到网格的右下角。
        现在考虑网格中有障碍物，那样将会有多少条不同的路径？
        网格中的障碍和空位置分别用 1 和 0 来表示。

        obstacleGrid = [[0]]
        1

        obstacleGrid = [[0,0,0],[0,1,0],[0,0,0]]
        2
     */
    int uniquePathsWithObstacles(vector<vector<int>> &obstacleGrid) {
        // write your code here
        // 别管这么多，就用动态规划吧
        if (obstacleGrid.empty() || obstacleGrid[0].empty() || obstacleGrid[0][0] == 1) {
            return 0;
        }
        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();
        vector<vector<int>> dp(obstacleGrid.size(), vector<int>(obstacleGrid[0].size(),0));
        dp[0][0] = 1;
        for (int r = 0; r < obstacleGrid.size(); ++r) {
            for (int c = 0; c < obstacleGrid[0].size(); ++c) {
                if (r == 0 && c == 0) {
                    continue;
                }
                if (obstacleGrid[r][c] == 1) {
                    continue;
                }
                if (r == 0) {
                    dp[r][c] = dp[r][c-1];
                } else if (c == 0) {
                    dp[r][c] = dp[r-1][c];
                } else {
                    dp[r][c] = dp[r-1][c] + dp[r][c-1];
                }
            }
        }
        return dp[m-1][n-1];
    }
};

int main() {
    Solution s;
    vector<vector<int>> nums;
    auto test = [&s](vector<vector<int>> nums) {
        cout << "nums: ";
        for (auto& i : nums) {
            for (auto& j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
        // cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.uniquePathsWithObstacles(nums);
        cout << " res=" << res << endl;
        // for (auto& v : res) {
        //     for (auto& i : v) {
        //         cout << i << " ";
        //     }
        //     cout << endl;
        // }
    };

    nums = {{0}};
    test(nums);

    nums = {{0,0,0},{0,1,0},{0,0,0}};
    test(nums);
}