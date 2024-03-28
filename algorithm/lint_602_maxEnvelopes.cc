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
     * @param envelopes: a number of envelopes with widths and heights
     * @return: the maximum number of envelopes
     * 
     * 给一定数量的信封，带有整数对 (w, h) 分别代表信封宽度和高度。
     * 一个信封的宽高均大于另一个信封时可以放下另一个信封。
        求最大的信封嵌套层数。

        输入：[[5,4],[6,4],[6,7],[2,3]]
        输出：3
        解释：
        最大的信封嵌套层数是 3 ([2,3] => [5,4] => [6,7])。

        输入：[[4,5],[4,6],[6,7],[2,3],[1,1]]
        输出：4
        解释：
        最大的信封嵌套层数是 4 ([1,1] => [2,3] => [4,5] / [4,6] => [6,7])。

        求极值，数组开始无序，但是可以有序，可能用动态规划
        类似接龙问题
        status dp[i] 表示到达第i个信封的最大嵌套层数
        function dp[i] = max(dp[j] + 1) for j in [1,i-1] if envelopes[i].w > envelopes[j].w && envelopes[i].h > envelopes[j].h
        init dp[0] = 0
        result max(dp[i]) for i in [1,n]
     */
    int maxEnvelopes(vector<vector<int>> &envelopes) {
        // write your code here
        // return maxEnvelopes_0(envelopes);
        return maxEnvelopes_1(envelopes);
    }

    int maxEnvelopes_0(vector<vector<int>> &envelopes) {
        if (envelopes.size() == 0) {
            return 0;
        }
        if (envelopes.size() == 1) {
            return 1;
        }
        sort(envelopes.begin(), envelopes.end(), [](vector<int>& a, vector<int>& b){
            return a[0] < b[0] || (a[0] == b[0] && a[1] < b[1]); // w升序，w相等时h降序，有优化空间
        });
        vector<int> dp(envelopes.size() + 1, 1);
        dp[0] = 0;
        dp[1] = 1;
        int max_len = 0;
        for (int i = 2; i <= envelopes.size(); ++i) {
            for (int j = 1; j < i; ++j) {
                if (envelopes[j-1][0] < envelopes[i-1][0] &&
                    envelopes[j-1][1] < envelopes[i-1][1]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                    max_len = dp[i] > max_len ? dp[i] : max_len;
                }
            }
        }
        // cout << "dp:" << endl;
        // for (auto& i : dp) {
        //     cout << i << " ";
        // }
        // cout << endl;
        return max_len;
    }

    int maxEnvelopes_1(vector<vector<int>> &envelopes) {
        // 写错了
        if (envelopes.size() == 0) {
            return 0;
        }
        if (envelopes.size() == 1) {
            return 1;
        }
        sort(envelopes.begin(), envelopes.end(), [](vector<int>& a, vector<int>& b){
            return a[0] < b[0] || (a[0] == b[0] && a[1] > b[1]);
            // w升序，w相等时h降序，有优化空间
            // 当前为h，往前找到第一个小于h的信封，返回其索引，该信封w必定小于当前信封w
        });
        vector<int> dp(envelopes.size() + 1, 1);
        dp[0] = 0;
        for (int i = 1; i <= envelopes.size(); ++i) {
            dp[i] = INT_MAX;
        }
        int max_len = 0;
        for (int i = 0; i < envelopes.size(); ++i) {
            for (auto& it : dp) {
                if (it == INT_MAX) {
                    cout << "inf ";
                } else {
                    cout << it << " ";
                }
            }
            cout << endl;
            int pos = get_first_greater(dp, envelopes[i][1]);
            dp[pos] = envelopes[i][1];
            max_len = max(max_len, pos);
        }
        // cout << "dp:" << endl;
        // for (auto& i : dp) {
        //     cout << i << " ";
        // }
        // cout << endl;
        return max_len;
    }
    int get_first_greater(vector<int>& dp, int target) {
        int start = 0;
        int end = dp.size() - 1;
        while (start + 1 < end) {
            int mid = start + (end - start) / 2;
            if (dp[mid] < target) {
                start = mid;
            } else {
                end = mid;
            }
        }
        if (dp[start] >= target) {
            return start;
        }
        return end;
    }
};


int main() {
    Solution s;
    vector<vector<int>> nums;
    auto test = [&s](vector<vector<int>>& nums) {
        auto res = s.maxEnvelopes(nums);
        cout << "nums: ";
        for (auto& it : nums) {
            cout << "[" << it[0] << "," << it[1] << "] ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        cout << "permutaions: "<< res << endl;
    };

    nums = {{5,4},{6,4},{6,7},{2,3}};
    test(nums);

    nums = {{4,5},{4,6},{6,7},{2,3},{1,1}};
    test(nums);
}