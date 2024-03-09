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
     * @param wordSet: A dictionary of words dict
     * @return: A boolean
     * 
     * 给定字符串 s 和单词字典 dict，判断是否可以利用字典 dict 中的出现的单词拼接出 s，
     * 其中字典 dict 中的单词可以重复使用。
        因为我们已经使用了更强大的数据，所以普通的DFS方法无法解决此题。

        s = "lintcode"
        dict = ["lint", "code"]
        返回 true，因为 "lintcode" 可以被拆分为 "lint code"

        s = "a"
        dict = ["a"]
        返回 true


     */
    bool wordBreak(string &s, unordered_set<string> &wordSet) {
        // write your code here
        // return wordBreak_0(s, wordSet);
        // return wordBreak_1(s, wordSet);
        // return wordBreak_2(s, wordSet);
        return wordBreak_3(s, wordSet);
    }
    bool wordBreak_0(string& s, unordered_set<string>& wordDict) {
        // 按位遍历，太长了
        if (s.size() == 0 || wordDict.size() == 0) {
            return false;
        }
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());
        unordered_map<string,bool> memo;
        bool ret = wordBreak_0_core(s, 0, wordSet, memo);
        for (auto& it : memo) {
            cout << "memo: " << it.first << " " << it.second << endl;
        }
        return ret;
    }
    bool wordBreak_0_core(string& s, int start, unordered_set<string>& wordSet, unordered_map<string,bool>& memo) {
        if (start == s.size()) {
            return true;
        }
        for (int pos = start + 1; pos <= s.size(); ++pos) {
            string lsub = s.substr(start, pos - start);
            auto it = memo.find(lsub);
            if (it == memo.end()) {
                if (wordSet.find(lsub) == wordSet.end()) {
                    memo[lsub] = false;
                    continue;
                }
                memo[lsub] = true;
            } else if (it->second == false) {
                continue;
            }
            if (wordBreak_0_core(s, pos, wordSet, memo)) {
                string rsub = s.substr(pos, s.size() - pos);
                memo[rsub] = true;
                return true;
            }
        }
        return false;
    }

    bool wordBreak_1(string& s, unordered_set<string>& wordDict) {
        // 按树进行遍历
        if (s.size() == 0 || wordDict.size() == 0) {
            return false;
        }
        unordered_map<char,vector<string>> graph;
        for (auto& it : wordDict) {
            graph[it[0]].push_back(it);
        }
        return wordBreak_1_core(s, 0, graph);
    }
    bool wordBreak_1_core(string& s, int start, unordered_map<char,vector<string>>& graph) {
        if (start == s.size()) {
            return true;
        }
        auto it = graph.find(s[start]);
        if (it != graph.end()) {
            for (auto& str : it->second) {
                if (str.size() > s.size() - start) {
                    continue;
                }
                bool match = true;
                for (int i = 0; i < str.size(); ++i) {
                    if (str[i] != s[start + i]) {
                        match = false;
                        break;
                    }
                }
                if (!match) {
                    continue;
                }
                if (wordBreak_1_core(s, start + str.size(), graph)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool wordBreak_2(string& s, unordered_set<string>& wordDict) {
        // 按树进行遍历，更少的空间，记忆化搜索,用最大单词剪枝s
        // 仍然有可能会栈溢出
        if (s.size() == 0) {
            return true;
        }
        if (wordDict.size() == 0) {
            return false;
        }
        int max_len = 0;
        for (auto& it : wordDict) {
            max_len = max(max_len, (int)it.size());
        }
        unordered_map<int,bool> memo;
        return wordBreak_2_core(s, 0, max_len, wordDict, memo);
    }
    bool wordBreak_2_core(string& s, int start, int max_len, unordered_set<string>& wordDict, unordered_map<int,bool>& memo) {
        if (start == s.size()) {
            return true;
        }
        if (memo.find(start) != memo.end()) {
            return memo[start];
        }
        for (int i = start; i < s.size() && i - start < max_len; ++i) {
            string sub = s.substr(start, i - start + 1);
            if (wordDict.find(sub) == wordDict.end()) {
                continue;
            }
            if (wordBreak_2_core(s, i + 1, max_len, wordDict, memo)) {
                // 后续会重新计算i+1位置数据
                // 返回时进行i+1时，会重复计算，用记忆化搜索
                return true;
            }
        }
        memo[start] = false; // 以start作为出发点，所有的单词都不匹配
        return false;
    }

    bool wordBreak_3(string& s, unordered_set<string>& wordSet) {
        // 动态规划，迭代方式
        // i是表示j的某个长度的前缀。
        if (s.size() == 0) {
            return true;
        }
        if (wordSet.size() == 0) {
            return false;
        }
        int n = s.size();
        vector<bool> dp(n + 1, false);
        dp[0] = true;
        //  ------------i-->j---------
        // 如果0-i i-j都是true，那么0-j也是true
        for (int j = 0; j <= n; ++j) {
            for (int i = 0; i < j; ++i) { // j往前不可能一直增长，最多到dict的max len
                if (dp[i] && wordSet.find(s.substr(i, j - i)) != wordSet.end()) {
                    dp[j] = true;
                    break;
                }
            }
        }
        return dp.back();
    }

    bool wordBreak_4(string s, unordered_set<string>& wordSet) {
        // 动态规划，迭代方式
        // 但是j是表示可能匹配的字符串的长度，所以加以minSize和maxSize的上限。
        // 显然，这个方法更好，因为字符串的长度不会很长，所以j循环次数少而且效率很高，因为一旦匹配上，就可以退出循环。
        // i从头扫到尾,dp[0]一开始等于true，最后看dp[len].
        int n = s.size();
        vector<bool> dp(n + 1, false);
        int minSize = INT_MAX, maxSize = INT_MIN;
        for (auto w : wordSet) {
            minSize = min(minSize, (int)w.size());
            maxSize = max(maxSize, (int)w.size());
        }
        dp[0] = true;
        //  ------------i-->---------
        //       <--j---| 
        //
        for (int i = 1; i <= n; ++i) {
            for (int j = minSize; j <= maxSize; j++) {
                if (i >= j && dp[i - j] && wordSet.find(s.substr(i - j, j)) != wordSet.end()) {
                    dp[i] = true;
                    break;
                }
            }
            //for (int j = 0; j < i; ++j) {
            //    if (dp[j] && wordSet.find(s.substr(j, i - j)) != wordSet.end()) {
            //        dp[i] = true;
            //        break;
            //    }
            //}
        }
        return dp[n];
    }

    bool wordBreak_5(string &s, unordered_set<string> &dict) {
        // 动态规划，迭代方式 不过是pos从尾扫到头，一开始dp[len] = true，最后看dp[0]。
        vector<bool> dp(s.size() + 1, false);
        int len = s.size();
        dp[len] = true;
        int minSize = INT_MAX, maxSize = 0;
        for (auto w : dict) {
            minSize = min(minSize, (int)w.size());
            maxSize = max(maxSize, (int)w.size());
        }
        //  ------<--pos-----------------------
        //            |----i-----> 
        for (int pos = len - 1; pos >= 0; pos--) {
            for (int i = minSize; i <= maxSize && pos + i <= len; i++) {
                string str = s.substr(pos, i);
                if (dict.find(str) == dict.end()) continue;
                if (dp[pos + i]) {
                    dp[pos] = true;
                    break;
                }
            }
        }
        return dp[0];
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
        cout << "word=[" << str << "] ";
        auto res = s.wordBreak(str, dicts);
        cout << "permutaions: ";
        if (res) {
            cout << "true" << endl;
        } else {
            cout << "false" << endl;
        }
    };

    nums = {"de", "ding", "co", "code", "lint"};
    str = "lintcode";
    test(nums, str);

    nums = {"lint", "code"};
    str = "lintcode";
    test(nums, str);

    nums = {"a"};
    str = "a";
    test(nums, str);

    nums = {"lin", "code"};
    str = "lintcode";
    test(nums, str);

    nums = {"a","b"};
    str = "abbb";
    test(nums, str);
}