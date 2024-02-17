#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>

using namespace std;

class Solution {
public:
    /**
     * @param nums: the given array
     * @return:  the number of triplets chosen from the array that can make triangles
     * 三角形要满足a+b>c
     * 固定c，找a+b
     * 从小到大排序，从左到右，先固定c，从右侧找a+b，一定满足a+b>c，注意除重
     */
    int triangleNumber(vector<int> &nums) { // no duplicated
        // Write your code here
        if (nums.size() < 3) {
            return 0;
        }
        sort(nums.begin(), nums.end());
        int count = 0;
        for (int cpos = 0; cpos < nums.size() - 2; cpos++) {
            if (cpos > 0 && nums[cpos] == nums[cpos - 1]) {
                continue; //  除重
            }
            for (int apos = cpos + 1; apos < nums.size() - 1; apos++) {
                if ((apos > cpos + 1) && nums[apos] == nums[apos - 1]) {
                    continue;
                }
                for (int bpos = apos + 1; bpos < nums.size(); bpos++) {
                    if ((bpos > apos + 1) && nums[bpos] == nums[bpos - 1]) {
                        continue;
                    }
                    if (nums[apos] + nums[bpos] > nums[cpos]) {
                        cout << "[" << nums[cpos] << "," << nums[apos] << "," << nums[bpos] << "]" << endl;
                        count++;
                    }
                }
            }
        }
        return count;
    }

    int triangleNumber_opt_1(vector<int> &nums) { // no dulplicated
        // Write your code here
        if (nums.size() < 3) {
            return 0;
        }
        sort(nums.begin(), nums.end());
        vector<pair<int,int>> nums_count;
        pair<int,int> tmp;
        for (int i = 0; i < nums.size(); i++) {
            if (i > 0 && nums[i] == nums[i - 1]) {
                nums_count.back().second++;
                continue;
            }
            nums_count.push_back({nums[i], 1});
        }

        int count = 0;
        for (int cpos = 0; cpos < nums_count.size(); cpos++) {
            if (nums_count[cpos].second >= 3) {
                cout << "[" << nums_count[cpos].first << "," << nums_count[cpos].first << "," << nums_count[cpos].first << "]" << endl;
                count++;
            }
            for (int apos = cpos + 1; apos < nums_count.size(); apos++) {
                if (nums_count[cpos].second >= 2 && (nums_count[cpos].first * 2 > nums_count[apos].first)) {
                    cout << "[" << nums_count[cpos].first << "," << nums_count[cpos].first << "," << nums_count[apos].first << "]" << endl;
                    count++;
                }
                if (nums_count[apos].second >= 2) {
                    cout << "[" << nums_count[cpos].first << "," << nums_count[apos].first << "," << nums_count[apos].first << "]" << endl;
                    count++;
                }
                for (int bpos = apos + 1; bpos < nums_count.size(); bpos++) {
                    if (nums_count[cpos].first + nums_count[apos].first > nums_count[bpos].first) {
                        cout << "[" << nums_count[cpos].first << "," << nums_count[apos].first << "," << nums_count[bpos].first << "]" << endl;
                        count++;
                    }
                }
            }
        }
        return count;
    }

    int triangleNumber_opt_2(vector<int> &nums) { // dulplicated exists
        // Write your code here
        if (nums.size() < 3) {
            return 0;
        }
        sort(nums.begin(), nums.end());
        vector<pair<int,int>> nums_count;
        pair<int,int> tmp;
        for (int i = 0; i < nums.size(); i++) {
            if (i > 0 && nums[i] == nums[i - 1]) {
                nums_count.back().second++;
                continue;
            }
            nums_count.push_back({nums[i], 1});
        }

        int count = 0;
        for (int cpos = 0; cpos < nums_count.size(); cpos++) {
            if (nums_count[cpos].second >= 3) {
                //CCC
                int cnt = nums_count[cpos].second * (nums_count[cpos].second - 1) * (nums_count[cpos].second - 2) / 6;
                count += cnt;
                cout << "[" << nums_count[cpos].first << "," << nums_count[cpos].first << "," << nums_count[cpos].first << "] cnt: " << cnt << endl;
            }
            for (int apos = cpos + 1; apos < nums_count.size(); apos++) {
                if (nums_count[cpos].second >= 2 && (nums_count[cpos].first * 2 > nums_count[apos].first)) {
                    //cca
                    int cnt = nums_count[cpos].second * (nums_count[cpos].second - 1) / 2 * nums_count[apos].second;
                    count += cnt;
                    cout << "[" << nums_count[cpos].first << "," << nums_count[cpos].first << "," << nums_count[apos].first << "] cnt: " << cnt << endl;
                }
                if (nums_count[apos].second >= 2) {
                    //aab
                    int cnt = nums_count[apos].second * (nums_count[apos].second - 1) / 2 * nums_count[cpos].second;
                    count += cnt;
                    cout << "[" << nums_count[cpos].first << "," << nums_count[apos].first << "," << nums_count[apos].first << "] cnt: " << cnt << endl;
                }
                for (int bpos = apos + 1; bpos < nums_count.size(); bpos++) {
                    if (nums_count[cpos].first + nums_count[apos].first > nums_count[bpos].first) {
                        //cab
                        int cnt = nums_count[cpos].second * nums_count[apos].second * nums_count[bpos].second;
                        count += cnt;
                        cout << "[" << nums_count[cpos].first << "," << nums_count[apos].first << "," << nums_count[bpos].first << "] cnt: " << cnt << endl;
                    }
                }
            }
        }
        return count;
    }

    int triangleNumber_opt_3(vector<int> &nums) { // dulplicated exists
        // 双指针法
        // Write your code here
        if (nums.size() < 3) {
            return 0;
        }
        sort(nums.begin(), nums.end());
        int count = 0;
        for (int cpos = 2; cpos < nums.size(); cpos++) {
            int lpos = 0;
            int rpos = cpos - 1;
            while (lpos < rpos) {
                if (nums[lpos] + nums[rpos] > nums[cpos]) {
                    count += rpos - lpos;
                    rpos--;
                } else {
                    lpos++;
                }
            }
        }
        return count;
    }
};

void print_result(vector<int> nums, int ret) {
    cout << "nums: [";
    for (auto i : nums) {
        cout << i << " ";
    }
    cout << "] ret: " << ret << endl;

}
int main () {
    Solution s;
    vector<int> nums;
    int ret;
    std::function<int(vector<int>&)> func = std::bind(&Solution::triangleNumber_opt_3, s, std::placeholders::_1);

    nums = {1,1,2,3};
    ret = func(nums);
    print_result(nums, ret);

    nums = {3,4,6,7};
    ret = func(nums);
    print_result(nums, ret);

    nums = {2,2,3,4};
    ret = func(nums);
    print_result(nums, ret);

    nums = {3,3,3};
    ret = func(nums);
    print_result(nums, ret);

    nums = {3,3,3,3};
    ret = func(nums);
    print_result(nums, ret);

    nums = {2,6,9,6,7,8,8,7};
    ret = func(nums);
    print_result(nums, ret);
    return 0;
}