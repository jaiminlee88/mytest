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
     * @param nums1: an integer array
     * @param nums2: an integer array
     * @return: an integer array
     *          we will sort your return value in output
     * 
     * 给出两个数组，写出一个方法求出它们的交集
     * 
     *  输入: nums1 = [1, 2, 2, 1], nums2 = [2, 2], 
        输出: [2]

        输入: nums1 = [1, 2], nums2 = [2], 
        输出: [2].

        可以用两个set
        可以先排序，用二分法查找O(nlog)
     */
    vector<int> intersection(vector<int> &nums1, vector<int> &nums2) {
        // write your code here
        if ((nums1.size() < 1 && nums2.size() < 1)
        || (nums1.size() < 1 && nums2.size() > 1)
        || (nums1.size() > 1 && nums2.size() < 1)) {
            return {};
        }
        vector<int> res;
        unordered_set<int> s1(nums1.begin(), nums1.end());
        unordered_set<int> s2(nums2.begin(), nums2.end());
        for (auto& it : s1) {
            if (s2.count(it) != 0) {
                res.push_back(it);
            }
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
        auto res = s.permute_3(nums);
        cout << "permutaions: "<< endl;
        for (auto& v : res) {
            for (auto& i : v) {
                cout << i << " ";
            }
            cout << endl;
        }
    };

    nums = {};
    test(nums);

    nums = {1};
    test(nums);

    nums = {1,2};
    test(nums);

    nums = {1, 2, 3};
    test(nums);

    nums = {1,2,3,4};
    test(nums);
}