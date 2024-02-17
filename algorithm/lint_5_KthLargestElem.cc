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
    int kthLargestElement(int k, vector<int> &nums) {
        // write your code here
        if (k > nums.size() || k <= 0) {
            return -1;
        }
        return kthLargestElement_core(k - 1, nums, 0, nums.size() - 1);
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
            while (l <= r && nums[l] > pivot) {
                ++l;
            }
            while (l <= r && nums[r] < pivot) {
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
            while (vec[l] > pivot) {
                ++l;
            }
            while (vec[r] < pivot) {
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

void print_K(Solution& s, std::vector<int>& nums, int k) {
    vector<int> nums_tmp = nums;
    int val = s.kthLargestElement(k, nums_tmp);
    s.quick_sort_core_1(nums_tmp, 0, nums_tmp.size() - 1);
    print_result(nums, nums_tmp);
    cout << "Kth=" << k << " val=" << val << endl;
}

int main (){
    Solution s;

    vector<int> nums = {4,5,6,8,9, 1,2,3,10,7};
    print_K(s, nums, 1);
    print_K(s, nums, 2);
    print_K(s, nums, 3);
    print_K(s, nums, 4);
    print_K(s, nums, 5);
    print_K(s, nums, 6);
    print_K(s, nums, -1);

    vector<int> nums1 = {1,3,4,2};
    print_K(s, nums1, 4);

    vector<int> nums2 = {1,3,4};
    print_K(s, nums2, 2);

    vector<int> nums3 = {8,9,10};
    print_K(s, nums3, 3);
    return 0;
}