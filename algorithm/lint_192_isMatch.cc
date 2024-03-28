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
     * @param s: A string
     * @param p: A string includes "?" and "*"
     * @return: is Match?
     * 
     * 给定一个字符串 s 和一个字符模式 p ，实现一个支持 '?' 和 '*' 的通配符匹配。匹配规则如下：
        '?' 可以匹配任何单个字符。
        '*' 可以匹配任意字符串（包括空字符串）。
        两个串完全匹配才算匹配成功。

        s 仅包含小写英文字母
        p 包含小写英文字母，? 和 *

        输入:
        "aa"
        "a"
        输出: false

        输入:
        "aa"
        "aa"
        输出: true

        输入:
        "aaa"
        "aa"
        输出: false

        输入:
        "aa"
        "*"
        输出: true
        说明: '*' 可以替换任何字符串

        输入:
        "aa"
        "a*"
        输出: true

        输入:
        "ab"
        "?*"
        输出: true
        说明: '?' -> 'a' '*' -> 'b'

        输入:
        "aab"
        "c*a*b"
        输出: false

        本质上还是可以用DFS解决，逐步匹配，如果模式用完，但是没有到结尾，说明不匹配
     */
    bool isMatch(string &s, string &p) {
        // write your code here
        // return isMatch_0(s, p);
        // return isMatch_1(s, p);
        // return isMatch_2(s, p);
        return isMatch_3(s, p);
    }

    bool isMatch_0(string& s, string& p) {
        if (s == p) {
            return true;
        }
        if (s.size() != 0 && p.size() == 0) {
            return false;
        }
        int max_non_star = 0;
        for (auto c : p) {
            if (c != '*') {
                max_non_star++;
            }
        }
        return isMatch_0_core(s, 0, p, 0, max_non_star);
    }

    bool isMatch_0_core(string& s, int sstart, string& p, int pstart, int max_non_star) {
        if (pstart >= p.size() && sstart >= s.size()) {
            return true;
        }
        if (pstart < p.size() && sstart >= s.size()) {
            // 字符为空，模式还有，如果都是*，则匹配
            for (int i = pstart; i < p.size(); ++i) {
                if (p[i] != '*') {
                    return false;
                }
            }
            return true;
        }
        // if (pstart < p.size() && sstart >= s.size()) {
        //     return false; // ?????
        // }
        // cout << " sstart: " << sstart << " pstart: " << pstart << endl;
        if (p[pstart] == '*') { // 任意字符串,包括空字符串,如果多个*，只需要最后一个即可
            while(pstart + 1 < p.size() && p[pstart + 1] == '*') {
                pstart++;
            }
            // // * 匹配空字符串
            for (int i = sstart; i <= s.size() - max_non_star; ++i) {
                if (isMatch_0_core(s, i, p, pstart + 1, max_non_star)) {
                    return true;
                }
            }
            // return isMatch_0_core(s, sstart, p, pstart + 1, max_non_star) ||
            //        isMatch_0_core(s, sstart + 1, p, pstart, max_non_star);
        } else { // 单个字符，不包括空字符
            return (p[pstart] == '?' || p[pstart] == s[sstart])
                    && isMatch_0_core(s, sstart + 1, p, pstart + 1, max_non_star - 1);
        }
        return false;
    }


    bool isMatch_1(string& s, string& p) {
        // DFS 还是会超时
        if (s == p) {
            return true;
        }
        if (s.size() != 0 && p.size() == 0) {
            return false;
        }
        int max_non_star = 0;
        for (auto c : p) {
            if (c != '*') {
                max_non_star++;
            }
        }
        return isMatch_1_core(s, 0, p, 0, max_non_star);
    }

    bool isMatch_1_core(string& s, int sstart, string& p, int pstart, int max_non_star) {
        if (pstart >= p.size() && sstart >= s.size()) {
            return true;
        }
        if (pstart < p.size() && sstart >= s.size()) {
            // 字符为空，模式还有，如果都是*，则匹配
            for (int i = pstart; i < p.size(); ++i) {
                if (p[i] != '*') {
                    return false;
                }
            }
            return true;
        }
        // if (pstart < p.size() && sstart >= s.size()) {
        //     return false; // ?????
        // }
        // cout << " sstart: " << sstart << " pstart: " << pstart << endl;
        if (p[pstart] == '*') { // 任意字符串,包括空字符串,如果多个*，只需要最后一个即可
            // while(pstart + 1 < p.size() && p[pstart + 1] == '*') {
            //     pstart++;
            // }
            return isMatch_0_core(s, sstart, p, pstart + 1, max_non_star) ||
                   isMatch_0_core(s, sstart + 1, p, pstart, max_non_star);
        } else { // 单个字符，不包括空字符
            return (p[pstart] == '?' || p[pstart] == s[sstart])
                    && isMatch_0_core(s, sstart + 1, p, pstart + 1, max_non_star - 1);
        }
        return false;
    }

    bool isMatch_2(string& s, string& p) {
        // DFS + 记忆化搜索
        if (s == p) {
            return true;
        }
        if (s.size() != 0 && p.size() == 0) {
            return false;
        }
        unordered_map<string, bool> memo;
        return isMatch_2_core(s, 0, p, 0, memo);
    }

    bool isMatch_2_core(string& s, int sstart, string& p, int pstart,
                        unordered_map<string,bool>& memo) {
        if (pstart >= p.size() && sstart >= s.size()) {
            return true;
        }
        if (pstart < p.size() && sstart >= s.size()) {
            // 字符为空，模式还有，如果都是*，则匹配
            for (int i = pstart; i < p.size(); ++i) {
                if (p[i] != '*') {
                    return false;
                }
            }
            return true;
        }
        string key = to_string(sstart) + "_" + to_string(pstart);
        if (memo.find(key) != memo.end()) {
            // cout << "memo hit " << key << endl;
            return memo[key];
        }
        if (p[pstart] != '*') {
            return (p[pstart] == '?' || p[pstart] == s[sstart])
                    && isMatch_2_core(s, sstart + 1, p, pstart + 1, memo);
        }
        bool match = isMatch_2_core(s, sstart, p, pstart + 1, memo) || // 吃掉一个*
                   isMatch_2_core(s, sstart + 1, p, pstart, memo); // 吃掉一个字符
        memo[key] = match; // 记录遍历的状态
        return match;
    }

    bool isMatch_3(string& s, string& p) {
        // 动态规划
        /* 
            相当于求最大匹配？可能用动态规划？
            匹配型问题
            status   dp[i][j] 表示s的前i个字符和p的前j个字符是否匹配
            function dp[i][j] = dp[i-1][j-1] && (s[i] == p[j] || p[j] == '?') // 单个字符
                                dp[i-1][j] || dp[i][j-1] // * 匹配0个或者多个字符
                                *不为空字符串时，s[i-1]与p[j]的*已经匹配过，该*还能继续匹配s[i]
                                *为空字符串时，s[i]与p[j-1]匹配，那么*可以不用匹配s[i]
            init     dp[0][0] = true
                     dp[i][0] = false 字符串不为空，模式为空，i>0
                     dp[0][j] = dp[0][j-1] && p[j] == '*'
            result   dp[s.size()][p.size()]
        */
        if (s == p) {
            return true;
        }
        if (s.size() != 0 && p.size() == 0) {
            return false;
        }
        vector<vector<bool>> dp(s.size() + 1, vector<bool>(p.size() + 1, false));
        dp[0][0] = true;
        for (int j = 1; j <= p.size(); j++) {
            dp[0][j] = dp[0][j-1] && p[j - 1] == '*';
        }
        for (int i = 1; i <= s.size(); ++i) {
            for (int j = 1; j <= p.size(); ++j) {
                if (p[j - 1] == '*') { // 不符合上面情况，应该是*的情况
                    dp[i][j] = dp[i-1][j] || dp[i][j-1];
                } else {
                    dp[i][j] = dp[i-1][j-1] && (s[i - 1] == p[j - 1] || p[j - 1] == '?'); // 只依赖了两个状态，可以用滚动数组优化，注意每次要初始化
                }
            }
        }
        return dp[s.size()][p.size()];
    }

};


int main() {
    // string a = "abc";
    // cout << a.substr(0,0) << endl;
    // cout << a.substr(0,1) << endl;
    // if (a[0] == 'a') {
    //     cout << "a[0] == 'a'" << endl;
    // }
    // return 0;
    Solution s;
    vector<int> nums;
    string pattern;
    string str;
    auto test = [&s](string& pattern, string& str) {
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.isMatch(str, pattern);
        cout << "str: " << str << " pattern: " << pattern ;
        if (res) {
            cout << " [match]" << endl;
        } else {
            cout << " [not match]" << endl;
        }
    };

    // str ="aa";
    // pattern = "a";
    // test(pattern, str);

    // str ="aa";
    // pattern = "aa";
    // test(pattern, str);

    // str ="aaa";
    // pattern = "aa";
    // test(pattern, str);

    str ="aa";
    pattern = "*";
    test(pattern, str);

    str ="aa";
    pattern = "a*";
    test(pattern, str);

    str ="ab";
    pattern = "?*";
    test(pattern, str);

    str ="aab";
    pattern = "c*a*b";
    test(pattern, str);

    str ="cab";
    pattern = "c*a*b";
    test(pattern, str);

    str ="cab";
    pattern = "c****a****b";
    test(pattern, str);

    str ="cab";
    pattern = "c*a******";
    test(pattern, str);

    str = "aa";
    pattern = "a";
    test(pattern, str);
    return 0;
}