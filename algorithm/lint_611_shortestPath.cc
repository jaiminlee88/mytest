#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
using namespace std;

/**
 * Definition for a point.
 * struct Point {
 *     int x;
 *     int y;
 *     Point() : x(0), y(0) {}
 *     Point(int a, int b) : x(a), y(b) {}
 * };
 */

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int a, int b) : x(a), y(b) {}
};

class Solution {
public:
    /**
     * @param grid: a chessboard included 0 (false) and 1 (true)
     * @param source: a point
     * @param destination: a point
     * @return: the shortest path 
     * 给定骑士在棋盘上的 初始 位置(一个2进制矩阵 0 表示空 1 表示有障碍物)，
     * 找到到达 终点 的最短路线，返回路线的长度。如果骑士不能到达则返回 -1 
     * x和y，只能一个移动一步，一个移动两步
     */
    vector<Point> delta = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2},
                           {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
    bool validPoint(Point p, vector<vector<bool>> &grid, vector<vector<bool>> &visited) {
        if (p.x < 0 || p.x >= grid.size() || p.y < 0 || p.y >= grid[0].size()) {
            return false;
        }
        if (grid[p.x][p.y] == true || visited[p.x][p.y] == true) {
            return false;
        }
        return true;
    }

    int shortestPath(vector<vector<bool>> &grid, Point source, Point destination) {
        // write your code here
        if (grid.size() == 0) {
            return -1;
        }
        int row = grid.size();
        int col = grid[0].size();
        if (!validPoint(source) || !validPoint(destination)) {
            return -1;
        }
        int step = -1;
        vector<vector<bool>> visited; // 未必是最好的，也许可以map
        visited.resize(row, vector<bool>(col, false));
        std::queue<Point> q;
        q.push(source);
        visited[source.x][source.y] = true;
        while (!q.empty()) {
            int size = q.size();
            ++step;
            // cout << "---" << endl;
            for (int i = 0; i < size; ++i) {
                Point cur = q.front();
                q.pop();
                // cout << "cur.x: " << cur.x << " cur.y: " << cur.y << endl;
                if (cur.x == destination.x && cur.y == destination.y) {
                    return step;
                }
                for (auto& d : delta) {
                    Point new_point = cur;
                    new_point.x += d.x;
                    new_point.y += d.y;
                    if (!validPoint(new_point, grid, visited)) {
                        continue;
                    }
                    q.push(new_point);
                    visited[new_point.x][new_point.y] = true;
                }
            }
        }
        return -1;
    }
};


int main() {
    Solution s;
    vector<vector<bool>> grid;
    Point src(0, 0);
    Point dst(0, 1);
    auto test = [&s] (vector<vector<bool>>& grid, Point source, Point destination) {
        cout << "============" << endl;
        for (auto row : grid) {
            for (auto col : row) {
                cout << col << " ";
            }
            cout << endl;
        }
        cout << s.shortestPath(grid, source, destination) << endl;
    };

    grid = {{0,0,0},
            {0,0,0},
            {0,0,0}};
    src = {2,0};
    dst = {2,2};
    test(grid, src, dst); // 2

    src = {0,0};
    dst = {2,2};
    test(grid, src, dst); // 2

    grid = {{0,1,0},
            {0,0,1},
            {0,0,0}};
    src = {2,0};
    dst = {2,2};
    test(grid, src, dst); // 2
return 0;
    grid = {{1, 0},
            {0, 1}};
    src = {0,0};
    dst = {0,1};
    test(grid, src, dst); // 2

    grid = {{0, 1, 0, 0, 1},
            {1, 1, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 1, 1},
            {0, 0, 0, 0, 1}}; // 4
    src = {0,0};
    dst = {4,4};
    test(grid, src, dst);

    grid = {};
    src = {0,0};
    dst = {0,1};
    test(grid, src, dst); // 0

    grid = {{1}};
    src = {0,0};
    dst = {0,1};
    test(grid, src, dst); // 1

    grid = {{1, 0, 1}};
    src = {0,2};
    dst = {0,2};
    test(grid, src, dst); // 1

    grid = {{0, 1, 0, 1, 0},
            {1, 1, 0, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 0, 1, 0}}; // 3
    src = {0,2};
    dst = {3,3};
    test(grid, src, dst);

    grid = {{1,1,1,1,1,1},
            {1,0,0,0,0,1},
            {1,0,1,1,0,1},
            {1,0,0,0,0,1},
            {1,1,1,1,1,1}};
    src = {1,1};
    dst = {3,4};
    test(grid, src, dst);

    grid = {{1,1,1,1,1},
            {1,0,0,0,1},
            {1,0,1,0,1},
            {1,0,0,0,1},
            {1,1,1,1,1}};
    src = {1,1};
    dst = {3,3};
    test(grid, src, dst);
    return 0;
}