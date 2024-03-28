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
using namespace std;


class Solution {
public:
    /**
     * @param s: a string
     * @param k: a integer
     * @return: return a integer
     * 
     * 给定一个仅包含大写英文字母的字符串，
     * 您可以将字符串中的任何一个字母替换为的另一个字母，最多替换k次。 
     * 执行上述操作后，找到最长的，只含有同一字母的子字符串的长度。
     * 
     * 输入:
        "ABAB"
        2
        输出:
        4
        解释:
        将两个'A’替换成两个’B’，反之亦然。

        输入:
        "AABABBA"
        1
        输出:
        4
        解释:
        将中间的 'A’ 替换为 'B' 后得到 “AABBBBA"。
        子字符串"BBBB" 含有最长的重复字符, 长度为4。

        尝试逐个修改，然后计算最长的子串长度，
        lpos到达某个节点时，以该为起始，修改或者不修改lpos，rpos往后依次计数，并用完K，看最后谁最长
        应为是一段一段出现的，也许从某个节点开始，就出现想要的段了
        上面这个不太行

        暴力方法
        尝试逐个修改，然后计算最长的子串长度
        从某个节点i为中心，k次修改，分别尝试放左边和右边，看总和最长的
        或者用 同向双指针 确定所有起点i和终点j，找到i和j之间最多的字符，然后用k次修改，看最长的
     */
    int characterReplacement(string &s, int k) {
        // write your code here
        // return characterReplacement_0(s, k);
        // return characterReplacement_1(s, k);
        // return characterReplacement_2(s, k);
        return characterReplacement_3(s, k);
    }

    int characterReplacement_0(string &s, int k) {
        // write your code here
        // 很耗时
        if (s.size() <= 1) {
            return s.size();
        }
        int lpos = 0;
        int rpos = 0;
        int max_len = 0;
        int tmp_len = 0;
        for (lpos = 0; lpos < s.size(); ++lpos) {
            if (lpos > 0 && s[lpos] == s[lpos - 1]) {
                continue;
            }
            for (int lk = 0; lk <= k; ++lk) {
                tmp_len = left_count(s, 0, lpos - 1, lk, s[lpos]) + right_count(s, lpos + 1, s.size() - 1, k - lk, s[lpos]) + 1;
                max_len = max(max_len, tmp_len);
            }
        }
        return max_len;
    }

    int left_count(string &s, int lpos, int rpos, int k, int tmp) {
        if (rpos < 0) {
            return 0;
        }
        int cnt = 0;
        while (rpos >= lpos) {
            if (s[rpos] == tmp) {
                --rpos;
                ++cnt;
                continue;
            }
            if (k > 0) {
                --rpos;
                --k;
                ++cnt;
                continue;
            }
            break;
        }
        return cnt;
    }
    int right_count(string &s, int lpos, int rpos, int k, int tmp) {
        if (lpos >= s.size()) {
            return 0;
        }
        int cnt = 0;
        while (lpos <= rpos) {
            if (s[lpos] == tmp) {
                ++lpos;
                ++cnt;
                continue;
            }
            if (k > 0) {
                ++lpos;
                --k;
                ++cnt;
                continue;
            }
            break;
        }
        return cnt;
    }

    int characterReplacement_1(string &s, int k) {
        // 太复杂了，counter还要回退
        if (s.size() <= 1) {
            return s.size();
        }
        int j = 0;
        int answer = 0;
        int maxfreq = 0;
        int count = 0;
        unordered_map<char, int> counter;
        for (int i = 0; i < s.size(); i++) {
            int max_cnt = 0;
            for (int j = i; j < s.size(); j++) {
                // i - j 之间重复最多的字符是啥
                counter[s[j]]++; // 记录对应字符个数
                if (counter[s[j]] > max_cnt) {
                    max_cnt = counter[s[j]];
                    if (j - i + 1 - max_cnt <= k) { // 需要几次替换
                        answer = max(answer, j - i + 1);
                    }
                }
            }
            for (int j = i; j < s.size(); j++) {
                counter[s[j]]--; // 记录对应字符个数
            }
        }
        return answer;
    }

    int characterReplacement_2(string &s, int k) {
        // 同向双指针，不会退方法, i - j之间，控制到j时刚好需要更改k次
        if (s.size() <= 1) {
            return s.size();
        }
        int j = 0;
        int answer = 0;
        int maxfreq = 0;
        unordered_map<char, int> counter;
        for (int i = 0; i < s.size(); i++) {
            while (j < s.size() && (j - i - maxfreq <= k)) {
                counter[s[j]]++;
                if (counter[s[j]] > maxfreq) {
                    maxfreq = counter[s[j]];
                }
                j++;
            }
            // 此时j-1位是满足刚好需要更改k次的情况，j是下一个字符
            if (j - i - maxfreq > k) { // 没到最后字符
                answer = max(answer, j - i - 1);
            } else {
                answer = max(answer, j - i); // 最后字符了
            }
            counter[s[i]]--;
            maxfreq = 0;
            for (auto& it : counter) { // 重新找到最大的
                if (it.second > maxfreq) {
                    maxfreq = it.second;
                }
            }
        }
        return answer;
    }

    int characterReplacement_3(string s, int k) {
        // gpt写的
        // 双指针，滑动窗口
        int left = 0, maxCount = 0, maxLength = 0;
        unordered_map<char, int> countMap;

        for (int right = 0; right < s.length(); ++right) {
            ++countMap[s[right]];
            maxCount = max(maxCount, countMap[s[right]]);

            while (right - left + 1 - maxCount > k) {
                --countMap[s[left]];
                ++left;
            }

            maxLength = max(maxLength, right - left + 1);
        }

        return maxLength;
    }
};

int main() {
    Solution s;
    vector<int> nums;
    vector<int> nums0;
    vector<int> nums1;
    vector<vector<int>> nums3;
    string str;
    string str0;
    string str1;
    int k = 0;
    auto test = [&s](string& str, int k) {
        auto res = s.characterReplacement(str, k);
        cout << "nums: " << str << " k: " << k << " res: " << res << endl;
    };

    str = "AABABBA";
    k = 2;
    test(str, k);

    str = "ABAB";
    k = 1;
    test(str, k);

    str = "ABBBB";
    k = 1;
    test(str, k);

    str = "ABAB";
    k = 0;
    test(str, k);

    str = "AABBB";
    k = 1;
    test(str, k);

    // str = "KRSCDCSONAJNHLBMDQGIFCPEKPOHQIHLTDIQGEKLRLCQNBOHNDQGHJPNDQPERNFSSSRDEQLFPCCCARFMDLHADJADAGNNSBNCJQOF";
    // k = 4;
    // test(str, k);
    return 0;
}