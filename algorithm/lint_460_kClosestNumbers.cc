#include<iostream>
#include<vector>

using namespace std;


class Solution {
public:
    /**
     * @param a: an integer array
     * @param target: An integer
     * @param k: An integer
     * @return: an integer array
     * 给一个目标数 target, 一个非负整数 k, 一个按照升序排列的数组 A。
     * 在A中找与target最接近的k个整数。返回这k个数并按照与target的接近程度从小到大排序，
     * 如果接近程度相当，那么小的数排在前面。
     * 
     * 找到接近k的位置，然后左右扩展
     */
    vector<int> kClosestNumbers(vector<int> &a, int target, int k) {
        // write your code here
        if (k > a.size()) {
            return {};
        }
        vector<int> res;
        int lpos = 0;
        int rpos = a.size() - 1;
        while(lpos + 1 < rpos) {
            int mpos = lpos + (rpos - lpos) / 2;
            if (a[mpos] < target) {
                lpos = mpos;
            } else {
                rpos = mpos;
            }
        }
        if (lpos == rpos) {
            rpos++;
        }
        cout << a[lpos] << " " << a[rpos] << endl;
        while(k) {
            if (lpos >=0 && rpos >= a.size()) {
                res.push_back(a[lpos]);
                --lpos;
                --k;
            } else if (lpos < 0 && rpos < a.size()) {
                res.push_back(a[rpos]);
                ++rpos;
                --k;
            } else if ((lpos >= 0) && (rpos < a.size())) {
                if (abs(a[lpos] - target) < abs(a[rpos] - target)) {
                    res.push_back(a[lpos]);
                    --lpos;
                } else if (abs(a[lpos] - target) > abs(a[rpos] - target)) {
                    res.push_back(a[rpos]);
                    ++rpos;
                } else {
                    res.push_back(a[lpos]);
                    --lpos;
                }
                --k;
            } else {
                break;
            }
        }
        return res;
    }
};

void print_result(vector<int> origin, vector<int> result) {
    cout << "origin: [";
    for (auto i : origin) {
        cout << i << " ";
    }
    cout << "] ";
    cout << "result: [";
    for (auto i : result) {
        cout << i << " ";
    }
    cout << "]" << endl;
}
int main() {
    Solution s;
    vector<int> nums;
    vector<int> result;
    int target;
    int k;

    nums = {1, 2, 3};
    target = 2;
    k = 3;
    result = s.kClosestNumbers(nums, target, k);
    print_result(nums, result);

    nums = {1, 4, 6, 8};
    target = 3;
    k = 3;
    result = s.kClosestNumbers(nums, target, k);
    print_result(nums, result);

    nums = {1,10,15,25,35,45,50,59};
    target = 30;
    k = 7;
    result = s.kClosestNumbers(nums, target, k);
    print_result(nums, result);

    nums = {1,1,1,2,2,3,3,3,4,4,4,5,5,5};
    target = 3;
    k = 4;
    result = s.kClosestNumbers(nums, target, k);
    print_result(nums, result);

}