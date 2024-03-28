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
#include <string>

using namespace std;


class Solution {
public:
    /**
     * @param s: a string,  encoded message
     * @return: an integer, the number of ways decoding
     * 
     * 有一个消息包含A-Z通过以下规则编码
     * 'A' -> 1
        'B' -> 2
        ...
        'Z' -> 26
        现在给你一个加密过后的消息，问有几种解码的方式

        输入: "12"
        输出: 2
        解释: 它可以被解码为 AB (1 2) 或 L (12).

        输入: "10"
        输出: 1

        求方案数，字符有序，可能用动态规划
        区间0-i的字符有多少种解码方式，典型区间型问题
        status   dp[i] 表示字符串s的前i个字符有多少种解码方式
        function dp[i] = dp[i-1]*decodeOK(s[i-1]) + dp[i-2]*decodeOK(s[i-2,i-1])
                 往前看最多两位 -----i-2---->i-1---->i
                 dp[i] += dp[i-1] if s[i - 1] != 0
                 dp[i] += dp[i-2] if 9 < s[i-2,i-1] <= 26
        init     dp[0] = 1
        result   dp[n]
     */
    int numDecodings(string &s) {
        // write your code here
        return numDecodings_0(s);
    }

    int numDecodings_0(string &s) {
        if (s.size() == 0) {
            return 0;
        }
        vector<int> dp(s.size() + 1, 0);
        dp[0] = 1;
        dp[1] = s[0] != '0' ? 1 : 0;
        for (int i = 2; i <= s.size(); ++i) {
            // if (s[i - 1] != '0') {
            //     dp[i] += dp[i - 1];
            // }
            // int two = stoi(s.substr(i - 2, 2));
            // if (9 < two && two <= 26) {
            //     dp[i] += dp[i - 2];
            // }

            int two = stoi(s.substr(i - 2, 2));
            dp[i] = dp[i - 1] * (s[i - 1] != '0') + dp[i - 2] * (9 < two && two <= 26);
            // 可以用滚动数组优化，压缩到3位
        }
        cout << "dp:" << endl;
        for (auto d : dp) {
            cout << d << " ";
        }
        cout << endl;
        return dp[s.size()];
    }
};


int main() {
    Solution s;
    string nums;
    auto test = [&s](string& nums) {
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.numDecodings(nums);
        cout << "nums: " << nums << " permutaions: " << res << endl;
    };

    // nums = "";
    // test(nums);

    // nums = "0";
    // test(nums);

    // nums = "12";
    // test(nums);

    // nums = "10";
    // test(nums);

    nums = "1001";
    test(nums);

    nums = "19261001";
    test(nums);
}