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
     * @param pattern: a string,denote pattern string
     * @param str: a string, denote matching string
     * @return: a boolean
     * 
     * 给定一个pattern和一个字符串str，查找str是否遵循相同的模式。
        这里遵循的意思是一个完整的匹配，在一个字母的模式和一个非空的单词str之间
        有一个双向连接的模式对应。(如果a对应s，那么b不对应s。
        例如，给定的模式= "ab"， str = "ss"，返回false）。


        输入:
        pattern = "abab"
        str = "redblueredblue"
        输出: true
        说明: "a"->"red","b"->"blue"
        abab
        red blue red blue

        输入:
        pattern = "aaaa"
        str = "asdasdasdasd"
        输出: true
        说明: "a"->"asd"
        aaaa
        asd asd asd asd

        输入:
        pattern = "aabb"
        str = "xyzabcxzyabc"
        输出: false
        aabb
        xyz abc xzy abc

        可以用dfs做，暴力搜索，逐位匹配，并记录模式，如果发现新位置不匹配模式，就回溯，逐位匹配太过耗时

     */
    bool wordPatternMatch(string &pattern, string &str) {
        // write your code here
        return wordPatternMatch_0(pattern, str);
    }

    bool wordPatternMatch_0(string& pattern, string& str) {
        // DFS
        // 记忆搜索在这里并不适用，因为每次都是不同的匹配
        // aa aaa | aa aa 记忆 aa aa->YY 不匹配 (aaaa, YY)=false, 所以 (aaa, YY)=true
        // X  Y   | Y  Y
        // aaa aa | aa aa 根据记忆 aa aa->YY 不匹配, 但是aaa aa->YY YY 匹配
        // X   Y  | Y  Y
        if (pattern.size() == 0 || str.size() == 0) {
            return false;
        }
        unordered_map<char, string> pmap;
        unordered_set<string> pset; // 存储已经匹配的字符串，确保不重复
        bool ret = wordPatternMatch_0_core(pattern, str, 0, 0, pmap, pset);
        cout << "pmap:" << endl;
        for (auto& it : pmap) {
            cout << it.first << "->" << it.second << endl;
        }
        return ret;
    }

    bool wordPatternMatch_0_core(string& pattern,
                                 string& str,
                                 int pstart,
                                 int sstart,
                                 unordered_map<char,string>& pmap,
                                 unordered_set<string>& pset
                                 ) {
        if (pstart == pattern.size() && sstart == str.size()) {
            return true;
        }
        auto it = pmap.find(pattern[pstart]);
        string s{""}; // 对应标志
        if (it != pmap.end()) {
            s = it->second;
        }
        if (s.size() + sstart > str.size()) {
            return false; // 说明已经没有足够的字符来匹配
        }
        if (str.substr(sstart, s.size()) != s) {
            return false;
        }
        if (s.size() != 0) {
            return wordPatternMatch_0_core(pattern, str, pstart + 1, sstart + s.size(), pmap, pset);
        }
        for (int i = sstart; i < (str.size() - (pattern.size() - pstart - 1)); ++i) {
            // 可以优化一下
            s = str.substr(sstart, i - sstart + 1); // 从一个开始位置，逐个增加字符
            if (pset.find(s) != pset.end()) {
                continue; // 已经有字符匹配过了
            }
            pmap[pattern[pstart]] = s; // 暂存对应链接
            pset.insert(s);
            if (wordPatternMatch_0_core(pattern, str, pstart + 1, i + 1, pmap, pset)) {
                return true;
            }
            pset.erase(s); // 回溯
            pmap.erase(pattern[pstart]); // 回溯
        }
        return false;
    }

    bool wordPatternMatch_0(string& pattern, string& str) {
        // DFS暴力搜索复杂度为O(n^m)，n为pattern长度，m为str长度
        // DFS的复杂度已经是多项式复杂度，动态规划 似乎并没有更好
    }
};


int main() {
    // string a = "abc";
    // cout << a.substr(0,0) << endl;
    // cout << a.substr(0,1) << endl;
    // return 0;
    Solution s;
    vector<int> nums;
    string pattern;
    string str;
    auto test = [&s](string& pattern, string& str) {
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.wordPatternMatch(pattern, str);
        cout << "pattern: " << pattern << " str: " << str ;
        if (res) {
            cout << " match" << endl;
        } else {
            cout << " not match" << endl;
        }
    };

    // pattern = "abab";
    // str = "redblueredblue";
    // test(pattern, str);

    // pattern = "aaaa";
    // str = "asdasdasdasd";
    // test(pattern, str);

    // pattern = "aabb";
    // str = "xyzabcxzyabc";
    // test(pattern, str);

    pattern = "itwasthebestoftimes";
    str = "ittwaastthhebesttoofttimes";
    test(pattern, str);
}