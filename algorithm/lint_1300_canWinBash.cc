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
using namespace std;


class Solution {
public:
    /**
     * @param n: an integer
     * @return: whether you can win the game given the number of stones in the heap
     * 
     * 你正在和朋友玩一个游戏：桌子上有一堆石头，每一次你们都会从中拿出1到3个石头。
     * 拿走最后一个石头的人赢得游戏。游戏开始时，你是先手。
        假设两个人都绝对理性，都会做出最优决策。给定石头的数量，判断你是否会赢得比赛。
        举例：有四个石头，那么你永远不会赢得游戏。不管拿几个，最后一个石头一定会被你的朋友拿走。

        输入：n = 4 
        输出：False
        解析：先手取走1,2或者3，对方都会取走最后一个

        输入：n = 5 
        输出：True
        解析：先手拿1个，必胜
     */
    bool canWinBash(int n) {
        // return canWinBash0(n);
        return canWinBash1(n);
    }
    bool canWinBash0(int n) {
        // Write your code here
        // 太深了，超时，会stack overflow
        // n的结果，取决于n-1, n-2, n-3的结果
        if (n < 1) {
            return false;
        }
        if (n <= 3) {
            return true;
        }
        unordered_map<int, bool> memo;
        return canWinBash0_core(n, memo);
    }
    bool canWinBash0_core(int n, unordered_map<int, bool>& memo) {
        // 第一次进入一定是我先手
        if (n <= 3) {
            return true;
        }
        if (memo.find(n) != memo.end()) {
            return memo[n];
        }
        for (int i = 1; i <= 3; ++i) {
            if (!canWinBash0_core(n - i, memo)) {
                memo[n] = true; // 对方输了，就是我赢了
                return true;
            }
        }
        return false;
    }
    bool canWinBash1(int n) {
        // Write your code here
        // n的结果，取决于n-1, n-2, n-3的结果
        if (n < 1) {
            return false;
        }
        if (n <= 3) {
            return true;
        }
        vector<bool> dp(n + 1, false);
        dp[1] = dp[2] = dp[3] = true;
        for (int i = 4; i <= n; ++i) {
            dp[i] = !(dp[i-1] || dp[i-2] || dp[i-3]);
        }
        return dp[n];
    }
    
    bool canWinBash3(int n) {
        // 找规律法，通过打印答案找出规律
        if (n < 1) {
            return false;
        }
        if (n <= 3) {
            return true;
        }

        int mod = n % 4;
        if (mod == 0) { // 4个可走一轮，对方一定赢
            return false;
        }
        return true;
    }
};


int main() {
    Solution s;
    vector<int> nums;
    auto test = [&s](int n) {
        cout << "n: " << n;
        auto res = s.canWinBash(n);
        if (res) {
            cout << " can win" << endl;
        } else {
            cout << " can not win" << endl;
        }
    };

    test(4);
}