#include <iostream>
#include <vector>

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
        this->next = nullptr;
    }
};

class Solution {
public:
    /**
     * @param hashTable: A list of The first node of linked list
     * @return: A list of The first node of linked list which have twice size
     * 哈希表容量的大小在一开始是不确定的。如果哈希表存储的元素太多（如超过容量的十分之一），
     * 我们应该将哈希表容量扩大一倍，并将所有的哈希值重新安排.
        哈希函数为：
        int hashcode(int key, int capacity) {
            return key % capacity;
        }
        这里有三个数字9，14，21，其中21和9共享同一个位置因为它们有相同的哈希值1(21 % 4 = 9 % 4 = 1)。我们将它们存储在同一个链表中。
        重建哈希表，将容量扩大一倍，我们将会得到：
        size=3, capacity=8
        index: 0 1 2 3 4 5 6 7
        hash : [null, 9, null, null, null, 21, 14, null]
        给定一个哈希表，返回重哈希后的哈希表。

        样例
        样例 1

        输入 : [null, 21->9->null, 14->null, null]
        输出 : [null, 9->null, null, null, null, 21->null, 14->null, null]
        注意事项
        哈希表中负整数的下标位置可以通过下列方式计算：

        C++/Java：如果你直接计算-4 % 3，你会得到-1，你可以应用函数：a % b = (a % b + b) % b得到一个非负整数。
        Python：你可以直接用-1 % 3，你可以自动得到2。

     */
    vector<ListNode*> rehashing(vector<ListNode*> hashTable) {
        if (hashTable.empty()){
            return {};
        }
        int ori_capacity = hashTable.size();
        vector<int> hashTable_vals;
        for (auto& it : hashTable) {
            ListNode* p = it;
            while (p != nullptr && p->val != -1) {
                hashTable_vals.push_back(p->val);
                p = p->next;
            }
        }
        if (static_cast<double>(hashTable_vals.size()) / ori_capacity < 0.1) {
            return hashTable;
        }
        cout << "ori_capacity: " << ori_capacity << ", ori_size: " << hashTable_vals.size() << endl;
        int new_capacity = ori_capacity * 2;
        vector<ListNode*> new_hashtable(new_capacity, new ListNode(-1));
        auto get_hashkey = [](int value, int capacity) {
            return value % capacity;
        };
        for (auto& val : hashTable_vals) {
            int hashkey = get_hashkey(val, new_capacity);
            ListNode* p = new ListNode(val);
            ListNode* head = new_hashtable[hashkey];
            while (head->next != nullptr && head->next->val != -1) {
                head = head->next;
            }
            if (head->next == nullptr) { // -1
                new_hashtable[hashkey] = p;
                p->next = head;
                continue;
            }
            ListNode* tmp = head->next;
            head->next = p;
            p->next = tmp;
        }
        return new_hashtable;
    }
};

vector<ListNode*> construct_hashmap(vector<vector<int>>& nums) {
    vector<ListNode*> hashTable;
    for (auto& v : nums) {
        ListNode* head = new ListNode(-1);
        ListNode* p = head;
        for (auto& n : v) {
            p->next = new ListNode(n);
            p = p->next;
        }
        hashTable.push_back(head->next);
        delete head;
    }
    return hashTable;
}

int main() {
    Solution s;
    auto test = [&s](vector<vector<int>> nums) {
        cout << "==============" << endl;
        vector<ListNode*> hash_table = construct_hashmap(nums);
        cout << "original hash table: " << endl;
        for (auto& head : hash_table) {
            ListNode* p = head;
            while (p != nullptr) {
                cout << p->val << " ";
                p = p->next;
            }
            cout << endl;
        }
        auto res = s.rehashing(hash_table);
        cout << "rehashed hash table: " << endl;
        for (auto& head : res) {
            ListNode* p = head;
            while (p != nullptr) {
                cout << p->val << " ";
                p = p->next;
            }
            cout << endl;
        }
    };

    test(vector<vector<int>>{{-1}, {21,9,-1}, {14}, {-1}});
    test(vector<vector<int>>{{-1}, {9,-1}, {-1}, {-1},{-1},{-1},{-1},{-1},{-1},{-1},{-1}});
}