#include <iostream>
#include <string>
#include <functional>
#include <vector>
using namespace std;

class Solution {
public:
    /**
     * @param s: input string
     * @return: a string as the longest palindromic substring
     */
    string longestPalindrome(string &s) {
        // write your code here
        return longestPalindrome4(s);
    }
    string longestPalindrome1(string &s) { // 枚举所有子串，n^3
        if (s.empty()) {
            return "";
        }
        std::string result = "";
        for (int lpos = 0; lpos < s.size(); lpos++) {
            for (int rpos = s.size() -1; rpos >= lpos; rpos--) {
                // test the squence from lpos to rpos, at least 3 chars
                int i = lpos;
                int j = rpos;
                // cout << "substr: " << s.substr(lpos, rpos - lpos + 1) << endl;
                while (i <= j) {
                    if (s[i] != s[j]) {
                        break;
                    }
                    ++i;
                    --j;
                    if (i >= j && (rpos - lpos + 1) > result.size()) {
                        result = s.substr(lpos, rpos - lpos + 1);
                        // printf("i=%d, j=%d\n", i, j);
                        // cout << "result : " << result << endl;
                        break;
                    }
                }
            }
        }
        return result;
    }

    string longestPalindrome1_1(string &s) { // n^3
        if (s.empty()) {
            return "";
        }
        std::string result = "";
        for (int lpos = 0; lpos < s.size(); lpos++) {
            for (int rpos = s.size() -1; rpos >= lpos; rpos--) {
                if (isPalindrome(lpos, rpos, s)) {
                    if ((rpos - lpos + 1) > result.size()) {
                        result = s.substr(lpos, rpos - lpos + 1);
                    }
                }
            }
        }
        return result;
    }
    bool isPalindrome(int lpos, int rpos, string& s) {
        // cout << "substr: " << s.substr(lpos, rpos - lpos + 1) << endl;
        while (lpos <= rpos) {
            if (s[lpos] != s[rpos]) {
                return false;
            }
            ++lpos;
            --rpos;
            if (lpos >= rpos) {
                // printf("i=%d, j=%d\n", i, j);
                // cout << "result : " << result << endl;
                return true;
            }
        }
        return false;
    }

    string longestPalindrome2(string &s) { // n^2，中间往两边走，枚举中心点
        if (s.empty()) {
            return "";
        }
        std::string result = "";
        for (int i = 0; i < s.size(); i++) {
            int lpos = i;
            int rpos = i;
            isPalindrome1(lpos, rpos, s);
            if ((rpos - lpos + 1) > result.size()) {
                result = s.substr(lpos, rpos - lpos + 1);
            }
            if ((i + 1 < s.size()) && (s[i] == s[i+1])) {
                lpos = i;
                rpos = i + 1;
                isPalindrome1(lpos, rpos, s);
                if ((rpos - lpos + 1) > result.size()) {
                    result = s.substr(lpos, rpos - lpos + 1);
                }
            }
        }
        return result;
    }
    void isPalindrome1(int& lpos, int& rpos, string& s) { // 把公共代码提出来，双指针法
        if (rpos >= s.size()) {
            rpos = lpos;
            return;
        }
        while ((lpos - 1 >= 0) && (rpos + 1 < s.size())) {
            if (s[lpos - 1] == s[rpos + 1]) {
                --lpos;
                ++rpos;
                continue;
            }
            break;
        }
    }

    string longestPalindrome3(string &s) { // 动态规划法，从右到左，更到的状态依赖于之前的状态，把之前的状态保存起来
        // s(i,j) = s(i+1, j-1) && s[i] = s[j] 同时满足则 s(i,j) 是回文串
        if (s.empty()) {
            return "";
        }
        bool** dp = new bool*[s.size()];
        for (int i = 0; i < s.size(); ++i) {
            dp[i] = new bool[s.size()];
        }
        for (int i = 0; i < s.size(); i++) {
            for (int j=0; j < s.size(); j++) {
                if (i == j) {
                    dp[i][j] = true;
                } else {
                    dp[i][j] = false;
                }
            }
        }
        std::string result = "";
        // 
        for (int lpos = s.size() - 1; lpos >= 0; lpos--) {
            for (int rpos = lpos; rpos < s.size(); rpos++) {
                if (lpos == rpos) {
                    if (rpos - lpos + 1 > result.size()) {
                        result = s.substr(lpos, rpos - lpos + 1);
                    }
                    continue;
                }
                // status : dp[i]][j]表示区间是不是回文子串
                // function : dp[i][j] = dp[i+1][j-1] && s[i] == s[j]
                // init : dp[i][i] = true, dp[i][i+1] = s[i] == s[i+1]
                // 第i的状态依赖于i+1的状态，而j状态以来于j-1 所以i从右到左，j从左到右
                // s(i,j) = s(i+1, j-1) && s[i] = s[j] 同时满足则 s(i,j) 是回文串
                if ((lpos + 1 < s.size()) && (rpos - 1 >= 0 )) {
                    if ( ((dp[lpos + 1][rpos -1]) && (s[lpos] == s[rpos])) ||
                         ((rpos - lpos == 1) && (s[lpos] == s[rpos])) ) {
                        dp[lpos][rpos] = true;
                        if (rpos - lpos + 1 > result.size()) {
                            result = s.substr(lpos, rpos - lpos + 1);
                        }
                    }
                }
            }
        }
        return result;
    }

    string longestPalindrome4(string &s) {
        if (s.empty()) {
            return "";
        }
        // status : dp[i]][j]表示区间是不是回文子串
        // function : dp[i][j] = dp[i+1][j-1] && s[i] == s[j]
        // init : dp[i][i] = true, dp[i][i+1] = s[i] == s[i+1]
        // 第i的状态依赖于i+1的状态，而j状态以来于j-1 所以i从右到左，j从左到右
        // 也可以根据区间长度来遍历，从小到大，先确定长度，再确定起点
        int max_len = 1;
        int lpos = 0;
        vector<vector<bool>> dp(s.size(), vector<bool>(s.size(), false));
        for (int i = 0; i < s.size(); i++) {
            dp[i][i] = true;
            if (i + 1 < s.size() && s[i] == s[i + 1]) {
                dp[i][i+1] = true;
                max_len = 2;
                lpos = i;
            }
        }
        for (int i = s.size() - 1; i >= 0; i--) { // i depends on i+1, thus from right to left
            for (int j = i + 2; j < s.size(); j++) { // j depends on j-1, thus from left to right, 7 8 9 至少有3个字符时才判断
                dp[i][j] = dp[i+1][j-1] && s[i] == s[j];
                if (dp[i][j] == true && (j - 1 + 1 > max_len)) {
                    max_len = j - i + 1;
                    lpos = i;
                }
            }
        }
        // cout << "max_len:" << max_len << " lpos:" << lpos << endl;
        // cout << "dp:" << endl;
        // for (auto& v : dp) {
        //     for (auto e : v) {
        //         cout << e << " ";
        //     }
        //     cout << endl;
        // }
        return s.substr(lpos, max_len);
    }
};


int main (){
    // 寻找最长回文子串
    // O(n)复杂度的是Manacher算法，这里不实现了
    Solution s;
    std::function<string(string&)> func = std::bind(&Solution::longestPalindrome4, s, std::placeholders::_1);
    // string str = "abcdzdcab";
    // cout << "origin:" << str << " -> " << func(str) << endl;
    // string str1 = "";
    // cout << "origin:" << str1 << " -> " << func(str1) << endl;
    // string str2 = "a";
    // cout << "origin:" << str2 << " -> " << func(str2) << endl;
    // string str3 = "aa";
    // cout << "origin:" << str3 << " -> " << func(str3) << endl;
    // string str4 = "aaa";
    // cout << "origin:" << str4 << " -> " << func(str4) << endl;
    // string str5 = "aaaa";
    // cout << "origin:" << str5 << " -> " << func(str5) << endl;
    // string str6 = "aaaaa";
    // cout << "origin:" << str6 << " -> " << func(str6) << endl;
    // string str7 = "aba";
    // cout << "origin:" << str7 << " -> " << func(str7) << endl;
    // string str8 = "abba";
    // cout << "origin:" << str8 << " -> " << func(str8) << endl;
    // string str9 = "abcdzzdcab";
    // cout << "origin:" << str9 << " -> " << func(str9) << endl;
    string str10 = "aaaabaaa";
    cout << "origin:" << str10 << " -> " << func(str10) << endl;
    return 0;
}