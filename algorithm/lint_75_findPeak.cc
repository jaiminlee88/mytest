#include <iostream>
#include <vector>

using namespace std;


class Solution {
public:
    /**
     * @param a: An integers array.
     * @return: return any of peek positions.
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