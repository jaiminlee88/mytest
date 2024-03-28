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
    /**
     * @param pages: an array of integers
     * @param k: An integer
     * @return: an integer
     * 
     * 给定n本书，第i本书有pages[i]页。有k个人来抄这些书。
        这些书排成一行，每个人都可以索取连续一段的书。例如，
        一个抄书人可以连续地将书从第i册复制到第j册，但是他不能复制第1册、第2册和第4册（没有第3册）。
        他们在同一时间开始抄书，每抄一页书都要花1分钟。.
        为了让最慢的抄书人能在最早的时间完成书的分配，最好的策略是什么？
        请返回最慢抄书人花费的最短时间。

        输入: pages = [3, 2, 4], k = 2
        输出: 5
        解释: 第一个人复印前两本书, 耗时 5 分钟. 第二个人复印第三本书, 耗时 4 分钟.

        输入: pages = [3, 2, 4], k = 3
        输出: 4
        解释: 三个人各复印一本书.

        求极值，有序数组，可能用动态规划
        区间型问题
        status dp[i][j] 表示前i本书，j个人复印的最小时间
        func  dp[i][j] = min(dp[i][j], max(dp[i-k][j-1], sum(pages[i-k+1, i-1]))), for k in [1, i]
        init  dp[0][0]=0 dp[0][j]=0 dp[i][0]=INT_MAX
        result dp[n][k]

        二分法
     */
    int copyBooks(vector<int> &pages, int k) {
        // write your code here
        // return copyBooks_0(pages, k);
        // return copyBooks_1(pages, k);
        return copyBooks_2(pages, k);
    }

    int copyBooks_0(vector<int> &pages, int k) {
        // 动态规划方法
        if (pages.size() == 0) {
            return 0;
        }
        if (k == 0) {
            return INT_MAX;
        }
        vector<vector<int>> dp(pages.size() + 1, vector<int>(k+1, INT_MAX));
        vector<int> presum(pages.size() + 1, 0);
        dp[0][0] = 0; // dp[i][0] = INT_MAX;
        for (int j = 1; j <= k; j++) {
            dp[0][j] = 0;
        }
        for (int i = 1; i <= pages.size(); i++) {
            presum[i] = presum[i-1] + pages[i-1];
        }
        for (int i = 1; i <= pages.size(); i++) {
            for (int j = 1; j <= k; j++) {
                for (int prev = 0; prev < i; prev++) {
                    dp[i][j] = min(dp[i][j], max(dp[prev][j-1], presum[i] - presum[prev]));
                }
            }
        }
        cout << "dp:" << endl;
        for (auto& i : dp) {
            for (auto& j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
        return dp[pages.size()][k];
    }

    int copyBooks_1(vector<int> &pages, int k) {
        // 可以迭代法
        if (pages.size() == 0) {
            return 0;
        }
        if (k == 0) {
            return INT_MAX;
        }
        return copyBooks_1_core(pages, k, 0, pages.size() - 1);
    }
    int copyBooks_1_core(vector<int> &pages, int k, int start, int end) {
        if (k == 1) {
            int sum = 0;
            for (int i = start; i <= end; i++) {
                sum += pages[i];
            }
            return sum;
        }
        if (start == end) {
            return pages[start];
        }

        int sum = 0;
        int min_ret = INT_MAX;
        for (int m = 1; m <= (end - start + 1); m++) {
            sum += pages[start + m - 1];
            int right = copyBooks_1_core(pages, k-1, start + m, end);
            min_ret = min(min_ret, max(sum, right));
        }
        return min_ret;
    }

    int copyBooks_2(vector<int> &pages, int k) {
        // 二分法，时间固定，人数不定
        // 时间从0到无限大，二分找到最小的时间
        // 给定的时间，看看需要多少人，如果人数不够，时间太少，需要增加时间
        if (pages.size() == 0) {
            return 0;
        }
        if (k == 0) {
            return -1;
        }
        int stime = 0;
        int etime = INT_MAX;
        while (stime + 1 < etime) {
            int mtime = stime + (etime - stime) / 2;
            if (getNumOfCopiers(pages, mtime) <= k) {
                // 当前给定的时间，可以用很少的人完成
                // 说明时间还可以再少点
                etime = mtime;
            } else {
                // 当前给定的时间，需要更多的人才能完成
                // 说明时间不够，需要增加时间
                stime = mtime;
            }
        }
        if (getNumOfCopiers(pages, stime) <= k) {
            return stime;
        }
        return etime;
    }
    int getNumOfCopiers(vector<int> &pages, int lim_time) {
        int copiers = 0;
        int lastcopied = lim_time;
        
        for (auto& p : pages) {
            if (p > lim_time) {
                return INT_MAX;
            }
            if (lastcopied + p > lim_time) {
                copiers++; // 第一个人尽可能多的复制，直到超过lim_time，就再加一人
                lastcopied = 0;
            }
            lastcopied += p;
        }
        cout << "lim_time: " << lim_time << " copiers:" << copiers << endl;
        return copiers;
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
        auto res = s.copyBooks(nums, k);
        cout << "k: " << k << " permutaions: "<< res << endl;
    };

    nums = {3, 2, 4};
    test(nums,2);

    nums = {3, 2, 4};
    test(nums,3);
}