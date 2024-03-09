#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <list>
#include <climits>

using namespace std;

class Solution {
public:
    /**
     * @param grid: a 2D grid
     * @return: An integer
     * Build Post Office II
        Given a 2D grid, each cell is either a wall 2, 
        an house 1 or empty 0 (the number zero, one, two), find a place to build a post office so that the sum of the distance from the post office to all the houses is smallest.

        Return the smallest sum of distance. Return -1 if it is not possible.

        Example
        Example 1:
        Input：[[0,1,0,0,0],[1,0,0,2,1],[0,1,0,0,0]]
        Output：8
        Explanation： Placing a post office at (1,1), the distance that post office to all the house sum is smallest.

        Example 2:
        Input：[[0,1,0],[1,0,1],[0,1,0]]
        Output：4
        Explanation： Placing a post office at (1,1), the distance that post office to all the house sum is smallest.
        Challenge
        Solve this problem within O(n^3) time.

        Notice
        You cannot pass through wall and house, but can pass through empty.
        You only build post office on an empty.

        从每个1开始，使用BFS遍历，记录到每个0的距离，然后累加，找到最小的距离
        但是要注意，如果有的0是不可达的，那么就不能放post office
     */
    int shortestDistance(vector<vector<int>> &grid) {
        if (grid.size() == 0 || grid[0].size() == 0) {
            return -1;
        }
        int house_cnt = 0;
        std::vector<std::vector<int>> distance(grid.size(), std::vector<int>(grid[0].size(), 0));
        std::unordered_map<int, int> house_cpy;
        std::vector<std::pair<int, int>> house_pos;
        std::queue<std::pair<int, int>> q;
        std::unordered_set<int> visited;
        std::vector<std::pair<int,int>> delta = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        for (int i  = 0; i < grid.size(); ++i) {
            for (int j =0; j < grid[0].size(); ++j) {
                if (grid[i][j] == 1) {
                    ++house_cnt;
                    distance[i][j] = 0;
                    house_pos.push_back(std::pair<int, int>(i, j));
                } else if (grid[i][j] == 2) {
                    distance[i][j] = -1;
                } else {
                    house_cpy[grid[0].size() * i + j] = 0;
                }
            }
        }
        if (house_cnt == 0) {
            return 0;
        }

        // BFS
        for (auto& pos : house_pos) {
            visited.clear();
            q.push(pos);
            visited.insert(grid[0].size() * pos.first + pos.second); // 可写可不写
            int step = 0;
            while (q.empty() == false) {
                ++step;
                int size = q.size();
                for (int i = 0; i < size; ++i) {
                    auto pos = q.front();
                    q.pop();
                    for (auto& dlt : delta) {
                        int r = pos.first + dlt.first;
                        int c = pos.second + dlt.second;
                        if (r < 0 || r >= grid.size() || c < 0 || c >= grid[0].size()
                        || grid[r][c] != 0
                        || visited.find(grid[0].size() * r + c) != visited.end()) {
                            continue;
                        }
                        q.push(std::pair<int, int>(r, c));
                        visited.insert(grid[0].size() * r + c);
                        distance[r][c] += step;
                        ++house_cpy[grid[0].size() * r + c];

                        // cout << "distance[" << pos.first << "," << pos.second << "] = " << distance[pos.first][pos.second] << " -> distance[" << r << "," << c << "] = " << distance[r][c] << endl;
                    }
                }
            }
        }

        // cout << "distance:" << endl;
        // for (auto& it : distance) {
        //     for (auto& it_1 : it) {
        //         std::cout << it_1 << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // for (auto& it : house_cpy) {
        //     std::cout << it.first << " house_cpy: " << it.second << std::endl;
        // }

        int min_distance = INT_MAX;
        std::pair<int, int> min_pos;
        for (int r = 0; r < grid.size(); ++r) {
            for (int c = 0; c < grid[0].size(); ++c) {
                if (house_cpy[grid[0].size() * r + c] == house_cnt) {
                    cout << "r=" << r << " c=" << c << " house_cpy=" << (grid[0].size()*r +c) << " distance=" << distance[r][c] << endl;
                    if (distance[r][c] < min_distance) {
                        min_distance = distance[r][c];
                        min_pos = std::pair<int, int>(r, c);
                    }
                }
            }
        }
        cout << "distance[" << min_pos.first << "," << min_pos.second << "]=" << min_distance << endl;
        return min_distance == INT_MAX ? -1 : min_distance;
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
        auto ret = s.shortestDistance(grid);
        cout << "shortest distance = " << ret << endl;
    };

    grid = {{0,1,0,0,0},
            {1,0,0,2,1},
            {0,1,0,0,0}}; // 8
    test(grid);

    grid = {{0,1,0},{1,0,1},{0,1,0}}; // 4
    test(grid);

    return 0;
}