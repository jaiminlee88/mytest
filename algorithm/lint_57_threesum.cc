#include <vector>
#include <iostream>
#include <unordered_map>
#include <iterator>
#include <algorithm>
using namespace std;

class Solution {
public:
    /**
     * @param numbers: Give an array numbers of n integer
     * @return: Find all unique triplets in the array which gives the sum of zero.
     *          we will sort your return value in output
     */
    /*
    有重复数字，且要求结果不能重复
    a + b + c = 0， -c = a + b转换成two sum问题
    如果是有序数list，可以两头夹逼，假设a<b<c, 固定b，两边分别找a和c，O(N^2)
    实际上从中间找可能无法排除重复或者漏掉，可以固定a，再找b和c，找b和c的时候，可以固定b，再找c，就可以避免遗漏
    以上操作确保了a 和 b相互独立且各自不重复，如果是四个数字，可以固定a和b，再找c和d
    如果是无序数list，可以先排序
    如果用hash法，用map，记录数据重复个数，然后遍历map

    */
    vector<vector<int>> threeSum(vector<int> &numbers) {
        // write your code here
        if (numbers.size() < 3) {
            return {};
        }
        sort(numbers.begin(), numbers.end());
        vector<vector<int>> result;
        for (int pos = 0; pos < numbers.size(); pos++) {
            if (pos > 0 && numbers[pos] == numbers[pos - 1]) {
                continue;
            }
            // 定第一个，找后面两个，不要从中间找，因为中间找会有重复
            cout << "[find] " << numbers[pos] << "[" << pos << "]" << endl;
            threeSum_core(numbers, result, pos);
        }
        return result;
    }

    void threeSum_core(vector<int>& numbers, vector<vector<int>>& result, int pos) {
        if (pos < 0 || pos >= numbers.size()) {
            return;
        }

        int target = -numbers[pos];
        int lpos = pos + 1;
        int rpos = numbers.size() - 1;
        while (lpos < rpos) {
            if (lpos >= rpos) {
                break;
            }
            // 先判断
            if ((numbers[lpos] + numbers[rpos]) > target) {
                --rpos;
                continue;
            } else if ((numbers[lpos] + numbers[rpos]) < target) {
                ++lpos;
                continue;
            }
            
            cout << "[found] " << target << "[" << pos << "] ==> [" << numbers[lpos] << "(" << lpos << ")," << numbers[rpos] << "(" << rpos << ")]" << endl;
            vector<int> vec = {numbers[lpos], numbers[rpos], numbers[pos]};
            sort(vec.begin(), vec.end());
            result.push_back(vec);
            --rpos;
            ++lpos;
            while (lpos < rpos) {
                if (numbers[lpos] == numbers[lpos - 1]) {
                    lpos++;
                    continue;
                }
                // 右边不用看，因为左边已经固定了，右边只会变化
                break;
            }
        }
    }
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


void print_result(std::vector<int>& numbers, std::vector<vector<int>>& result) {
    cout << "numbers:[";
    for (auto& it : numbers) {
        cout << it << ", ";
    }
    cout << "] result: ";
    for (auto& vec : result) {
        cout << "[";
        for (auto i : vec) {
            std::cout << i << ",";
            }
        std::cout << "] ";
    }
    cout << std::endl;
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
    vector<int> numbers;
    vector<vector<int>> result;

    numbers = {0,0,0,0};
    result = s.threeSum(numbers);
    print_result(numbers, result);

    numbers = {0,0,7,6,0,};
    result = s.threeSum(numbers);
    print_result(numbers, result);

    // numbers = {11, 2, 7, 15};
    // result = s.threeSum(numbers);
    // print_result(numbers, result);

    // numbers = {-1,0,1,2,-1,-4};
    // result = s.threeSum(numbers);
    // print_result(numbers, result);

    numbers = {1,0,-1,-1,-1,-1,0,1,1,1};
    result = s.threeSum(numbers);
    print_result(numbers, result);

    // numbers = {-2,-3,-4,-5,-100,99,1,4,4,4,5,1,0,-1,2,3,4,5};
    // result = s.threeSum(numbers);
    // print_result(numbers, result);
    return 0;
}