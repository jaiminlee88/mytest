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
     * @param a: a sparse matrix
     * @param b: a sparse matrix
     * @return: the result of A * B
     * 
     *  给定两个 稀疏矩阵 A 和 B，返回AB的结果。
        您可以假设A的列数等于B的行数。

        输入：
        [[1,0,0],[-1,0,3]]
        [[7,0,0],[0,0,0],[0,0,1]]
        输出：
        [[7,0,0],[-7,0,3]]
        解释：
        A = [
        [ 1, 0, 0],
        [-1, 0, 3]
        ]

        B = [
        [ 7, 0, 0 ],
        [ 0, 0, 0 ],
        [ 0, 0, 1 ]
        ]

-


             |  1 0 0 |   | 7 0 0 |   |  7 0 0 |
        AB = | -1 0 3 | x | 0 0 0 | = | -7 0 3 |
                          | 0 0 1 |

        输入：
        [[1,0],[0,1]]
        [[0,1],[1,0]]
        输出：
        [[0,1],[1,0]]

        如果是稀疏矩阵，可以只存储非0的元素，然后进行乘法
     */
    vector<vector<int>> multiply(vector<vector<int>> &a, vector<vector<int>> &b) {
        // write your code here
        return multiply_0(a, b);
    }

    vector<vector<int>> multiply_0(vector<vector<int>> &a, vector<vector<int>> &b) {
        if (a.size() < 1 || b.size() < 1) {
            return {};
        }
        vector<vector<int>> res(a.size(), vector<int>(b[0].size(), 0));
        for (int r = 0; r < a.size(); ++r) {
            for (int c = 0; c < b[0].size(); ++c) {
                for (int k = 0; k < a[0].size(); ++k) {
                    res[r][c] += a[r][k] * b[k][c];
                }
            }
        }
        return res;
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