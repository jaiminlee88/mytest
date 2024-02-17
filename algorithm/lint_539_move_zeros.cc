#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /**
     * @param nums: an integer array
     * @return: nothing
     * 给一个数组 nums 写一个函数将 0 移动到数组的最后面，非零元素保持原数组的顺序
     */
    void moveZeroes(vector<int> &nums) { // 同向双指针法
        // write your code here
        if (nums.size() < 2) {
            return;
        }
        int zero_pos = 0;
        int num_pos = 0;
        while (true) {
            if (zero_pos >= nums.size() || num_pos >= nums.size()) {
                break;
            }
            if (nums[zero_pos] != 0) {
                ++zero_pos;
                continue;
            }
            if (num_pos <= zero_pos) {
                num_pos = zero_pos + 1;
                continue;
            }
            if (nums[num_pos] == 0) {
                ++num_pos;
                continue;
            }
            std::swap(nums[zero_pos], nums[num_pos]);
        }

        void moveZeroes_1(vector<int> &nums) { // 把所有除0以外的数字移动到开头，后尾补齐0
            if (nums.size() < 2) {
                return;
            }
            int lpos = 0;
            for (int num_pos = 0; num_pos < nums.size(); ++num_pos) {
                if (nums[num_pos] != 0) {
                    nums[lpos] = nums[num_pos];
                    ++lpos;
                }
            }
            while (lpos < nums.size()) {
                nums[lpos] = 0;
                ++lpos;
            }
        }
    }
};

void print_result(vector<int> &nums, vector<int> &result) {
    cout << "orig: [";
    for (auto i : nums) {
        cout << i << " ";
    }
    cout << "] result: [";
    for (auto i : result) {
        cout << i << " ";
    }
    cout << "]" << endl;
}

int main() {
    Solution s;
    vector<int> nums;
    vector<int> ori_nums;

    nums = {0, 1, 0, 3, 12};
    ori_nums = nums;
    s.moveZeroes(nums);
    print_result(ori_nums, nums);

    nums = {0, 0, 0, 3, 1};
    ori_nums = nums;
    s.moveZeroes(nums);
    print_result(ori_nums, nums);

    nums = {0, 0, 0, 0, 0};
    ori_nums = nums;
    s.moveZeroes(nums);
    print_result(ori_nums, nums);

    nums = {1, 2, 3, 4, 5};
    ori_nums = nums;
    s.moveZeroes(nums);
    print_result(ori_nums, nums);

    nums = {-1,2,-3,4,0,1,0,-2,0,0,1};
    ori_nums = nums;
    s.moveZeroes(nums);
    print_result(ori_nums, nums);
    return 0;
}
