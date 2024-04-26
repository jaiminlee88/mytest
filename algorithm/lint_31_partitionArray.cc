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
#include <bitset>
#include <deque>
using namespace std;


class Solution {
public:
    /**
     * @param nums: The integer array you should partition
     * @param k: An integer
     * @return: The index after partition
     * 给出一个整数数组 nums 和一个整数 k。划分数组（即移动数组 nums 中的元素），使得：

        所有小于k的元素移到左边
        所有大于等于k的元素移到右边
        返回数组划分的位置，即数组中第一个位置 i，满足 nums[i] 大于等于 k。
     */
    int partitionArray(vector<int> &nums, int k) {
        // write your code here
        return partitionArray_0(nums, k);
    }
    int partitionArray_0(vector<int> &nums, int k) {
        if (nums.empty()) {
            return 0;
        }
        if (nums.size() == 1) {
            return nums[0] >= k ? 0 : 1;
        }
        int pleft = 0;
        int pright = nums.size() - 1;
        while (pleft < pright) {
            while (nums[pleft] < k && pleft < pright) {
                pleft++;
            }
            while (nums[pright] >= k && pleft < pright) {
                pright--;
            }
            if (pleft < pright) {
                swap(nums[pleft], nums[pright]);
            }
        }
        return nums[pleft] >= k ? pleft : pleft + 1;
    }
    void swap(int& a, int& b) {
        int tmp = a;
        a = b;
        b = tmp;
    }
};


int main() {
    Solution s;
    vector<int> nums;
    vector<int> nums0;
    vector<int> nums1;
    vector<vector<int>> nums3;
    vector<vector<char>> nums4;
    vector<string> nums5;
    string str;
    string str0;
    string str1;
    int k = 0;
    auto test = [&s](vector<int>& nums, int k) {
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.partitionArray(nums,k);
        cout << "permutaions: "<< res << endl;
    };

    nums = {};
    test(nums,9);

    nums = {1};
    test(nums,9);

    nums = {3,2,2,1};
    test(nums,2);
}