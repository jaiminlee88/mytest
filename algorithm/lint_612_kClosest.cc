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
    * @param points a list of points
    * @param origin a point
    * @param k an integer
    * @return the k closest points
    *
    * 描述：
        给定一些 points 和一个 origin，从 points 中找到 k 个离 origin 最近的点。
        按照距离由小到大返回。如果两个点有相同距离，则按照x值来排序；若x值也相同，就再按照y值排序。

        样例：
        给出 points = [[4,6],[4,7],[4,4],[2,5],[1,1]], origin = [0, 0], k = 3
        返回 [[1,1],[2,5],[4,4]]

        快速排序法，根据距离大小排
        或者用优先对列 
    */
    vector<Point> kClosest(vector<Point>& points, Point& origin, int k) {
        // return kClosest_0(points, origin, k);
        return kClosest_1(points, origin, k);
    }
    // vector<Point> kClosest_0(vector<Point>& points, Point& origin, int k) {
    //     // 不太行
    //     if (k < 1 || points.size() < 1 || k > points.size()) {
    //         return {};
    //     }
    //     std::function<bool(Point&, Point&)> cmp = [&origin](Point& a, Point& b) -> bool {
    //         // 不行，报错
    //         int dist_a = (a.x - origin.x)*(a.x - origin.x) + (a.y - origin.y)*(a.y - origin.y);
    //         int dist_b = (b.x - origin.x)*(b.x - origin.x) + (b.y - origin.y)*(b.y - origin.y);
    //         if (dist_a == dist_b) {
    //             if (a.x == b.x) {
    //                 return a.y < b.y;
    //             }
    //             return a.x < b.x;
    //         }
    //         return dist_a < dist_b;
    //     };
    //     priority_queue<Point, vector<Point>, decltype(cmp)> q;
    //     vector<Point> _points = points;
    //     vector<Point> res;
    //     // cout << 
    //     // for (auto& it : _points) {
    //     //     cout << it.x << "," << it.y << endl;
    //     // }
    //     for (auto& p : _points) {
    //         q.push(p);
    //     }
    //     for (int i = 0; i < k; i++) {
    //         res.push_back(q.top());
    //         q.pop();
    //     }
    //     return res;
    // }

    vector<Point> kClosest_1(vector<Point>& points, Point& origin, int k) {
        if (k < 1 || points.size() < 1) {
            return {};
        }
        vector<Point> _points = points;
        sort(_points.begin(), _points.end(), [origin](Point& a, Point& b) {
            int dist_a = (a.x - origin.x)*(a.x - origin.x) + (a.y - origin.y)*(a.y - origin.y);
            int dist_b = (b.x - origin.x)*(b.x - origin.x) + (b.y - origin.y)*(b.y - origin.y);
            if (dist_a == dist_b) {
                if (a.x == b.x) {
                    return a.y < b.y;
                }
                return a.x < b.x;
            }
            return dist_a < dist_b;
        });
        vector<Point> res;
        for (int i = 0; i < k; ++i) {
            res.push_back(_points[i]);
        }
        return res;
    }
};


int main() {
    Solution s;
    vector<Point> nums;
    Point origin;
    int k;
    auto test = [&s](vector<Point>& nums, Point& origin, int k) {
        cout << "origin: (" << origin.x << "," << origin.y <<") k: " << k << endl;
        cout << "nums: ";
        for (auto& i : nums) {
            cout << "(" << i.x << "," << i.y <<") ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.kClosest(nums, origin, k);
        cout << "permutaions: "<< endl;
        for (auto& i : res) {
            cout << "(" << i.x << "," << i.y <<") ";
        }
        cout << endl;
    };

    nums =  {{4,6},{4,7},{4,4},{2,5},{1,1}};
    origin = {0, 0};
    k = 3;
    test(nums, origin, k);
}