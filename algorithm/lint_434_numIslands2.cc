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

/**
 * Definition for a point.
 * struct Point {
 *     int x;
 *     int y;
 *     Point() : x(0), y(0) {}
 *     Point(int a, int b) : x(a), y(b) {}
 * };
 */

class Solution {
public:
    /**
     * @param n: An integer
     * @param m: An integer
     * @param operators: an array of point
     * @return: an integer array
     * 
     * 给定 n, m, 分别代表一个二维矩阵的行数和列数, 
     * 并给定一个大小为 k 的二元数组A. 初始二维矩阵全0. 
     * 二元数组A内的k个元素代表k次操作, 设第i个元素为 (A[i].x, A[i].y), 
     * 表示把二维矩阵中下标为A[i].x行A[i].y列的元素由海洋变为岛屿. 
     * 问在每次操作之后, 二维矩阵中岛屿的数量. 你需要返回一个大小为k的数组.
     * 
        输入: n = 4, m = 5, A = [[1,1],[0,1],[3,3],[3,4]]
        输出: [1,1,2,2]
        解释: 
        0.  00000
            00000
            00000
            00000
        1.  00000
            01000
            00000
            00000
        2.  01000
            01000
            00000
            00000
        3.  01000
            01000
            00000
            00010
        4.  01000
            01000
            00000
            00011

        输入: n = 3, m = 3, A = [[0,0],[0,1],[2,2],[2,1]]
        输出: [1,1,2,2]

        未完成
     */
    vector<int> numIslands2(int n, int m, vector<Point> &operators) {
        // write your code here
    }
};


int main() {
    Solution s;
    vector<int> nums;
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