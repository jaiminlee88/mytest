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


class RandomizedSet {
public:
    /**
     * 设计一个数据结构实现在平均 O(1) 的复杂度下执行以下所有的操作。
            insert(val): 如果这个元素不在set中，则插入。
            remove(val): 如果这个元素在set中，则从set中移除。
            getRandom: 随机从set中返回一个元素。每一个元素返回的可能性必须相同。
        

        // 初始化空集set
        RandomizedSet randomSet = new RandomizedSet();
        // 1插入set中。返回正确因为1被成功插入
        randomSet.insert(1);
        // 返回错误因为2不在set中
        randomSet.remove(2);
        // 2插入set中，返回正确，set现在有[1,2]。
        randomSet.insert(2);
        // getRandom 应该随机的返回1或2。
        randomSet.getRandom();
        // 从set中移除1，返回正确。set现在有[2]。
        randomSet.remove(1);
        // 2已经在set中，返回错误。
        randomSet.insert(2);
        // 因为2是set中唯一的数字，所以getRandom总是返回2。
        randomSet.getRandom();
     * 

        用hash表记录value->index下标,如果不存在则index为-1
        删除，用vector记录value，如果需要删除，通过hash表找到下标，讲vector最后一个元素与该下标元素交换，然后结尾往前移动一位, O(1)
        增加，如果hash表中存在该值，返回false，否则在vector尾部加入该值，hash表记录该值的下标 O(1)
        随机返回，返回vector中的随机下标 O(1)
     * **/
    unordered_map<int,int> m;
    vector<int> v;
    int last_index = 0; // 该位置不放置新数
    RandomizedSet() {
        // do intialization if necessary
        srand(time(0));
        reset();
    }

    void reset() {
        m.clear();
        v.clear();
        last_index = 0;
    }
    /*
     * @param val: a value to the set
     * @return: true if the set did not already contain the specified element or false
     */
    bool insert(int val) {
        // write your code here
        // 如果这个元素不在set中，则插入。
        auto iter = m.find(val);
        if (iter == m.end() || iter->second == -1) { // 尽量不删除hash表中的元素，因为删除hash表中的元素会导致hash表的rehash，效率低
            v.push_back(val);
            m[val] = last_index;
            ++last_index;
            return true;
        }
        return false;
    }

    /*
     * @param val: a value from the set
     * @return: true if the set contained the specified element or false
     */
    bool remove(int val) {
        // write your code here
        // 如果这个元素在set中，则从set中移除。
        // if (last_index == 0) {
        //     return false;
        // }
        auto iter = m.find(val);
        if (iter == m.end() || iter->second == -1) {
            return false;
        }
        int index = iter->second;
        // [1,last]
        // [1,2,last]
        swap(v[index], v[last_index - 1]);
        if (last_index > 1) {
            m[v[index]] = index;
        }
        --last_index;
        iter->second = -1;
        v.pop_back();
        return true;
    }

    /*
     * @return: Get a random element from the set
     */
    int getRandom() {
        // write your code here
        // 随机从set中返回一个元素。每一个元素返回的可能性必须相同。
        if (last_index == 0) {
            return -1;
        }
        int pos = rand() % last_index;
        return v[pos];
    }
};

/**
 * Your RandomizedSet object will be instantiated and called as such:
 * RandomizedSet obj = new RandomizedSet();
 * bool param = obj.insert(val);
 * bool param = obj.remove(val);
 * int param = obj.getRandom();
 */


int main() {
    RandomizedSet s;
    vector<int> nums;
    auto test = [&s](vector<int>& nums) {
        s.reset();
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        vector<string> res;
        bool ret = false;
        for (auto& it : nums) {
            if (it == 0) {
                int i = s.getRandom();
                res.push_back(std::to_string(i));
                continue;
            } else if (it > 0) {
                ret = s.insert(it);
                // cout << "insert " << it << " ret: " << ret << endl;
            } else {
                ret = s.remove(-it);
            }
            if (ret == true) {
                res.push_back("true");
            } else {
                res.push_back("false");
            }
        }
        cout << " permutaions: ";
        for (auto& i : res) {
            cout << i << " ";
        }
        cout << endl;
    };

    nums = {1,2,2,0,-1,2,0};
    test(nums);
}