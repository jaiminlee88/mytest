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
    /**
     * @param matrix: A list of lists of integers
     * @param target: An integer you want to search in matrix
     * @return: An integer indicate the total occurrence of target in the given matrix
     * 
     * 写出一个高效的算法来搜索m×n矩阵中的值，返回这个值出现的次数。这个矩阵具有以下特性：
     * 每行中的整数从左到右是排序的。每一列的整数从上到下是排序的。
     * 在每一行或每一列中没有重复的整数。
     * 
     * 矩阵 = [[3,4]]
        target = 3
        1

        矩阵 = [
            [1, 3, 5, 7],
            [2, 4, 7, 8],
            [3, 5, 9, 10]
            ]
        target = 3
        2

        最后一行的每个数都会比前面所有列都大，包括自己的列
        最后一行，找到比k大的那列，找到比k小的那列，中间就包含了k
     */

    int searchMatrix(vector<vector<int>> &matrix, int target) {
        // write your code here
        // return searchMatrix_0(matrix, target);
        return searchMatrix_1(matrix, target);
    }
    int searchMatrix_0(vector<vector<int>> &matrix, int target) {
        // 理论复杂度不会低于O(N)
        if (matrix.size() < 1 && matrix[0].size() < 1) {
            return 0;
        }
        if (matrix.size() == 1 && matrix[0].size() < 1) {
            return 0;
        }
        if (matrix.size() == 1 && matrix[0].size() == 1) {
            return matrix[0][0] == target ? 1 : 0;
        }
        int rcnt = matrix.size();
        int ccnt = matrix[0].size();
        // 找到比k大的那列，找到比k小的那列，中间就包含了k
        int lpos = 0;
        int rpos = ccnt - 1;
        int mpos = 0;
        while (lpos + 1 < rpos) {
            mpos = lpos + (rpos - lpos) / 2;
            if (matrix[rcnt - 1][mpos] > target) {
                rpos = mpos;
            } else {
                lpos = mpos;
            }
        }
        while (rpos + 1 < ccnt && matrix[0][rpos] <= target) {
            ++rpos;
        }
        lpos = rpos;
        while (lpos > 0 && matrix[rcnt - 1][lpos] >= target) {
            --lpos;
        }
        int total_cnt = 0;
        for (int r = 0; r < rcnt; ++r) {
            for (int c = lpos; c <= rpos; ++c) {
                if (matrix[r][c] == target) {
                    cout << "r: " << r << " c: " << c << endl;
                    ++total_cnt;
                }
            }
        }
        return total_cnt;
    }
    int searchMatrix_1(vector<vector<int>> &matrix, int target) {
        // 每次去掉一行或者一列，每次都走对角线，更快 O(N+M)=O(max(N,M))
        if (matrix.size() < 1 && matrix[0].size() < 1) {
            return 0;
        }
        if (matrix.size() == 1 && matrix[0].size() < 1) {
            return 0;
        }
        int rcnt = matrix.size();
        int ccnt = matrix[0].size();
        int r = rcnt - 1;
        int c = 0;
        int total_cnt  = 0;
        while (r >= 0 && c <= ccnt - 1) {
            if (matrix[r][c] > target) {
                --r;
                continue;
            } else if (matrix[r][c] < target) {
                ++c;
                continue;
            }
            ++total_cnt;
            --r;
            ++c;
        }
        return total_cnt;
    }
};


int main() {
    Solution s;
    vector<int> nums;
    vector<int> nums0;
    vector<int> nums1;
    vector<vector<int>> nums3;
    string str;
    string str0;
    string str1;
    int k = 0;
    auto test = [&s](vector<vector<int>> nums, int k) {
        cout << "nums: " << endl;
        for (auto& i : nums) {
            for (auto j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        // auto res = s.searchMatrix(nums, k);
        auto res = s.searchMatrix(nums, k);
        cout << "k: " << k << " res: " << res << endl;
    };

    nums3 ={{}};
    test(nums3, 3);

    nums3 = {
        {1, 3, 5, 7},
        {10, 11, 16, 20},
        {23, 30, 34, 50}
    };
    test(nums3, 3);

    nums3 = {{5}};
    test(nums3, 2);

    nums3 = {
        {1,2,3,9},
        {2,3,9,10},
        {9,100,109,200}};
    test(nums3, 9);
}