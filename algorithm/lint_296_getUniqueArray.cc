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
using namespace std;


class Solution {
public:
    /*
    给定一个长度为NN的整数数组arrarr，
    返回去掉重复元素之后的数组（去掉重复元素前后数组元素相对次序不变）
    */
    vector<int> getUniqueArray(vector<int> &arr) {
        // return getUniqueArray_0(arr);
        // return getUniqueArray_1(arr);
        return getUniqueArray_2(arr);
    }
    vector<int> getUniqueArray_0(vector<int> &arr) {
        unordered_set<int> s;
        vector<int> res;
        for (int i = 0; i < arr.size(); ++i) {
            s.insert(arr[i]);
        }
        for (auto& i : s) {
            res.push_back(i);
        }
        return res;
    }

    vector<int> getUniqueArray_1(vector<int> &arr) {
        // 原地实现的话怎么办？
        if (arr.size() < 1) {
            return arr;
        }
        sort(arr.begin(), arr.end());
        int lpos = 0;
        int rpos = lpos + 1;
        while (lpos < arr.size() && rpos < arr.size()) {
            if (arr[lpos] != arr[rpos]) {
                ++lpos;
                if (lpos < arr.size()) {
                    arr[lpos] = arr[rpos];
                }
            }
            ++rpos;
        }
        return vector<int>(arr.begin(), arr.begin() + lpos + 1);
    }

    vector<int> getUniqueArray_2(vector<int> &arr) {
        // 原地实现的话怎么办？同向双指针，标准写法
        if (arr.size() < 1) {
            return arr;
        }
        sort(arr.begin(), arr.end());
        int lpos = 0;
        int rpos = 1;
        for (lpos = 0; lpos < arr.size(); ++lpos) {
            while (rpos < arr.size() && arr[lpos] == arr[rpos]) {
                ++rpos;
            }
            if (rpos >= arr.size()) {
                break;
            }
            arr[lpos + 1] = arr[rpos];
        }
        return vector<int>(arr.begin(), arr.begin() + lpos + 1);
    }
};


int main() {
    Solution s;
    vector<int> nums;
    vector<int> nums0;
    vector<int> nums1;
    vector<vector<int>> nums3;
    string str;
    string str0;
    string str1;
    int k = 0;
    auto test = [&s](vector<int>& nums) {
        cout << "nums: ";
        for (auto i : nums) {
            cout << i << " ";
        }
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.getUniqueArray_0(nums);
        cout << "permutaions: " ;
        for (auto i : res) {
            cout << i << " ";
        }
        cout << endl;
    };

    nums = {3,4,3,6};
    test(nums);

    nums = {1,2,2,3,3,3,4,4};
    test(nums);

    nums = {1,1};
    test(nums);

    nums = {1};
    test(nums);

    nums = {};
    test(nums);

    nums = {1,1,1};
    test(nums);
}