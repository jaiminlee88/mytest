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
#include <map>
using namespace std;


class Solution {
public:
    /**
     * @param maze: the maze
     * @param start: the start
     * @param destination: the destination
     * @return: the shortest distance for the ball to stop at the destination
     * 
     * 在迷宫中有一个球，里面有空的空间和墙壁。球可以通过滚上，下，左或右移动，
     * 但它 不会停止滚动直到撞到墙上 。当球停止时，它可以选择下一个方向。
        给定球的起始位置，目标和迷宫，找到最短距离的球在终点停留。
        距离是由球从起始位置(被排除)到目的地(包括)所走过的空空间的数量来定义的。
        如果球不能停在目的地，返回-1。
        迷宫由二维数组表示。1表示墙和0表示空的空间。你可以假设迷宫的边界都是墙。开始和目标坐标用行和列索引表示。


        1.在迷宫中只有一个球和一个目的地。
        2.球和目的地都存在于一个空的空间中，它们最初不会处于相同的位置。
        3.给定的迷宫不包含边框(比如图片中的红色矩形)，但是你可以假设迷宫的边界都是墙。
        4.迷宫中至少有2个空的空间，迷宫的宽度和高度都不会超过100。
     * 样例 1:
	输入:  
	(rowStart, colStart) = (0,4)
	(rowDest, colDest)= (4,4)
	0 0 1 0 0
	0 0 0 0 0
	0 0 0 1 0
	1 1 0 1 1
	0 0 0 0 0

	输出:  12
	
	解释:
	(0,4)->(0,3)->(1,3)->(1,2)->(1,1)->(1,0)->(2,0)->(2,1)->(2,2)->(3,2)->(4,2)->(4,3)->(4,4)

样例 2:
	输入:
	(rowStart, colStart) = (0,4)
	(rowDest, colDest)= (0,0)
	0 0 1 0 0
	0 0 0 0 0
	0 0 0 1 0
	1 1 0 1 1
	0 0 0 0 0

	输出:  6
	
	解释:
	(0,4)->(0,3)->(1,3)->(1,2)->(1,1)->(1,0)->(0,0)
	
     */

    bool valid_pos(vector<vector<int>>& maze, set<pair<int,int>>& visited, int x, int y) {
        return x >= 0 && x < maze.size() && y >= 0 && y < maze[0].size()
              && maze[x][y] == 0 && visited.find({x, y}) == visited.end();
    }

    vector<pair<int,int>> deltas = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    int shortestDistance(vector<vector<int>> &maze, vector<int> &start, vector<int> &destination) {
        // write your code here
        // return shortestDistance_0(maze, start, destination); // BFS，没有考虑球的惯性，不能解决问题
        // return shortestDistance_1(maze, start, destination); // 有点问题
        return shortestDistance_2(maze, start, destination); // BFS，考虑球的惯性，可以解决问题
        // return shortestDistance_3(maze, start, destination); // BFS，别人写的
    }

    int shortestDistance_3(vector<vector<int>> &maze, vector<int> &start, vector<int> &destination) {
        // write your code here
        int rows = maze.size();
        if (rows == 0)
            return false;
        int cols = maze[0].size();
        maze.insert(maze.begin(), vector<int>(cols, 1));
        maze.push_back(vector<int>(cols, 1));
        rows += 2;
        for (int i = 0; i < rows; i ++)
        {
            maze[i].insert(maze[i].begin(), 1);
            maze[i].push_back(1);
        }
        cols += 2;
        vector<vector<int>> dp(rows, vector<int>(cols, INT_MAX));
        int sx = start[0] + 1;
        int sy = start[1] + 1;
        int dx = destination[0] + 1;
        int dy = destination[1] + 1;
        dp[sx][sy] = 0;
        queue<pair<int, int>> q;
        q.push(make_pair(sx, sy));
        return findRoute(maze, q, dp, dx, dy);
    }

    int findRoute(vector<vector<int>>& maze, queue<pair<int, int>> q, vector<vector<int>>& dp, int dx, int dy) {
        while (!q.empty()) {
            int x = q.front().first;
            int y = q.front().second;
            q.pop();
            int tmpx = x;
            int tmpy = y;
            while (maze[tmpx][tmpy] != 1)
            {
                tmpx --;
            }
            if (dp[tmpx+1][tmpy] > dp[x][y] + (x-tmpx-1))
            {
                dp[tmpx+1][tmpy] = dp[x][y] + (x-tmpx-1);
                q.push(make_pair(tmpx+1, tmpy));
            }
            tmpx = x;
            tmpy = y;
            while (maze[tmpx][tmpy] != 1)
            {
                tmpx ++;
            }
            if (dp[tmpx-1][tmpy] > dp[x][y] + (tmpx-1-x))
            {
                dp[tmpx-1][tmpy] = dp[x][y] + (tmpx-1-x);
                q.push(make_pair(tmpx-1, tmpy));
            }
            tmpx = x;
            tmpy = y;
            while (maze[tmpx][tmpy] != 1)
            {
                tmpy --;
            }
            if (dp[tmpx][tmpy+1] > dp[x][y] + (y-tmpy-1))
            {
                dp[tmpx][tmpy+1] = dp[x][y] + (y-tmpy-1);
                q.push(make_pair(tmpx, tmpy+1));
            }
            tmpx = x;
            tmpy = y;
            while (maze[tmpx][tmpy] != 1)
            {
                tmpy ++;
            }
            if (dp[tmpx][tmpy-1] > dp[x][y] + (tmpy-1-y))
            {
                dp[tmpx][tmpy-1] = dp[x][y] + (tmpy-1-y);
                q.push(make_pair(tmpx, tmpy-1));
            }
        }
        return dp[dx][dy] == INT_MAX ? -1 : dp[dx][dy];
    }

    int shortestDistance_1(vector<vector<int>> &maze, vector<int> &start, vector<int> &destination) {
        // DFS, 注意，球运动有惯性，直至撞墙才停止，所以DFS
        if (maze.size() < 1 || maze[0].size() < 1) {
            return -1;
        }
        set<pair<int,int>> visited;
        if (valid_pos(maze, visited, start[0], start[1]) == false ||
            valid_pos(maze, visited, destination[0], destination[1]) == false) {
            return -1;
        }
        if (start[0] == destination[0] && start[1] == destination[1]) {
            return 0;
        }
        int distance = 0;
        int min_distance = INT32_MAX;
        visited.insert({start[0], start[1]});
        shortestDistance_1_core(maze, visited, {start[0],start[1]}, {destination[0], destination[1]}, distance, min_distance);
        return min_distance == INT32_MAX ? -1 : min_distance;
    }

    pair<int,int> next_pos(vector<vector<int>>& maze, pair<int,int> pos, pair<int,int> delta, pair<int,int> dst) {
        int x = pos.first + delta.first;
        int y = pos.second + delta.second;
        while (true) {
            if (x == dst.first && y == dst.second) {
                return {x, y};
            }
            if (x >= 0 && x < maze.size() && y >= 0 && y < maze[0].size() && maze[x][y] == 0) {
                x += delta.first;
                y += delta.second;
            } else {
                break;
            }
        }
        return {x - delta.first, y - delta.second};
    }
    void shortestDistance_1_core(vector<vector<int>> &maze, set<pair<int,int>>& visited, pair<int,int> start, pair<int,int> destination, int& distance, int& min_distance) {
        if (start.first == destination.first && start.second == destination.second) {
            min_distance = min(min_distance, distance);
            return;
        }
        // cout << "start: (" << start.first << "," << start.second << ") " << " distance: " << distance << endl;
        set<pair<int,int>> visited_tmp;
        for (auto& delta : deltas) {
            auto next = next_pos(maze, start, delta);
            if (visited.find(next) != visited.end()) {
                continue;
            }
            visited.insert(next);
            distance += abs(next.first - start.first) + abs(next.second - start.second);
            shortestDistance_1_core(maze, visited, next, destination, distance, min_distance);
            distance -= abs(next.first - start.first) + abs(next.second - start.second);
            visited.erase(next);
        }
        return;
    }


    int shortestDistance_0(vector<vector<int>> &maze, vector<int> &start, vector<int> &destination) {
        // BFS, 因为每个方向的步数不一样，需要用map记录每个位置的步数
        if (maze.size() < 1 || maze[0].size() < 1) {
            return -1;
        }
        set<pair<int,int>> visited;
        queue<pair<int,int>> q;
        if (valid_pos(maze, visited, start[0], start[1]) == false ||
            valid_pos(maze, visited, destination[0], destination[1]) == false) {
            return -1;
        }
        if (start[0] == destination[0] && start[1] == destination[1]) {
            return 0;
        }
        int distance = 0;
        // BFS，球会一直滚动，直至撞墙才停止
        q.push({start[0], start[1]});
        visited.insert({start[0], start[1]});
        while (true) {
            int size = q.size();
            if (size == 0) {
                break;
            }
            for (int i = 0; i < size; ++i) {
                auto pos = q.front();
                cout << "pos: (" << pos.first << "," << pos.second << ") ";
                q.pop();
                if (pos.first == destination[0] && pos.second == destination[1]) {
                    return distance;
                }
                for (auto& delta : deltas) {
                    int x = pos.first + delta.first;
                    int y = pos.second + delta.second;
                    if (valid_pos(maze, visited, x, y)) {
                        q.push({x, y});
                        visited.insert({x, y});
                    }
                }
            }
            ++distance;
            cout << endl;
        }
        return -1;
    }

    pair<int,int> next_pos(vector<vector<int>>& maze, pair<int,int> pos, pair<int,int> delta) {
        int x = pos.first + delta.first;
        int y = pos.second + delta.second;
        while (x >= 0 && x < maze.size() && y >= 0 && y < maze[0].size() && maze[x][y] == 0) {
            x += delta.first;
            y += delta.second;
        }
        return {x - delta.first, y - delta.second};
    }

    int shortestDistance_2(vector<vector<int>> &maze, vector<int> &start, vector<int> &destination) {
        // BFS, 因为每个方向的步数不一样，需要用map记录每个位置的步数
        if (maze.size() < 1 || maze[0].size() < 1) {
            return -1;
        }
        set<pair<int,int>> visited;
        queue<pair<int,int>> q;
        if (valid_pos(maze, visited, start[0], start[1]) == false ||
            valid_pos(maze, visited, destination[0], destination[1]) == false) {
            return -1;
        }
        if (start[0] == destination[0] && start[1] == destination[1]) {
            return 0;
        }
        map<pair<int,int>, int> distance;
        distance[{start[0], start[1]}] = 0;
        // BFS，球会一直滚动，直至撞墙才停止
        q.push({start[0], start[1]});
        while (q.size() > 0) {
            int size = q.size();
            int step  = INT32_MAX;
            for (int i = 0; i < size; ++i) {
                auto pos = q.front();
                // cout << "pos: (" << pos.first << "," << pos.second << ") ";
                q.pop();
                // if (pos.first == destination[0] && pos.second == destination[1]) {
                //     return distance[pos];
                // }
                for (auto& delta : deltas) {
                    auto next = next_pos(maze, pos, delta);
                    cout << "pos: (" << next.first << "," << next.second << ") ";
                    int step_tmp =  distance[pos] + abs(next.first - pos.first) + abs(next.second - pos.second);
                    if (distance.find(next) != distance.end() && distance[next] <= step_tmp) {
                        continue; // 实际上得不出最优路径
                    }
                    distance[next] = distance[pos] + abs(next.first - pos.first) + abs(next.second - pos.second);
                    q.push(next);
                }
            }
            cout << endl;
        }
        if (distance.find({destination[0], destination[1]}) != distance.end()) {
            return distance[{destination[0], destination[1]}];
        }
        return -1;
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
    auto test = [&s](vector<vector<int>>& nums, vector<int>& nums0, vector<int>& nums1) {
        cout << "------------------" << endl;
        cout << "nums: " << endl;
        for (auto& i : nums) {
            for (auto& j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
        cout << "start: ("<< nums0[0] << "," << nums0[1] << ")  end: (" << nums1[0] << "," << nums1[1] << ")" << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.shortestDistance(nums, nums0, nums1);
        cout << "permutaions: " << res << endl;
    };

    // nums3 = {
    //     {0},
    // };
    // nums0 = {0,0};
    // nums1 = {0,0};
    // test(nums3, nums0, nums1);

    // nums3 = {
    //     {1},
    // };
    // nums0 = {0,0};
    // nums1 = {0,0};
    // test(nums3, nums0, nums1);

    nums3 = {
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 0, 1, 1},
        {0, 0, 0, 0, 0}
    };
    nums0 = {0,4};
    nums1 = {4,4};
    test(nums3, nums0, nums1);

    nums3 = {
        {0,0,1,0,0},
	    {0, 0, 0, 0, 0},
	    {0, 0, 0, 1, 0},
	    {1, 1, 0, 1, 1},
	    {0, 0, 0, 0, 0}
    };
    nums0 = {0,4};
    nums1 = {0,0};
    test(nums3, nums0, nums1);

    nums3 = {
        {0,0,0,1,1,1,1,1},
        {0,1,0,0,0,0,0,0},
        {0,1,1,1,1,1,1,0},
        {0,0,0,0,0,0,0,0}
    };
    nums0 = {3,0};
    nums1 = {1,2};
    test(nums3, nums0, nums1);

    // nums3 = {
    //     {1,0},
    //     {0,1}
    // };
    // nums0 = {0,1};
    // nums1 = {1,0};
    // test(nums3, nums0, nums1);

    // nums3 = {
    //     {1,0},
    //     {0,0}
    // };
    // nums0 = {0,1};
    // nums1 = {1,0};
    // test(nums3, nums0, nums1);

    // nums3 = {
    //     {0,0},
    //     {0,0},
    //     {0,0},
    //     {0,0},
    //     {0,1},
    //     {1,0},
    // };
    // nums0 = {0,1};
    // nums1 = {5,1};
    // test(nums3, nums0, nums1);
}