#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
using namespace std;


/**
 * Definition for Directed graph.
 * struct DirectedGraphNode {
 *     int label;
 *     vector<DirectedGraphNode *> neighbors;
 *     DirectedGraphNode(int x) : label(x) {};
 * };
 */

struct DirectedGraphNode {
    int label;
    vector<DirectedGraphNode *> neighbors;
    DirectedGraphNode(int x) : label(x) {};
};

class Solution {
public:
    /**
     * @param words: a list of words
     * @return: a string which is correct order
     * 有一种新的使用拉丁字母的外来语言。但是，你不知道字母之间的顺序。
     * 你会从词典中收到一个非空的单词列表，其中的单词在
     * 这种新语言的规则下按字典顺序排序。  -->很重要，意味着单词内的字符都是有序的，单词前后也是有序的
     * 请推导出这种语言的字母顺序。
     *
     * 你可以假设所有的字母都是小写。
        如果字符串 a 是字符串 b 的前缀，且 b 出现在 a 之前，那么这个顺序是无效的。
        如果顺序是无效的，则返回空字符串。
        这里可能有多个有效的字母顺序，返回以正常字典顺序看来最小的。
        一个字符串中的字母默认是同一等级的，且按照人类字典序排序。

        输入：["wrt","wrf","er","ett","rftt"]
        输出："wertf"
        解释：
        从 "wrt"和"wrf" ,我们可以得到 't'<'f'
        从 "wrt"和"er" ,我们可以得到'w'<'e'
        从 "er"和"ett" ,我们可以得到 get 'r'<'t'
        从 "ett"和"rftt" ,我们可以得到 'e'<'r'
        所以返回 "wertf"

        输入：["z","x"]
        输出："zx"
        解释：
        从 "z" 和 "x"，我们可以得到 'z' < 'x'
        所以返回"zx"

        本质上存在依赖关系
     */
    bool isPrefix(string& word1, string& word2) {
        if (word2.size() > word1.size()) {
            return false;
        }
        for (int i = 0; i < word2.size(); ++i) {
            if (word1[i] != word2[i]) {
                return false;
            }
        }
        return true;
    }
    string alienOrder(vector<string> &words) {
        // Write your code here
        string sequence{""};
        if (words.size() == 0) {
            return sequence;
        } else if (words.size() == 1) {
            return words[0];
        }

        std::unordered_map<char, std::unordered_set<char>> graph;
        std::unordered_map<char, int> indegree;
        for (int i = 0; i < words.size(); ++i) {
            for (int j = 0; j < words[i].size(); ++j) {
                char c = words[i][j];
                if (graph.find(c) == graph.end()) {
                    graph[c] = std::unordered_set<char>();
                }
                if (indegree.find(c) == indegree.end()) {
                    indegree[c] = 0;
                }
            }
        }
        for (int i = 0; i < words.size() - 1; ++i) {
            string word1 = words[i];
            string word2 = words[i + 1];
            int len = std::min(word1.size(), word2.size());
            for (int j = 0; j < len; ++j) {
                if (word1[j] != word2[j]) {
                    graph[word1[j]].insert(word2[j]);
                    break;
                }

                if ((j == len - 1) && (word1.size() > word2.size())) {
                    return "";
                }
            }
            // for (int k = 0; k < i; ++k) {
            //     if (isPrefix(words[k], word2)) {
            //         return "";
            //     }
            // }
        }
        for (auto& it : graph) {
            auto it_iter = indegree.find(it.first);
            if (it_iter == indegree.end()) {
                indegree[it.first] = 0;
            }
            for (auto& it_1 : it.second) {
                indegree[it_1]++;
            }
        }

        for (auto& it : graph) {
            std::cout << it.first << "->";
            for (auto& it_1 : it.second) {
                std::cout << it_1 << " ";
            }
            std::cout << std::endl;
        }
        for (auto& it : indegree) {
            std::cout << it.first << " degree=" << it.second << std::endl;
        }

        std::priority_queue<char, vector<char>, greater<char>> q;
        for (auto& it : indegree) {
            if (it.second == 0) {
                q.push(it.first);
            }
        }
        while(q.empty() == false) {
            char curnode = q.top();
            q.pop();
            sequence += curnode;
            auto it = graph.find(curnode);
            if (it == graph.end()) {
                continue;
            }
            for (auto& neighbor : it->second) {
                indegree[neighbor]--;
                if (indegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }
        return sequence.size() == indegree.size() ? sequence : "";
    }

    string alienOrder_1(vector<string> &words) {
        if (words.size() == 0) {
            return "";
        }
        unordered_map<char, int> indegree; //入度表//如果a->b，则b的入度为1
        for (int i = 0; i < words.size(); i++) {
            for (char c : words[i]) {
                indegree[c] = 0; //初始时，所有字符的入度都为0
            }
        }//比如a->b，a->c，则key为a，value为{b, c}//记录字符的后继字符（邻居）
        unordered_map<char, unordered_set<char>> graph;
        for (int i = 0; i < words.size() - 1; i++) {//比较相邻字符串(当前字符串和下一个字符串），找边
            string cur = words[i];
            string next = words[i + 1];//需要比较的长度是两个字符串中较小的长度，重叠的部分才有可能产生边//如bck和bckf只需要比较3个字符长度即可,
            int len = min(cur.size(), next.size()); 
            int j = 0;
            for (; j < len; j++) {
                if (cur[j] != next[j]) { //找到一条边如absfk和abcf，找到一条边s->c
                    if (!graph[cur[j]].count(next[j])) { //同一条边不要重复加入度
                        graph[cur[j]].insert(next[j]);
                        indegree[next[j]]++; 
                    }
                    break; //找到一条边就跳出当前循环
                }
            }
            if (j < cur.size() && j == next.size()) { //cur的长度 > next的长度，如bckf 和 bck，这种情况就无字典序
                return "";
            }
        }
        
        for (auto& it : graph) {
            std::cout << it.first << "->";
            for (auto& it_1 : it.second) {
                std::cout << it_1 << " ";
            }
            std::cout << std::endl;
        }
        for (auto& it : indegree) {
            std::cout << it.first << " degree=" << it.second << std::endl;
        }
        
        //拓扑排序
        string ans;
        priority_queue<char, vector<char>, greater<char>> que;
        for (auto it : indegree) {
            if (it.second == 0) { //所有入度为0的点先入队
                que.push(it.first);
            }
        }
        while (!que.empty()) {
            char cur = que.top();
            que.pop();
            ans += cur;
            if (graph.count(cur)) {
                for (char next : graph[cur]) {
                    if (--indegree[next] == 0) { //cur的邻居的入度全部减1，再将邻居入度为0的入队
                        que.push(next); 
                    }
                }
            }
        }//如"zy""zx"，应该输出"yxz"//"ad","abc"应该输出"abcd"
        return ans.size() == indegree.size() ? ans : "";
    }
};

int main() {
    Solution s;
    vector<string> seqs;
    auto test = [&s] (vector<string>& grid) {
        cout << "============" << endl;
        cout << "grid: ";
        for (auto row : grid) {
            cout << row << " ";
        }
        cout << endl;
        string ret = s.alienOrder_1(grid);
        cout << "sequence=" << ret << endl;
    };

    seqs = {"wrt","wrf","er","ett","rftt"};
    test(seqs);

    seqs = {"z","x"};
    test(seqs);

    seqs = {"zy","zx"}; // yxz
    test(seqs);

    seqs = {"ab","adc"};
    test(seqs);

    seqs = {"abc","bcd","qwert","ab"};
    test(seqs);
}