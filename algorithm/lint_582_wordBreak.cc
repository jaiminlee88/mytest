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
     * @param wordDict: A set of words.
     * @return: All possible sentences.
     *          we will sort your return value in output
     * 
     * 给一字串s和单词的字典dict,在字串中增加空格来构建一个句子，并且所有单词都来自字典。
        返回所有有可能的句子。

        输入："lintcode"，["de","ding","co","code","lint"]
        输出：["lint code", "lint co de"]
        解释：
        插入一个空格是"lint code"，插入两个空格是 "lint co de"

        输入："a"，[]
        输出：[]
        解释：字典为空
     */
    vector<string> wordBreak(string &s, unordered_set<string> &wordDict) {
        // write your code here
        // return wordBreak_0(s, wordDict);
        return wordBreak_1(s, wordDict);
    }

    vector<string> wordBreak_0(string &s, unordered_set<string> &wordset) {
        // DFS方法
        if (s.size() == 0 || wordset.size() == 0) {
            return {};
        }
        vector<string> res;
        vector<string> path;
        unordered_set<int> memo;
        int max_len = 0;
        for (auto& it : wordset) {
            max_len = max(max_len, (int)it.size());
        }
        wordBreak_0_core(s, 0, wordset, max_len, path, res, memo);
        return res;
    }
    void wordBreak_0_core(string& s,
                        int start,
                        unordered_set<string>& wordset,
                        int& max_len,
                        vector<string>& path,
                        vector<string>& res,
                        unordered_set<int>& memo) {
        if (start == s.size()) {
            string s;
            for (int i = 0; i < path.size(); ++i) {
                if (i == path.size() - 1) {
                    s += path[i];
                } else {
                    s += path[i] + " ";
                }
            }
            res.push_back(s);
        }
        if (memo.find(start) != memo.end()) {
            return;
        }
        bool found = false;
        for (int i = start; i < s.size() && i < start + max_len; ++i) {
            if (wordset.find(s.substr(start, i - start + 1)) == wordset.end()) {
                continue;
            }
            path.push_back(s.substr(start, i - start + 1));
            found = true;
            wordBreak_0_core(s, i + 1, wordset, max_len, path, res, memo);
            path.pop_back();
        }
        if (!found) {
            memo.insert(start);
        }
    }
    vector<string> wordBreak_1(string &s, unordered_set<string> &wordset) {
        // 动态规划方法，内存使用过多
        if (s.size() == 0 || wordset.size() == 0) {
            return {};
        }
        int max_len = 0;
        for (auto& it : wordset) {
            max_len = max(max_len, (int)it.size());
        }
        vector<vector<string>> dp(s.size() + 1, vector<string>()); // 依次扩展到s[i]的时候，所有可能的句子
        dp[0] = {""};
        for (int j = 1; j <= s.size(); ++j) {
            int start = max(0, j - max_len);
            for (int i = start; i < j; ++i) {
                // cout << "i: " << i << " j: " << j  << " " << s.substr(i, j - i) << endl;
                if (dp[i].size() == 0) {
                    continue;
                }
                if (wordset.find(s.substr(i, j - i)) == wordset.end()) {
                    continue;
                }
                for (auto& it : dp[i]) {
                    if (it.size() == 0) {
                        dp[j].push_back(s.substr(i, j - i));
                        continue;
                    }
                    dp[j].push_back(it + " " + s.substr(i, j - i));
                }
            }
            // for (int i = 0; i < j - max_len; ++i) {
            //     dp[i].clear();
            // }
        }
        cout << "dp:" << endl;
        for (int j = 0; j < dp.size(); j++) {
            cout << "j: " << j << " ";
            for (auto& i : dp[j]) {
                cout << "[" << i << "] ";
            }
            cout << endl;
        }
        return dp.back();
    }
};


int main() {
    // string a = "abc";
    // cout << a.substr(0, 0) << endl;
    // cout << a.substr(0, 1) << endl;
    // cout << a.substr(1, 2) << endl;
    // cout << a.substr(1, 3) << endl;
    // return 0;
    Solution s;
    unordered_set<string> nums;
    string str;
    auto test = [&s](unordered_set<string>& dicts, string str) {
        cout << "dicts: ";
        for (auto& i : dicts) {
            cout << "["<< i << "] ";
        }
        cout << "word=[" << str << "] " << endl;
        auto res = s.wordBreak(str, dicts);
        cout << "permutaions: ";
        for (auto& i : res) {
            cout << "["<< i << "] ";
        }
        cout << endl;
    };

    nums = {"cat", "mat", "ca", "tm", "at", "c", "dog", "og", "do"};
    str = "catmat";
    test(nums, str);

    nums = {"a"};
    str = "";
    test(nums, str);
}