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
 * class Interval {
 * public:
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

class Solution {
public:
    /**
     * @param list1: one of the given list
     * @param list2: another list
     * @return: the new sorted list of interval
     * 合并两个已排序的区间列表，并将其作为一个新的有序区间列表返回。新的区间列表应该通过拼接两个列表的区间并按升序排序。
     * 
     *  输入:  list1 = [(1,2),(3,4)] and list2 = [(2,3),(5,6)]
        输出: [(1,4),(5,6)]
        解释:
        (1,2),(2,3),(3,4) --> (1,4)
        (5,6) --> (5,6)

        输入: list1 = [(1,2),(3,4)] 和 list2 = [(4,5),(6,7)]
        输出: [(1,2),(3,5),(6,7)]
        解释:
        (1,2) --> (1,2)
        (3,4),(4,5) --> (3,5)
        (6,7) --> (6,7)

        每次取出最小的区间，看看能否继续合并
     */
    vector<Interval> mergeTwoInterval(vector<Interval> &list1, vector<Interval> &list2) {
        // write your code here
        return mergeTwoInterval_0(list1, list2);
    }
    void merge(vector<Interval> & li, int& pos, Interval& cur, vector<Interval>& res) {
        if (cur.start == INT_MAX) {
            cur = li[pos];
        } else {
            if (cur.end >= li[pos].end) {
                ++pos;
                return;
            } else if (cur.end >= li[pos].start) {
                cur.end = li[pos].end;
            } else {
                res.push_back(cur);
                cur = li[pos];
            }
        }
        ++pos;
    }
    vector<Interval> mergeTwoInterval_0(vector<Interval> &list1, vector<Interval> &list2) {
        vector<Interval> res;
        int p1 = 0;
        int p2 = 0;
        Interval cur{INT_MAX, INT_MAX};
        while (p1 < list1.size() && p2 < list2.size()) {
            cout << "p1: " << p1 << " p2: " << p2 << endl;
            if (list1[p1].start < list2[p2].start) {
                merge(list1, p1, cur, res);
            } else {
                merge(list2, p2, cur, res);
            }
        }

        if (p1 < list1.size()) {
            while (p1 < list1.size()) {
                merge(list1, p1, cur, res);
            }
        } else {
            while (p2 < list2.size()) {
                merge(list2, p2, cur, res);
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
    auto test = [&s](vector<int>& nums) {
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        // auto res = s.permute_3(nums);
        // cout << "permutaions: "<< endl;
        // for (auto& v : res) {
        //     for (auto& i : v) {
        //         cout << i << " ";
        //     }
        //     cout << endl;
        // }
    };


    
    vector<Interval> l1= {{2,4},{12,17},{18,21},{23,25},{40,42},{51,54},{58,59},{63,67},{77,78},{84,88},{89,92},{97,111},{121,125},{129,133},{138,140},{149,152},{155,158},{175,177},{182,185},{188,192},{196,218},{223,226},{228,231},{234,237},{259,265},{271,274},{276,282},{287,288},{299,300},{301,303},{304,308},{314,317},{318,323},{327,331},{333,342},{345,357},{358,360},{362,365},{367,375},{376,382},{386,404},{413,414},{418,421},{423,432},{434,436},{439,442},{445,448},{450,454},{457,459},{464,465},{483,484},{493,501},{504,506},{533,534},{539,541},{551,554},{557,565},{566,568},{575,587},{588,594},{603,604},{611,625},{632,633},{634,647},{649,655},{665,675},{683,685},{694,698},{706,708},{711,714},{719,721},{724,726},{737,746},{753,756},{770,785},{796,809},{810,811},{814,828},{833,835},{840,848},{851,852},{853,855},{857,865},{866,868},{870,871},{874,875},{878,881},{882,885},{898,900},{902,907},{921,924},{936,946},{947,948},{952,957},{959,965},{966,967},{972,975},{976,982},{985,986},{987,988}};
    vector<Interval> l2 = {{3,16},{17,20},{23,28},{34,42},{46,58},{62,64},{65,68},{75,76},{79,82},{85,88},{92,96},{99,100},{101,108},{110,117},{119,120},{121,126},{127,130},{134,136},{137,143},{145,146},{147,156},{175,182},{186,187},{199,207},{214,221},{227,239},{248,257},{259,265},{271,280},{283,285},{291,293},{314,319},{322,326},{342,343},{346,348},{350,352},{356,360},{383,393},{394,411},{412,418},{428,431},{432,439},{443,445},{456,458},{460,478},{488,493},{497,498},{499,505},{506,510},{512,514},{532,534},{536,547},{561,562},{568,569},{571,572},{580,582},{584,592},{594,596},{605,610},{628,636},{637,641},{644,649},{653,657},{665,667},{673,674},{679,682},{683,684},{685,687},{688,693},{696,698},{700,703},{705,718},{720,721},{728,729},{730,734},{738,742},{746,757},{759,761},{765,768},{770,790},{796,798},{802,805},{815,832},{836,844},{864,865},{874,875},{878,881},{884,887},{888,890},{896,897},{898,901},{903,905},{909,915},{916,928},{930,940},{944,951},{952,957},{960,963},{969,990},{991,1000}};
    auto res = s.mergeTwoInterval(l1, l2);
    for (auto& i : res) {
        cout << "(" << i.start << "," << i.end << ") ";
    }
    cout << endl;
}