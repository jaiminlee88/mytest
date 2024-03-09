#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <climits>
#include <stack>
#include <list>
using namespace std;


class Solution {
public:
    /**
     * @param start: a string
     * @param end: a string
     * @param dict: a set of string
     * @return: a list of lists of string
     *          we will sort your return value in output
     * 
     * 给出两个单词（start和end）和一个字典，找出所有从start到end的最短转换序列。
        变换规则如下：
        每次只能改变一个字母。
        变换过程中的中间单词必须在字典中出现。

        所有单词具有相同的长度。
        所有单词都只包含小写字母。
        题目确保存在合法的路径。
        单词数量小于等于10000
        单词长度小于等于10

        start = "a"
        end = "c"
        dict =["a","b","c"]
            [["a","c"]]

        start ="hit"
        end = "cog"
        dict =["hot","dot","dog","lot","log"]
            [["hit","hot","dot","dog","cog"],["hit","hot","lot","log","cog"]]

        直接用深度优先搜索
        可以先构建图，然后用DFS遍历，如果用BFS遍历可找到最短路径
     */
    vector<vector<string>> findLadders(string &start, string &end, unordered_set<string> &dict) {
        // return findLadders_0(start, end, dict);
        return findLadders_1(start, end, dict);
    }
    vector<vector<string>> findLadders_0(string &start, string &end, unordered_set<string> &dict) {
        // write your code here
        if (start == end) {
            return {{start,end}};
        }
        if (start.empty() || end.empty() || dict.empty()) {
            return {};
        }
        vector<vector<string>> res;
        vector<string> path;
        unordered_map<string,bool> visited;
        for (auto& str : dict) {
            visited[str] = false;
        }
        path.push_back(start);
        findLadders_core(start, end, dict, res, path, visited, -1);
        return res;
    }
    void findLadders_core(string &start,
                          string &end,
                          unordered_set<string> &dict,
                          vector<vector<string>> &res,
                          vector<string> &path,
                          unordered_map<string,bool> &visited,
                          int lastpos) {
        // a-z 97-122
        for(int pos = 0; pos < start.size(); ++pos) {
            if (pos == lastpos) {
                continue;
            }
            char old = start[pos];
            for (char c = 'a'; c <= 'z'; ++c) {
                start[pos] = c;
                if (start == end) {
                    path.push_back(end);
                    res.push_back(path);
                    path.pop_back();
                    break;
                }
                if (dict.find(start) != dict.end() && !visited[start]) {
                    // 树可能很深
                    visited[start] = true;
                    path.push_back(start);
                    findLadders_core(start, end, dict, res, path, visited, pos);
                    path.pop_back();
                    visited[start] = false;
                }
            }
            start[pos] = old;
        }
    }

    vector<vector<string>> findLadders_1(string &start, string &end, unordered_set<string> &dict) {
        /* write your code here
        // 另外解法，既然是搜索方案，一定是DFS，怎么搜索方便，就是用图
        // start 和 end 加入dict，然后构建图，就能构成完整路径，然后用bfs
        // dict ={"hot","dot","dog","lot","log"} + {hit，cog};
        // hot->[dot,lot,hit]
        // dot->[hot,lot,dog]
        // dog->[cog,log,dot]
        // lot->[dot,hot,log]
        // log->[cog,dog,lot]
        // hit->[hot]
        // cog->[dog,log]
        // 建立图后就可以从hit开始搜索了
        */
        if (start == end) {
            return {{start}};
        }
        if (dict.empty() || start.empty() || end.empty()) {
            return {};
        }
        unordered_map<string, vector<string>> graph;
        dict.insert(start);
        dict.insert(end);
        for (auto word : dict) {
            string new_word = word;
            for (int pos = 0; pos < word.size(); ++pos) {
                char old = new_word[pos];
                for (char c = 'a'; c <= 'z'; ++c) {
                    new_word[pos] = c;
                    if (new_word == word) {
                        continue;
                    }
                    if (dict.find(new_word) != dict.end()) {
                        graph[word].push_back(new_word);
                    }
                }
                new_word[pos] = old;
            }
        }
        cout << "graph:" << endl;
        for (auto& it : graph) {
            cout << it.first << "->";
            for (auto& v : it.second) {
                cout << v << " ";
            }
            cout << endl;
        }
        vector<vector<string>> res;
        unordered_set<string> visited;
        vector<string> path;
        int min_len = INT_MAX;
        path.push_back(start);
        visited.insert(start);
        findLadders_1_core(graph, start, end, res, path, visited, min_len);
        return res;
    }
    void findLadders_1_core(unordered_map<string, vector<string>> &graph,
                            string start,
                            string &end,
                            vector<vector<string>> &res,
                            vector<string> &path,
                            unordered_set<string> &visited,
                            int &min_len) {
        if (start == end) {
            if (min_len == INT_MAX || path.size() == min_len) {
                min_len = path.size();
                res.push_back(path);
                return;
            } else if (path.size() < min_len) {
                min_len = path.size();
                res.clear();
                res.push_back(path);
            }
            return;
        }
        if (path.size() == min_len) {
            return;
        }
        for (auto& next : graph[start]) {
            if (visited.find(next) != visited.end()) {
                continue;
            }
            path.push_back(next);
            visited.insert(next);
            findLadders_1_core(graph, next, end, res, path, visited, min_len);
            path.pop_back();
            visited.erase(next);
        }
    }
};


int main() {
    Solution s;
    vector<vector<char>> nums;
    vector<string> words;
    string start, end;
    unordered_set<string> dict;
    auto test = [&s](string start, string end, unordered_set<string>& dict) {
        cout << "start: " << start << " end: " << end << " dict: " << endl;
        for (auto& v : dict) {
            cout << v << " ";
        }
        cout << endl;
        // auto res = s.findLadders(start,end,dict);
        auto res = s.findLadders_1(start, end, dict);
        cout << "permutaions: "<< endl;
        for (auto& v : res) {
            cout << "[";
            for (auto& vv : v) {
                cout << vv << " ";
            }
            cout << "]" << endl;
        }
        cout << endl;
    };
    auto construct_board = [](vector<string> nums) -> vector<vector<char>> {
        vector<vector<char>> board;
        for (auto& i : nums) {
            vector<char> v;
            for (auto& j : i) {
                v.push_back(j);
            }
            board.push_back(v);
        }
        return board;
    };

    start = "a";
    end = "c";
    dict ={"a","b","c"};
    test(start, end, dict);

    start = "hit";
    end = "cog";
    dict ={"hot","dot","dog","lot","log"};
    test(start, end, dict);

    start = "hit";
    end = "cog";
    dict ={};
    test(start, end, dict);
    return 0;
}