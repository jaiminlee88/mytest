#include <vector>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;
void subsetsByNum(vector<vector<int>>& subsetn,
                      vector<int>& tmp,
                      vector<int>& origin_num,
                      int count,
                      int pos) {
    if (count == 0) {
        // for (auto& it:tmp) {
        //     printf("%d ", it);
        // }
        // printf("\n");
        vector<int> _tmp(tmp);
        sort(_tmp.begin(), _tmp.end(), [](int a, int b) { return a < b; });
        subsetn.push_back(_tmp);
        return;
    }
    for (int i = pos; i <= (origin_num.size() - count); ++i) {
        tmp.push_back(origin_num[i]);
        // cout << "push " << origin_num[i] << " " << endl;
        // copy(tmp.begin(), tmp.end(), ostream_iterator<int>(cout, " ")); cout << endl;

        subsetsByNum(subsetn, tmp, origin_num, count - 1, i + 1);
        // cout << "before pop: " ;
        // copy(tmp.begin(), tmp.end(), ostream_iterator<int>(cout, " ")); cout << endl;
        // cout << "pop " << tmp.back() << endl;
        tmp.pop_back();
        // copy(tmp.begin(), tmp.end(), ostream_iterator<int>(cout, " ")); cout << endl;
    }
}

vector<vector<int>> subsets(vector<int> &nums) {
    // write your code here
    vector<vector<int>> ret;
    ret.push_back(vector<int>());
    if (nums.size() == 0) {
        return ret;
    }
    ret.push_back(nums); // 满
    for (int count = 1; count < nums.size(); ++count) {
        vector<vector<int>> subsetn;
        vector<int> tmp;
        // cout << "count=" << count << endl;
        subsetsByNum(subsetn, tmp, nums, count, 0);
        for (auto& it: subsetn) {
            ret.push_back(it);
        }
    }
    return ret;
}


int main() {
    vector<int> nums = {4,0,1};
    vector<vector<int>> ret = subsets(nums);
    cout << "output:" << endl;
    for (auto& it: ret) {
        for (auto& it2: it) {
            printf("%d ", it2);
        }
        printf("\n");
    }

    cout << "=============" << endl;
    vector<int> nums2 = {};
    copy(nums2.begin(), nums2.end(), ostream_iterator<int>(cout, " ")); cout << endl;
    nums2.push_back(1);
    copy(nums2.begin(), nums2.end(), ostream_iterator<int>(cout, " ")); cout << endl;
    nums2.push_back(2);
    copy(nums2.begin(), nums2.end(), ostream_iterator<int>(cout, " ")); cout << endl;
    nums2.push_back(3);
    copy(nums2.begin(), nums2.end(), ostream_iterator<int>(cout, " ")); cout << endl;
    cout << "nums2.back()=" << nums2.back() << endl;
    nums2.pop_back();
    copy(nums2.begin(), nums2.end(), ostream_iterator<int>(cout, " ")); cout << endl;
    nums2.pop_back();
    copy(nums2.begin(), nums2.end(), ostream_iterator<int>(cout, " ")); cout << endl;
    return 0;
}