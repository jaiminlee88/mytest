#include <vector>
#include <iostream>
#include <unordered_map>
#include <iterator>
#include <algorithm>
using namespace std;

class Solution {
public:
    /**
     * @param numbers: An array of Integer
     * @param target: target = numbers[index1] + numbers[index2]
     * @return: [index1, index2] (index1 < index2)
     */
    // 暴力法
    // vector<int> twoSum(vector<int> &numbers, int target) {
    //     // write your code here
    //     if (numbers.size() < 2) {
    //         return {};
    //     }
    //     vector<int> index;
    //     for (int i = 0; i < numbers.size(); i++) {
    //         fori (int j = i + 1; j < numbers.size(); j++) {
    //             if (numbers[i] + numbers[j] == target) {
    //                 index.push_back(i);
    //                 index.push_back(j);
    //             }
    //         }
    //     }
    //     return index;
    // }

    // 直接用hash_map记录下标
    // vector<int> twoSum(vector<int> &numbers, int target) {
    //     // write your code here
    //     if (numbers.size() < 2) {
    //         return {};
    //     }
    //     unordered_map<int, int> hash_map;
    //     for (int i = 0; i < numbers.size(); i++) {
    //         hash_map[numbers[i]] = i;
    //         int tofind = target - numbers[i];
    //         // std::unordered_map<int, int>::iterator it = hash_map.find(tofind);
    //         auto it = hash_map.find(tofind);
    //         if (it != hash_map.end() && (it->second != i)) {
    //             return (it->second > i) ? vector<int>{i, it->second} : vector<int>{it->second, i};
    //         } else {
    //             for (int j = i + 1; j < numbers.size(); j++) {
    //                 if (numbers[j] == tofind) {
    //                     return (i < j) ? vector<int>{i, j} : vector<int>{j, i}; // 直接从剩下队列中找到
    //                 } else {
    //                     hash_map[numbers[j]] = j; // 否则直接记录下标
    //                 }
    //             }
    //         }
    //     }
    //     return {};
    // }

    // 双指针法
    vector<int> twoSum(vector<int> &numbers, int target) {
        // write your code here
        if (numbers.size() < 2) {
            return {};
        }
        vector<int> new_nums = numbers;
        sort(new_nums.begin(), new_nums.end());
        int lpos = 0;
        int rpos = new_nums.size() - 1;
        while (lpos < rpos) {
            if (new_nums[lpos] + new_nums[rpos] > target) {
                --rpos;
            } else if (new_nums[lpos] + new_nums[rpos] < target) {
                ++lpos;
            } else {
                int pos1 = 0;
                int pos2 = 0;
                for (int i = 0; i < numbers.size(); i++) {
                    if (numbers[i] == new_nums[lpos]) {
                        pos1 = i;
                        continue;
                    }
                    if (numbers[i] == new_nums[rpos]) {
                        pos2 = i;
                        continue;
                    }
                }
                return (pos1 < pos2) ? vector<int>{pos1, pos2} : vector<int>{pos2, pos1};
            }
        }
        return {};
    }
};


void print_result(std::vector<int>& numbers, int target, std::vector<int>& result) {
    cout << "numbers:[";
    for (auto& it : numbers) {
        cout << it << ", ";
    }
    cout << "] target: " << target << " result: [";
    for (auto i : result) {
        std::cout << i << ",";
    }
    std::cout << "]" << std::endl;
}
int main() {
    /*描述
    给一个整数数组，找到两个数使得他们的和等于一个给定的数 target。
    你需要实现的函数twoSum需要返回这两个数的下标, 并且第一个下标小于第二个下标。注意这里下标的范围是 0 到 n-1。
    
    注意
    过程分为两步，加入元素和查找元素，要明确哪一步动作更多，频次更高
    如果是查找元素更多，那么用hash_map，可以解决元素重复问题，如果用set必须强假设元素不重复。
    如果是加入元素更多，list可能更好？那么用双指针法。
    */
    Solution s;

    vector<int> numbers = {2, 7, 11, 15};
    int target = 9;
    vector<int> result = s.twoSum(numbers, target);
    print_result(numbers, target, result);

    vector<int> numbers2 = {15, 2, 7, 11};
    int target2 = 10;
    vector<int> result2 = s.twoSum(numbers2, target2);
    print_result(numbers2, target2, result2);

    vector<int> numbers3 = {};
    int target3 = 10;
    vector<int> result3 = s.twoSum(numbers3, target3);
    print_result(numbers3, target3, result3);

    vector<int> numbers4 = {1, 0, -1};
    int target4 = 0;
    vector<int> result4 = s.twoSum(numbers4, target4);
    print_result(numbers4, target4, result4);

    vector<int> numbers5 = {1,4,2,3,5,9};
    int target5 = 8;
    vector<int> result5 = s.twoSum(numbers5, target5);
    print_result(numbers5, target5, result5);

    vector<int> numbers6 = {0,4,3,0};
    int target6 = 0;
    vector<int> result6 = s.twoSum(numbers6, target6);
    print_result(numbers6, target6, result6);
    return 0;
}