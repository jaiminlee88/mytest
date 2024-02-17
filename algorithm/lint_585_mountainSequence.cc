#include <iostream>
#include <vector>

using namespace std;


class Solution {
public:
    /**
     * @param nums: a mountain sequence which increase firstly and then decrease
     * @return: then mountain top
     * 给 n 个整数的山脉数组，即先增后减的序列，找到山顶（最大值）。
     */
    int mountainSequence(vector<int> &nums) {
        // write your code here
        // return mountainSequence_0(nums);
        return mountainSequence_1(nums);
    }

    int mountainSequence_0(vector<int> &nums) {
        // write your code here
        if (nums.size() == 0) {
            return 0;
        }
        int max_num = nums[0];
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] > max_num) {
                max_num = nums[i];
            } else {
                break;
            }
        }
        return max_num;
    }

    int mountainSequence_1(vector<int> &nums) {
        // 注意是严格递增递减的序列,没有相等值
        // 看中点走向
        // write your code here
        if (nums.size() == 0) {
            return 0;
        }
        int max_num = nums[0];
        int lpos = 0;
        int rpos = nums.size() - 1;
        while (lpos + 1 < rpos) {
            int mpos = lpos + (rpos - lpos) / 2;
            if (mpos + 1 < nums.size() && nums[mpos] < nums[mpos + 1]) { // 必须没有重复点
                lpos = mpos + 1;
            } else if (mpos + 1 < nums.size() && nums[mpos] > nums[mpos + 1]) {
                rpos = mpos;
            }
        }
        if (nums[lpos] > nums[rpos]) {
            max_num = nums[lpos];
        } else {
            max_num = nums[rpos];
        }
        return max_num;
    }
};

void print_result(vector<int> &nums, int max_num) {
    cout << "nums: [";
    for (auto &i : nums) {
        cout << i << " ";
    }
    cout << "] max_num: " << max_num << endl;
}
int main() {
    Solution s;
    vector<int> nums;
    int max_num = 0;

    nums = {1, 2, 4, 8, 6};
    max_num = s.mountainSequence(nums);
    print_result(nums, max_num);

    return 0;
}