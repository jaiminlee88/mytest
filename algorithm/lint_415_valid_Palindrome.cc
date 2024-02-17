#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /**
     * @param s: A string
     * @return: Whether the string is a valid palindrome
     */
    // bool isPalindrome(string &s) {
    //     if (s.empty() || (s.size() == 1)) {
    //         return true;
    //     }
    //     for (int lpos = 0; lpos < s.size(); lpos++) {
    //         int rpos = lpos;
    //         if (checkPalindrome(s, lpos, rpos)) {
    //             cout << s.substr(lpos, rpos - lpos + 1) << endl;
    //             return true;
    //         }
    //         rpos = lpos + 1;
    //         if (checkPalindrome(s, lpos, rpos)) {
    //             return true;
    //         }
    //     }
    //     return false;
    // }
    // bool checkPalindrome(string& s, int lpos, int rpos) {
    //     if (s.empty() || (s.size() ==1)) {
    //         return true;
    //     }
    //     if (lpos > rpos) {
    //         return false;
    //     }
    //     while ((lpos >= 0) && (rpos < s.size())) {
    //         if (!isValidChar(s[lpos])) {
    //             --lpos;
    //             continue;
    //         }
    //         if (!isValidChar(s[rpos])) {
    //             --rpos;
    //             continue;
    //         }
    //         if (s[lpos] == s[rpos]) {
    //             --lpos;
    //             ++rpos;
    //             continue;
    //         }
    //         if (rpos - lpos == 1) {
    //             return false;
    //         }
    //     }
    //     return true;
    // }
    bool isValidChar(char& c) {
        if (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <='z'))
            ) {
                return true;
        }
        return false;
    }
    bool isPalindrome(string &s) { // 从两边往中间比较好，因为找不到中点
        if (s.empty() || (s.size() == 1)) {
            return true;
        }
        int lpos = 0;
        int rpos = s.size() - 1;
        while (lpos <= rpos) {
            if (!isValidChar(s[lpos])) {
                ++lpos;
                continue;
            }
            if (!isValidChar(s[rpos])) {
                --rpos;
                continue;
            }
            if (s[lpos] >= 'A' && s[lpos] <= 'Z') {
                s[lpos] += 32;
            }
            if (s[rpos] >= 'A' && s[rpos] <= 'Z') {
                s[rpos] += 32;
            }
            if (s[lpos] != s[rpos]) {
                return false;
            }
            ++lpos;
            --rpos;
        }
        return true;
    }
};


int main() {
    /*描述
    给定一个字符串，确定它是否是一个回文串。只考虑字母数字字符，忽略大小写。
    数字 48 - 57
    字母 A - Z 65 - 90
    字母 a - z 97 - 122
    a = A + 32
    */
    Solution s;
    string input = "A man, a plan, a canal: Panama";
    bool result = s.isPalindrome(input);
    cout << "input: " << input << " result: " << result << endl;

    input = "1b , 1";
    result = s.isPalindrome(input);
    cout << "input: " << input << " result: " << result << endl;

    input = "race a car";
    result = s.isPalindrome(input);
    cout << "input: " << input << " result: " << result << endl;


}