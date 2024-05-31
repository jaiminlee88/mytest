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

#include "../../algorithm/segment_tree.hh"
using namespace std;


class Solution {
    /*
    *
    // 给定一个数轴，以及一组线段，对于有线段相交的地方，定义深度为线段的数量，求平均相交深度。端点重合不考虑相交。
    // 平均相交深度的定义为，数轴上的一个点上如果有超过一条线段，则称存在一次相交，如果两个点之间的所有点上有的线
    段构成的集合是相同的，则这两个点上的相交是同一个相交。一个相交的深度是这个相交上线段的数量，要求的是这些深度值的平均值。
    --------
        -------
    // 0 1 2 3 4 5 6
    // ----
    //   ----
    //     ------
    //       ------

    // 3 intersections with depth 3 2 2

    // 0 1 2 3 4 5 6
    // -------------
    //   ----
    //       ----

    如果对线段的end进行排序，不断划分区间，在下一次划分区间的时候重新置0，重新计算深度
    重新划分end时机在于，依次遍历，上一个线段的end是否大于当前线段的start，如果大于，重新划分区间 end=start

    别想那么复杂，先找到区间，然后再找到区间内的线段，依次遍历所有区间，新线段到来就深度加一，离开线段就减一
    */
public:
    double AverageDepth(std::vector<std::pair<int, int>> segments) {
        // return AverageDepth_0(segments);
        return AverageDepth_1(segments);
    }

    double AverageDepth_1(std::vector<std::pair<int, int>> segments) {
        if (segments.size() <= 1) {
            return 0;
        }
        set<int, less<int>> points;
        for (auto &it : segments) {
            points.insert(it.first);
            points.insert(it.second);
        }
        vector<int> intervals(points.begin(), points.end());
        // 尝试建立一个线段树，统计每个区间的线段数量，
        // 也可以使用树状数组，每个区间的线段数量，然后求和
        segment_tree_inverval st(intervals);
        for (auto& it : segments) {
            st.change(it.first, it.second, 1);
        }
        int cur = intervals[0];
        int overlap_cnt = 0;
        int total_depth = 0;
        for (int i = 1; i < intervals.size(); ++i) {
            int cnt = st.query(intervals[i-1], intervals[i]);
            // cout << "[" << intervals[i-1] << "," << intervals[i] << "]" << "cnt: " << cnt << endl;
            if (cnt > 1) {
                total_depth += cnt;
                ++overlap_cnt;
            }
        }
        return overlap_cnt > 0 ? (double)total_depth / (double)overlap_cnt : 0;
    }

    double AverageDepth_0(std::vector<std::pair<int, int>> segments) {
        if (segments.size() <= 1) {
            return 0;
        }
        set<int, greater<int>> points;
        for (auto &it : segments) {
            points.insert(it.first);
            points.insert(it.second);
        }
        vector<pair<int,int>> intervals;
        int cur_point = *points.begin();
        points.erase(points.begin());
        while (points.size() > 0) {
            intervals.push_back({cur_point, *points.begin()});
            cur_point = *points.begin();
            points.erase(points.begin());
        }

        // for (auto& i : intervals) {
        //     cout << "(" << i.first << "," << i.second << ") ";
        // }
        // cout << endl;

        sort(segments.begin(), segments.end(), [](pair<int,int>& a, pair<int,int>& b){
            return a.second > b.second || (a.second == b.second && a.first > b.first);
        });

        // for (auto& it : segments) {
        //     cout << "(" << it.first << "," << it.second << ") | ";
        // }
        // cout << endl;

        int overlap_cnt = 0;
        int total_depth = 0;
        auto is_overlap = [](std::pair<int,int> p, int start, int end) {
            // cout << "p: [" << p.first << "," << p.second << "] (" << start << "," << end << ")" << endl;
            return !(p.second <= start || p.first >= end);
        };
        while (intervals.size() > 0) {
            int start = intervals[0].first;
            int end = intervals[0].second;
            intervals.erase(intervals.begin());
            int cnt = 0;
            for (auto& it : segments) {
                if (is_overlap(it, end, start)) {
                    ++cnt;
                }
            }
            if (cnt > 1) {
                total_depth += cnt;
                ++overlap_cnt;
            }
        }

        // cout << overlap_cnt << " " << total_depth << endl;
        return overlap_cnt > 0 ? (double)total_depth / (double)overlap_cnt : 0;
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
    vector<std::pair<int,int>> nums6;
    string str;
    string str0;
    string str1;
    int k = 0;
    auto test = [&s](vector<std::pair<int,int>>& nums) {
        cout << "nums: ";
        for (auto& i : nums) {
            cout << "(" << i.first << "," << i.second << ") ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.AverageDepth(nums);
        cout << " permutaions: "<< res << endl;
    };

    // list<int> l1 = {1,2,3};
    // int cnt = 0;
    // for (list<int>::iterator it = l1.begin(); it != l1.end();) {
    //     ++cnt;
    //     if (cnt == 2) {
    //         list<int>::iterator tmp = it;
    //         ++it;
    //         l1.erase(tmp);
    //         continue;
    //     }
    //     ++it;
    // }
    // for (auto& it : l1) {
    //     cout << it << " ";
    // }
    // cout << endl;

    nums6 = {{1,6},{2,4},{4,5},{4,6}};
    test(nums6);

    nums6 = {{1,6}, {2,3}, {3,4}};
    test(nums6);

    nums6 = {{1,6}};
    test(nums6);

    nums6 = {{1,2}, {2,3}, {3,4}};
    test(nums6);
}
