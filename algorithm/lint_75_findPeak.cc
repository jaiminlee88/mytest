#include <iostream>
#include <vector>

using namespace std;


class Solution {
public:
    /**
     * @param a: An integers array.
     * @return: return any of peek positions.
     * 给定一个整数数组(size为n)，其具有以下特点：
     * 相邻位置的数字是不同的
        A[0] < A[1] 并且 A[n - 2] > A[n - 1]
        假定P是峰值的位置则满足A[P] > A[P-1]且A[P] > A[P+1]，返回数组中任意一个峰值的位置。
        数组保证至少存在一个峰
        如果数组存在多个峰，返回其中任意一个就行
        数组至少包含 3 个数

        A = [1, 2, 1, 3, 4, 5, 7, 6]
        返回 1 或 6

        A = [1, 2, 3, 4, 1]
        返回 3
     */
    int findPeak(vector<int> &a) {
        // write your code here
        if (a.size() < 3) {
            return -1;
        }
        // return findPeak_0(a);
        return findPeak_1(a);
    }

    int findPeak_0(vector<int> &a) {
        for (int i = 1; i < a.size() - 1; ++i) {
            if (a[i] > a[i - 1] && a[i] > a[i + 1]) {
                return i;
            }
        }
        return -1;
    }

    /*
    *
        / ... 必有一顶点 ... \
    */
    int findPeak_1(vector<int> &a) {
        int lpos = 0;
        int rpos = a.size() - 1;
        while (lpos + 1 < rpos) {
            int mpos = lpos + (rpos - lpos) / 2;
            if (a[mpos] > a[mpos + 1]) {
                rpos = mpos;
            } else {
                lpos = mpos;
            }
        }
        if (a[lpos] > a[rpos]) {
            return lpos;
        } else {
            return rpos;
        }
        return -1;
    }
};

void print_result(vector<int> &nums, int target=0, int _num=0) {
    cout << "nums: ";
    for (int i = 0; i < nums.size(); ++i) {
        cout << " [" << i << "]->" << nums[i];
    }
    if (_num == -1) {
        cout << " peak not found" << endl;
        return;
    }
    cout << " peak at " << _num << "" << endl;
}
int main() {
    Solution s;
    vector<int> nums;
    int target;
    int _num = 0;

    nums = {1,2,3,4,1};
    _num = s.findPeak(nums);
    print_result(nums, 0, _num);

    nums = {1, 2, 1, 3, 4, 5, 7, 6};
    _num = s.findPeak(nums);
    print_result(nums, 0, _num);

    nums = {1,2,3,2}; // 在0点发生旋转
    target = 1;
    _num = s.findPeak(nums);
    print_result(nums, target, _num);

    nums = {4,6,10,1,2};
    target = 10;
    _num = s.findPeak(nums);
    print_result(nums, target, _num);

    nums = {4,6,10,1,2,3};
    target = 0;
    _num = s.findPeak(nums);
    print_result(nums, target, _num);

    nums = {4, 5, 6, 7, 0, 1, 2};
    target = 0;
    _num = s.findPeak(nums);
    print_result(nums, target, _num);

    nums = {1,2,1};
    target = 2;
    _num = s.findPeak(nums);
    print_result(nums, target, _num);

    nums = {2, 1};
    target = 1;
    _num = s.findPeak(nums);
    print_result(nums, target, _num);

    nums = {1};
    target = 1;
    _num = s.findPeak(nums);
    print_result(nums, target, _num);
    return 0;
}