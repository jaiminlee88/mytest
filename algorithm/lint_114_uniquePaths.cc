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
     * @param m: positive integer (1 <= m <= 100)
     * @param n: positive integer (1 <= n <= 100)
     * @return: An integer
     * 
     * 有一个机器人的位于一个 m×n 个网格左上角。
        机器人每一时刻只能向下或者向右移动一步。机器人试图达到网格的右下角。
        问有多少条不同的路径？

        n = 1
        m = 3
        1

        n = 3
        m = 3
        6
     */
    int uniquePaths(int m, int n) {
        // write your code here
        // return uniquePaths0(m, n);
        // return uniquePaths1(m, n);
        return uniquePaths2(m, n);
    }

    int uniquePaths0(int m, int n) {
        if (m < 1 || n < 1) {
            return 0;
        }
        if (m==1 || n==1) {
            return 1;
        }

        int path_cnt = 0;
        uniquePaths0_(m, n, 0, 0, path_cnt);
        return path_cnt;
    }

    void uniquePaths0_(int m, int n, int x, int y, int& path_cnt) {
        if (x == m-1 && y == n-1) {
            path_cnt++;
            return;
        }
        if (x < m-1) {
            uniquePaths0_(m, n, x+1, y, path_cnt);
        }
        if (y < n-1) {
            uniquePaths0_(m, n, x, y+1, path_cnt);
        }
    }

    int uniquePaths1(int m, int n) {
        // 自顶向下  动态规划 推荐
        if (m < 1 || n < 1) {
            return 0;
        }
        if (m==1 || n==1) {
            return 1;
        }
        vector<vector<int>> dp(m, vector<int>(n, 0));
        dp[0][0] = 1;
        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                if (r == 0 && c == 0) {
                    continue;
                } else if (r == 0) {
                    dp[r][c] = dp[r][c-1];
                } else if (c == 0) {
                    dp[r][c] = dp[r - 1][c];
                } else {
                    dp[r][c] = dp[r-1][c] + dp[r][c-1];
                }
            }
        }
        return dp[m-1][n-1];
    }

    int uniquePaths2(int m, int n) {
        // 自底向上  动态规划 不推荐
        // 即 dp[i][j]=dp[i+1][j]+dp[i][j+1, 从哪里来，最后回到触发点
        if (m < 1 || n < 1) {
            return 0;
        }
        if (m==1 || n==1) {
            return 1;
        }
        vector<vector<int>> dp(m, vector<int>(n, 0));
        dp[m-1][n-1] = 1;
        for (int r = m-1; r >= 0; --r) {
            for (int c = n-1; c >= 0; --c) {
                if (r == m-1 && c == n-1) {
                    continue;
                } else if (r == m-1) {
                    dp[r][c] = dp[r][c+1];
                } else if (c == n-1) {
                    dp[r][c] = dp[r+1][c];
                } else {
                    dp[r][c] = dp[r+1][c] + dp[r][c+1];
                }
            }
        }
        return dp[0][0];
    }
};


int main() {
    Solution s;
    vector<int> nums;
    auto test = [&s](int m, int n) {
        // cout << "nums: ";
        // for (auto& i : nums) {
        //     cout << i << " ";
        // }
        // cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.uniquePaths(m,n);
        cout << "m="<< m << " n=" << n << " res=" << res << endl;
        // for (auto& v : res) {
        //     for (auto& i : v) {
        //         cout << i << " ";
        //     }
        //     cout << endl;
        // }
    };

    test(1, 3);
    test(3, 3);
}