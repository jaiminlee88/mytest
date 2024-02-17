#include <iostream>
#include <vector>

using namespace std;
class Solution {
public:
    /**
     * @param nums: An integer array sorted in ascending order
     * @param target: An integer
     * @return: An integer
     */
    int findPosition(vector<int> &nums, int target) {
        // write your code here
        if (nums.size() <= 0) {
            return -1;
        }
        // return findPosition_core_recurse(nums, target, 0, nums.size() - 1);
        return findPosition_core_nonrecurse(nums, target);
    }

    int findPosition_core_recurse(vector<int> &nums, int target, int lpos, int rpos) {
        if (lpos >= rpos) {
            if (target == nums[lpos]) {
                return lpos;
            }
            return -1;
        }
        int m = lpos + (rpos - lpos) / 2;
        if (nums[m] == target) {
            return m;
        }
        if (target < nums[m]) {
            return findPosition_core_recurse(nums, target, lpos, m -1);
        } else {
            return findPosition_core_recurse(nums, target, m + 1, rpos);
        }
    }
    int findPosition_core_nonrecurse(vector<int> nums, int target) {
        int lpos = 0;
        int rpos = nums.size() - 1;
        while (lpos < rpos) {
            int m = (lpos + rpos) / 2;
            if (nums[m] == target) {
                rpos = m; // 总是要lpos前进一步，否则总在原地循环，如 【2】
                continue;
            }
            if (nums[m] > target) {
                rpos = m - 1;
            } else {
                lpos = m + 1;
            }
        }
        if (nums[lpos] == target) {
            return lpos;
        }
        return  -1;
    }
};

int main () {
    Solution s;
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cout << "ori:[" ;
    for (int i = 0; i < nums.size(); i++) {
        cout << nums[i] << " ";
    }
    cout << "]" << endl;
    cout << "5 at " << s.findPosition(nums, 5) << endl;
    cout << "1 at " << s.findPosition(nums, 1) << endl;
    cout << "10 at " << s.findPosition(nums, 10) << endl;
    cout << "0 at " << s.findPosition(nums, 0) << endl;

    vector<int> nums1 = {1,2,3};
    cout << "ori:[" ;
    for (int i = 0; i < nums.size(); i++) {
        cout << nums[i] << " ";
    }
    cout << "]" << endl;
    cout << "1 at " << s.findPosition(nums1, 1) << endl;
    cout << "2 at " << s.findPosition(nums1, 2) << endl;
    cout << "3 at " << s.findPosition(nums1, 3) << endl;

    vector<int> nums2 = {1,2,3,4};
    cout << "ori:[" ;
    for (int i = 0; i < nums2.size(); i++) {
        cout << nums2[i] << " ";
    }
    cout << "]" << endl;
    cout << "1 at " << s.findPosition(nums2, 1) << endl;
    cout << "2 at " << s.findPosition(nums2, 2) << endl;
    cout << "3 at " << s.findPosition(nums2, 3) << endl;
    cout << "4 at " << s.findPosition(nums2, 4) << endl;

    vector<int> nums3 = {};
    cout << "ori:[" ;
    for (int i = 0; i < nums3.size(); i++) {
        cout << nums3[i] << " ";
    }
    cout << "]" << endl;
    cout << "0 at " << s.findPosition(nums3, 0) << endl;

    vector<int> nums4 = {1,2,2,4,5,5};
    cout << "ori:[" ;
    for (int i = 0; i < nums4.size(); i++) {
        cout << nums4[i] << " ";
    }
    cout << "]" << endl;
    cout << "1 at " << s.findPosition(nums4, 1) << endl;
    cout << "2 at " << s.findPosition(nums4, 2) << endl;
    cout << "4 at " << s.findPosition(nums4, 4) << endl;
    cout << "5 at " << s.findPosition(nums4, 5) << endl;
    cout << "6 at " << s.findPosition(nums4, 6) << endl;

    vector<int> nums5 = {2};
    cout << "ori:[" ;
    for (int i = 0; i < nums5.size(); i++) {
        cout << nums5[i] << " ";
    }
    cout << "]" << endl;
    cout << "2 at " << s.findPosition(nums5, 2) << endl;
    cout << "1 at " << s.findPosition(nums5, 1) << endl;

    vector<int> nums6 = {2,2};
    cout << "ori:[" ;
    for (int i = 0; i < nums6.size(); i++) {
        cout << nums6[i] << " ";
    }
    cout << "]" << endl;
    cout << "2 at " << s.findPosition(nums6, 2) << endl;

    vector<int> nums7 = {2,2,2};
    cout << "ori:[" ;
    for (int i = 0; i < nums7.size(); i++) {
        cout << nums7[i] << " ";
    }
    cout << "]" << endl;
    cout << "2 at " << s.findPosition(nums7, 2) << endl;

    vector<int> nums8 = {2,2,2,2};
    cout << "ori:[" ;
    for (int i = 0; i < nums8.size(); i++) {
        cout << nums8[i] << " ";
    }
    cout << "]" << endl;
    cout << "2 at " << s.findPosition(nums8, 2) << endl;

    vector<int> nums9 = {1,2,2,4,5,5};
    cout << "ori:[" ;
    for (int i = 0; i < nums9.size(); i++) {
        cout << nums9[i] << " ";
    }
    cout << "]" << endl;
    cout << "1 at " << s.findPosition(nums9, 1) << endl;
    cout << "2 at " << s.findPosition(nums9, 2) << endl;
    cout << "5 at " << s.findPosition(nums9, 5) << endl;
    cout << "6 at " << s.findPosition(nums9, 6) << endl;
    return 0;
}
