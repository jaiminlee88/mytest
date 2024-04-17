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


class Solution {
public:
    /*
     * @param A: sorted integer array A which has m elements, but size of A is m+n
     * @param m: An integer
     * @param B: sorted integer array B which has n elements
     * @param n: An integer
     * @return: nothing
     * 
     * 合并两个排序的整数数组 A 和 B 变成一个新的数组。
        原地修改数组 A 把数组 B 合并到数组 A 的后面。

        A = [1,2,3]
        m = 3
        B = [4,5]
        n = 2
        [1,2,3,4,5]

        A = [1,2,5]
        m = 3
        B = [3,4]
        n = 2
        [1,2,3,4,5]

        可以从前往后，也可以从后往前（更快）
     */
    void move_right(int arr[], int start, int end) {
        while ((start < end - 1) && arr[start] > arr[start + 1]) {
            int tmp = arr[start];
            arr[start] = arr[start + 1];
            arr[start + 1] = tmp;
            ++start;
        }
    }
    void mergeSortedArray(int A[], int m, int B[], int n) {
        // write your code here
        if (n == 0) {
            return;
        }
        if (m == 0) {
            for (int i = 0; i < n; ++i) {
                A[i] = B[i];
            }
            return;
        }
        int pa = 0;
        int pb = 0;
        while (pa < m && pb < n) {
            if (A[pa] > B[pb]) {
                int tmp = A[pa] ^ B[pb];
                A[pa] = A[pa] ^ tmp;
                B[pb] = B[pb] ^ tmp;
                move_right(B, pb, n);
            }
            ++pa;
        }
        for (int i = 0; i < n; ++i) {
            A[m + i] = B[i];
        }
    }
};

void move(int arr[], int start, int end) {
    int tmp = arr[start];
    arr[start] = arr[end];
    arr[end] = tmp;
}
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

    int a[] = {1,2,3,0,0,0};
    move(a, 1, 2);
    for (int i = 0; i < 6; ++i) {
        cout << a[i] << " ";
    }
    cout << endl;
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
}