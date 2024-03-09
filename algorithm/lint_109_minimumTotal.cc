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
     * @param triangle: a list of lists of integers
     * @return: An integer, minimum path sum
     * 
     * 给定一个数字三角形，找到从顶部到底部的最小路径和。每一步可以移动到下面一行的相邻数字上。
     * 如果只用额外空间复杂度O(n)完成，可以获得加分，其中n是数字三角形的总行数。
     * 
     * triangle = [
                 [2],
                [3,4],
               [6,5,7],
              [4,1,8,3]
            ]
     * 11

     triangle = [
             [2],
            [3,2],
           [6,5,7],
          [4,4,8,1]
        ]
    * 12

    可以用DFS，逐个路径遍历，找到最小的路径和
    可以动态规划
     */
    int minimumTotal(vector<vector<int>> &triangle) {
        // write your code here
        // return minimumTotal_0(triangle);
        return minimumTotal_1(triangle);
        // return minimumTotal_2(triangle);
    }

    int minimumTotal_0(vector<vector<int>> &triangle) {
        // write your code here
        // 用BFS遍历，复杂度太高 O(2^n)
        if (triangle.size() < 1) {
            return 0;
        }
        int mincost = INT_MAX;
        int cost = 0;
        vector<int> path;
        path.push_back(triangle[0][0]);
        cost += triangle[0][0];
        minimumTotal_0_core(triangle, path, 0, 0, cost, mincost);
        return mincost;
    }
    void minimumTotal_0_core(vector<vector<int>> &triangle,
                            vector<int> &path,
                            int row,
                            int pos,
                            int& cost,
                            int& mincost) {
        if (row >= triangle.size() - 1) {
            mincost = min(cost, mincost);
            cout << "--------- cost=" << cost << " mincost=" << mincost << endl;
            for (auto& i : path) {
                cout << i << " ";
            }
            cout << endl;
            return;
        }
        int new_row = row + 1;
        for (int delta = 0; delta <= 1; delta++) {
            int new_pos = pos + delta;
            path.push_back(triangle[new_row][new_pos]);
            cost += triangle[new_row][new_pos];
            minimumTotal_0_core(triangle, path, new_row, new_pos, cost, mincost);
            path.pop_back();
            cost -= triangle[new_row][new_pos];
        }
    }

    int minimumTotal_1(vector<vector<int>> &triangle) {
        // 分治法，返回最右cost
        // 增加记忆化搜索 复杂度O(n^2)
        if (triangle.size() < 1) {
            return 0;
        }
        unordered_map<int,int> memo; // 记录中间结果，避免重复计算
        return minimumTotal_1_core(triangle, 0, 0, memo);
    }

    int minimumTotal_1_core(vector<vector<int>> &triangle, int row, int pos, unordered_map<int,int>& memo) {
        if (row == triangle.size()) {
            return 0;
        }
        int key = (row * row + row + 2) * 0.5 + pos;
        if (memo.find(key) != memo.end()) {
            return memo[key];
        }
        int left  = minimumTotal_1_core(triangle, row + 1, pos, memo);
        int right = minimumTotal_1_core(triangle, row + 1, pos + 1, memo);
        memo[key] = min(left, right) + triangle[row][pos];
        return memo[key];
    }

    int minimumTotal_2(vector<vector<int>> &triangle) {
        // 动态规划法
        // 关键在于是否能找到逻辑依赖
        // dis[row,col] = min(dis[row-1,col-1], dis[row-1,col]) + triangle[row,col]
        //                dis[row-1, col] + triangle[row,col], if col == 0
        //                dis[row-1, col-1]+ triangle[row,col], if col == row
        if (triangle.size() < 1) {
            return 0;
        }
        vector<vector<int>> dp;
        for (int i = 0; i < triangle.size(); ++i) {
            dp.push_back(vector<int>(triangle[i].size(), 0));
        }
        dp[0][0] = triangle[0][0];
        for (int r = 1; r < triangle.size(); ++r) {
            for (int c = 0; c < triangle[r].size(); ++c) {
                if (c == 0) {
                    dp[r][c] = dp[r-1][c] + triangle[r][c];
                } else if (c == r) {
                    dp[r][c] = dp[r-1][c-1] + triangle[r][c];
                } else {
                    dp[r][c] = min(dp[r-1][c-1], dp[r-1][c]) + triangle[r][c];
                }
            }
        }
        int mincost = INT_MAX;
        for (auto& it : dp[triangle.size() - 1]) {
            mincost = min(mincost, it);
        }
        return mincost;
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
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.minimumTotal(nums);
        cout << "permutaions: "<< res << endl;
    };

    nums = {
                 {2},
                {3,4},
               {6,5,7},
              {4,1,8,3}
    };
    test(nums);

    nums = {
             {2},
            {3,2},
           {6,5,7},
          {4,4,8,1}
        };
    test(nums);

    nums = {{-1},{2,3},{1,-1,-3}};
    test(nums);
}