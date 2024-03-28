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
using namespace std;


class Solution {
public:
    /*
    有 n 个城市，其中一些彼此相连，另一些没有相连。如果城市 a 与城市 b 直接相连，
    且城市 b 与城市 c 直接相连，那么城市 a 与城市 c 间接相连。
    省份 是一组直接或间接相连的城市，组内不含其他没有相连的城市。
    给你一个 n x n 的矩阵 isConnected ，其中 isConnected[i][j] = 1 表示第 i 个城市和第 j 个城市
    直接相连，而 isConnected[i][j] = 0 表示二者不直接相连。
    返回矩阵中 省份 的数量。

    输入：isConnected = [[1,1,0],[1,1,0],[0,0,1]]
    输出：2

    输入：isConnected = [[1,0,0],[0,1,0],[0,0,1]]
    输出：3

    从任意省份触发，采用BFS方式遍历树，如果发现还有没遍历完的树，就是新的省份
    注意，为了防止重复遍历，需要一个visited数组

    */
    int findCircleNum(vector<vector<int>>& isConnected) {
        if (isConnected.size() == 0) {
            return 0;
        }
        vector<bool> visited(isConnected.size(), false);
        queue<int> q;
        int cnt = 0;
        for (int i = 0; i < isConnected.size(); i++) {
            if (visited[i]) {
                continue;
            }
            // BFS
            q.push(i);
            visited[i] = true;
            ++cnt;
            while (q.size()) {
                int size = q.size();
                for (int c = 0; c < size; ++c) {
                    auto x = q.front();
                    q.pop();
                    for (int y = 0; y < isConnected.size(); ++y) {
                        if (x!= y && isConnected[x][y] == 1 && !visited[y]) {
                            q.push(y);
                            visited[y] = true;
                        }
                    }
                }
            }
        }
        return cnt;
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
        auto res = s.findCircleNum(nums);
        cout << "permutaions: "<< res << endl;
    };

    nums = {{1,1,0},{1,1,0},{0,0,1}};
    test(nums);

    nums = {{1,0,0},{0,1,0},{0,0,1}};
    test(nums);
}