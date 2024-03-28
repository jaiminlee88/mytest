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
     * @param str: the string
     * @return: the number of substrings 
     * 
     * 给出一个只包含0或1的字符串str,请返回这个字符串中全为0的子字符串的个数
     * 
     * 输入:"00010011"
        输出:9
        解释:
        "0"子字符串有5个,
        "00"子字符串有3个,
        "000"子字符串有1个。
        所以返回9

        输入:
        "010010"
        输出:
        5
     */
    int stringCount(string &str) {
        // Write your code here.
        return stringCount_0(str);
    }

    int stringCount_0(string &str) {
        // 同向双指针
        if (str.size() < 1) {
            return 0;
        }
        int cnt  = 0;
        int lpos = 0;
        int rpos = 0;
        while (lpos < str.size()) {
            if (str[lpos] != '0') {
                ++lpos;
                continue;
            }
            rpos = lpos + 1;
            while (rpos < str.size() && str[rpos] == '0') {
                ++rpos;
            }
            // 计算该0串子串个数
            cnt += calsubstr(str, lpos, rpos);
            lpos = rpos;
        }
        return cnt;
    }

    int calsubstr(string& str, int lpos, int rpos) {
        // 任取两个点，计算子串个数
        return (rpos - lpos) * (rpos - lpos - 1) / 2 + (rpos - lpos);
    }
};


int main() {
    Solution s;
    vector<int> nums;
    vector<int> nums0;
    vector<int> nums1;
    vector<vector<int>> nums3;
    string str;
    int k = 0;
    auto test = [&s](string& str) {
        auto res = s.stringCount(str);
        cout << "srt: " << str << " res: " << res << endl;
    };

    str = "00010011";
    test(str);

    str = "010010";
    test(str);
}