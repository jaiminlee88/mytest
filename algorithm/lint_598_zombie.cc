#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <list>

using namespace std;

class Solution {
public:
    /**
     * @param grid: a 2D integer grid
     * @return: an integer
     * Given a 2D grid, each cell is either 
     * a wall 2, a zombie 1 or people 0 (the number zero, one, two).
     * Zombies can turn the nearest people(up/down/left/right) into zombies every day,
     * but can not through wall. How long will it take to turn all people into zombies?
     * Return -1 if can not turn all people into zombies.
     * 找到所有zombie的位置，对于每个zombie，找到最近的人(一步)，上下左右一直感染，直至碰到墙或者边界
     * 直到所有人都被感染
     * 可以用BFS来做,将所有的zombie同时放入queue中，然后就跟正常bfs一样了。
     */

    bool infect(int r, int c, vector<vector<int>>& grid, int& human_cnt) {
        if (r < 0 || r >= grid.size() || c < 0 || c >= grid[0].size()) {
            return false;
        }
        if (grid[r][c] == 0) {
            grid[r][c] = 1;
            human_cnt--;
            return true;
        }
        return false;
    }
    int zombie(vector<vector<int>> &grid) {
        if (grid.empty() || grid[0].empty()) {
            return -1;
        }
        int zombie_cnt = 0;
        int human_cnt = 0;
        std::list<std::pair<int, int>> lq; // zombie
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[0].size(); ++j) {
                if (grid[i][j] == 1) {
                    lq.push_back(std::pair<int, int>(i, j));
                    zombie_cnt++;
                } else if (grid[i][j] == 0) {
                    human_cnt++;
                }
            }
        }
        if (human_cnt == 0) {
            return 0;
        }
        if (zombie_cnt == 0) {
            return -1;
        }
        int days = 0;
        while (lq.size() > 0) {
            ++days;
            int size = lq.size();
            bool found = false;
            std::list<std::pair<int,int>>::iterator iter = lq.begin();
            for (int i = 0; i < size; ++i) {
                int r = (*iter).first;
                int c = (*iter).second;
                if (infect(r - 1, c, grid, human_cnt)) {
                    lq.push_back(std::pair<int, int>(r - 1, c));
                    found = true;
                }
                if (infect(r + 1, c, grid, human_cnt)) {
                    lq.push_back(std::pair<int, int>(r + 1, c));
                    found = true;
                }
                if (infect(r, c - 1, grid, human_cnt)) {
                    lq.push_back(std::pair<int, int>(r, c - 1));
                    found = true;
                }
                if (infect(r, c + 1, grid, human_cnt)) {
                    lq.push_back(std::pair<int, int>(r, c + 1));
                    found = true;
                }
                auto iter_tmp = iter;
                iter++;
                lq.erase(iter_tmp);
            }
            if (!found) {
                days--;
                break;
            }
        }
        return human_cnt == 0 ? days : -1;
    }

    int zombie_1(vector<vector<int>> &grid) {
        // BFS
        int m = grid.size();
        if (m == 0) return 0;
        int n = grid[0].size();

        queue<pair<int, int>> q;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == 1) 
                    q.push({i, j});
            }
        }

        vector<int> dx = {1, -1, 0, 0};
        vector<int> dy = {0, 0, 1, -1};
        int round = 0;
        while(!q.empty()) {
            int qSize = q.size();
            round++;
            for (int i = 0; i < qSize; ++i) {
                pair<int, int> frontNode = q.front();
                q.pop();
                for (int j = 0; j < 4; ++j) {
                    int newX = frontNode.first + dx[j];
                    int newY = frontNode.second + dy[j];
                    if (newX >= 0 && newX < m && newY >= 0 && newY < n && grid[newX][newY] == 0) {
                        // if the cell is not a wall, we convert human to zombie
                        q.push({newX, newY});
                        grid[newX][newY] = 1;
                    }
                }
            }
        }
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == 0) return -1;
            }
        }
        return round - 1;
    }
};

int main(){
    Solution s;
    vector<vector<int>> grid;
    int ret;

    auto test = [&s](vector<vector<int>>& grid){
        cout << "grid------------------" << endl;
        if (grid.empty()) {
            cout << "{}" << endl;
        } else {
            for (auto& it : grid) {
                for (auto& it_1 : it) {
                    cout << it_1 << " ";
                }
                cout << endl;
            }
        }
        cout << "days = " << s.zombie(grid) << endl;
    };

    grid = {{0,1,2,0,0},
            {1,0,0,2,1},
            {0,1,0,0,0}}; // 2
    test(grid);

    grid = {{0,0,0},
            {0,0,0},
            {0,0,1}}; // 4
    test(grid);

    grid = {{0,0,0},
            {0,0,0},
            {0,0,0}}; // 0
    test(grid);

    grid  = {};
    test(grid);

    grid = {{1}};
    test(grid);

    grid = {{0,2,1}};
    test(grid);

    return 0;
}