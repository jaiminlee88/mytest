#include <iostream>
#include <vector>
#include "segment_tree.hh"
#include <random>

using namespace std;
int main () {
    // cout << "hello world" << endl;
    vector<int> nums;
    vector<pair<int,int>> intervals;
    auto test = [](vector<int>& nums, vector<pair<int,int>>& intervals){
        cout << "nums: ";
        for (auto& it : nums) {
            cout << it << " ";
        }
        cout << endl;
        int max = 0;
        int min = INT_MAX;
        for (auto& it : nums) {
            if (it > max) {
                max = it;
            }
            if (it < min) {
                min = it;
            }
        }
        // cout << "max: " << max << " min: " << min << endl;
        segment_tree_sum_int st(min, max);
        // cout << "init... done" << endl;
        for (auto& it : nums) {
            st.insert(it);
        }
        // cout << "hahahah" << endl;
        // for (auto& it : st.tree) {
        //     cout << "[" << it.l << "," << it.r << "]->" << it.val << endl;
        // }
        // cout << "established..." << endl;
        for (auto& it : intervals) {
            cout << "[" << it.first << "," << it.second << "]->";
            cout << "sum: " << st.query(0, it.first, it.second) << endl;
        }
    };

    nums = {6,5,2,4,1};
    intervals = {{1,2}, {4,10}};
    test(nums, intervals);

    auto gen_random = [](vector<int>& nums, int down, int up, int n){
        nums.clear();
        std::random_device rd;  // 随机数种子
        std::mt19937 gen(rd()); // 随机数引擎
        std::uniform_int_distribution<> dis(down, up); // 生成 [1, 10000] 范围内的整数
        std::vector<int> random_numbers;
        for (int i = 0; i < n; ++i) {
            nums.push_back(dis(gen)); // 生成随机数并存储到容器中
        }
    };
    gen_random(nums, 1, 10000, 100);
    intervals = {{1,2}, {4,10}, {2,100}, {2000,5000}, {5000,10000}};
    test(nums, intervals);

    auto test_interval = [](vector<int>& nums,
                            vector<pair<int,int>>& intervals,
                            vector<pair<int,int>>& querys){
        cout << "nums: ";
        for (auto& it : nums) {
            cout << it << " ";
        }
        cout << endl;
        segment_tree_inverval st(nums);
        int cnt  = 0;
        for (auto& it : st._tree) {
            cout << "[" << cnt << "]->(" << it.l << "," << it.r << "," << it.val << ") " << endl;
            ++cnt;
        }
        for (auto& it : intervals) {
            st.change(it.first, it.second, 1);
        }
        cnt = 0;
        for (auto& it : st._tree) {
            cout << "[" << cnt << "]->(" << it.l << "," << it.r << "," << it.val << ") " << endl;
            ++cnt;
        }
        for (auto& it : querys) {
            cout << "[" << it.first << "," << it.second << "]->";
            cout << "sum: " << st.query(it.first, it.second) << endl;
        }
    };
    vector<pair<int,int>> querys;
    nums = {1,2,4,5,6};
    intervals = {{1,6},{2,4},{4,5},{4,6}};
    querys = {{1,2},{2,4},{4,5},{5,6}};
    test_interval(nums, intervals, querys);
}