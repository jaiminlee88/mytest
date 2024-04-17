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
 * Definition of singly-linked-list:
 * class ListNode {
 * public:
 *     int val;
 *     ListNode *next;
 *     ListNode(int val) {
 *        this->val = val;
 *        this->next = NULL;
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

class Solution {
public:
    /**
     * @param l1: ListNode l1 is the head of the linked list
     * @param l2: ListNode l2 is the head of the linked list
     * @return: ListNode head of linked list
     * 
     * 将两个排序（升序）链表合并为一个新的升序排序链表
     */
    ListNode* mergeTwoLists(ListNode *l1, ListNode *l2) {
        // write your code here
        if (l1 == nullptr) {
            return l2;
        }
        if (l2 == nullptr) {
            return l1;
        }
        // 1->3->6->7
        // 2->5->8
        ListNode* dummy = new ListNode(0);
        ListNode* cur = dummy;
        ListNode* p1 = l1;
        ListNode* p2 = l2;
        ListNode* tmp = nullptr;
        while (p1 != nullptr && p2 != nullptr) {
            if (p1->val < p2->val) {
                tmp = p1->next;
                cur->next = p1;
                p1 = tmp;
            } else {
                tmp = p2->next;
                cur->next = p2;
                p2 = tmp;
            }
            cur = cur->next;
        }
        // 1->3->6->7
        // 2->2->5->8
        if (p1 != nullptr) {
            cur->next = p1;
        } else if (p2 != nullptr) {
            cur->next = p2;
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
    int k = 0;
    auto test = [&s](vector<int>& nums) {
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        auto res = s.permute_3(nums);
        cout << "permutaions: "<< endl;
        for (auto& v : res) {
            for (auto& i : v) {
                cout << i << " ";
            }
            cout << endl;
        }
    };

    nums = {};
    test(nums);

    nums = {1};
    test(nums);

    nums = {1,2};
    test(nums);

    nums = {1, 2, 3};
    test(nums);

    nums = {1,2,3,4};
    test(nums);
}