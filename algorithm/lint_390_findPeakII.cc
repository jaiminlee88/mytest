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
    /*
    给定一个整数矩阵 A, 它有如下特性:
    相邻的整数不同
    矩阵有 n 行 m 列。
    对于所有的 i < n, 都有 A[i][0] < A[i][1] && A[i][m - 2] > A[i][m - 1]
    对于所有的 j < m, 都有 A[0][j] < A[1][j] && A[n - 2][j] > A[n - 1][j]
    我们定义一个位置 [i,j] 是峰值, 当且仅当它满足:

       A[i][j] > A[i + 1][j]
    && A[i][j] > A[i - 1][j]
    && A[i][j] > A[i][j + 1]
    && A[i][j] > A[i][j - 1]
    找到该矩阵的一个峰值元素, 返回它的坐标.

    样例 1:
    输入:
    [
      [1, 2, 3, 6,  5],
      [16,41,23,22, 6],
      [15,17,24,21, 7],
      [14,18,19,20,10],
      [13,14,11,10, 9]
    ]
    输出: [1,1]
    解释: [2,2] 也是可以的. [1,1] 的元素是 41, 大于它四周的每一个元素 (2, 16, 23, 17).

    样例 2:
    输入:
        [
        [1, 5, 3],
        [4,10, 9],
        [2, 8, 7]
        ]
    输出: [1,1]
    解释: 只有这一个峰值

    挑战
    O(n+m) 的时间复杂度.
    如果你 认为 你使用了 O(nlogm) 或 O(mlogn) 的算法,
    能否证明它的复杂度其实是 O(n+m)?
    或者想一个类似的算法但是复杂度是O(n+m)？

    注意事项
    保证至少存在一个峰值, 而如果存在多个峰值, 返回任意一个即可.

    暴力法：找一个行列方向看都是峰值的值，从第二行第二列开始，如果不是峰值，就往大的方向走，直到找到峰值

    找到一行最大值，这个值一定是该行的峰值，然后在该列找到最大值，这个值一定是该列的峰值
    */
public:
    vector<int> findPeakII(vector<vector<int>> &A) {
        // return findPeakII_0(A);
        return findPeakII_1(A);
    }

    vector<int> findPeakII_1(vector<vector<int>>& A) {
        if (A.size() < 3 || A[0].size() < 3) {
            return {};
        }

        int up = 0;
        int down = A.size() - 1;
        while (up < down) {
            int mid = up + (down - up) / 2;
            int max_num = A[mid][0];
            int max_idx = 0;
            for (int c = 1; c < A[mid].size() - 1; ++c) {
                // 找到该行所有峰值
                if (A[mid][c] > max_num) {
                    max_num = A[mid][c];
                    max_idx = c;
                }
            }
            if (A[mid][max_idx] > A[mid - 1][max_idx] && A[mid][max_idx] > A[mid + 1][max_idx]) {
                return {mid, max_idx};
            }
            if (A[mid][max_idx] < A[mid - 1][max_idx]) {
                down = mid;
            } else {
                up = mid;
            }
        }
        return {};
    }

    vector<int> findPeakII_0(vector<vector<int>> &A) {
        // 暴力法
        if (A.size() < 3 || A[0].size() < 3) {
            return {};
        }

        for (int r = 1; r < A.size() - 1; ++r) {
            for (int c = 1; c < A[0].size() - 1; ++c) {
                if (A[r][c] > A[r - 1][c] && A[r][c] > A[r + 1][c] && A[r][c] > A[r][c - 1] && A[r][c] > A[r][c + 1]) {
                    return {r, c};
                }
            }
        }
        return {};
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
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.findPeakII(nums);
        if (res.size() == 2) {
            cout << "permutaions: nums[" << res[0] << "," << res[1] << "]=" << nums[res[0]][res[1]]<< endl;
        } else {
            cout << "permutaions: not found" << endl;
        }
    };

    nums3 = {
      {1, 2, 3, 6,  5},
      {16,41,23,22, 6},
      {15,17,24,21, 7},
      {14,18,19,20,10},
      {13,14,11,10, 9}
    };
    test(nums3);

    nums3 = {
        {1, 5, 3},
        {4,10, 9},
        {2, 8, 7}
    };
    test(nums3);
}