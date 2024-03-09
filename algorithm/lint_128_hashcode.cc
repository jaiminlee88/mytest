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
     * @param key: A string you should hash
     * @param hASH_SIZE: An integer
     * @return: An integer
     * 
     * 在数据结构中，哈希函数是用来将一个字符串（或任何其他类型）
     * 转化为小于哈希表大小且大于等于零的整数。一个好的哈希函数可以尽可能少地产生冲突。
     * 一种广泛使用的哈希函数算法是使用数值 33，假设任何字符串都是基于 33 的一个大整数，比如：
    hashcode("abcd")
        =(ascii(a)∗33^3
            +ascii(b)∗33^2
            +ascii(c)∗3^3
            +ascii(d)) % HASH_SIZE
        =(97∗33^3+98∗33^2+99∗33+100)
        =(97∗33^3+98∗33^2 +99∗33+100) % HASH_SIZE
        =3595978=3595978 % HASH_SIZE
        其中HASH_SIZE表示哈希表的大小(可以假设一个哈希表就是一个索引 0 ~ HASH_SIZE - 1的数组)。
        给出一个字符串作为 key 和一个哈希表的大小，返回这个字符串的哈希值
     */
    int hashCode(string &key, int hASH_SIZE) {
        // write your code here
        // 33 出现的冲突比较小，质数冲突小
        long long ret = 0;
        for (int pos=0; pos < key.size(); pos++) {
            ret = (ret * 33 % hASH_SIZE + key[pos] % hASH_SIZE) % hASH_SIZE;
        }
        return ret;
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
    auto test = [&s](string str, int hASH_SIZE){
        int res = s.hashCode(str, hASH_SIZE);
        cout << "hashcode(\"" << str << "\", " << hASH_SIZE << ")=" << res << endl;
    };

    test("abcd", 1000);
    test("abcd", 100);
}