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
     * @param colors: A list of integer
     * @param k: An integer
     * @return: nothing
     * 
     * 给定一个有n个对象（包括k种不同的颜色，并按照1到k进行编号）的数组，
     * 将对象进行分类使相同颜色的对象相邻，并按照1,2，...k的顺序进行排序。
     * 不能使用代码库中的排序函数来解决这个问题
        k <= n

     * 输入: 
        [3,2,2,1,4] 
        4
        输出: 
        [1,2,2,3,4]

        输入: 
        [2,1,1,2,2] 
        2
        输出: 
        [1,1,2,2,2]

        直接快速排序？
     */
    void sortColors2(vector<int> &colors, int k) {
        // write your code here
        // return sortColors2_0(colors, k); 
        // return sortColors2_1(colors, k);
        // return sortColors2_2(colors, k);
        return sortColors2_3(colors, k);
    }

    void sortColors2_3(vector<int> &colors, int k) {
        // 归并排序 O(nlogk)
        if (colors.size() < 2) {
            return;
        }
        
    }

    void sortColors2_2(vector<int> &colors, int k) {
        // 计数法
        // 1. 遍历数组，统计每个颜色的个数
        unordered_map<int, int> color_count;
        for (auto& i : colors) {
            color_count[i]++;
        }
        // 2. 重新填充数组
        int pos = 0;
        for (int i = 1; i <= k; i++) {
            for (int j = 0; j < color_count[i]; j++) {
                colors[pos++] = i;
            }
        }
    }

    void sortColors2_0(vector<int> &colors, int k) {
        sort(colors.begin(), colors.end());
    }

    void sortColors2_1(vector<int> &colors, int k) {
        // 快速排序
        if (colors.size() < 2) {
            return;
        }
        quick_sort(colors, 0, colors.size() - 1);
    }
    void quick_sort(vector<int> &colors, int start, int end) {
        // 快速排序
        if (start >= end) {
            return;
        }
        int lpos = start;
        int rpos = end;
        int mpos = lpos + (rpos - lpos) / 2;
        int pivot = colors[mpos]; // 选取中间值，一定要取出来，否则colors[mpos]会变化
        while (lpos <= rpos) {
            while (colors[lpos] < pivot) {
                lpos++;
            }
            while (colors[rpos] > pivot) {
                rpos--;
            }
            if (lpos <= rpos) {
                swap(colors[lpos], colors[rpos]);
                lpos++;
                rpos--;
            }
        }
        quick_sort(colors, start, rpos);
        quick_sort(colors, lpos, end);
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
        s.sortColors2(nums, k);
        cout << "permutaions: "<< endl;
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
    };

    nums = {};
    test(nums,0);

    nums = {1};
    test(nums,1);

    nums = {1,2};
    test(nums,2);

    nums = {3,2,2,1,4};
    test(nums,4);

    nums = {2,1,1,2,2};
    test(nums,2);

    nums = {2,3,1,1,2,2};
    test(nums,3);

    nums = {3,2,3,3,4,3,3,2,4,4,1,2,1,1,1,3,4,3,4,2};
    test(nums,4);
}