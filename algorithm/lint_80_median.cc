#include<iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /**
     * @param k: An integer
     * @param nums: An array
     * @return: the Kth largest element
     */
    int median(vector<int> &nums) {
        if (nums.size() <= 0) {
            return -1;
        }
        if (nums.size() % 2 == 0) {
            return kthLargestElement_core(nums.size() / 2 - 1, nums, 0, nums.size() - 1);
        } else {
            return kthLargestElement_core(nums.size() / 2, nums, 0, nums.size() - 1);
        }
    }

    int kthLargestElement_core(int kpos, vector<int> &nums, int lpos, int rpos) {
        if ((kpos < lpos) || (kpos > rpos) || (lpos > rpos)) {
            cout << "aaaaaa k:" << kpos << " lpos:" << lpos << " rpos:" << rpos << endl;
            return -1;
        }
        int m = lpos + (rpos - lpos) / 2;
        int l = lpos;
        int r = rpos;
        int pivot = nums[m];
        while (l <= r) {
            while (l <= r && nums[l] < pivot) {
                ++l;
            }
            while (l <= r && nums[r] > pivot) {
                --r;
            }
            if (l <= r) {
                std::swap(nums[l], nums[r]);
                ++l;
                --r;
            }
        }

        if ((l - r) == 2 && (r + 1) == kpos) {
            return nums[kpos];
        }
        if (kpos <= r) {
            return kthLargestElement_core(kpos, nums, lpos, r);
        } else {
            return kthLargestElement_core(kpos, nums, l, rpos);
        }
    }

    static void quick_sort_core_1(vector<int>& vec, int lpos, int rpos) {
        if (vec.size() <= 1 || lpos >= rpos) {
            return;
        }
        int l = lpos;
        int r = rpos;
        int m = lpos + (rpos - lpos) / 2;
        int pivot = vec[m];
        while (l <= r) {
            while (vec[l] < pivot) {
                ++l;
            }
            while (vec[r] > pivot) {
                --r;
            }
            if (l <= r) {
                std::swap(vec[l], vec[r]);
                ++l;
                --r;
            }
        }
        quick_sort_core_1(vec, lpos, r);
        quick_sort_core_1(vec, l, rpos);
    }
};

void print_result(std::vector<int>& numbers, std::vector<int>& result) {
    cout << "numbers:[";
    for (auto& it : numbers) {
        cout << it << ", ";
    }
    cout << "] result:[";
    for (auto i : result) {
        std::cout << i << ",";
    }
    std::cout << "]" << std::endl;
}

void print_K(Solution& s, std::vector<int>& nums) {
    vector<int> nums_tmp = nums;
    int val = s.median(nums_tmp);
    s.quick_sort_core_1(nums_tmp, 0, nums_tmp.size() - 1);
    print_result(nums, nums_tmp);
    cout << "median val=" << val << endl;
}

int main (){
    Solution s;

    vector<int> nums = {4,5,6,8,9, 1,2,3,10,7};
    print_K(s, nums);
    

    vector<int> nums1 = {1,3,4,2};
    print_K(s, nums1);

    vector<int> nums2 = {1,3,4};
    print_K(s, nums2);

    vector<int> nums3 = {8,9,10};
    print_K(s, nums3);

    vector<int> nums4 = {7,9,4,5};
    print_K(s, nums4);
    return 0;
}