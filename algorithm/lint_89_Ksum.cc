#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /**
     * @param a: An integer array
     * @param k: A positive integer (k <= length(A))
     * @param target: An integer
     * @return: An integer
     * 给定 n 个不同的正整数，整数 k(k≤n)以及一个目标数字 target。　
     * 在这 n 个数里面找出 k 个数，使得这 k 个数的和等于目标数字，求问有多少种方案？
     * 用矩阵dp[i][j][t]表示前i个数中选j个数，和为t的方案数
     * 
     * 动态规划
     */
    int kSum(vector<int> &a, int k, int target) {
        // write your code here
        if (a.size() < 1) {
            return 0;
        }
        return kSum_0(a, k, target, 0, a.size() - 1);
    }

    int kSum_0(vector<int> &a, int k, int target, int start, int end) {


    }
};

void print_result(std::vector<int>& numbers, std::vector<vector<int>>& result, int target, int k=0, int count=0) {
    cout << "numbers:[";
    for (auto& n : numbers) {
        cout << n << ",";
    }
    cout << "], target=" << target << " k=" << k << " count=" << count << endl;
    for (auto& r : result) {
        cout << "[";
        for (auto& n : r) {
            cout << n << ",";
        }
        cout << "]" << endl;
    }
}
int main() {
    vector<int> nums;
    vector<vector<int>> result;
    int k = 0;
    int target = 0;
    int count = 0;

    Solution s;
    nums = {1,2,3,4};
    k = 2;
    target = 5;
    count = s.kSum(nums, k, target);
    print_result(nums, result, target, k, count);
}