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
     * 给出一个不含重复数字的排列，求这些数字的所有排列按字典序排序后该排列的编号。其中，编号从1开始。
     * 
     * 输入:[1,2,4]
        输出:1

       输入:[3,2,1]
        输出:6
            1 2 3
            1 3 2
            2 1 3
            2 3 1
            3 1 2
            3 2 1
            3 开头
            2开头，
            1开头，比3小的有两个选择，后续两位可以2*1=2种选择

        从右到左，遍历
        遍历到A[i]，判断第i+1位到最后一位，有多少个比A[i]小的数，设为count
        则A[i]的排列数为count * (n-i-1)!
        把所有的数都加起来，即为结果
     */
    long long permutationIndex(vector<int> &a) {
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
        vector<int> nums;
        long long res = 1;
        for (int i = a.size() - 1; i >= 0; --i) {
            if (nums.size() < 1) {
                nums.push_back(a[i]);
                continue;
            }
            int cnt = 0;
            for (int j = 0; j < nums.size(); ++j) {
                if (nums[j] < a[i]) {
                    ++cnt;
                }
            }
            long long tmp = cal_permutation(a.size() - i - 1);
            res += cnt * tmp;
            nums.push_back(a[i]);
        }
        return res;
    }

    long long cal_permutation(int i) {
        long long ret = 1;
        while (i > 0) {
            ret *= i;
            --i;
        }
        return ret;
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
        auto res = s.permutationIndex(nums);
        cout << "permutaions: " << res << endl;
    };

    // nums = {};
    // test(nums);

    // nums = {1};
    // test(nums);

    // nums = {1,2};
    // test(nums);

    // nums = {1, 2, 3};
    // test(nums);

    // nums = {1,2,3,4};
    // test(nums);

    nums = {1,2,4};
    test(nums);

    nums = {3,2,1};
    test(nums);
}