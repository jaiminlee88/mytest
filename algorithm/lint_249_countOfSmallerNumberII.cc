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
using namespace std;

template<int N, int interval>
struct NumBlocks {
    static constexpr int get_block_size() {
        return N / interval + 1;
    }
};

template <int N, int interval>
using Blocks = std::vector<std::bitset<interval>>;

class Solution {
public:
    /**
     * @param a: an integer array
     * @return: A list of integers includes the index of the first number and the index of the last number
     给定一个整数数组（下标由 0 到 n-1， n 表示数组的规模，ai的取值范围由 0 到10000）。
     对于数组中的每个 ai 元素，请计算 ai 前的数中比它小的元素的数量，并返回数量数组。
     

     输入:
        [1,2,7,8,5]
        输出:
        [0,1,2,3,2]
    
    输入:
        [7,8,2,1,3]
        输出:
        [0,1,0,0,2]
    
    暴力法，时间复杂度O(n^2)
    分块法，因为ai范围在0-10000，所以可以分块，每个块大小为100，
            块数100，分别统计每个块数字出现次数，时间复杂度O(n)
    线段树
     */
    vector<int> countOfSmallerNumberII(vector<int> &a) {
        // write your code here
        return countOfSmallerNumberII_0(a);
    }

    vector<int> countOfSmallerNumberII_0(vector<int> &a) {
        if (a.empty()) {
            return {};
        }
        if (a.size() == 1) {
            return {0};
        }

        // std::vector<std::bitset<100>> blocks(10000/100 + 1);
        std::bitset<10001> blocks;  // 也可以在使用时动态计算块大小
        vector<int> bit_cnt(10001, 0);
        vector<int> block_cnt(10000/100 + 1, 0);
        vector<int> res(a.size(), 0);
        for (int i = 0; i < a.size(); ++i) {
            if (blocks.test(a[i]) == false) {
                blocks.set(a[i]);
            }
            bit_cnt[a[i]]++;
            block_cnt[a[i] / 100]++;
            int cnt = 0;
            for (int j = 0; j < a[i] / 100; ++j) {
                cnt += block_cnt[j];
            }
            for (int j = (a[i] / 100) * 100; j < a[i]; ++j) {
                cnt += bit_cnt[j];
            }
            res[i] = cnt;
        }
        return res;
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
        auto res = s.countOfSmallerNumberII(nums);
        cout << "permutaions: "<< endl;
        for (auto& v : res) {
            cout << v << " ";
        }
        cout << endl;
    };

    nums = {};
    test(nums);

    nums = {1};
    test(nums);

    nums = {1,2,7,8,5};
    test(nums);

    nums = {7,8,2,1,3};
    test(nums);

    // nums = {1,2,3,4};
    // test(nums);
}