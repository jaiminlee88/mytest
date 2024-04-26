#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /**
     * @param a: An integer array.
     * @return: nothing
     * 给出一个含有正整数和负整数的数组，重新排列成一个正负数交错的数组。
     */
    void rerange(vector<int> &a) {
        // write your code here
        if (a.size() < 2) {
            return;
        }
        int lpos = 0;
        int rpos = a.size() - 1;
        while (lpos <= rpos) { // 分为正负两部分
            while (lpos < rpos && a[lpos] < 0) {
                ++lpos;
            }
            while (rpos > lpos && a[rpos] > 0) {
                --rpos;
            }
            if (lpos <= rpos) {
                std::swap(a[lpos], a[rpos]);
                ++lpos;
                --rpos;
            }
        }
        bool need_exchange = true;
        lpos = 0;
        rpos = a.size() - 1;
        if (a.size() % 2) { // 奇数
            int mid = a[a.size() / 2];
            if (mid > 0) {
                --rpos;
            } else {
                ++lpos;
            }
        }
        while (lpos < rpos) {
            if (need_exchange) {
                std::swap(a[lpos], a[rpos]);
            }
            ++lpos;
            --rpos;
            need_exchange = !need_exchange;
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
int main () {
    Solution s;
    vector<int> nums;
    vector<int> ori_nums;

    nums = {4, 5, 6, -1, -2, -3};
    ori_nums = nums;
    s.rerange(nums);
    print_result(ori_nums, nums);

    nums = {-1, -1, 2};
    ori_nums = nums;
    s.rerange(nums);
    print_result(ori_nums, nums);

    nums = {-1,2,2};
    ori_nums = nums;
    s.rerange(nums);
    print_result(ori_nums, nums);

    nums = {4, 5, 6, 7, -2, -3};
    ori_nums = nums;
    s.rerange(nums);
    print_result(ori_nums, nums);

    nums = {-33,-19,30,26,21,-9};
    ori_nums = nums;
    s.rerange(nums);
    print_result(ori_nums, nums);
}