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
using namespace std;


class Solution {
public:
    /**
     * @param a: An integer array
     * @param b: An integer array
     * @return: a double whose format is *.5 or *.0
     * 
     * 两个排序的数组A和B分别含有m和n个数，找到两个排序数组的中位数，要求时间复杂度应为O(log(m+n))。
     *  这里的中位数等同于数学定义里的中位数。
        中位数是排序后数组的中间值。
        如果有数组中有n个数且n是奇数，则中位数为A[(n−1)/2]。
        如果有数组中有n个数且n是偶数，则中位数为 (A[(n−1)/2]+A[(n−1)/2+1])/2.
        比如：数组A=[1,2,3]的中位数是2，数组A=[1,19]的中位数是10。

        A = [1,2,3,4,5,6]
        B = [2,3,4,5]
        3.50

        A = [1,2,3]
        B = [4,5]
        3.00

        暴力法，时间复杂度O(m+n)
        每次找两个数组中尾数最大那个
     */

    double findMedianSortedArrays(vector<int> &a, vector<int> &b) {
        // return findMedianSortedArrays_0(a, b);
        // return findMedianSortedArrays_1(a, b);
        return findMedianSortedArrays_2(a, b);
    }
    double findMedianSortedArrays_2(vector<int> &a, vector<int> &b) {
        if (a.size() < 1 && b.size() < 1) {
            return 0;
        }
        int total = a.size() + b.size();
        if (total % 2 == 0) {
            return (findMedianSortedArrays_2_core(a, b, 0, 0, total / 2) + findMedianSortedArrays_2_core(a, b, 0, 0, total / 2 + 1)) / 2;
        }
        return findMedianSortedArrays_2_core(a, b, 0, 0, total / 2 + 1);
    }

    double findMedianSortedArrays_2_core(vector<int> &a, vector<int> &b, int astart, int bstart, int k) {
        if (astart >= a.size()) {
            return b[bstart + k - 1];
        }
        if (bstart >= b.size()) {
            return a[astart + k - 1];
        }
        if (k == 1) {
            return min(a[astart], b[bstart]);
        }
        int a_key = astart + k / 2 - 1 < a.size() ? a[astart + k / 2 - 1] : INT_MAX;
        int b_key = bstart + k / 2 - 1 < b.size() ? b[bstart + k / 2 - 1] : INT_MAX;
        if (a_key < b_key) {
            return findMedianSortedArrays_2_core(a, b, astart + k / 2, bstart, k - k / 2);
        } else {
            return findMedianSortedArrays_2_core(a, b, astart, bstart + k / 2, k - k / 2);
        }

    }

    double findMedianSortedArrays_1(vector<int> &a, vector<int> &b) {
        // 暴力法
        if (a.size() < 1 && b.size() < 1) {
            return 0;
        }
        vector<int> res;
        int pa = 0;
        int pb = 0;
        while (true) {
            if (pa >= a.size() && pb >= b.size()) {
                break;
            }
            if (pa >= a.size()) {
                res.push_back(b[pb]);
                ++pb;
                continue;
            }
            if (pb >= b.size()) {
                res.push_back(a[pa]);
                ++pa;
                continue;
            }
            if (a[pa] < b[pb]) {
                res.push_back(a[pa]);
                ++pa;
            } else {
                res.push_back(b[pb]);
                ++pb;
            }
        }
        if (res.size() % 2 == 0) {
            return (res[res.size() / 2] + res[res.size() / 2 - 1]) / 2.0;
        }
        return res[res.size() / 2];
    }

    double findMedianSortedArrays_0(vector<int> &a, vector<int> &b) {
        // write your code here
        // 还有问题？？？？？？？？？？？？
        if (a.size() < 1 && b.size() < 1) {
            return 0;
        }
        int mid = (a.size() + b.size()) / 2; // 第mid个
        int aend = a.size() - 1;
        int bend = b.size() - 1;
        cout << "==========a.size()=" << a.size() << " b.size()=" << b.size() << " mid=" << mid << endl;
        if ((a.size() + b.size()) % 2 == 0) {
            // return (findMedianSortedArrays_0_core(a, b, aend, bend, mid) + findMedianSortedArrays_0_core(a, b, aend, bend, mid + 1)) / 2.0;
            int res1 = findMedianSortedArrays_0_core(a, b, aend, bend, mid);
            int res2 = findMedianSortedArrays_0_core(a, b, aend, bend, mid + 1);
            cout << "res1: " << res1 << " res2: " << res2 << endl;
            return (res1 + res2) / 2.0;
        }
        return findMedianSortedArrays_0_core(a, b, aend, bend, mid + 1);
    }
    double findMedianSortedArrays_0_core(vector<int> &a, vector<int> &b, int aend, int bend, int Nth_num) {
        cout << "aend=" << aend << " bend=" << bend << " mid=" << Nth_num << endl;
        if (aend < 0) {
            return b[Nth_num - 1];
        }
        if (bend < 0) {
            return a[Nth_num - 1];
        }
        if (aend + bend + 2 == Nth_num) {
            cout << "a[aend]=" << a[aend] << " b[bend]=" << b[bend] << endl;
            return max(a[aend], b[bend]);
        }
        if (a[aend] > b[bend]) {
            if (aend == 0 || aend == 1) {
                return findMedianSortedArrays_0_core(a, b, aend - 1, bend, Nth_num);
            } else if (aend % 2 == 0) {
                return findMedianSortedArrays_0_core(a, b, aend / 2, bend, Nth_num);
            }
            return findMedianSortedArrays_0_core(a, b, aend / 2 + 1, bend, Nth_num);
        }
        if (bend == 0 || bend == 1) {
            return findMedianSortedArrays_0_core(a, b, aend, bend - 1, Nth_num);
        } else if (bend % 2 == 0) {
            return findMedianSortedArrays_0_core(a, b, aend, bend / 2, Nth_num);
        }
        return findMedianSortedArrays_0_core(a, b, aend, bend / 2 + 1, Nth_num);
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
    auto test = [&s](vector<int>& nums, vector<int>& nums1) {
        cout << "nums0: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
        cout << "nums1: ";
        for (auto& i : nums1) {
            cout << i << " ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.findMedianSortedArrays(nums, nums1);
        cout << "permutaions: "<< res << endl;
        cout << "===================" << endl;
    };

    // nums = {1,2,3,4,5,6};
    // nums1 = {2,3,4,5};
    // test(nums, nums1);

    // nums1 = {1,2,3,4,5,6};
    // nums = {2,3,4,5};
    // test(nums, nums1);

    // nums = {1,2,3};
    // nums1 = {4,5};
    // test(nums, nums1);

    // nums = {4,5};
    // nums1 = {1,2,3};
    // test(nums, nums1);

    // nums = {4,5};
    // nums1 = {4,5};
    // test(nums, nums1);

    // nums = {4,5};
    // nums1 = {4};
    // test(nums, nums1);

    // nums = {4};
    // nums1 = {4};
    // test(nums, nums1);

    // nums = {3};
    // nums1 = {4};
    // test(nums, nums1);

    // nums = {};
    // nums1 = {4};
    // test(nums, nums1);

    nums = {1,5,9};
    nums1 = {2,3,4,6,7,8};
    test(nums, nums1);
    
    nums = {1,1,14,31,33,40,42,66,71,74,113,117,124,125,127,137,143,184,187,188,221,222,224,248,251,269,293,294,315,324,330,353,358,366,368,389,389,408,424,432,433,451,452,456,459,475,480,483,484,496,509,515,519,523,559,567,568,593,598,600,612,623,626,628,632,633,634,646,654,663,681,696,706,709,717,723,746,753,790,790,798,824,826,847,849,857,866,879,882,894,894,913,925,938,942,961,974,988,988,989,998};
    nums1 = {3,4,5,6,9,15,17,20,21,21,23,25,27,27,28,29,31,32,37,41,43,47,49,50,52,52,52,54,59,60,67,68,71,72,73,77,78,84,86,88,88,91,94,98,98,98,100,102,105,106,107,107,110,117,118,120,122,124,126,129,131,134,135,144,147,154,158,158,163,164,164,170,171,171,172,172,176,178,180,183,184,185,189,196,197,199,200,200,204,208,214,217,223,226,227,231,231,232,232,237,243,244,245,251,258,259,266,271,274,277,279,280,280,281,283,284,284,284,286,288,290,296,299,301,302,302,302,303,305,308,308,309,311,313,313,316,322,323,326,327,328,329,331,331,337,340,340,342,343,345,346,349,349,349,350,354,366,366,375,376,377,377,379,382,389,390,391,392,393,394,397,397,397,399,400,400,402,402,403,404,405,405,408,414,415,416,416,416,419,421,422,426,426,427,430,432,433,436,440,443,445,448,448,454,455,456,456,457,458,459,459,462,465,466,467,471,475,493,500,501,505,507,509,511,512,512,513,513,514,514,515,516,517,518,520,521,523,524,525,528,533,535,535,536,537,539,542,542,544,545,547,551,552,553,554,554,556,557,557,558,559,559,561,563,565,568,570,578,578,579,580,580,581,581,588,590,591,592,592,593,594,595,597,601,603,603,605,607,610,611,612,612,612,614,617,620,621,622,622,624,624,625,625,627,627,627,632,635,635,637,638,642,644,644,647,647,650,651,652,653,654,655,657,660,664,667,670,671,672,673,673,676,677,682,685,685,686,686,688,694,695,695,697,699,700,700,704,704,707,709,713,713,715,716,716,717,719,721,725,732,736,740,742,745,746,749,752,754,755,756,756,757,760,762,763,765,766,768,768,768,772,774,775,775,780,784,784,785,785,788,790,791,792,794,796,796,798,800,802,802,804,806,806,808,813,814,816,817,817,818,824,824,825,825,827,830,832,834,834,837,841,842,843,845,846,848,852,855,855,855,860,861,866,866,872,874,875,875,877,883,886,892,892,895,895,897,898,898,900,900,900,904,904,905,906,907,909,909,914,914,914,915,922,924,927,928,930,931,936,938,939,941,944,945,946,947,948,950,955,956,960,961,967,967,969,971,972,978,979,981,982,984,984,989,990,993,997,999,999,1001};
    test(nums, nums1);
}