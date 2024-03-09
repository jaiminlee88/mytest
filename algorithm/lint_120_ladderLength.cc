#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
using namespace std;


class Solution {
public:
    /**
     * @param start: a string
     * @param end: a string
     * @param dict: a set of string
     * @return: An integer
     * 给出两个单词（start和end）和一个字典，找出从start到end的最短转换序列，输出最短序列的长度。
     * 变换规则如下：
     *   每次只能改变一个字母。
     *   变换过程中的中间单词必须在字典中出现。(起始单词和结束单词不需要出现在字典中)
     * 
     * 如果不存在这样的转换序列，返回 0。
        所有单词具有相同的长度。
        所有单词只由小写字母组成。
        字典中不存在重复的单词。
        你可以假设 beginWord 和 endWord 是非空的，且二者不相同。

        len(dict)<=5000,len(start)<5

        BFS可以解决问题
        DFS在图分支多时可能适合，
        而BFS在图的分支不多时可能更适合
     */
    int ladderLength(string &start, string &end, unordered_set<string> &dict) {
        // write your code here
        if (start.size() != end.size()) {
            return -1;
        }
        if (dict.size() == 0) {
            return -1;
        }
        if (start == end) {
            return 1;
        }
        if (start.size() == 1 || diff_only_one(start, end)) {
            return 2;
        }
        std::unordered_set<string> visited;
        std::queue<string> q;
        std::vector<string> path;
        q.push(start);
        visited.insert(start);
        int level = 1;
        path.push_back(start);
        while (q.empty() == false) {
            int size = q.size();
            ++level;
            for (int i = 0; i < size; ++i) {
                string curword = q.front();
                q.pop();
                // 尝试修改每一个字符，发现存在就放入队列环宇城
                for (int pos = 0; pos < curword.size(); ++pos) {
                    // a ~ z 97 ~ 122
                    string word = curword;
                    for (int i = 97; i <= 122; ++i) {
                        word[pos] = i;
                        auto it = dict.find(word);
                        if (it == dict.end()) {
                            continue;
                        }
                        auto it2 = visited.find(word);
                        if (it2 != visited.end()) {
                            continue;
                        }
                        // 查看是否只有一位不同
                        if (diff_only_one(word, end)) {
                            return level + 1;
                        }

                        q.push(word);
                        visited.insert(word);
                    }
                }
            }
        }
        return -1;
    }
    bool diff_only_one(string& word, string& end) {
        int diff = 0;
        for (int j = 0; j < word.size(); ++j) {
            if (word[j] != end[j]) {
                ++diff;
                if (diff > 1) {
                    break;
                }
            }
        }
        return diff == 1;
    }

    int ladderLength_1(string &start, string &end, unordered_set<string> &dict) {
         // write your code here
         // 只在dict内部找，而不是全部24个字符都找一遍
         // 但是建立图本身就是一个O(n^2)的操作
         // 也可以边查找边建立图
        if (start.size() != end.size()) {
            return -1;
        }
        if (dict.size() == 0) {
            return -1;
        }
        if (start == end) {
            return 1;
        }
        if (start.size() == 1 || diff_only_one(start, end)) {
            return 2;
        }
        dict.insert(end);
        std::unordered_map<string, vector<string>> graph;
        graph[start] = {};
        for (auto word : dict) {
            if (start != word && diff_only_one(start, word)) {
                graph[start].push_back(word);
            }
            for (auto word2 : dict) {
                if (word == word2) {
                    continue;
                }
                if (diff_only_one(word, word2)) {
                    graph[word].push_back(word2);
                }
            }
        }
        std::queue<string> q;
        std::unordered_set<string> visited;
        int level = 1;
        q.push(start);
        visited.insert(start);
        while(q.empty() == false) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                string curword = q.front();
                q.pop();
                for (auto& str : graph[curword]) {
                    if (str == end) {
                        return level + 1;
                    }
                    if (visited.find(str) != visited.end()) {
                        continue;
                    }
                    q.push(str);
                    visited.insert(str);
                }
            }
            ++level;
        }
        return -1;
    }

    int ladderLength_2(string &start, string &end, unordered_set<string> &dict) {
         // write your code here
         // 只在dict内部找，而不是全部24个字符都找一遍
         // 但是建立图本身就是一个O(n^2)的操作
         // 也可以边查找边建立图, 最差还是会回退
        if (start.size() != end.size()) {
            return -1;
        }
        if (dict.size() == 0) {
            return -1;
        }
        if (start == end) {
            return 1;
        }
        if (start.size() == 1 || diff_only_one(start, end)) {
            return 2;
        }
        dict.insert(end);
        std::unordered_map<string, vector<string>> graph;
        std::queue<string> q;
        std::unordered_set<string> visited;
        int level = 1;
        q.push(start);
        visited.insert(start);
        while(q.empty() == false) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                string curword = q.front();
                q.pop();

                auto word_list = graph.find(curword);
                if (word_list == graph.end()) {
                    for (auto word : dict) {
                        if (word == curword) {
                            continue;
                        }
                        if (diff_only_one(word, curword)) {
                            graph[curword].push_back(word);
                        }
                    }
                    word_list = graph.find(curword);
                }

                for (auto& str : word_list->second) {
                    if (str == end) {
                        return level + 1;
                    }
                    if (visited.find(str) != visited.end()) {
                        continue;
                    }
                    q.push(str);
                    visited.insert(str);
                }
            }
            ++level;
        }
        return -1;
    }
};



int main() {
    Solution s;
    unordered_set<string> dict;
    string start, end;

    start = "a";
    end = "c";
    dict = {"a","b","c"};
    cout << s.ladderLength_2(start, end, dict) << endl; // 2

    start ="hit";
    end = "cog";
    dict = {"hot","dot","dog","lot","log"};
    cout << s.ladderLength_2(start, end, dict) << endl; // 5
}