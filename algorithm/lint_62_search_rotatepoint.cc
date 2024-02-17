#include <iostream>
#include <vector>

using namespace std;


class Solution {
public:
    /**
     * @param a: an integer rotated sorted array
     * @param target: an integer to be searched
     * @return: an integer
     * 给定一个有序数组，但是数组以某个元素作为支点进行了旋转
     * (比如，0 1 2 4 5 6 7 可能成为4 5 6 7 0 1 2)。
     * 给定一个目标值target进行搜索，如果在数组中找到目标值返回数组中的索引位置，否则返回-1。
     * 你可以假设数组中不存在重复的元素。
     */
    int search(vector<int> &a, int target) {
        // write your code here
        // return search_0(a, target);
        // return search_1(a, target);
        return search_2(a, target);
    }

    int search_0(vector<int> &a, int target) {
        // write your code here
        for (int i = 0; i < a.size(); ++i) {
            if (a[i] == target) {
                return i;
            }
        }
        return -1;
    }

    int search_1(vector<int> &a, int target) {
        // 旋转，有序，单调不重复
        // write your code here
        if (a.size() < 1) {
            return -1;
        } else if (a.size() == 1) {
            return a[0] == target ? 0 : -1;
        }

        int lpos = 0;
        int rpos = a.size() - 1;
        int min_pos = 0;
        while(lpos + 1 < rpos) {
            int mpos = lpos + (rpos - lpos) / 2;
            if (a[mpos] < a[rpos]) {
                rpos = mpos;
            } else {
                lpos = mpos;
            }
        }
        min_pos = a[lpos] < a[rpos] ? lpos : rpos;
        if (min_pos == 0) {
            lpos = 0;
            rpos = a.size() - 1;
        } else if (target >= a[0]) {
            lpos = 0;
            rpos = min_pos - 1;
        } else {
            lpos = min_pos;
            rpos = a.size() - 1;
        }
        while (lpos + 1 < rpos) {
            int mpos = lpos + (rpos - lpos) / 2;
            if (a[mpos] < target) {
                lpos = mpos;
            } else {
                rpos = mpos;
            }
        }
        if (a[lpos] == target) {
            return lpos;
        } else if (a[rpos] == target) {
            return rpos;
        }
        return -1;
    }

    int search_2(vector<int> &a, int target) {
        // 旋转，有序，单调不重复
        // write your code here
        if (a.size() < 1) {
            return -1;
        } else if (a.size() == 1) {
            return a[0] == target ? 0 : -1;
        }
        int lpos = 0;
        int rpos = a.size() - 1;
        while (lpos + 1 < rpos) {
            int mpos = lpos + (rpos - lpos) / 2;
            if (a[mpos] > a[rpos]) { // 左半段
                if (a[lpos] <= target && target <= a[mpos]) {
                    rpos = mpos;
                } else {
                    lpos = mpos;
                }
            } else { // 右半段
                if (a[mpos] <= target && target <= a[rpos]) {
                    lpos = mpos;
                } else {
                    rpos = mpos;
                }
            }
        }
        if (a[lpos] == target) {
            return lpos;
        } else if (a[rpos] == target) {
            return rpos;
        }
        return -1;
    }
};


void print_result(vector<int> &nums, int target, int _num) {
    cout << "nums: ";
    for (int i = 0; i < nums.size(); ++i) {
        cout << nums[i] << "[" << i << "] ";
    }
    if (_num == -1) {
        cout << " target: " << target << " not found" << endl;
        return;
    }
    cout << " target: " << target << " at " << _num << "" << endl;
}
int main() {
    Solution s;
    vector<int> nums;
    int target;
    int _num = 0;

    nums = {1,2,3}; // 在0点发生旋转
    target = 1;
    _num = s.search(nums, target);
    print_result(nums, target, _num);

    nums = {4,6,10,1,2};
    target = 10;
    _num = s.search(nums, target);
    print_result(nums, target, _num);

    nums = {4,6,10,1,2,3};
    target = 0;
    _num = s.search(nums, target);
    print_result(nums, target, _num);

    nums = {4, 5, 6, 7, 0, 1, 2};
    target = 0;
    _num = s.search(nums, target);
    print_result(nums, target, _num);

    nums = {2, 1};
    target = 1;
    _num = s.search(nums, target);
    print_result(nums, target, _num);

    nums = {1};
    target = 1;
    _num = s.search(nums, target);
    print_result(nums, target, _num);
    return 0;
}