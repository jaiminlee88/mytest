#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <climits>
#include <list>
#include <stack>
#include <set>
#include <queue>
#include <numeric>
#include <bitset>
#include <deque>
using namespace std;


/**
 * Definition of ListNode
 * class ListNode {
 * public:
 *     int val;
 *     ListNode *next;
 *     ListNode(int val) {
 *         this->val = val;
 *         this->next = NULL;
 *     }
 * }
 */

class ListNode {
public:
    int val;
    ListNode *next;
    ListNode(int val) {
        this->val = val;
        this->next = NULL;
    }
};

class cmp {
public:
    bool operator()(const ListNode* a, const ListNode* b) {
        return a->val > b->val;
    }
};
class Solution {
public:
    /**
     * @param lists: a list of ListNode
     * @return: The head of one sorted list.
     * 合并 k 个排序链表（序列为升序序列），并且返回合并后的排序链表（序列为升序序列）。尝试分析和描述其复杂度。
     * 
     * lists = [2->4->null,null,-1->null]
     * -1->2->4->null
     * 
     * lists = [2->6->null,5->null,7->null]
     * 2->5->6->7->null
     * 
     * 用vector遍历，取一个链接到dummy后
     * 
     * 用优先队列，每次取最小的链接到dummy后
     * 
     * 也可以两两归并
     */
    ListNode *mergeKLists(vector<ListNode *> &lists) {
        // write your code here
        return mergeKLists_0(lists);
    }

    ListNode *mergeKLists_0(vector<ListNode *> &lists) {
        ListNode* dummy = new ListNode(0);
        ListNode* cur = dummy;
        priority_queue<ListNode*, vector<ListNode*>, cmp> q;
        for (auto& node : lists) {
            if (node != nullptr) {
                q.push(node);
            }
        }
        while (q.empty() == false) {
            ListNode* node = q.top();
            q.pop();
            if (node == nullptr) {
                continue;
            }
            cur->next = node;
            cur = cur->next;
            if (node->next != nullptr) {
                q.push(node->next);
            }
        }
        cur = dummy->next;
        delete dummy;
        return cur;
    }
};



int main() {
    Solution s;
    vector<int> nums;
    vector<int> nums0;
    vector<int> nums1;
    vector<vector<int>> nums3;
    vector<vector<char>> nums4;
    vector<string> nums5;
    string str;
    string str0;
    string str1;
    // int k = 0;
    // auto test = [&s](vector<int>& nums) {
    //     cout << "nums: ";
    //     for (auto& i : nums) {
    //         cout << i << " ";
    //     }
    //     cout << endl;
    //     // auto res = s.permute(nums);
    //     // auto res = s.permute_0(nums);
    //     // auto res = s.permute_1(nums);
    //     // auto res = s.permute_2(nums);
    //     auto res = s.permute_3(nums);
    //     cout << "permutaions: "<< endl;
    //     for (auto& v : res) {
    //         for (auto& i : v) {
    //             cout << i << " ";
    //         }
    //         cout << endl;
    //     }
    // };

    // nums = {};
    // test(nums);

    // nums = {1};
    // test(nums);

    // nums = {1,2};
    // test(nums);

    // nums = {1, 2, 3};
    // test(nums);

    // nums = {1,2,3,4};
    // test(nums);


}