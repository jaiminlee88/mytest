#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <climits>
#include <list>
#include <stack>
using namespace std;


class Solution {
public:
 /**
     * @param nums: a continuous stream of numbers
     * @param number: a number
     * @return: returns the first unique number
     * 
     * 给一个连续的数据流,
     * 写一个函数返回终止数字到达时的第一个唯一数字（包括终止数字）,
     * 如果找不到这个终止数字, 返回 -1.
     * 
     * 输入： 
        [1, 2, 2, 1, 3, 4, 4, 5, 6]
        5
        输出： 3

        输入：
        [1, 2, 2, 1, 3, 4, 4, 5, 6]
        7
        输出： -1

        输入：
        [1, 2, 2, 1, 3, 4]
        3
        输出： 3

        用一个队列和哈希表来做,队列记住顺序，哈希表记住次数
        用vector和hashmap也可以，vector内存连续，vector可以用下标访问，记住顺序和次数，hashmap记住值对应下标
        或者用queue和hash，queue记住顺序，hash记住是否出现过bool，如果没有出现则为true（并加入队列），再次出现为false
            读取queue中数据，如果hash中为true则返回
        只用hashmap也可以，但是要遍历nums两次，nums中的数据可能很大
     */
    int firstUniqueNumber(vector<int> &nums, int number) {
        // Write your code here
        if (nums.size() == 0) {
            return -1;
        }
        unordered_map<int, int> m;
        queue<int> q;
        bool found_end = false;
        for (auto& num : nums) {
            auto it = m.find(num);
            if (it == m.end()) {
                q.push(num);
            }
            m[num]++;
            if (num == number) {
                found_end = true;
                break;
            }
        }
        if (found_end) {
            while(q.empty() == false) {
                int num = q.front();
                q.pop();
                if (m[num] == 1) {
                    return num; // 终止数永远unique
                }
            }
        }
        return -1;
    }
};


int main() {
    Solution s;
    vector<int> nums;
    auto test = [&s](vector<int>& nums, int number) {
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.firstUniqueNumber(nums, number);
        cout << " end=" << number << " permutaions: " << res << endl;
    };

    nums = {1, 2, 2, 1, 3, 4, 4, 5, 6};
    test(nums, 5);

    nums = {1, 2, 2, 1, 3, 4, 4, 5, 6};
    test(nums, 7);

    nums = {1, 2, 2, 1, 3, 4};
    test(nums, 3);
}