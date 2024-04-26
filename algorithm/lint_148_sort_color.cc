#include <vector>
#include <iostream>

using namespace std;

class Solution {
public:
    /**
     * @param nums: A list of integer which is 0, 1 or 2
     * @return: nothing
     * 给定一个包含红，白，蓝且长度为 n 的数组，将数组元素进行分类使相同颜色的元素相邻，
     * 并按照红、白、蓝的顺序进行排序。
     * 我们使用整数 0，1 和 2 分别代表红，白，蓝。
     * 可以用quick sort
     * 也可以按照 0 1 2顺序，碰到0往左放，碰到2往右放
     */
    void sortColors(vector<int> &nums) {
        // write your code here
        if (nums.size() < 2) {
            return;
        }
        // sortColors_core_1(nums, 0, nums.size() - 1);
        sortColors_core_2(nums);
    }
    void sortColors_core_2(vector<int> &nums) {
        if (nums.size() < 2) {
            return;
        }
        int index = 0; // 移动指针
        int left = 0;
        int right = nums.size() - 1;
        while (index <= right) {
            if (nums[index] == 0) {
                std::swap(nums[index], nums[left]);
                ++left;
                ++index;
            } else if (nums[index] == 2) {
                std::swap(nums[index], nums[right]);
                --right;
            } else { // ignore 1
                ++index;
            }
        }
    }
    void sortColors_core(vector<int>& nums, int start, int end) {
        if (start >= end) {
            return;
        }
        int lpos = start;
        int rpos = end;
        int pivot = nums[(lpos + rpos) / 2];
        while (lpos <= rpos) {
            while (lpos <= rpos && nums[lpos] < pivot) {
                ++lpos;
            }
            while (rpos >= lpos && nums[rpos] > pivot) {
                --rpos;
            }
            if (lpos <= rpos) {
                std::swap(nums[lpos], nums[rpos]);
                ++lpos;
                --rpos;
            }
        }
        sortColors_core(nums, start, rpos);
        sortColors_core(nums, lpos, end);
    }

    void sortColors_core_1(vector<int>& nums, int start, int end) { // 不断扩大边界
        if (start >= end) {
            return;
        }
        // s三个指针，lpos指向0的右边界，rpos指向2的左边界，tpos指向当前位置
        int lpos = start;
        int rpos = end;
        int tpos = 0;
        while (tpos < rpos) {
            if (nums[tpos] == 2) {
                std::swap(nums[tpos], nums[rpos]);
                --rpos;
            }
            if (nums[tpos] == 0) {
                std::swap(nums[tpos], nums[lpos]);
                ++lpos;
            }
            if (tpos < lpos) {
                tpos = lpos;
            } else {
                ++tpos;
            }
        }
    }
};

void print_result(vector<int> nums, vector<int> result) {
    cout << "orig: [";
    for (auto& it : nums) {
        cout << it << ", ";
    }
    cout << "] result: [";
    for (auto& it : result) {
        cout << it << ", ";
    }
    cout << "]" << endl;
}

int main() {
    Solution s;
    vector<int> nums;
    vector<int> ori_nums;

    nums = {1, 0, 1, 2};
    ori_nums = nums;
    s.sortColors(nums);
    print_result(ori_nums, nums);

    nums = {2,0,2,2,1,2,2,1,2,0,0,0,1};
    ori_nums = nums;
    s.sortColors(nums);
    print_result(ori_nums, nums);

    nums = {2,0,0,1,2,0,2};
    ori_nums = nums;
    s.sortColors(nums);
    print_result(ori_nums, nums);
}