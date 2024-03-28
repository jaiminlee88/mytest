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
     * @param a: A string
     * @param b: A string
     * @return: The length of longest common subsequence of A and B
     * 给出两个字符串，找到最长公共子序列(LCS)，返回LCS的长度。
     * 最长公共子序列的定义：
        最长公共子序列问题是在一组序列（通常2个）中找到最长公共子序列
        （注意：不同于子串，LCS不需要是连续的子串）。该问题是典型的计算机科学问题，
        是文件差异比较程序的基础，在生物信息学中也有所应用。

        给出"ABCD" 和 "EDCA"，这个LCS是 "A" (或 D或C)，返回1
        给出 "ABCD" 和 "EACB"，这个LCS是"AC"，返回2

        公共子序列不要求连续
        用DFS暴力搜索，时间复杂度O(2^n)
        求极值，且字符串有序，可能用动态规划
        典型区间型问题，大区间依赖小区间
        status   dp[i][j]表示字符串a的前i个字符和字符串b的前j个字符的最长公共子序列长度
        function dp[i][j] = dp[i-1][j-1] + 1 if a[i] == b[j]
                            = max(dp[i-1][j], dp[i][j-1]) if a[i] != b[j]
        init
        result
     */

    int longestCommonSubsequence(string &a, string &b) {
        // write your code here
        if ((a.size() > 0 && b.size() < 1) ||
            (a.size() < 1 && b.size() > 0) ||
            (a.size() < 1 && b.size() < 1)) {
            return 0;
        }
        vector<vector<int>> dp(a.size() + 1, vector<int>(b.size() + 1, 0));
        // dp[0][j] = 0, dp[i][0] = 0
        for (int i = 1; i <= a.size(); ++i) {
            for (int j = 1; j <= b.size(); j++) {
                // cout << "i: " << i << " j: " << j << " a[i-1]=" << a[i-1] << " b[j-1]=" << b[j-1] << endl;
                // cout << "dp[i-1][j-1]: " << dp[i-1][j-1] << " dp[i-1][j]: " << dp[i-1][j] << " dp[i][j-1]: " << dp[i][j-1] << endl;
                if (a[i - 1] == b[j - 1]) {
                    // 方式一
                    dp[i][j] = dp[i-1][j-1] + 1;
                    // // 方式二，相对保险
                    // int tmp = dp[i-1][j] > dp[i][j-1] ? dp[i-1][j] : dp[i][j-1];
                    // dp[i][j] = tmp > dp[i-1][j-1] + 1 ? tmp : dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = max(dp[i-1][j], dp[i][j-1]); // i字符于j字符不可能同时出现在最长公共子序列中
                }
                // 可以用滚动数组优化空间
            }
        }
        cout << "dp:" << endl;
        for (int i = 0; i <= a.size(); ++i) {
            for (int j = 0; j <= b.size(); j++) {
                cout << dp[i][j] << " ";
            }
            cout << endl;
        }
        return dp[a.size()][b.size()];
    }
};

int main() {
    Solution s;
    vector<int> nums;
    string a,b;
    auto test = [&s](string& a, string& b) {
        auto res = s.longestCommonSubsequence(a,b);
        cout << "a: " << a << " b: " << b << " res: " << res << endl;
    };

    a = "ABCD";
    b = "EDCA";
    test(a,b);

    a = "ABCD";
    b = "EACB";
    test(a,b);
}