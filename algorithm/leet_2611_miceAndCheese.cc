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
     * 有两只老鼠和 n 块不同类型的奶酪，每块奶酪都只能被其中一只老鼠吃掉。
        下标为 i 处的奶酪被吃掉的得分为：
        如果第一只老鼠吃掉，则得分为 reward1[i] 。
        如果第二只老鼠吃掉，则得分为 reward2[i] 。
        给你一个正整数数组 reward1 ，一个正整数数组 reward2 ，和一个非负整数 k 。
        请你返回第一只老鼠恰好吃掉 k 块奶酪的情况下，最大 得分为多少。
        意思是说，在n块奶酪中，第一只老鼠吃掉k块奶酪，第二只老鼠吃掉n-k块奶酪，求第一只老鼠吃掉k块奶酪的最大得分

        可以用DFS遍历所有情况，但是会超时，类似001100这样的
        当1有k位时，2有n-k位，求最大值
        可以求出01全排列，然后求最大值

        假设都由第二只老鼠吃掉，得分为reward2的和sum，如果k=0，直接返回sum
        如果k=1，第一只老鼠吃一块，吃哪块呢？
        sum=sum+reward1[i]-reward2[i]的最大值
        每一次都这么选择，最后得到的sum就是最大值

        求极值，数组有序，可能用动态规划
        区间型问题
        dp[i][j] 表示吃到第i块奶酪，第一只s老鼠吃了j块奶酪的最大得分
        dp[i][j] = max(dp[i-1][j-1]+reward1[i-1], dp[i-1][j]+reward2[i-1])
        dp[0][0] = 0, dp[0][j]=0, dp[i][0]=reward2[i-1]都给第二只老鼠吃
        return dp[n][k]
        搞得通吗？？？
    */
    int miceAndCheese(vector<int>& reward1, vector<int>& reward2, int k) {
        // return miceAndCheese_0(reward1, reward2, k);
        // return miceAndCheese_1(reward1, reward2, k);
        return miceAndCheese_2(reward1, reward2, k);
    }

    int miceAndCheese_2(vector<int>& reward1, vector<int>& reward2, int k) {
        // 堆来实现, 还是有问题
        if (reward1.size() == 0 || reward2.size() == 0) {
            return 0;
        }
        if (k == 1 && reward1.size() == 1) {
            return reward1[0];
        }
        if (k == 0) {
            return accumulate(reward2.begin(), reward2.end(), 0);
        }
        priority_queue<int, vector<int>, less<int>> q;                    //小根堆
        for (int i = 0; i < reward1.size(); i++) {
            q.push(reward1[i] - reward2[i]);
        }
        // 老鼠1吐出小的，老鼠二吃掉大的
        int score = 0;
        for (int i = 0; i < reward2.size(); i++) {
            score += reward2[i];
        }
        cout << "score: " << score << endl;
        for (int i = 0; i < k; i++) {
            if (q.size() < 1) {
                break;
            }
            auto diff = q.top();
            q.pop();
            score += diff;
        }
        return score;
    }

    int miceAndCheese_0(vector<int>& reward1, vector<int>& reward2, int k) {
        // 动态规划，存在前后闭环依赖，但是搞不通
        if (reward1.size() == 0 || reward2.size() == 0) {
            return 0;
        }
        if (k == 1 && reward1.size() == 1) {
            return reward1[0];
        }
        if (k == 0) {
            return accumulate(reward2.begin(), reward2.end(), 0);
        }
        int n = reward1.size();
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
        dp[1][0] = reward2[0];
        for (int i = 1; i <= n; i++) {
            dp[i][0] = dp[i-1][0] + reward2[i-1];
        }
        vector<int> pre;
        for (int i = 1; i <= reward1.size(); ++i) {
            for (int j = 1; j <= k; ++j) {
                dp[i][j] = max(dp[i-1][j], dp[i-1][j-1] + reward1[i-1]);
            }
        }
        cout << "pre: ";
        for (auto& i : pre) {
            cout << i << " ";
        }
        cout << endl;
        cout << "dp:" << endl;
        for (auto& i : dp) {
            for (auto& j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
        return dp[n][k];
    }

    int miceAndCheese_1(vector<int>& reward1, vector<int>& reward2, int k) {
        // 全排列法
        if (reward1.size() == 0 || reward2.size() == 0) {
            return 0;
        }
        if (k == 1 && reward1.size() == 1) {
            return reward1[0];
        }
        if (k == 0) {
            return accumulate(reward2.begin(), reward2.end(), 0);
        }
        int n = reward1.size();
        vector<int> tmp(k,1);
        for (int i = 0; i < n - k; i++) {
            tmp.push_back(0);
        }
        int maxScore = 0;
        miceAndCheese_1_core(reward1, reward2, tmp, maxScore, 0);
        return maxScore;
    }
    void miceAndCheese_1_core(vector<int>& reward1,
                              vector<int>& reward2,
                              vector<int>& tmp,
                              int& maxScore,
                              int start) {
        if (start >= reward1.size()) {
            for (auto& i : tmp) {
                cout << i << " ";
            }
            cout << endl;
            int score = 0;
            for (int i = 0; i < reward1.size(); i++) {
                if (tmp[i] == 1) {
                    score += reward1[i];
                } else {
                    score += reward2[i];
                }
            }
            maxScore = max(maxScore, score);
            return;
        }
        unordered_set<int> s;
        for (int i = start; i < reward1.size(); i++) {
            // 全排列
            // if (tmp[start] == tmp[i] && start != i) { // 这个写法错误，会导致重复
            //     continue;
            // }
            if (s.count(tmp[i])) {
                continue;
            }
            s.insert(tmp[i]);
            swap(tmp[start], tmp[i]);
            miceAndCheese_1_core(reward1, reward2, tmp, maxScore, start + 1);
            swap(tmp[start], tmp[i]);
        }
    }
};


int main() {
    Solution s;
    vector<int> nums;
    vector<int> nums1;
    int k;
    auto test = [&s](vector<int>& nums, vector<int>& nums1, int k) {
        auto res = s.miceAndCheese(nums, nums1, k);
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << "nums1: ";
        for (auto& i : nums1) {
            cout << i << " ";
        }
        cout << " k : " << k;
        cout << " permutaions: "<< res << endl;
    };

    nums = {1,1,3,4};
    nums1 = {4,4,1,1};
    test(nums, nums1, 2); // 15

    nums = {1,1};
    nums1 = {1,1};
    test(nums, nums1, 1); // 2

    nums = {2,1};
    nums1 = {1,2};
    test(nums, nums1, 1); // 4

    nums = {1,2,1,2,1,2};
    nums1 = {2,1,1,2,2,1};
    test(nums, nums1, 0); // 9

    nums = {3,1,1,3};
    nums1 = {3,2,3,1};
    test(nums, nums1, 3); // 10

    nums = {3,1,1,3};
    nums1 = {3,2,3,1};
    test(nums, nums1, 2); // 11

    // nums = {54,59,94,87,32,10,55,44,21,73,12,70,89,49,13,34,78,20,20,75,90,44,48,74,78,32,70,76,79,49,50,69};
    // nums1 = {78,43,69,22,32,67,65,38,51,4,21,27,82,61,12,85,62,60,67,16,22,3,5,16,13,35,13,41,72,85,20,54};
    // test(nums, nums1, 3);
    return 0;
}