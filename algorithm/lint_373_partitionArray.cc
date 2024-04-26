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
    /*
     * @param nums: an array of integers
     * @return: nothing
     * 分割一个整数数组，使得奇数在前偶数在后。
     * 给定 [1, 2, 3, 4]，返回 [1, 3, 2, 4]。
     */

    void partitionArray(vector<int> &nums) {
        // write your code here
        partitionArray_0(nums);
    }
    void partitionArray_0(vector<int> &nums) {
        if (nums.empty()) {
            return;
        }
        auto is_odd = [](int i) {
            return i % 2;
        };
        auto swap = [](int& a, int& b) {
            int tmp = a ^ b;
            a = a ^ tmp;
            b = b ^ tmp;
        };
        int pleft = 0;
        int pright = nums.size() - 1;
        while (pleft <= pright) {
            while (true) {
                if (pleft <= pright && is_odd(nums[pleft])) {
                    ++pleft;
                }
                break;
            }
            while (true) {
                if (pright >= pleft && !is_odd(nums[pright])) {
                    --pright;
                }
                break;
            }
            if (pleft <= pright) {
                swap(nums[pleft], nums[pright]);
                ++pleft;
                --pright;
            }
        }
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
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        s.partitionArray(nums);
        cout << "permutaions: "<< endl;
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
    };

    nums = {1,2,3,4};
    test(nums);

    nums = {2};
    test(nums);

    nums = {1};
    test(nums);

    nums = {1,2};
    test(nums);

    nums = {1,2,3};
    test(nums);

    nums = {2,1};
    test(nums);

    nums = {2,1,3};
    test(nums);

    nums = {2,2,1};
    test(nums);
}