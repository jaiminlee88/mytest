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
     * @param word1: A string
     * @param word2: A string
     * @return: The minimum number of steps
     * 
     * 给出两个单词word1和word2，计算出将word1 转换为word2的最少操作次数。
        你可进行三种操作：
        插入一个字符
        删除一个字符
        替换一个字符

        word1 = "horse"
        word2 = "ros"
        3
        Explanation:
        horse -> rorse (replace 'h' with 'r')
        rorse -> rose (remove 'r')
        rose -> ros (remove 'e')

        word1 = "intention"
        word2 = "execution"
        Explanation:
        intention -> inention (remove 't')
        inention -> enention (replace 'i' with 'e')
        enention -> exention (replace 'n' with 'x')
        exention -> exection (替换 'n' 为 'c')
        exection -> execution (插入 'u')

        实在想不出别的办法，只能暴力搜索了

        求极值，且字符串有序，可能用动态规划
        大区间依赖小区间，典型区间型问题
        status d[i][j] 表示a字符串的前i个字符和b字符串的前j个字符的最小操作次数，初始化为操作次数为max
        function dp[i][j] = dp[i-1][j-1] if a[i] = b[j]
                          = min(dp[i-1][j] + 1, // add
                                dp[i][j-1] + 1, // del
                                dp[i-1][j-1] + 1 // replace
                          )
        init dp[0,0] = 0, dp[0,j] = j, dp[i,0] = i
        result dp[m][n]
     */
    int minDistance(string &word1, string &word2) {
        // return minDistance_0(word1, word2);
        return minDistance_1(word1, word2);
    }
    int minDistance_0(string &word1, string &word2) {
        // write your code here
        if (word1.size() == 0 && word2.size() == 0) {
            return 0;
        }
        vector<vector<int>> dp(word1.size() + 1, vector<int>(word2.size() + 1, INT_MAX));
        dp[0][0] = 0;
        for (int i = 1; i <= word1.size(); i++) {
            dp[i][0] = i;
        }
        for (int j = 1; j <= word2.size(); j++) {
            dp[0][j] = j;
        }
        // cout << "before dp: " << endl;
        // for (auto& v : dp) {
        //     for (auto& i : v) {
        //         cout << i << " ";
        //     }
        //     cout << endl;
        // }
        for (int i = 1; i <= word1.size(); i++) {
            for (int j = 1; j <= word2.size(); j++) {
                if (word1[i-1] == word2[j-1]) {
                    dp[i][j] = dp[i-1][j-1];
                    continue;
                }
                int tmp = min(dp[i-1][j] + 1, dp[i][j-1] + 1);
                tmp = min(tmp, dp[i-1][j-1] + 1);
                dp[i][j] = min(dp[i][j], tmp);
                // 可以用滚动数组优化
            }
        }
        cout << "dp: " << endl;
        for (auto& v : dp) {
            for (auto& i : v) {
                cout << i << " ";
            }
            cout << endl;
        }
        cout << "------------------------------------"  << dp[word1.size()][word2.size()] << endl;
        return dp[word1.size()][word2.size()];
    }

    int minDistance_1(string &word1, string &word2) {
        // write your code here  滚动数组优化
        if (word1.size() == 0 && word2.size() == 0) {
            return 0;
        }
        vector<vector<int>> dp(2, vector<int>(word2.size() + 1, INT_MAX));
        dp[0][0] = 0;
        dp[1][0] = 1;
        for (int j = 1; j <= word2.size(); j++) {
            dp[0][j] = j;
        }

        int curr_indx = 0;
        int prev_indx = 0;
        for (int i = 1; i <= word1.size(); i++) {
            curr_indx = i % 2;
            prev_indx = (i - 1) % 2;
            dp[curr_indx] = vector<int>(word2.size() + 1, INT_MAX);
            dp[curr_indx][0] = i;
            // for (auto it : dp[prev_indx]) {
            //     cout << it << " ";
            // }
            // cout << endl;   
            // for (auto it : dp[curr_indx]) {
            //     cout << it << " ";
            // }
            // cout << endl;
            for (int j = 1; j <= word2.size(); j++) {
                if (word1[i - 1] == word2[j-1]) {
                    dp[curr_indx][j] = dp[prev_indx][j-1];
                    continue;
                }
                int tmp = min(dp[prev_indx][j] + 1, dp[curr_indx][j-1] + 1);
                tmp = min(tmp, dp[prev_indx][j-1] + 1);
                dp[curr_indx][j] = min(dp[curr_indx][j], tmp);
            }
            // for (auto it : dp[curr_indx]) {
            //     cout << it << " ";
            // }
            // cout << endl;   
        }

        // cout << "curr_indx: " << curr_indx << " prev_indx: " << prev_indx << endl;
        // cout << "dp: " << endl;
        // for (auto& v : dp) {
        //     for (auto& i : v) {
        //         cout << i << " ";
        //     }
        //     cout << endl;
        // }
        return dp[curr_indx][word2.size()];
    }
};

int main() {
    Solution s;
    vector<int> nums;
    string a,b;
    auto test = [&s](string& a, string& b) {
        auto res = s.minDistance(a,b);
        cout << "a: " << a << " b: " << b << " res: " << res << endl;
    };

    a = "horse";
    b = "ros";
    test(a,b);

    a = "intention";
    b = "execution";
    test(a,b);
}