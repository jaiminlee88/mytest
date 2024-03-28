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
#include <numeric>
using namespace std;


class Solution {
public:
    /**
     * @param image: a binary matrix with '0' and '1'
     * @param x: the location of one of the black pixels
     * @param y: the location of one of the black pixels
     * @return: an integer
     * 
     * 一个由二进制矩阵表示的图，0 表示白色像素点，
     * 1 表示黑色像素点。黑色像素点是联通的，即只有一块黑色区域。
     * 像素是水平和竖直连接的，给一个黑色像素点的坐标 (x, y) ，
     * 返回囊括所有黑色像素点的矩阵的最小面积。
     * 
     * 
     * 输入：["0010","0110","0100"]，x=0，y=2
        输出：6
        解释：
        矩阵左上角坐标是(0, 1), 右下角的坐标是(2, 2)

        输入：["1110","1100","0000","0000"], x = 0, y = 1
        输出：6
        解释：
        矩阵左上角坐标是(0,  0), 右下角坐标是(1, 2)

        输入：["1110","1111","1111","1110"], x = 0, y = 1
        输出：9

        想把所有黑色节点都圈起来，找到最小的矩形
        需要找到最下面的节点，最左边，最右边，最上边的节点
     */
    int minArea(vector<vector<char>> &image, int x, int y) {
        // write your code here
        return minArea_0(image, x, y);
    }
    int minArea_0(vector<vector<char>> &image, int r, int c) {
        // write your code here
        if (image.size() < 1) {
            return 0;
        }
        int m = image.size();
        int n = image[0].size();
        // 从0-x找最左边的节点
        int left, right, up, down;
        int spos = 0;
        int epos = c;
        int mpos = 0;

        spos = 0;
        epos = c;
        while (spos + 1 < epos) {
            mpos = spos + (epos - spos) / 2;
            if (check_col(image, mpos)) {
                epos = mpos;
            } else {
                spos = mpos;
            }
        }
        if (check_col(image, spos)) {
            left = spos;
        } else {
            left = epos;
        }
        // cout << "left : " << left << " spos: " << spos << " epos: " << epos << endl;

        spos = c;
        epos = n - 1;
        while (spos + 1 < epos) {
            mpos = spos + (epos - spos) / 2;
            if (check_col(image, mpos)) {
                spos = mpos;
            } else {
                epos = mpos;
            }
        }
        if (check_col(image, epos)) {
            right = epos;
        } else {
            right = spos;
        }
        // cout << "right: " << right << " spos: " << spos << " epos: " << epos << endl;

        spos = 0;
        epos = r;
        while (spos + 1 < epos) {
            mpos = spos + (epos - spos) / 2;
            if (check_row(image, mpos)) {
                epos = mpos;
            } else {
                spos = mpos;
            }
        }
        if (check_row(image, spos)) {
            up = spos;
        } else {
            up = epos;
        }
        cout << "up: " << up << " spos: " << spos << " epos: " << epos << endl;

        spos = r;
        epos = m - 1;
        while (spos + 1 < epos) {
            mpos = spos + (epos - spos) / 2;
            if (check_row(image, mpos)) {
                spos = mpos;
            } else {
                epos = mpos;
            }
        }
        if (check_row(image, epos)) {
            down = epos;
        } else {
            down = spos;
        }
        // cout << "down: " << down << " spos: " << spos << " epos: " << epos << endl;

        cout << "left: " << left << " right: " << right << " up: " << up << " down: " << down << endl;
        return (right - left + 1) * (down - up + 1);
    }

    bool check_row(vector<vector<char>> &image, int r) {
        if (r < 0 || r >= image.size()) {
            return false;
        }
        for (int i = 0; i < image[0].size(); ++i) {
            if (image[r][i] == '1') {
                return true;
            }
        }
        return false;
    }
    bool check_col(vector<vector<char>> &image, int c) {
        if (c < 0 || c >= image[0].size()) {
            return false;
        }
        for (int i = 0; i < image.size(); ++i) {
            if (image[i][c] == '1') {
                return true;
            }
        }
        return false;
    }
};


int main() {
    Solution s;
    vector<int> nums;
    vector<int> nums0;
    vector<int> nums1;
    vector<vector<int>> nums3;
    vector<vector<char>> nums4;
    vector<string> nums5;
    string str;
    string str0;
    string str1;
    int k = 0;
    auto test = [&s](vector<vector<char>>& nums, int x, int y) {
        cout << "nums: " << endl;
        for (auto& i : nums) {
            for (auto& j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.minArea(nums, x, y);
        cout << "x: " << x << " y: " << y << " permutaions: " << res << endl;
    };

    nums4 = {
        {'0', '0', '1', '0'},
        {'0', '1', '1', '0'},
        {'0', '1', '0', '0'},
    };
    test(nums4, 0, 2);

    nums4 = {
        {'1', '1', '1', '0'},
        {'1', '1', '0', '0'},
        {'0', '0', '0', '0'},
        {'0', '0', '0', '0'},
    };
    test(nums4, 0, 1);

    nums4 = {
        {'1', '1', '1', '0'},
        {'1', '1', '1', '1'},
        {'1', '1', '1', '1'},
        {'1', '1', '1', '0'},
    };
    test(nums4, 0, 1);

    auto convert = [](vector<string>& nums){
        vector<vector<char>> res;
        for (auto& i : nums) {
            vector<char> tmp;
            for (auto j : i) {
                tmp.push_back(j);
            }
            res.push_back(tmp);
        }
        return res;
    };

    nums5 = {"11111111111","11111111111","11111111111","11111111111","11111111111","11111111111","11001111111","01001111111","00101111111","11111111111","11111111111","11111111111","11101111111","11111111101","11111110111","10111111100","11111111100","11111011100","11001111111","11111111111","01111111111","01111111111","00111101111"};
    nums4 = convert(nums5);
    cout << nums4.size() << " " << nums4[0].size() << endl;
    test(nums4, 21, 10);
}