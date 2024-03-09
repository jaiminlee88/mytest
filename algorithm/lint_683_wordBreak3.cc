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
     * @param dict: A set of word
     * @return: the number of possible sentences.
     * 
     * 给出一个单词表和一条去掉所有空格的句子，
     * 根据给出的单词表添加空格, 返回可以构成的句子的数量, 
     * 保证构成的句子中所有的单词都可以在单词表中找到.
     * 
     *  输入：
        "CatMat"
        ["Cat", "Mat", "Ca", "tM", "at", "C", "Dog", "og", "Do"]
        输出： 3
        解释：
        我们可以有如下三种方式：
        "CatMat" = "Cat" + "Mat"
        "CatMat" = "Ca" + "tM" + "at"
        "CatMat" = "C" + "at" + "Mat"

        输入：
            "a"
            []
            输出：
            0
        
        本质上跟107题拆单词一样
     */
    int wordBreak3(string &s, unordered_set<string> &dict) {
        // Write your code here
        // return wordBreak3_0(s, dict);
        return wordBreak3_1(s, dict);
    }

    int wordBreak3_0(string &s, unordered_set<string> &dict) {
        // DFS方法
        if (s.size() == 0 || dict.size() == 0) {
            return 0;
        }
        int max_len = 0;
        for (auto& it : dict) {
            max_len = max(max_len, (int)it.size());
        }
        string str = s;
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        unordered_set<string> wordset;
        for (auto& it : dict) {
            string word = it;
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            wordset.insert(word);
        }
        // cout << "max_len: " << max_len << endl;
        // for (auto& it : wordset) {
        //     cout << "wordset: " << it << endl;
        // }
        // cout << str << endl;
        int cnt = 0;
        unordered_map<int, bool> memo;
        vector<string> tmp;
        wordBreak3_0_core(str, 0, wordset, max_len, cnt, memo, tmp);
        // cout << "memo:" << endl;
        // for (auto& it : memo) {
        //     cout << it.first << " " << it.second << endl;
        // }
        // cout << endl;
        return cnt;
    }
    void wordBreak3_0_core(string& str,
                            int start,
                            unordered_set<string>& wordset,
                            int max_len,
                            int& cnt,
                            unordered_map<int, bool>& memo,
                            vector<string>& tmp) {
        if (start == str.size()) {
            // cout << "tmp: ";
            // for (auto& it : tmp) {
            //     cout << it << "->";
            // }
            // cout << endl;
            ++cnt;
            return;
        }
        if (memo.find(start) != memo.end() && memo[start] == false) {
            return;
        }
        bool found = false;
        for (int i = start; i < start + max_len && i < str.size(); ++i) {
            if (wordset.find(str.substr(start, i - start + 1)) == wordset.end()) {
                continue;
            }
            memo[start] = true;
            found = true;
            tmp.push_back(str.substr(start, i - start + 1));
            wordBreak3_0_core(str, i + 1, wordset, max_len, cnt, memo, tmp);
            tmp.pop_back();
        }
        if (!found) {
            memo[start] = false;
        }
    }

    int wordBreak3_1(string &s, unordered_set<string> &dict) {
        // 动态规划
        if (s.size() == 0 || dict.size() == 0) {
            return 0;
        }
        int max_len = 0;
        for (auto& it : dict) {
            max_len = max(max_len, (int)it.size());
        }
        string str = s;
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        unordered_set<string> wordset;
        for (auto& it : dict) {
            string word = it;
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            wordset.insert(word);
        }
        vector<int> dp(str.size() + 1, 0); //  dp[i]表示s[0:i]的子串可以被分割种类数
        dp[0] = 1;
        for (int j = 1; j <= str.size(); ++j) {
            int start = max(0, j - max_len); // j往前不可能一直增长，最多到dict的max len
            for (int i = start; i < j; ++i) {
                // cout << "i: " << i << " j: " << j << " str: " << str.substr(i, j - i) << endl;
                if (dp[i] && wordset.find(str.substr(i, j - i)) != wordset.end()) {
                    dp[j] += dp[i];
                }
            }
        }
        cout << "dp: ";
        for (auto& it : dp) {
            cout << it << " ";
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
        auto res = s.wordBreak3(str, dicts);
        cout << "permutaions: " << res << endl;
    };

    nums = {"Cat", "Mat", "Ca", "tM", "at", "C", "Dog", "og", "Do"};
    str = "CatMat";
    test(nums, str);

    nums = {"a"};
    str = "";
    test(nums, str);
}