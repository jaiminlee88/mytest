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
     * @param board: A list of lists of character
     * @param words: A list of string
     * @return: A list of string
     *          we will sort your return value in output
     * 
     * 给出一个由小写字母组成的矩阵和一个字典。找出所有同时在字典和矩阵中出现的单词。
     * 一个单词可以从矩阵中的任意位置开始，可以向左/右/上/下四个相邻方向移动。
     * 一个字母在一个单词中只能被使用一次。且字典中不存在重复单词
     * 
     * 输入：["doaf","agai","dcan"]，["dog","dad","dgdg","can","again"]
        输出：["again","can","dad","dog"]
        解释：
        d o a f
        a g a i
        d c a n
        矩阵中查找，返回 ["again","can","dad","dog"]。

        输入：["a"],["b"]
        输出：[]
        解释：
        a
        矩阵中查找，返回 []。
     */
    vector<string> wordSearchII(vector<vector<char>> &board, vector<string> &words) {
        // write your code here
        return wordSearchII_0(board, words);
    }

    vector<string> wordSearchII_0(vector<vector<char>> &board, vector<string> &words) {
        if (board.empty() || words.empty()) {
            return {};
        }
        vector<std::pair<int,int>> delta = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        vector<vector<bool>> visited(board.size(), vector<bool>(board[0].size(),false));
        unordered_map<char, vector<std::pair<int,int>>> char2pos;
        cout << board.size()<< endl;
        for (int r = 0; r < board.size(); ++r) {
            for (int c = 0; c < board[0].size(); ++c) {
                char2pos[board[r][c]].push_back({r,c});
            }
        }
        vector<string> res;
        for (auto& word : words) {
            vector<std::pair<int,int>> nodes = char2pos[word[0]];
            for (auto& pos : nodes) {
                // cout << "start pos: " << pos.first << " " << pos.second << endl;
                visited[pos.first][pos.second] = true;
                if (find_word(board, word, visited, char2pos, delta, res, pos, 0)) {
                    visited[pos.first][pos.second] = false;
                    res.push_back(word);
                    break;
                }
                visited[pos.first][pos.second] = false;
            }
        }
        return res;
    }

    bool valid_pos(vector<vector<char>> &board, vector<vector<bool>> &visited,  int r, int c) {
        return r >= 0 && r < board.size() && c >= 0 && c < board[0].size() && visited[r][c] == false;
    }
    bool find_word(vector<vector<char>> &board,
                   string &word,
                   vector<vector<bool>> &visited,
                   unordered_map<char, vector<std::pair<int,int>>> &char2pos,
                   vector<std::pair<int,int>> &delta,
                   vector<string> &res,
                   pair<int,int> &pos,
                   int start) {
        if (start == word.size() - 1) {
            return true;
        }
        for (auto& d : delta) {
            pair<int,int> next_pos = {pos.first + d.first, pos.second + d.second};
            if (!valid_pos(board, visited, next_pos.first, next_pos.second)) {
                continue;
            }
            // cout << "to pos: " << next_pos.first << " " << next_pos.second << endl;
            if (board[next_pos.first][next_pos.second] == word[start + 1]) {
                visited[next_pos.first][next_pos.second] = true;
                if (find_word(board, word, visited, char2pos, delta, res, next_pos, start + 1)) {
                    visited[next_pos.first][next_pos.second] = false;
                    return true;
                }
                visited[next_pos.first][next_pos.second] = false;
            }
        }
        return false;
    }
};


int main() {
    Solution s;
    vector<vector<char>> nums;
    vector<string> words;
    auto test = [&s](vector<vector<char>> &board, vector<string> &words) {
        cout << "board: " << endl;
        for (auto& i : board) {
            for (auto& j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
        cout << "words: ";
        for (auto& i : words) {
            cout << i << " ";
        }
        cout << endl;
        auto res = s.wordSearchII(board,words);
        cout << "permutaions: "<< endl;
        for (auto& v : res) {
            cout << v << " ";
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

    // nums = construct_board({"a"});
    // words = {"b"};
    // test(nums, words);

    // nums = construct_board({"doaf","agai","dcan"});
    // words = {"dog","can","dad","again"};
    // test(nums, words);

    // nums = construct_board({"abce","sfcs","adee"});
    // words = {"as","ab","cf","da","ee","e","adee","eeda"};
    // test(nums, words);

    nums = construct_board({"doaf","agai","dcan"});
    words = {"dog","dad","dgdg","can","again"};
    test(nums, words);
    return 0;
}