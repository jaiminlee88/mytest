#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
using namespace std;

class Solution {
public:
    /**
     * @param grid: a boolean 2D matrix
     * @return: an integer
     * 给一个 01 矩阵，求不同的岛屿的个数。
        0 代表海，1 代表岛，如果两个 1 相邻，那么这两个 1 属于同一个岛。我们只考虑上下左右为相邻。
     */
    int numIslands(vector<vector<bool>> &grid) {
        // write your code here
        if (grid.size() == 0) {
            return 0;
        }
        int island_cnt = 0;
        int row = grid.size();
        int col = grid[0].size();
        std::unordered_map<string, int> visited_island;
        numIslands_1(grid, row, col, 0, 0, island_cnt, visited_island);
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                string island_num = to_string(i) + "," + to_string(j);
                if (grid[i][j] == true) {
                    cout << "(1)" << visited_island[island_num] << " ";
                } else {
                    cout << "(0)" << visited_island[island_num] << " ";
                }
            }
            cout << endl;
        }
        return island_cnt;
    }

    void numIslands_1(vector<vector<bool>> &grid, int row, int col, int cur_row, int cur_col,
                    int& island_cnt, std::unordered_map<string, int>& visited_island) {
        // DFS + BFS(扩张岛屿)
        for (int r = 0; r < row; ++r) {
            for (int c = 0; c < col; ++c) {
                string island_num = to_string(r) + "," + to_string(c);
                if (visited_island.find(island_num) != visited_island.end()) {
                    continue;
                }
                if (grid[r][c] == true) {
                    ++island_cnt;
                    trace_island(r, c, row, col, island_cnt, grid, visited_island);
                } else {
                    visited_island[island_num] = 0;
                }
            }
        }
    }

    void trace_island(int cur_row, int cur_col, int row, int col, int& island_cnt,
                     vector<vector<bool>> &grid, std::unordered_map<string, int>& visited_island) {
        if (cur_row < 0 || cur_row >= row || cur_col < 0 || cur_col >= col) {
            return;
        }
        if (grid[cur_row][cur_col] == false) {
            return;
        }
        string island_num = to_string(cur_row) + "," + to_string(cur_col);
        if (visited_island.find(island_num) != visited_island.end()) { // 已经访问过
            return;
        }
        if (grid[cur_row][cur_col] == true) {
            visited_island[island_num] = island_cnt;
            trace_island(cur_row - 1, cur_col, row, col, island_cnt, grid, visited_island);
            trace_island(cur_row + 1, cur_col, row, col, island_cnt, grid, visited_island);
            trace_island(cur_row, cur_col - 1, row, col, island_cnt, grid, visited_island);
            trace_island(cur_row, cur_col + 1, row, col, island_cnt, grid, visited_island);
        }
    }

    void trace_island(int cur_row, int cur_col, int row, int col,
                     vector<vector<bool>> &grid, vector<vector<bool>>& visited_island) {
        // DFS 递归方法 或 遍历法都可以
        if (cur_row < 0 || cur_row >= row || cur_col < 0 || cur_col >= col) {
            return;
        }
        if (grid[cur_row][cur_col] == false) {
            return;
        }
        if (visited_island[cur_row][cur_col] == true) { // 已经访问过
            return;
        }
        visited_island[cur_row][cur_col] = true;
        trace_island(cur_row - 1, cur_col, row, col, grid, visited_island);
        trace_island(cur_row + 1, cur_col, row, col, grid, visited_island);
        trace_island(cur_row, cur_col - 1, row, col, grid, visited_island);
        trace_island(cur_row, cur_col + 1, row, col, grid, visited_island);
    }
    int numIslands_2(vector<vector<bool>> &grid) {
         // write your code here
        if (grid.size() == 0) {
            return 0;
        }
        vector<vector<bool>> visited;
        visited.resize(grid.size(), vector<bool>(grid[0].size(), false));
        int row = grid.size();
        int col = grid[0].size();
        int island_cnt = 0;
        for (int r = 0; r < row; ++r) {
            for (int c = 0; c < col; ++c) {
                if (visited[r][c] == true) {
                    continue;
                }
                if (grid[r][c] == true) {
                    island_cnt++;
                    trace_island(r, c, row, col, grid, visited);
                    continue;
                }
                visited[r][c] = true;
            }
        }
        return island_cnt;
    }
};


int main() {
    Solution s;
    vector<vector<bool>> grid;
    auto test_island = [&s] (vector<vector<bool>>& grid) {
        cout << s.numIslands_2(grid) << endl;
    };

    grid = {{true , false},
            {false, true}};
    test_island(grid); // 2

    grid = {{false, true , false, false, true},
            {true , true , false, false, false},
            {false, false, true , false, false},
            {false, false, false, true , true},
            {false, false, false, false, true}}; // 4
    test_island(grid);

    grid = {};
    test_island(grid); // 0

    grid = {{true}};
    test_island(grid); // 1

    grid = {{true, false, true}};
    test_island(grid); // 1

    grid = {{false, true , false, true , false},
            {true , true , false, false, false},
            {false, false, false, true , false},
            {false, false, false, true , false}}; // 3
    test_island(grid);

    grid = {{1,1,1,1,1,1},{1,0,0,0,0,1},{1,0,1,1,0,1},{1,0,0,0,0,1},{1,1,1,1,1,1}};
    test_island(grid);

    grid = {{1,1,1,1,1},
            {1,0,0,0,1},
            {1,0,1,0,1},
            {1,0,0,0,1},
            {1,1,1,1,1}};
    test_island(grid);
    return 0;
}