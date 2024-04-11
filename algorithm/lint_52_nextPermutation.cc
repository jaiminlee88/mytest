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
     * @param nums: A list of integers
     * @return: A list of integers
     * 
     * 给定一个整数数组来表示排列，按升序找出其下一个排列。
     * 可以用来做全排列
     * 也可以用来计算第K个排列
     * 
     * 数组 = [1]，下一个排列是 [1]
     * 数组 = [1,3,2,3]，下一个排列是 [1,3,3,2]
     * 数组 = [4,3,2,1]，下一个排列是 [1,2,3,4] 没有字典序更大的排列时，输出字典序最小的排列。
     * 
     * 
     
        -
            -
                -
    -               -   -   -
                                -   -
        |<-      逆序              ->|
    |<-  替换 ->|
     */
    vector<int> nextPermutation(vector<int> &nums) {
        // write your code here
        return nextPermutation_0(nums);
    }
    void swap(vector<int> &nums, int i, int j) {
        if (i < j) {
            int tmp = nums[i] ^ nums[j];
            nums[i] = nums[i] ^ tmp;
            nums[j] = nums[j] ^ tmp;
        }
    }
    vector<int> nextPermutation_0(vector<int> &nums) {
        if (nums.size() < 1) {
            return {};
        }
        if (nums.size() == 1) {
            return nums;
        }
        // 从后往前找到第一个递减的数
        int i = nums.size() - 1;
        int j = 0;
        while (i > 0 && nums[i - 1] >= nums[i]) {
            --i;
        }
        if (i != 0) {
            // 从后往前找到第一个比nums[i-1]大的数
            j = nums.size() - 1;
            while (j > i && nums[j] <= nums[i - 1]) {
                --j;
            }
            swap(nums, i - 1, j);
        }

        // 从i开始到最后的数逆序
        j = nums.size() - 1;
        while (i < j) {
            swap(nums, i, j);
            i++;
            j--;
        }
        return nums;
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
    auto test = [&s](vector<int>& nums) {
        cout << "nums        : ";
        if (nums.size() < 1) {
            cout << "null ";
        } else {
            for (auto& i : nums) {
                cout << i << " ";
            }
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.nextPermutation(nums);
        cout << "permutaions : ";
        for (auto& v : res) {
            cout << v << " ";
        }
        cout << endl;
    };

    nums = {};
    test(nums);

    nums = {1};
    test(nums);

    nums = {1,2,1};
    test(nums);

    nums = {1,3,2};
    test(nums);

    nums = {1,3,2,3}; // 1,3,3,2
    test(nums);

    nums = {1,2,3,3}; // 1 3 2 3
    test(nums);

    nums = {1, 2, 3};
    test(nums);

    nums = {4,3,2,1};
    test(nums);

    nums = {1,2,3,4,5,6,7,8,9,10,11,11,11,23,4,5,6,7,100,99,98,97,96,95,94,93,92,91,5};
         // 1 2 3 4 5 6 7 8 9 10 11 11 11 23 4 6 5 5 7 91 92 93 94 95 96 97 98 99 100
         // 1,2,3,4,5,6,7,8,9,10,11,11,11,23,4,5,6,91,5,7,92,93,94,95,96,97,98,99,100
    test(nums);
}