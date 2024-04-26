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
#include <bitset>
#include <deque>
using namespace std;


class Solution {
public:
    /**
     * @param chars: The letter array you should sort by Case
     * @return: nothing
     * 
     * 给定一个只包含字母的字符串 chars，按照先小写字母后大写字母的顺序进行排序。
        对于不同的语言，chars将以不用的形式给出，例如对于字符串 "abc" ，将以下面的形式给出

        Java: char[] chars = {'a', 'b', 'c'};
        Python：chars = ['a', 'b', 'c']
        C++：string chars = "abc";
        你需要实现原地算法解决这个问题，不需要返回任何值，我们会根据排序后的chars判断你的结果是否正确。

        chars = "abAcD"
        返回 "abcAD"

        chars = "ABC"
        返回 "ABC"

        不要求大写字母顺序，只要求小写字母在前，大写字母在后。
     */
    void sortLetters(vector<char> &chars) {
        // write your code here
        if (chars.empty() || chars.size() == 1) {
            return;
        }
        auto is_small_letter = [](char& c) {
            return c >= 'a' && c <= 'z';
        };

        int lpos = 0;
        int rpos = chars.size() - 1;
        while (lpos <= rpos) {
            while (lpos < rpos && is_small_letter(chars[lpos])) {
                ++lpos;
            }
            while (rpos > lpos && !is_small_letter(chars[rpos])) {
                --rpos;
            }
            if (lpos <= rpos) {
                swap(chars[lpos], chars[rpos]);
                ++lpos;
                --rpos;
            }
        }
    }
};

int main() {
    Solution s;
    vector<int> nums;
    vector<int> nums0;
    vector<int> nums1;
    vector<vector<int>> nums3;
    vector<vector<char>> nums4;
    vector<string> nums5;
    vector<char> nums6;
    string str;
    string str0;
    string str1;
    int k = 0;
    auto test = [&s](string str) {
        vector<char> nums(str.begin(), str.end());
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        s.sortLetters(nums);
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
    };

    str = "abAcD";
    test(str);

}