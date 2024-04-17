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
     * @param board: the sudoku puzzle
     * @return: nothing
     * 
     * 编写一个程序，通过填充空单元来解决数独难题。
        空单元由数字0表示。
        你可以认为只有一个唯一的解决方案。
        数独怎么玩：
            数独是一个9x9的网格，其中有一些预先填充的数字，这些数字是1到9。
            你的任务是通过填充剩余的空单元来解决数独，使得每一行，每一列和每一个3x3的子网格都包含1到9的数字。
            一个有效的数独解决方案必须满足以下规则：
            数字1-9在每一行只能出现一次。
            数字1-9在每一列只能出现一次。
            数字1-9在每一个以粗实线分隔的3x3子网格中只能出现一次。
            空单元由数字0表示。
            一个数独。

        给定的数独谜题:
        [
        [0,0,9,7,4,8,0,0,0],
        [7,0,0,0,0,0,0,0,0],
        [0,2,0,1,0,9,0,0,0],
        [0,0,7,0,0,0,2,4,0],
        [0,6,4,0,1,0,5,9,0],
        [0,9,8,0,0,0,3,0,0],
        [0,0,0,8,0,3,0,2,0],
        [0,0,0,0,0,0,0,0,6],
        [0,0,0,2,7,5,9,0,0]
        ]

        返回结果：
        [
        [5,1,9, 7,4,8, 6,3,2],
        [7,8,3, 6,5,2, 4,1,9],
        [4,2,6, 1,3,9, 8,7,5],

        [3,5,7, 9,8,6, 2,4,1],
        [2,6,4, 3,1,7, 5,9,8],
        [1,9,8, 5,2,4, 3,6,7],

        [9,7,5, 8,6,3, 1,2,4],
        [8,3,2, 4,9,1, 7,5,6],
        [6,4,1, 2,7,5, 9,8,3]
        ]
        每行的数字必须是1-9，每列的数字必须是1-9，每个3x3的子网格的数字必须是1-9。

        遍历每个格子，如果是空的，就尝试填入1-9，如果填入的数字合法，就继续递归下去，如果递归成功，就返回true，否则就回溯。

        搜索顺序优化
        或者全局找到一个空格，该空格可以填入的数字可能性最少，然后依次遍历该数字，再去尝试递归下去。确保了从最少可能性的格子先填
     */
    void solveSudoku(vector<vector<int>> &board) {
        // write your code here
        vector<set<int>> rows(9);
        vector<set<int>> cols(9);
        for (int r = 0; r < board.size(); ++r) {
            for (int c = 0; c < board[r].size(); ++c) {
                if (board[r][c] != 0) {
                    rows[r].insert(board[r][c]);
                    cols[c].insert(board[r][c]);
                }
            }
        }
        // solveSudoku_0(board, rows, cols, 0, 0);
        solveSudoku_1(board, rows, cols, 0);
        return;
    }

    bool valid_num_1(vector<vector<int>> &board,
                   int num, int r, int c,
                   vector<set<int>>& rows,
                   vector<set<int>>& cols) {
        for (int i = 0; i < 9; ++i) { // 直接在原来的基础上检查
            if (board[i][c] == num) {
                return false;
            }
            if (board[r][i] == num) {
                return false;
            }
            if (board[r / 3 * 3 + i / 3][c / 3 * 3 + i % 3] == num) { // 都能检查3 * 3
                return false;
            }
        }
        return true;
    }
    bool valid_num(vector<vector<int>> &board,
                   int num, int r, int c,
                   vector<set<int>>& rows,
                   vector<set<int>>& cols) {
        if (rows[r].count(num) > 0) { // count 太慢了
            return false;
        }
        if (cols[c].count(num) > 0) {
            return false;
        }

        // check 3 * 3
        int end_r = r / 3 * 3 + 2;
        int end_c = c / 3 * 3 + 2;
        if (end_r == r && end_c == c) {
            set<int> s;
            s.insert(num);
            for (int i = r - 2; i <= r; ++i) {
                for (int j = c - 2; j <= c; ++j) { // 有些格子其实没必要检查
                    if (board[i][j] != 0 && s.count(board[i][j]) > 0) {
                        return false;
                    }
                    s.insert(board[i][j]);
                }
            }
        }
        return true;
    }

    bool solveSudoku_1(vector<vector<int>> &board,
                       vector<set<int>>& rows,
                       vector<set<int>>& cols,
                       int ith) {
        if (ith == 81) {
            return true;
        }
        int r = ith / 9;
        int c = ith % 9;
        bool predefined = board[r][c] != 0;
        if (predefined) {
            return solveSudoku_1(board, rows, cols, ith + 1);
        }
        for (int num = 1; num <= 9; ++num) {
            if (!valid_num_1(board, num, r, c, rows, cols)) {
                continue;
            }
            board[r][c] = num;
            rows[r].insert(num);
            cols[c].insert(num);
            if (solveSudoku_1(board, rows, cols, ith + 1)) {
                return true;
            }
            board[r][c] = 0;
            rows[r].erase(num);
            cols[c].erase(num);
        }
        return false;
    }
    bool solveSudoku_0(vector<vector<int>> &board,
                       vector<set<int>>& rows,
                       vector<set<int>>& cols,
                       int r, int c) {
        // for (auto& i : board) {
        //     for (auto& j : i) {
        //     cout << j << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        if (r == board.size()) {
            if (c == board[r - 1].size())
            return true;
        }
        bool predefined = board[r][c] != 0;
        if (predefined) {
            if (!valid_num(board, board[r][c], r, c, rows, cols)) {
                return false;
            }
            if (c == board[r].size() - 1) {
                return solveSudoku_0(board, rows, cols, r + 1, 0);
            }
            return solveSudoku_0(board, rows, cols, r, c + 1);
        }
        for (int num = 1; num <= 9; ++num) {
            if (valid_num(board, num, r, c, rows, cols)) {
                board[r][c] = num;
                rows[r].insert(num);
                cols[c].insert(num);
                bool ret;
                if (c == board[r].size() - 1) {
                    ret = solveSudoku_0(board, rows, cols, r + 1, 0);
                } else {
                    ret = solveSudoku_0(board, rows, cols, r, c + 1);
                }
                if (ret == true) {
                    return true;
                }
                board[r][c] = 0;
                rows[r].erase(num);
                cols[c].erase(num);
            }
        }
        return false;
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
        s.solveSudoku(nums);
        cout << "permutaions: "<< endl;
        for (auto& i : nums) {
            for (auto& j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
        cout << endl;
    };

    nums3 = {
        {0,0,9,7,4,8,0,0,0},
        {7,0,0,0,0,0,0,0,0},
        {0,2,0,1,0,9,0,0,0},
        {0,0,7,0,0,0,2,4,0},
        {0,6,4,0,1,0,5,9,0},
        {0,9,8,0,0,0,3,0,0},
        {0,0,0,8,0,3,0,2,0},
        {0,0,0,0,0,0,0,0,6},
        {0,0,0,2,7,5,9,0,0}
    };
    test(nums3);
}