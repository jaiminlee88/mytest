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
     * @param a: sorted integer array A
     * @param b: sorted integer array B
     * @return: A new sorted integer array
     * 
     * 将按升序排序的整数数组A和B合并，新数组也需有序。
     */
    vector<int> mergeSortedArray(vector<int> &a, vector<int> &b) {
        return mergeSortedArray_0(a, b);
    }

    vector<int> mergeSortedArray_0(vector<int> &a, vector<int> &b) {
        // write your code here
        if (a.empty()) {
            return b;
        }
        if (b.empty()) {
            return a;
        }
        int pa = 0;
        int pb = 0;
        vector<int> res;
        while (pa < a.size() && pb < b.size()) {
            if (a[pa] < b[pb]) {
                res.push_back(a[pa]);
                ++pa;
            } else {
                res.push_back(b[pb]);
                ++pb;
            }
        }
        if (pa < a.size()) {
            res.insert(res.end(), a.begin() + pa, a.end());
        }
        if (pb < b.size()) {
            res.insert(res.end(), b.begin() + pb, b.end());
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