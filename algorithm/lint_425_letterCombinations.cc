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
     * @param digits: A digital string
     * @return: all possible letter combinations
     *          we will sort your return value in output
     *
     * 给一个不包含0和1的数字字符串，每个数字代表一个字母，请返回其所有可能的字母组合。
     * 1->"abc", 2->"def", 3->"ghi", 4->"jkl", 5->"mno", 6->"pqrs", 7->"tuv", 8->"wxyz"
     * 
     * 输入: "23"
        输出: ["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"]
        解释: 
        '2' 可以是 'a', 'b' 或 'c'
        '3' 可以是 'd', 'e' 或 'f'

        输入: "5"
        输出: ["j", "k", "l"]

        纯粹求组合问题
        DFS 递归求解，或者非递归
     */
    vector<string> letterCombinations(string &digits) {
        // write your code here
        if (digits.size() == 0) {
            return {};
        }
        vector<vector<string>> numletter(10, vector<string>());
        numletter[2] = {"a", "b", "c"};
        numletter[3] = {"d", "e", "f"};
        numletter[4] = {"g", "h", "i"};
        numletter[5] = {"j", "k", "l"};
        numletter[6] = {"m", "n", "o"};
        numletter[7] = {"p", "q", "r", "s"};
        numletter[8] = {"t", "u", "v"};
        numletter[9] = {"w", "x", "y", "z"};
        vector<string> res;
        vector<string> path;
        letterCombinations_0(digits, numletter, res, path, 0);
        return res;
    }
    void letterCombinations_0(string &digits, 
                              vector<vector<string>> &numletter,
                              vector<string> &res,
                              vector<string> &path,
                              int start) {
        // 递归法
        if (start >= digits.size()) {
            string s;
            for (auto& c : path) {
                s += c;
            }
            res.push_back(s);
            return;
        }
        for (auto& it : numletter[digits[start] - '0']) {
            path.push_back(it);
            letterCombinations_0(digits, numletter, res, path, start + 1);
            path.pop_back();
        }
    }

    void letterCombinations_1() {
        // 非递归法
        // 用队列，每次取出一个元素，然后在后面加上新的元素，每次for队列长度，然后往里元素里添加新元素
    }
};


int main() {
    Solution s;
    vector<int> nums;
    string num_str;
    // auto test = [&s](vector<int>& nums) {
    //     cout << "nums: ";
    //     for (auto& i : nums) {
    //         cout << i << " ";
    //     }
    //     cout << endl;
    //     // auto res = s.permute(nums);
    //     // auto res = s.permute_0(nums);
    //     // auto res = s.permute_1(nums);
    //     // auto res = s.permute_2(nums);
    //     auto res = s.permute_3(nums);
    //     cout << "permutaions: "<< endl;
    //     for (auto& v : res) {
    //         for (auto& i : v) {
    //             cout << i << " ";
    //         }
    //         cout << endl;
    //     }
    // };

    auto test_str = [&s](string& num_str) {
        cout << "num_str: " << num_str << endl;
        auto res = s.letterCombinations(num_str);
        cout << "letterCombinations: "<< endl;
        for (auto& v : res) {
            cout << v << endl;
        }
    };

    num_str = "23";
    test_str(num_str);
}