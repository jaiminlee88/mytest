#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <climits>
#include <stack>
using namespace std;


class Solution {
public:
    /**
     * @param str: A string
     * @return: all permutations
     * 给出一个字符串，找到它的所有排列，注意同一个字符串不要打印两次。
     * 
     * 样例
        给出 "abb"，返回 ["abb", "bab", "bba"]。
        给出 "aabb"，返回["aabb", "abab", "baba", "bbaa", "abba", "baab"]
     */
    vector<string> stringPermutation2(string &str) {
        if (str.size() == 0) {
            return {};
        }
        vector<string> res;
        sort(str.begin(), str.end());
        stringPermutation2_0(str, res, 0);
        return res;
    }
    void stringPermutation2_0(string &str, vector<string> &res, int start) {
        if (start >= str.size()) {
            res.push_back(str);
            return;
        }
        for (int i = start; i < str.size(); i++) {
            if (i != start && str[i] == str[i-1]) {
                continue;
            }
            swap(str[i], str[start]);
            stringPermutation2_0(str, res, start + 1);
            swap(str[i], str[start]);
        }
    }
};


int main() {
    Solution s;
    auto test = [&s](string &str) {
        cout << "str: " << str << endl;
        string _str = str;
        auto res = s.stringPermutation2(_str);
        cout << "permutaions: [" << res.size() << "] "<< endl;
        for (auto& v : res) {
            cout << v << " ";
        }
        cout << endl;
    };

    string str;

    str = "abb";
    test(str);

    str = "aabb";
    test(str);

    str = "abcde";
    test(str);
}