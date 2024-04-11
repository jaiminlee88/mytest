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
     * @param a: An array of integers
     * @return: A long integer
     * 
     * 给出一个可能包含重复数字的排列，求这些数字的所有排列按字典序排序后该排列在其中的编号。编号从1开始
     * 
     *  输入:[1,4,2,2]
        输出:3
        1 2 2 4
        1 2 4 2
        1 4 2 2
        2 1 2 4
        2 1 4 2
        2 2 1 4
        2 2 4 1
        2 4 1 2
        2 4 2 1
        4 1 2 2
        4 2 1 2
        4 2 2 1
        从右到左，遍历

        输入 :[1,6,5,3,1]
        输出:24

        从右到左，遍历
        遍历到A[i]，判断第i+1位到最后一位，有多少个比A[i]小的数，设为count
        则A[i]的排列数为count * (n-i-1)!
        把所有的数都加起来，即为结果
        如何处理重复
     */
    long long permutationIndexII(vector<int> &a) {
        // write your code here
        return permutationIndex_0(a);
    }

    long long permutationIndex_0(vector<int> &a) {
        if (a.size() < 1) {
            return 0;
        }
        if (a.size() == 1) {
            return 1;
        }
        unordered_map<int,int> nums;
        long long res = 1;
        for (int i = a.size() - 1; i >= 0; --i) {
            if (nums.size() < 1) {
                nums[a[i]] += 1;
                continue;
            }
            int cnt = 0;
            nums[a[i]] += 1;
            for (auto& it : nums) {
                if (it.first < a[i]) {
                    // only caring about different numbers
                    nums[it.first] -= 1;
                    long long tmp = cal_permutation(a.size() - i - 1, nums);
                    res += tmp;
                    nums[it.first] += 1;
                }
            }
        }
        return res;
    }

    long long cal_permutation(int i, unordered_map<int,int>& nums) {
        // 含有重复数字的全排列
        long long ret = 1;
        while (i > 0) {
            ret *= i;
            --i;
        }
        for (auto& it : nums) {
            ret /= cal_permutation(it.second);
        }
        return ret;
    }

    long long cal_permutation(int i) {
        // 含有重复数字的全排列
        if (i == 1) {
            return 1;
        }
        long long ret = 1;
        while (i > 0) {
            ret *= i;
            --i;
        }
        return ret;
    }
};

git ad
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
        auto res = s.permutationIndexII(nums);
        cout << "permutaions: " << res << endl;
    };

    nums = {};
    test(nums);

    nums = {1};
    test(nums);

    nums = {1,2};
    test(nums);

    nums = {1, 2, 3};
    test(nums);

    nums = {1,4,2,2};
    test(nums);

    nums = {1,6,5,3,1};
    test(nums);
}