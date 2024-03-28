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
     * @param matrix: matrix, a list of lists of integers
     * @param target: An integer
     * @return: a boolean, indicate whether matrix contains target
     * 
     * 描述
        写出一个高效的算法来搜索 m × n矩阵中的值 target 。
        这个矩阵具有以下特性：
        每行中的整数从左到右是排序的。
        每行的第一个数大于上一行的最后一个整数。

        矩阵 = [[5]]
        target = 2
        解释：
        矩阵中没有包含2，返回false。

        矩阵 = [
        [1, 3, 5, 7],
        [10, 11, 16, 20],
        [23, 30, 34, 50]
        ]
        target = 3
        解释：
        矩阵中包含3，返回true。


        可以先找行，再找列

        或者 index = r * col_cnt + c 来遍历所有元素
        r = index / col_cnt
        c = index % col_cnt
        这样可以直接用二分法来找

     */
    bool searchMatrix(vector<vector<int>> &matrix, int target) {
        // write your code here
        // 二分法
        if (matrix.empty() || matrix[0].empty()) {
            return false;
        }
        int rcnt = matrix.size();
        int ccnt = matrix[0].size();
        int start_row = 0;
        int end_row = matrix.size() - 1;
        int target_row = -1;
        while (start_row + 1 < end_row) {
            int mid_row = start_row + (end_row - start_row) / 2;
            if (matrix[mid_row][0] == target) {
                cout << "r=" << mid_row << " c=" << 0 << endl;
                return true;
            }
            if (matrix[mid_row][0] < target) {
                start_row = mid_row;
            } else {
                end_row = mid_row - 1;
            }
        }
        if (matrix[start_row][0] <= target && target <= matrix[start_row][ccnt - 1]) {
            target_row = start_row;
        } else if (matrix[end_row][0] <= target && target <= matrix[end_row][ccnt - 1]) {
            target_row = end_row;
        } else {
            return false;
        }
        int lpos = 0;
        int rpos = matrix[target_row].size() - 1;
        while (lpos + 1 < rpos) {
            int mpos = lpos + (rpos - lpos) / 2;
            if (matrix[target_row][mpos] >= target) {
                rpos = mpos;
            } else {
                lpos = mpos;
            }
        }
        if (matrix[target_row][lpos] == target) {
            cout << "r=" << target_row << " c=" << lpos << endl;
            return true;
        } else if (matrix[target_row][rpos] == target) {
            cout << "r=" << target_row << " c=" << rpos << endl;
            return true;
        }
        return false;
    }

    bool searchMatrix_1(vector<vector<int>> &matrix, int target) {
        // 二分法，把二维数组当作一维数组来处理
        if (matrix.empty() || matrix[0].empty()) {
            return false;
        }
        int lpos = 0;
        int rpos = matrix.size() * matrix[0].size() - 1;
        int mpos, r, c;
        while (lpos + 1 < rpos) {
            mpos = lpos + (rpos - lpos) / 2;
            r = mpos / matrix[0].size();
            c = mpos % matrix[0].size();
            if (matrix[r][c] >= target) {
                rpos = mpos;
            } else {
                lpos = mpos;
            }
        }
        r = lpos / matrix[0].size();
        c = lpos % matrix[0].size();
        if (matrix[r][c] == target) {
            return true;
        }
        r = rpos / matrix[0].size();
        c = rpos % matrix[0].size();
        if (matrix[r][c] == target) {
            return true;
        }
        return false;
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
        auto res = s.searchMatrix_1(nums, k);
        cout << "k: " << k << " res: ";
        if (res) {
            cout << "[found]" << endl;
        } else {
            cout << "[not found]" << endl;
        }
    };

    nums3 = {
        {1, 3, 5, 7},
        {10, 11, 16, 20},
        {23, 30, 34, 50}
    };
    test(nums3, 3);

    nums3 = {{5}};
    test(nums3, 2);

    nums3 = {{1, 3, 5, 7}};
    test(nums3, 3);

    nums3 = {{1, 3, 3, 5, 7}};
    test(nums3, 3);

    nums3 = {
        {1, 3, 5, 7},
        {11, 11, 11, 20},
        {23, 30, 34, 50}
    };
    test(nums3, 11);
}