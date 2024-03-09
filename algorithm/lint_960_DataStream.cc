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


class DataStream {
public:
    /*
    我们需要实现一个叫 DataStream 的数据结构。并且这里有 两 个方法需要实现：
    void add(number) // 加一个新的数
    int firstUnique() // 返回第一个独特的数

    输入:
        add(1)
        add(2)
        firstUnique()
        add(1)
        firstUnique()
        输出:
        [1,2]

    输入:
        add(1)
        add(2)
        add(3)
        add(4)
        add(5)
        firstUnique()
        add(1)
        firstUnique()
        add(2)
        firstUnique()
        add(3)
        firstUnique()
        add(4)
        firstUnique()
        add(5)
        add(6)
        firstUnique()
        输出:
        [1,2,3,4,5,6]

    用一个hash表记录是否是独特的数，用vector或list记录曾经独特的数，可以用find找到并删除
    或者自制指针链表，hash记录值和对应的指针，指针指向链表中某个节点，
        如果hash存在某值，说明该数值之前出现过，如果second的指针不为nullptr，则该值必定在链表中，且唯一，可以找到该指针并删除O(1),然后指针置空
        如果hash不存在某值，说明该值之前没有出现过，将该值加入链表尾部，指针指向该节点。O(1)
        查找时只需要返回链表头部的值即可，O(1)
        很快了，记得链表可以用dummy节点表示下一个可以放入新元素的位置，这样不用每次都遍历到尾部
    */
    unordered_map<int, bool> m;
    vector<int> v;

    DataStream(){
        // do intialization if necessary
        reset();
    }

    void reset() {
        m.clear();
        v.clear();
    }
    /**
     * @param num: next number in stream
     * @return: nothing
     */
    void add(int num) {
        // write your code here
        auto iter = m.find(num);
        if (iter == m.end()) {
            m[num] = true;
            v.push_back(num);
            return;
        }
        if (iter->second == false) {
            return;
        }
        iter->second = false;
        auto it = find(v.begin(), v.end(), num);
        if (it != v.end()) {
            v.erase(it);
        }
    }

    /**
     * @return: the first unique number in stream
     */
    int firstUnique() {
        // write your code here
        if (v.size() == 0) {
            return -1;
        }
        return v[0];
    }
};


int main() {
    DataStream s;
    vector<int> nums;
    auto test = [&s](vector<int>& nums) {
        s.reset();
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        vector<int> res;
        for (auto& it : nums) {
            if (it == -1) {
                res.push_back(s.firstUnique());
            } else {
                s.add(it);
            }
        }
        cout << " permutaions: ";
        for (auto& i : res) {
            cout << i << " ";
        }
        cout << endl;
    };

    // nums = {1, 2, -1, 1, -1};
    // test(nums);

    // nums = {1,2,3,4,5,-1,1,-1,2,-1,3,-1,4,-1,5,6,-1};
    // test(nums);

    nums = {1,1,2,2,1,3,-1};
    test(nums);
}