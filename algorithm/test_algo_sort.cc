#include <vector>
#include <iostream>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <functional>
using namespace std;

class Solution {

public:
    /**
     * @param numbers: An array of Integer
     * @param target: target = numbers[index1] + numbers[index2]
     * @return: [index1, index2] (index1 < index2)
     */
    std::function<void(vector<int>&,int,int)> func;

    vector<int> sort(vector<int>& vec) {
        if (vec.size() < 1) {
            return vec;
        }
        vector<int> new_vec = vec;
        func(new_vec, 0, new_vec.size() - 1);
        return new_vec;
    }
    static void quick_sort_core(vector<int>& vec, int lpos, int rpos) { // O(nlogn)，最大O(n^2)
        // for (int i = 0; i < vec.size(); i++) {
        //     if (i == lpos) {
        //         cout << "["<< vec[i] << "],";
        //     } else if (i == rpos) {
        //         cout << "["<< vec[i] << "],";
        //     } else {
        //         cout << vec[i] << ",";
        //     }
        // }
        // cout << endl;
        if (lpos >= rpos || lpos < 0 || rpos >= vec.size()) {
            return;
        }
        int ori_lpos = lpos;
        int ori_rpos = rpos;
        int tar_pos = lpos; // 尽量不要选第一个数，因为第一个数可能是最大或者最小的数，导致不均匀分布，选中间数
        lpos++;
        while (lpos <= rpos) {
            // cout << "ahahah " << lpos << " " << rpos << endl;
            if ((rpos - lpos) <= 1) {
                break;
            }
            if (vec[lpos] <= vec[ori_lpos]) { // m <= p   p <= n 可以让数值均匀分布
                ++lpos;
                continue;
            }
            if (vec[rpos] > vec[ori_lpos]) {
                --rpos;
                continue;
            }
            std::swap(vec[lpos], vec[rpos]);
        }
        if (vec[lpos] > vec[rpos]) {
            std::swap(vec[lpos], vec[rpos]);
        }
        if (vec[ori_lpos] > vec[rpos]) {
            std::swap(vec[ori_lpos], vec[rpos]);
            tar_pos = rpos;
            // quick_sort_core(vec, ori_lpos, lpos);
            // quick_sort_core(vec, rpos, ori_rpos);
        } else if (vec[ori_lpos] > vec[lpos]) {
            std::swap(vec[ori_lpos], vec[lpos]);
            tar_pos = lpos;
            // quick_sort_core(vec, ori_lpos, lpos - 1);
            // quick_sort_core(vec, lpos, ori_rpos);
        } else {
            tar_pos = lpos;
            // quick_sort_core(vec, ori_lpos, lpos - 1);
            // quick_sort_core(vec, lpos, ori_rpos);
        }
        quick_sort_core(vec, ori_lpos, tar_pos - 1);
        quick_sort_core(vec, tar_pos, ori_rpos);
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

    static void merge_sort_core(vector<int>& vec, int lpos, int rpos) { // O(nlogn)
        if (vec.size() <= 1 || lpos >= rpos) {
            return;
        }
        int mid = (lpos + rpos) / 2;
        merge_sort_core(vec, lpos, mid);
        merge_sort_core(vec, mid + 1, rpos);
        int l = lpos;
        int r = mid + 1;
        vector<int> tmp; // 最好外部传入
        while(true) {
            if ((l <= mid) && (r <= rpos)) {
                if (vec[l] <= vec[r]) {
                    tmp.push_back(vec[l]);
                    ++l;
                } else {
                    tmp.push_back(vec[r]);
                    ++r;
                }
            } else if ((l > mid) && (r <= rpos)) {
                tmp.push_back(vec[r]);
                ++r;
            } else if ((l <= mid) && (r > rpos)) {
                tmp.push_back(vec[l]);
                ++l;
            } else {
                break;
            }
        }
        copy(tmp.begin(), tmp.end(), vec.begin() + lpos);
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

void print1(int){
    cout << "print1" << endl;
}
void print2(int){
    cout << "print2" << endl;
}
int main() {
    /*描述
    给一个整数数组，找到两个数使得他们的和等于一个给定的数 target。
    你需要实现的函数twoSum需要返回这两个数的下标, 并且第一个下标小于第二个下标。注意这里下标的范围是 0 到 n-1。
    */
    Solution s;
    // s.func = Solution::quick_sort_core;
    // cout << "test quick_sort_core" << endl;
    s.func = Solution::quick_sort_core_1;
    cout << "test quick_sort_core_1" << endl;
    // s.func = Solution::merge_sort_core;
    // cout << "test merge_sort_core" << endl;

    vector<int> numbers = {11, 2, 7, 15};
    vector<int> ret = s.sort(numbers);
    print_result(numbers, ret);

    vector<int> numbers2 = {3, 2, 4};
    vector<int> ret2 = s.sort(numbers2);
    print_result(numbers2, ret2);

    vector<int> numbers3 = {3, 3};
    vector<int> ret3 = s.sort(numbers3);
    print_result(numbers3, ret3);

    vector<int> numbers4 = {1};
    vector<int> ret4 = s.sort(numbers4);
    print_result(numbers4, ret4);

    vector<int> numbers5 = {};
    vector<int> ret5 = s.sort(numbers5);
    print_result(numbers5, ret5);

    vector<int> numbers6 = {1, 4, 2, 3, 5, 9};
    vector<int> ret6 = s.sort(numbers6);
    print_result(numbers6, ret6);

    vector<int> numbers7 = {2, 0, 1};
    vector<int> ret7 = s.sort(numbers7);
    print_result(numbers7, ret7);

    vector<int> numbers8 = {1, 0, -1};
    vector<int> ret8 = s.sort(numbers8);
    print_result(numbers8, ret8);

    vector<int> numbers9 = {3,3,3};
    vector<int> ret9 = s.sort(numbers9);
    print_result(numbers9, ret9);

    vector<int> numbers10 = {9,7,3,0,-6,-9,0,4,-1,0};
    vector<int> ret10 = s.sort(numbers10);
    print_result(numbers10, ret10);

    vector<int> numbers11 = {3,3,3, 3};
    vector<int> ret11 = s.sort(numbers11);
    print_result(numbers11, ret11);

    vector<int> numbers12 = {3,3,0,0,0,0};
    vector<int> ret12 = s.sort(numbers12);
    print_result(numbers12, ret12);

    vector<int> numbers13 = {3,2,1,4,5};
    vector<int> ret13 = s.sort(numbers13);
    print_result(numbers13, ret13);
    return 0;
}