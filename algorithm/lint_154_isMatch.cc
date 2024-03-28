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
     * 实现支持 '.' 和 '*' 的正则表达式匹配。
     * '.' 匹配任意一个字母。
     * '*' 匹配零个或者多个前面的元素，'*' 代表表达式内，可以匹配零个或多个*前面的那一个元素，.*就是说多个
     * '*' 前保证是一个非 '*' 元素。
     * 匹配应该覆盖整个输入字符串，而不仅仅是一部分。
     * 
     * .表示具体的某个字母，*表示与前一个字符匹配0次或多次，0次是指前面这个字符出现了0次，也就是没有出现，这个看不懂代码都是错的
     * 需要实现的函数是：

     *  isMatch("aa","a") → false
        isMatch("aa","aa") → true
        isMatch("aaa","aa") → false
        isMatch("aa", "a*") → true
        isMatch("aa", ".*") → true
        isMatch("ab", ".*") → true  ".*" 表示可匹配零个 或 多个（'*'）任意字符（'.'）。


        DFS法

        动态规划法
        求方案可行性，字符有序，可能用动态规划
        覆盖整个字符串，而不仅仅是一部分，区间型问题
           status   dp[i][j] 表示a字符串的前i个字符与b字符串的前j个字符是否匹配，true or false
           function dp[i][j] = dp[i-1][j-1] && (s[i] == p[j] || p[j] == '.') if p[j] != '*'
                             = dp[i][j-2] || (dp[i-1][j] && (s[i] == p[j-1] || p[j-1] == '.')) if p[j] == '*'
           init     dp[0][0] = true, dp[0][j] = true 当且仅当b字符串的前j个字符都是*时，dp[i][0] = false 字符串非空但是模式串为空
           result   dp[m][n] =
        
     */
    bool isMatch(string &s, string &p) {
        // write your code here
        // return isMatch_0(s,p);
        return isMatch_1(s,p);
        // return isMatch_2(s,p);
        // return isMatch_3(s,p);
    }

    bool isMatch_0(string s, string p) {
        // by chatgpt
        int m = s.length(), n = p.length();
        vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));

        // Base case: both empty strings match
        dp[0][0] = true;

        // Handle patterns like  "_*" "a*", "b*", "c*", etc. null string can match _*
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                // a*a*a*
                // *a*a*a*
                dp[0][j] = dp[0][j - 2];
            }
        }

        // Fill the dp table
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (p[j - 1] == '.' || p[j - 1] == s[i - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else if (p[j - 1] == '*') {
                    dp[i][j] = dp[i][j - 2] || (dp[i - 1][j] && (s[i - 1] == p[j - 2] || p[j - 2] == '.'));
                }
            }
        }

        return dp[m][n];
    }

    bool isMatch_1(string &s, string &p) {
        /* 动态规划
        
        动态规划法
        求方案可行性，字符有序，可能用动态规划
        覆盖整个字符串，而不仅仅是一部分，区间型问题
            status   dp[i][j] 表示a字符串的前i个字符与b字符串的前j个字符是否匹配，true or false
            function dp[i][j] = dp[i-1][j-1] && (s[i-1] == p[j-1] || p[j-1] == '.') if p[j] != '*'
                              = dp[i][j-2] || (dp[i-1][j] && (s[i] == p[j-1] || p[j-1] == '.')) if p[j] == '*'
            init    dp[0][0] = true, 
                    dp[0][j] = true 当且仅当b字符串的前j个字符都是a*a*.*.*.*a*...时，
                    dp[i][0] = false 字符串非空但是模式串为空
            result dp[s.size()][p.size()]

        使用动态规划来实现支持 '.' 和 '*' 的正则表达式匹配，可以定义一个二维的状态数组 dp[i][j]，
        其中 dp[i][j] 表示字符串 s 的前 i 个字符和模式 p 的前 j 个字符是否匹配。然后根据模式 p 的第 j 个字符来确定状态转移方程。
        定义 dp[i][j] 的状态转移方程如下：
            如果 p[j-1] 是一个字母，并且 s[i-1] == p[j-1] 或者 p[j-1] == '.'，那么 dp[i][j] 取决于 dp[i-1][j-1]。
            如果 p[j-1] 是 ''，那么可以有两种情况：
                a. '' 匹配零个前面的元素，即将 '' 及其前面的字符在模式中抛弃，dp[i][j] 取决于 dp[i][j-2]。
                b. '' 匹配一个或多个前面的元素，即 s[i-1] 和 p[j-2] 匹配且 dp[i-1][j] 为真，则 dp[i][j] 为真。
        */
        if (s.empty() || p.empty()) {
            return false;
        }
        vector<vector<bool>> dp(s.size() + 1, vector<bool>(p.size() + 1, false));
        dp[0][0] = true;
        // for (int j = 2; j <= p.size(); j += 2) { // handle like a* b* c*...
        //     dp[0][j] = dp[0][j - 2] && p[j - 1] == '*';
        // }
        for (int j = 1; j <= p.size(); j++) {
            if (p[j - 1] == '*') {
                dp[0][j] = dp[0][j - 2];
            }
        }
        for (int i = 1; i <= s.size(); i++) {
            for (int j = 1; j <= p.size(); j++) {
                dp[i][j] = dp[i-1][j-1] && (s[i-1] == p[j-1] || p[j-1] == '.');
                if (dp[i][j] == false) { //
                    if (p[j - 1] == '*') {
                        dp[i][j] = dp[i][j - 2] || // p中第j字符出现了0次
                                   (dp[i - 1][j] && (s[i - 1] == p[j - 2] || p[j - 2] == '.')); // p中第j字符出现了1次或多次
                                                                                                // s第i个字符和p中第j-1个字符匹配或者p中第j-1个字符是'.'
                                                                                                // 同时，s的前i-1个字符和p的前j个字符匹配
                    }
                }
            }
        }
        // cout << "dp: " << endl;
        // for (auto& v : dp) {
        //     for (auto i : v) {
        //         cout << i << " ";
        //     }
        //     cout << endl;
        // }
        return dp[s.size()][p.size()];
    }

    bool isMatch_2(string& s, string& p) {
        if (s == p) {
            return true;
        }
        if (s.size() != 0 && p.size() == 0) {
            return false;
        }
        unordered_map<string, int> memo;
        return isMatch_2_core(s, 0, p, 0, memo);
    }

    void set_memo(unordered_map<string, int>& memo, int sstart, int pstart, int value) {
        string tmp = to_string(sstart) + "_" + to_string(pstart);
        memo[tmp] = value;
    }

    bool isMatch_2_core(string& s, int sstart, string& p, int pstart, unordered_map<string, int>& memo) {
        // 通过s去匹配p
        // https://blog.csdn.net/roufoo/article/details/82659855
        if (sstart >= s.size() && pstart >= p.size()) {
            return true;
        }
        if (sstart < s.size() && pstart >= p.size()) {
            return false;
        }

        if (sstart >= s.size() && pstart < p.size()) {
            for (int i = pstart; i < p.size(); i + 2) {
                if ((i + 1 >= p.size()) || (p[i + 1] != '*')) {
                    return false;
                }
            }
            return true;
        }

        string tmp = to_string(sstart) + "_" + to_string(pstart);
        if (memo.find(tmp) != memo.end()) {
            return memo[tmp];
        }

        // 相等情况
        if (p[pstart] == '.') {
            if (pstart + 1 < p.size() && p[pstart + 1] == '*') {
                if (isMatch_2_core(s, sstart, p, pstart + 2, memo)) { // 匹配0个
                    return true;
                }
                for (int i = sstart; i < s.size(); i++) {
                    if (isMatch_2_core(s, i + 1, p, pstart, memo)) {
                        return true;
                    }
                }
            }
            return isMatch_2_core(s, sstart + 1, p, pstart + 1, memo);
        }
        if (p[pstart] == s[sstart]) {
            if (pstart + 1 < p.size() && p[pstart + 1] == '*') {
                if (isMatch_2_core(s, sstart, p, pstart + 2, memo)) { // 匹配0个
                    return true;
                }
                for (int i = sstart; i < s.size(); i++) {
                    if (s[i] != s[sstart]) {
                        sstart = i;
                        break;
                    }
                    if (i == s.size() - 1) {
                        sstart = i + 1;
                        break;
                    }
                    if (isMatch_2_core(s, i + 1, p, pstart, memo)) {
                        return true;
                    }
                }
                return isMatch_2_core(s, sstart, p, pstart + 2, memo);
            }
            return isMatch_2_core(s, sstart + 1, p, pstart + 1, memo);
        } else {
            if (pstart + 1 < p.size() && p[pstart + 1] == '*') {
                return isMatch_2_core(s, sstart, p, pstart + 2, memo);
            }
        }
        memo[tmp] = false;
        return false;
    }

    bool isletter(char c) {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }

    class visitedMemo {
    public:
        bool visited;
        bool memo;
        visitedMemo(bool v = false, bool m = false) : visited(v), memo(m) {}
    };

    bool isMatch_3(string &s, string &p) {
        /* 动态规划*/
        if (s.empty() || p.empty()) return false;
        //define vm[s.size()][p.size()]
        vector<vector<visitedMemo>> vm;
        vm.resize(s.size(), vector<visitedMemo>(p.size()));
        return helper(s, 0, p, 0, vm);
    }

    bool helper(string const &s, int sIndex, string const &p, int pIndex, vector<vector<visitedMemo>> &vm) {

        // if p reaches the end (pos is the last char + 1) first, s should also reaches the end
        
        if (pIndex == p.size()) {
            return (sIndex == s.size());
        }

        // if s reaches the end first
        // for example, s = "abb", p = "a**?*?*..." or p = "a*"
        if (sIndex == s.size()) {
            for (int i = pIndex; i < p.size(); i += 2) {  //note i+=2 as it can be abbz*x*y*...
                if (((i + 1) >= p.size()) || (p[i + 1] != '*')) return false;
            }
            return true;
        }

        if (vm[sIndex][pIndex].visited) {
            return vm[sIndex][pIndex].memo;
        }
        vm[sIndex][pIndex].visited = true;
        if ((pIndex + 1 < p.size()) && (p[pIndex + 1] == '*')) {
            if ((s[sIndex] == p[pIndex]) || (p[pIndex] == '.')) {
                // * presents one or more preceding element
                vm[sIndex][pIndex].memo = helper(s, sIndex + 1, p, pIndex, vm);
            }
            //for example: s = "abc", p = "a*abc"
            //s[1] and p[0..1] match fail, but we have second chance.
            if (!vm[sIndex][pIndex].memo) {
                // * represents zero preceding element
                vm[sIndex][pIndex].memo = helper(s, sIndex, p, pIndex + 2, vm);
            }
        #if 0
            if (helper(s, sIndex, p, pIndex + 2, vm)) {
                vm[sIndex][pIndex].memo = true;
            }
            else {
                if ((s[sIndex] == p[pIndex]) || (p[pIndex] == '.')) {
                    vm[sIndex][pIndex].memo = helper(s, sIndex + 1, p, pIndex, vm);
                } else {
                    vm[sIndex][pIndex].memo = false;
                }
            }
        #endif
        } else {
            if ((s[sIndex] == p[pIndex]) || (p[pIndex] == '.')) {
                vm[sIndex][pIndex].memo = helper(s, sIndex + 1, p, pIndex + 1, vm);
            } else {
                vm[sIndex][pIndex].memo = false;
            }
        }

        return vm[sIndex][pIndex].memo;
    }
    // 动态规划 https://blog.csdn.net/qq_46105170/article/details/109213897
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
    auto test = [&s](string str, string pattern) {
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

    // test("aa","a");
    test("aa","aa");
    test("aaa","aa");
    test("a", "a*");
    test("aa", "a*");
    test("aa", ".*");
    test("ab", ".*");
    test("aaa", "a*a");
    test("aab", "c*a*b");
    test("abc", "ab*");

    return 0;
}