#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    /**
     * @param numbers: Give an array
     * @param target: An integer
     * @return: Find all unique quadruplets in the array which gives the sum of zero
     *          we will sort your return value in output
     * 给一个包含n个数的整数数组S，在S中找到所有使得和为给定整数target的四元组(a, b, c, d)。
     * a+b+c+d=target = (a+b)+c+d, 固定a+b，找c+d
     */
    vector<vector<int>> fourSum(vector<int> &numbers, int target) {
        // write your code here
        if (numbers.size() < 4) {
            return {};
        }
        sort(numbers.begin(), numbers.end());
        vector<vector<int>> result;
        for (int apos = 0; apos < numbers.size(); apos++) {
            if (apos > 0 && numbers[apos] == numbers[apos - 1]) {
                continue;
            }
            for (int bpos = apos + 1; bpos < numbers.size(); bpos++) {
                if ((bpos > apos + 1) && numbers[bpos] == numbers[bpos - 1]) {
                    continue;
                }
                int new_target = target - numbers[apos] - numbers[bpos];
                cout<< "numbers[" << apos << "]= " << numbers[apos] 
                    << " numbers[" << bpos << "]= " << numbers[bpos] << " newtarget= " << new_target << endl;
                int lpos = bpos + 1;
                int rpos = numbers.size() - 1;
                while (lpos < rpos) {
                    if (numbers[lpos] + numbers[rpos] > new_target) {
                        --rpos;
                    } else if (numbers[lpos] + numbers[rpos] < new_target) {
                        ++lpos;
                    } else {
                        result.push_back({numbers[apos], numbers[bpos], numbers[lpos], numbers[rpos]});
                        ++lpos;
                        --rpos;
                        while(lpos < rpos && (numbers[lpos] == numbers[lpos - 1])) {
                            ++lpos;
                        }
                        while (lpos < rpos && (numbers[rpos] == numbers[rpos + 1])) {
                            --rpos;
                        }
                    }
                }
            }
        }
        return result;
    }
};

void print_result(std::vector<int>& numbers, std::vector<vector<int>>& result, int target) {
    cout << "numbers:[";
    for (auto& it : numbers) {
        cout << it << ", ";
    }
    cout << "] target: " << target << " result: ";
    for (auto& vec : result) {
        cout << "[";
        for (auto i : vec) {
            std::cout << i << ",";
            }
        std::cout << "] ";
    }
    cout << std::endl;
}

int main () {
    Solution s;
    vector<int> numbers;
    vector<vector<int>> result;
    int target = 0;

    numbers = {1, 0, -1, 0, -2, 2};
    target = 0;
    result = s.fourSum(numbers, target);
    print_result(numbers, result, target);

    numbers = {2,7,11,15};
    target = 3;
    result = s.fourSum(numbers, target);
    print_result(numbers, result, target);

    numbers = {1,0,-1,-1,-1,-1,0,1,1,1,2};
    target = 2;
    result = s.fourSum(numbers, target);
    print_result(numbers, result, target);
    return 0;
}