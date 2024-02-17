#include <iostream>
#include <vector>

using namespace std;


class Solution {
public:
    /**
     * @param nums: a rotated sorted array
     * @return: the minimum number in the array
     * 假设一个按升序排好序的数组在其某一未知点发生了旋转
     * （比如0 1 2 4 5 6 7 可能变成4 5 6 7 0 1 2）。你需要找到其中最小的元素
     * 数组中不存在重复元素，严格递增
     */
    int findMin(vector<int> &nums) {
        if (nums.size() == 0) {
            return 0;
        }
        if (nums.size() == 1) {
            return nums[0];
        }
        // write your code here
        // return findMin_0(nums);
        return findMin_1(nums);
    }

    int findMin_0(vector<int> &nums) {
        // write your code here
        int min_num = nums[0];
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] < min_num) {
                min_num = nums[i];
            }
        }
        return min_num;
    }

    int findMin_1(vector<int> &nums) {
        int lpos = 0;
        int rpos = nums.size() - 1;
        while (lpos + 1 < rpos) {
            int mpos = lpos + (rpos - lpos) / 2;
            if (nums[mpos] < nums[rpos]) {
                rpos = mpos;
            } else if (nums[mpos] > nums[lpos]) {
                lpos = mpos;
            }
        }
        return min(nums[lpos], nums[rpos]);
    }
};

void print_result(vector<int> &nums, int _num) {
    cout << "nums: [";
    for (auto &i : nums) {
        cout << i << " ";
    }
    cout << "] _num: " << _num << endl;
}
int main() {
    Solution s;
    vector<int> nums;
    int _num = 0;

    nums = {1,2,3}; // 在0点发生旋转
    _num = s.findMin(nums);
    print_result(nums, _num);

    nums = {4,6,10,1,2};
    _num = s.findMin(nums);
    print_result(nums, _num);

    nums = {4,6,10,1,2,3};
    _num = s.findMin(nums);
    print_result(nums, _num);

    nums = {4, 5, 6, 7, 0, 1, 2};
    _num = s.findMin(nums);
    print_result(nums, _num);

    nums = {2, 1};
    _num = s.findMin(nums);
    print_result(nums, _num);
    return 0;
}