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
    *
    给定一个整数数组，在该数组中，寻找三个数，分别代表三角形三条边的长度，问，可以寻找到多少组这样的三个数来组成三角形？

    Input: [3, 4, 6, 7]
    Output: 3
    Explanation:
    They are (3, 4, 6),
             (3, 6, 7),
             (4, 6, 7)

    Input: [4, 4, 4, 4]
    Output: 4
    Explanation:
    Any three numbers can form a triangle.
    So the answer is C(3, 4) = 4
    */
    int triangleCount(vector<int> &S) {
        return triangleCount_0(S);
    }
    int triangleCount_0(vector<int> &S) {
        if (S.size() < 3) {
            return 0;
        }
        sort(S.begin(), S.end());
        int cnt = 0;
        int min_num = 0;
        for (int i = S.size() - 1; i >= 2; i--) {
            for (int j = i - 1; j >= 1; j--) {
                min_num = S[i] -S[j] > 0 ? S[i] - S[j] : 0;
                int pos_min = find_min_pos(S, 0, j - 1, min_num);
                // cout << "i: " << i << " j: " << j << " min_num: " << min_num << " pos: " << pos_min << endl;
                if (pos_min >= 0 && j - pos_min > 0) {
                    cnt += j - pos_min;
                }
            }
        }
        return cnt;
    }
    int find_min_pos(vector<int> &S, int start, int end, int min_num) {
        // 二分法查找第一个大于等于min_num的位置
        int mid = 0;
        while (start + 1 < end) {
            mid = start + (end - start) / 2;
            if (S[mid] >= min_num) { // 找最后一个
                end = mid;
            } else {
                start = mid;
            }
        }
        while (S[start] <= min_num) {
            ++start;
        }
        return start;
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
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.triangleCount(nums);
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << " permutaions: " << res << endl;
    };

    nums = {};
    test(nums);

    nums = {1};
    test(nums);

    nums = {1,2};
    test(nums);

    nums = {1, 2, 3};
    test(nums);

    nums = {4, 2, 3};
    test(nums);

    nums = {1,2,3,4};
    test(nums);

    nums = {3, 4, 6, 7};
    test(nums);

    nums = {4, 4, 4, 4};
    test(nums);
}