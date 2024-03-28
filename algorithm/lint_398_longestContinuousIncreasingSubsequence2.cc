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
     * @param matrix: A 2D-array of integers
     * @return: an integer
     * 
     * Give you an integer matrix (with row size n, column size m)，
     * find the longest increasing continuous subsequence in this matrix. 
     * (The definition of the longest increasing continuous subsequence here 
     * can start at any row or column and go up/down/right/left any direction).
     * 
     * 给定一个整数矩阵. 找出矩阵中的最长连续上升子序列, 返回它的长度.
        最长连续上升子序列可以从任意位置开始, 向上/下/左/右移动.
     * Example
        Given a matrix:

        [
        [1 ,2 ,3 ,4 ,5],
        [16,17,24,23,6],
        [15,18,25,22,7],
        [14,19,20,21,8],
        [13,12,11,10,9]
        ]
        return 25
        解释 1->2->3->4-?5...->25

        用DFS遍历所有路径，找到最长的路径，但是要遍历所有路径，复杂度太高
        从每个点开始，找到最长的路径，然后找到最长的路径，如果下一个节点比当前节点大，就继续找，否则就停止，然后进行路径长度比较

        动态规划
        是求最值，数据不变，存在有序，可以试用动态规划
        坐标题型
        四要素解决动态规划问题
        status: dp[i][j] 表示从某节点到当前节点的最长路径
        funtion：dp[i][j] = max(dp[i][j], dp[i-1][j] + 1, dp[i+1][j] + 1, dp[i][j-1] + 1, dp[i][j+1] + 1)
                四个反向，如何保证有序，从数字处来
     */
    int longestContinuousIncreasingSubsequence2(vector<vector<int>> &matrix) {
        return longestContinuousIncreasingSubsequence2_0(matrix);
    }

    class node {
    public:
        int x;
        int y;
        int val;
        node(int _x, int _y, int _val) : x(_x), y(_y), val(_val) {}
    };
    int longestContinuousIncreasingSubsequence2_0(vector<vector<int>> &matrix) {
        if (matrix.size() == 0) {
            return 0;
        }
        int m = matrix.size();
        int n = matrix[0].size();
        vector<vector<int>> dp(m, vector<int>(n, 1));
        vector<vector<int>> delta = {{0,1},{0,-1},{1,0},{-1,0}};
        vector<node> nodes;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                nodes.push_back(node(i, j, matrix[i][j]));
            }
        }
        // 既然是有序，就要保证四个方向之前都要有序，要保证反向使用前有序，就要保证里面访问dp[i][j]都是有序的
        // 通过排序，保证有序
        sort(nodes.begin(), nodes.end(), [](node& a, node& b) {
            return a.val < b.val;
        });
        int max_len = 1;
        for (auto& node : nodes) {
            int i = node.x;
            int j = node.y;
            for (auto& d : delta) {
                int x = i + d[0];
                int y = j + d[1];
                if (!validnum(matrix, x, y)) {
                    continue;
                }
                if (matrix[i][j] > matrix[x][y]) {
                    dp[i][j] = max(dp[i][j], dp[x][y] + 1);
                    if (dp[i][j] > max_len) {
                        max_len = dp[i][j];
                    }
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
        return max_len;
    }
    bool validnum(vector<vector<int>> &matrix, int r, int c) {
        return r >= 0 && r < matrix.size() && c >= 0 && c < matrix[0].size();
    }
};

int main() {
    Solution s;
    vector<vector<int>> nums;
    auto test = [&s](vector<vector<int>>& nums) {
        cout << "nums: " << endl;
        for (auto& i : nums) {
            for (auto& j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.longestContinuousIncreasingSubsequence2(nums);
        cout << "permutaions: "<< res << endl;
    };

    nums = {
        {1 ,2 ,3 ,4 ,5},
        {16,17,24,23,6},
        {15,18,25,22,7},
        {14,19,20,21,8},
        {13,12,11,10,9}
        };
    test(nums);

    nums = {
        {1,4,5},
        {6,2,8},
        {3,7,9}
    };
    test(nums);
}