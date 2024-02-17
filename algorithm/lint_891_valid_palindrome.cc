#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /**
     * @param s: A string
     * @return: Whether the string is a valid palindrome
     */
    // bool isPalindrome(string &s) { // 从两边往中间比较好，因为找不到中点
    //     if (s.empty() || (s.size() == 1)) {
    //         return true;
    //     }
    //     int lpos = 0;
    //     int rpos = s.size() - 1;
    //     while (lpos <= rpos) {
    //         if (s[lpos] != s[rpos]) {
    //             if (Palindrome_core(s, lpos + 1, rpos) || Palindrome_core(s, lpos, rpos - 1)) {
    //                 return true;
    //             }
    //             return false;
    //         }
    //         ++lpos;
    //         --rpos;
    //     }
    //     return true;
    // }
    // bool Palindrome_core(std::string s, int lpos, int rpos) {
    //     if (s.empty() || (s.size() == 1)) {
    //         return true;
    //     }
    //     if (lpos > rpos) {
    //         return false;
    //     }
    //     while (lpos <= rpos) {
    //         if (s[lpos] != s[rpos]) {
    //             return false;
    //         }
    //         ++lpos;
    //         --rpos;
    //     }
    //     return true;
    // }


    // 上方有重复代码，可以优化，放到一个函数里
    bool validPalindrome(string &s) { // 从两边往中间比较好，因为找不到中点
        if (s.empty() || (s.size() == 1)) {
            return true;
        }
        int lpos = 0;
        int rpos = s.size() - 1;
        if (Palindrome_core(s, lpos, rpos, true)) {
            return true;
        }
        return false;
    }
    bool Palindrome_core(std::string s, int lpos, int rpos, bool need_del) {
        if (s.empty() || (s.size() == 1)) {
            return true;
        }
        if (lpos > rpos) {
            return false;
        }
        while (lpos <= rpos) {
            if (s[lpos] != s[rpos]) {
                if (need_del) {
                    if (Palindrome_core(s, lpos + 1, rpos, false) || Palindrome_core(s, lpos, rpos - 1, false)) {
                        return true;
                    }
                }
                return false;
            }
            ++lpos;
            --rpos;
        }
        return true;
    }
    // 或者
    // if func(s, lpos, rpos) != true // lpos和rpos为不相同的位置
    //    return func(lpos+1, rpos, 1) || func(lpos, rpos-1, 1) 来实现
};


int main() {
    /*描述
    给一个非空字符串 s，你最多可以删除一个字符。判断是否可以把它变成回文串。
    给定的字符串只包含小写字母
    字符串的长度最大为 50000
    数字 48 - 57
    字母 A - Z 65 - 90
    字母 a - z 97 - 122
    a = A + 32
    */
    Solution s;
    string input = "aba";
    bool result = s.isPalindrome(input);
    cout << "input:[" << input << "] result: " << result << endl;

    input = "abca";
    result = s.isPalindrome(input);
    cout << "input:[" << input << "] result: " << result << endl;

    input = "abc";
    result = s.isPalindrome(input);
    cout << "input:[" << input << "] result: " << result << endl;

    input = "a";
    result = s.isPalindrome(input);
    cout << "input:[" << input << "] result: " << result << endl;

    input = "ab";
    result = s.isPalindrome(input);
    cout << "input:[" << input << "] result: " << result << endl;

    input = "aa";
    result = s.isPalindrome(input);
    cout << "input:[" << input << "] result: " << result << endl;

    input = "aaa";
    result = s.isPalindrome(input);
    cout << "input:[" << input << "] result: " << result << endl;

    input = "";
    result = s.isPalindrome(input);
    cout << "input:[" << input << "] result: " << result << endl;

    input = "abcbca";
    result = s.isPalindrome(input);
    cout << "input:[" << input << "] result: " << result << endl;
}