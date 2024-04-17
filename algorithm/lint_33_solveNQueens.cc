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
#include <numeric>
#include <bitset>
#include <deque>
using namespace std;


class Solution {
public:
    /**
     * @param n: The number of queens
     * @return: All distinct solutions
     *          we will sort your return value in output
     * N皇后问题是将n个皇后放置在n*n的棋盘上，
     * 皇后彼此之间不能相互攻击(任意两个皇后不能位于同一行，同一列，同一斜线)。
     * 给定一个整数n，返回所有不同的N皇后问题的解决方案。
     * 每个解决方案包含一个明确的N皇后放置布局，其中“Q”和“.”分别表示一个女王和一个空位置。
     * 
     * n = 1
     * "Q" 只有一种
     * 
     * n = 4
     * [
        // Solution 1
            [".Q..",
            "...Q",
            "Q...",
            "..Q."
            ],
            // Solution 2
            ["..Q.",
            "Q...",
            "...Q",
            ".Q.."
            ]
        ]

        要互相不影响，每行是否存在受上一行影响，每列是否存在受上一列影响，每个斜线是否受上一个斜线影响
        暴力解法：O(S*n^n) S是方案总数
            从第一行开始，每一行放置一个皇后，然后判断是否合法，如果合法，继续放下一个皇后
            如果不合法，回溯，重新放置上一个皇后
            递归的终止条件是n消耗完
        

     */
    vector<vector<string>> solveNQueens(int n) {
        // write your code here
        return solveNQueens_0(n);
    }
    vector<vector<string>> solveNQueens_0(int n) {
        if (n < 1) {
            return {};
        }
        if (n == 1) {
            return {{"Q"}};
        }
        vector<vector<string>> res;
        vector<vector<int>> board(n, vector<int>(n, 0));
        vector<int> queen_pos;
        unordered_map<string, set<int>> visited;
        visited["col"] = {}; // 列
        visited["sum"] = {}; // 
        visited["diff"] = {};
        solveNQueens_0_core(board, 0, n, res, queen_pos, visited);
        return res;
    }

    void solveNQueens_0_core(vector<vector<int>>& board, int row, int n, vector<vector<string>>& res, vector<int>& queen_pos, unordered_map<string, set<int>>& visited) {
        if (n < 1) {
            // 递归终止条件
            gen_board(board, res);
            return;
        }
        for (int col = 0; col < board.size(); ++col) {
            // if (is_valid(board, row, col)) {
            // if (is_valid(queen_pos, row, col, board.size(), board[0].size())) {
            if (is_valid(visited, row, col, board.size(), board[0].size())) {
                board[row][col] = 1;

                visited["col"].insert(col);
                visited["sum"].insert(row + col);
                visited["diff"].insert(row - col);

                queen_pos.push_back(row * board.size() + col);
                solveNQueens_0_core(board, row + 1, n - 1, res, queen_pos, visited);
                queen_pos.pop_back();

                visited["col"].erase(col);
                visited["sum"].erase(row + col);
                visited["diff"].erase(row - col);

                board[row][col] = 0;
            }
        }
    }

    // 复杂度总是O(n^n)，很难优化
    void gen_board(vector<vector<int>>& board, vector<vector<string>>& res) {
        res.push_back({});
        for (int r = 0; r < board.size(); ++r) {
            string str;
            for (int c = 0; c < board[0].size(); ++c) {
                if (board[r][c] == 1) {
                    str.push_back('Q');
                } else {
                    str.push_back('.');
                }
            }
            res.back().push_back(str);
        }
    }

    // 实际没有优化效果， 瓶颈不再is_valid
    bool is_valid(unordered_map<string, set<int>>& visited, int row, int col, int row_cnt, int col_cnt) {
        if (row >= 0 && row <= row_cnt && col >= 0 && col <= col_cnt) {
            if (visited["col"].count(col) != 0
             || visited["sum"].count(row + col) != 0
             || visited["diff"].count(row - col) != 0) {
                return false;
            }
        }
        return true;
    }
    // 更好
    bool is_valid(vector<int>& queen_pos, int row, int col, int row_cnt, int col_cnt) {
        if (row >= 0 && row <= row_cnt && col >= 0 && col <= col_cnt) {
            for (int i = 0; i < queen_pos.size(); ++i) {
                int r = queen_pos[i] / col_cnt;
                int c = queen_pos[i] % col_cnt;
                if (r == row || c == col || r - row == c - col || r - row == col - c) {
                    return false;
                }
            }
        }
        return true;
    }

    bool is_valid(vector<vector<int>>& board, int row, int col) {
        int n = board.size();
        if (row >= 0 && row <= n && col >= 0 && col <= n) {
            for (int c = 0; c < n; ++c) {
                if (board[row][c] == 1) {
                    return false;
                }
            }
            for (int r = 0; r < n; ++r) {
                if (board[r][col] == 1) {
                    return false;
                }
            }
            for (int r = row, c = col; r >= 0 && c >= 0; --r, --c) {
                if (board[r][c] == 1) {
                    return false;
                }
            }
            for (int r = row, c = col; r < n && c < n; ++r, ++c) {
                if (board[r][c] == 1) {
                    return false;
                }
            }
            for (int r = row, c = col; r >= 0 && c < n; --r, ++c) {
                if (board[r][c] == 1) {
                    return false;
                }
            }
            for (int r = row, c = col; r < n && c >= 0; ++r, --c) {
                if (board[r][c] == 1) {
                    return false;
                }
            }
        }
        return true;
    }
};


int main() {
    Solution s;
    vector<int> nums;
    vector<int> nums0;
    vector<int> nums1;
    vector<vector<int>> nums3;
    vector<vector<char>> nums4;
    vector<string> nums5;
    string str;
    string str0;
    string str1;
    int k = 0;
    auto test = [&s](vector<int>& nums) {
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.permute_3(nums);
        cout << "permutaions: "<< endl;
        for (auto& v : res) {
            for (auto& i : v) {
                cout << i << " ";
            }
            cout << endl;
        }
    };

    nums = {};
    test(nums);

    nums = {1};
    test(nums);

    nums = {1,2};
    test(nums);

    nums = {1, 2, 3};
    test(nums);

    nums = {1,2,3,4};
    test(nums);
}