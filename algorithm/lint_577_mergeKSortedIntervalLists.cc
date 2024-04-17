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
#include <bitset>
#include <deque>
using namespace std;


/**
 * Definition of Interval:
 * classs Interval {
 *     int start, end;
 *     Interval(int start, int end) {
 *         this->start = start;
 *         this->end = end;
 *     }
 * }
 */

class Interval {
public:
    int start, end;
    Interval(int start, int end) {
        this->start = start;
        this->end = end;
    }
};

class item {
public:
    item(vector<Interval>& v, int p) : pos(p), vec(v) {}
    int pos;
    vector<Interval> vec;
};

class cmp {
public:
    bool operator() (item &a, item &b) {
        return a.vec[a.pos].start > b.vec[b.pos].start; // 小顶堆
    }
};

class Solution {
public:

    /**
     * @param intervals: the given k sorted interval lists
     * @return:  the new sorted interval list
     * 
     * 给定一个若干个闭区间组成的列表的列表，每个内层列表已经按照区间的左端点由小到大排好序了。
     * 要求将所有的区间做合并，然后按照从左到右的顺序输出。
     */
    vector<Interval> mergeKSortedIntervalLists(vector<vector<Interval>> &intervals) {
        return mergeKSortedIntervalLists_0(intervals);
    }
    vector<Interval> mergeKSortedIntervalLists_0(vector<vector<Interval>> &intervals) {
        if (intervals.empty()) {
            return {};
        }
        priority_queue<item, vector<item>, cmp> q;
        for (auto& v : intervals) {
            if (v.empty()) {
                continue;
            }
            q.push(item(v,0));
        }

        // while (q.empty() == false) {
        //     auto node = q.top();
        //     q.pop();
        //     cout << "node.pos " << node.vec[node.pos].start << " " << node.vec[node.pos].end << endl;
        // }
        vector<Interval> res;
        Interval cur(INT_MAX, INT_MAX);
        while (q.empty() == false) {
            auto node = q.top();
            q.pop();
            // cout << "node.pos [" << node.pos << "] ";
            // for (auto& i : node.vec) {
            //     cout << i.start << "," << i.end << " ";
            // }
            // cout << endl;
            if (cur.start == INT_MAX) {
                cur = node.vec[node.pos];
            } else {
                // cout << "ahahaha " << cur.start << " " << cur.end << endl;
                if (cur.end < node.vec[node.pos].start) {
                    res.push_back(cur);
                    cur = node.vec[node.pos];
                } else if (cur.end < node.vec[node.pos].end) {
                    cur.end = node.vec[node.pos].end;
                }
            }
            ++(node.pos);
            if (node.pos < node.vec.size()) {
                // cout << "ajajaja " << node.pos << endl;
                q.push(node);
            }
        }
        if (cur.start != INT_MAX) {
            res.push_back(cur);
        }
        return res;
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

    // nums = {};
    // test(nums);

    // nums = {1};
    // test(nums);

    // nums = {1,2};
    // test(nums);

    // nums = {1, 2, 3};
    // test(nums);

    // nums = {1,2,3,4};
    // test(nums);

    vector<vector<Interval>> intervals = {
                                {{1,2},{2,3},{5,7}},
                                {{4,5},{6,7},{10,12}},
                                {},
                                {{1,2},{2,3},{4,5},{5,6}},
                                {{3,5}, {8,9}}
                            };
    vector<Interval> res = s.mergeKSortedIntervalLists(intervals);
    for (auto& i : res) {
        cout << "[" << i.start << ", " << i.end << "]" << endl;
    }
    
    // item a(a, 0);s
}