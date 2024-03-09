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
#include <functional>
using namespace std;


/**
 * Definition for a point.
 * struct Point {
 *     int x;
 *     int y;
 *     Point() : x(0), y(0) {}
 *     Point(int a, int b) : x(a), y(b) {}
 * };
 */

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int a, int b) : x(a), y(b) {}
};

class Solution {
public:
    /**
     * 
     * add(number). Add a new number in the data structure.
        topk(). Return the top k largest numbers in this data structure. 
        k is given when we create the data structure.
        Example
        Example1

        Input:
        s = new Solution(3);
        s.add(3)
        s.add(10)
        s.topk()
        s.add(1000)
        s.add(-99)
        s.topk()
        s.add(4)
        s.topk()
        s.add(100)
        s.topk()

        Output:
        [10, 3]
        [1000, 10, 3]
        [1000, 10, 4]
        [1000, 100, 10]

        用最大堆
        如果heap.SIZE小于k，继续添加
        如果heap.SIZE大于k，弹出最小的
     * **/
    /*
    * @param k: An integer
    */
    int _k;
    priority_queue<int, vector<int>, greater<int>> _heap;
    Solution(int k) {
        _k = k;
    }

    /*
     * @param num: Number to be added
     * @return: nothing
     */
    void add(int num) {
        _heap.push(num);
        int size = _heap.size() - _k;
        while (size > 0) {
            _heap.pop();
            size--;
        }
    }

    /*
     * @return: Top k element
     */
    vector<int> topk() {
        vector<int> res;
        while (!_heap.empty()) {
            res.push_back(_heap.top());
            _heap.pop();
        }
        for (auto& i : res) {
            _heap.push(i);
        }
        return res;
    }
};



int main() {
    vector<int> nums;
    int k;
    auto test = [](vector<int>& nums,  int k) {
        Solution s(k);
        for (auto& i : nums) {
            cout << i << ",";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        vector<vector<int>> res;
        for (auto& i : nums) {
            if (i == INT32_MAX) {
                auto v = s.topk();
                res.push_back(v);
                continue;
            }
            s.add(i);
        }
        cout << "permutaions: "<< endl;
        for (auto& i : res) {
            cout << "(";
            for (auto& j : i) {
                cout << j << ",";
            }
            cout << ")";
        }
        cout << endl;
    };

    nums =  {3,10,INT32_MAX,1000,-99,INT32_MAX,4,INT32_MAX,100,INT32_MAX};
    k = 3;
    test(nums, k);
}