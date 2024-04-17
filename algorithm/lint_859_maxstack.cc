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


class cmp{
public:
    bool operator()(const pair<int,int>& a, const pair<int,int>& b){
        return a.first > b.first || (a.first == b.first && a.second > b.second);
    }
};


// class MaxStack {
// public:
//     set<pair<int,int>, cmp> _heap;
//     unordered_set<int> _popset;
//     stack<pair<int,int>> _stack;
//     int index = 1;
//     MaxStack() {
//         // do intialization if necessary
//         // _heap.clear();
//         // _hashset.clear();
//         // while (_stack.empty() == false) {
//         //     _stack.pop();
//         // }
//     }

//     /*
//      * @param number: An integer
//      * @return: nothing
//      */
//     void push(int number) {
//         // write your code here
//         // cout << "push: " << number << endl;
//         _stack.push({number, index});
//         _heap.insert({number, index});
//         ++index;
//         // cout << "push: done" << endl;
//     }

//     /*
//      * @return: An integer
//      */
//     int pop() {
//         // write your code here
//         if (_stack.empty()) {
//             return 0;
//         }
//         while (true) {
//             auto top = _stack.top();
//             if (_popset.count(top.first) != 0) {
//                 _popset.erase(top.first);
//                 _stack.pop();
//                 continue;
//             }
//             if (top == *_heap.begin()) {
//                 _stack.pop();
//                 _heap.erase(top);
//                 return top.first;
//             }
//             _popset.insert(top.first);
//             _stack.pop();
//             return top.first;
//         }
//     }

//     /*
//      * @return: An integer
//      */
//     int popMax() {
//         // write your code here
//         if (_heap.empty()) {
//             return 0;
//         }
//         while (true) {
//             auto top = *_heap.begin();
//             if (_popset.count(top.first) != 0) {
//                 _popset.erase(top.first);
//                 _heap.erase(top);
//                 continue;
//             }
//             if (_stack.top().first == top.first) {
//                 _stack.pop();
//                 _heap.erase(top);
//                 return top.first;
//             }
//             _popset.insert(top.first);
//             _heap.erase(top);
//             return top.first;
//         }
//     }

//     /*
//      * @return: An integer
//      */
//     int top() {
//         // write your code here
//         if (_stack.empty()) {
//             return 0;
//         }
//         return _stack.top().first;
//     }

//     /*
//      * @return: An integer
//      */
//     int peekMax() {
//         // write your code here
//         if (_heap.empty()) {
//             return 0;
//         }
//         return _heap.begin()->first;
//     }

// };

class MaxStack {
public:
    set<pair<int,int>, cmp> _heap;
    unordered_set<int> _popset;
    stack<pair<int,int>> _stack;
    int index = 1;
    MaxStack() {
        // do intialization if necessary
        // _heap.clear();
        // _hashset.clear();
        // while (_stack.empty() == false) {
        //     _stack.pop();
        // }
    }

    /*
     * @param number: An integer
     * @return: nothing
     */
    void push(int number) {
        // write your code here
        // cout << "push: " << number << endl;
        _stack.push({number, index});
        _heap.insert({number, index});
        ++index;
        // cout << "push: done" << endl;
    }

    /*
     * @return: An integer
     */

    void clearPoppedInStack() {
        while (!_stack.empty() && _popset.count(_stack.top().first) != 0) {
            _popset.erase(_stack.top().first);
            _stack.pop();
        }
    }

    void clearPoppedInHeap() {
        while (!_heap.empty() && _popset.count(_heap.begin()->first) != 0) {
            _popset.erase(_heap.begin()->first);
            _heap.erase(_heap.begin());
        }
    }

    int pop() {
        // write your code here
        clearPoppedInStack();
        if (_stack.empty()) {
            return 0;
        }
        auto top = _stack.top();
        _stack.pop();
        _popset.insert(top.first);
        return top.first;
    }

    /*
     * @return: An integer
     */
    int popMax() {
        // write your code here
        clearPoppedInHeap();
        if (_heap.empty()) {
            return 0;
        }
        auto top = *_heap.begin();
        _heap.erase(_heap.begin());
        _popset.insert(top.first);
        return top.first;
    }

    /*
     * @return: An integer
     */
    int top() {
        // write your code here
        clearPoppedInStack();
        if (_stack.empty()) {
            return 0;
        }
        return _stack.top().first;
    }

    /*
     * @return: An integer
     */
    int peekMax() {
        // write your code here
        clearPoppedInHeap();
        if (_heap.empty()) {
            return 0;
        }
        return _heap.begin()->first;
    }

};


int main() {
    MaxStack s;
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

    // set<int, less<int>> _heap;
    // _heap.insert(3);
    // _heap.insert(1);
    // _heap.insert(2);
    // while (_heap.empty() == false) {
    //     cout << *_heap.begin() << endl;
    //     _heap.erase(_heap.begin());
    // }

    // set<int, greater<int>> _set1;
    // _set1.insert(3);
    // _set1.insert(1);
    // _set1.insert(2);
    // while (_set1.empty() == false) {
    //     cout << *_set1.begin() << endl;
    //     _set1.erase(_set1.begin());
    // }

    // unordered_set<pair<int,int>> _hashset();

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


    // set<pair<int,int>, cmp> _heap;
    // _heap.insert({3, 1});
    // _heap.insert({1, 2});
    // _heap.insert({3, 2});
    // for (auto& it : _heap) {
    //     cout << it.first << " " << it.second << endl;
    // }

    // s.push(5);
    // s.push(1);
    // s.push(5);
    // cout << "hahaha" << endl;
    // cout << s.top() << endl;
    
    // s.popMax();
    // cout << s.top() << endl;
    // cout << s.peekMax() << endl;
    // s.pop();
    // s.top();
    // cout << s.peekMax() << endl;


    auto test1 = [](){
        MaxStack s;
        vector<int> nums;
        s.push(-928);
        s.push(850);
        nums.push_back(s.popMax());
        nums.push_back(s.top());
        nums.push_back(s.popMax());
        s.push(517);
        nums.push_back(s.peekMax());
        nums.push_back(s.top());;
        nums.push_back(s.peekMax());
        nums.push_back(s.popMax());
        s.push(-153);
        nums.push_back(s.top());
        nums.push_back(s.popMax());
        s.push(-17);
        nums.push_back(s.top());
        nums.push_back(s.popMax());
        s.push(928);
        nums.push_back(s.peekMax());
        nums.push_back(s.top());
        nums.push_back(s.top());
        nums.push_back(s.top());
        nums.push_back(s.pop());
        s.push(765);
        nums.push_back(s.top());
        nums.push_back(s.peekMax());
        nums.push_back(s.pop());
        s.push(-587);
        s.push(899);
        nums.push_back(s.pop());
        nums.push_back(s.top());
        nums.push_back(s.popMax());
        s.push(594);
        nums.push_back(s.peekMax());
        nums.push_back(s.pop());
        s.push(-43);
        nums.push_back(s.popMax());
        s.push(-778);
        nums.push_back(s.peekMax());
        nums.push_back(s.peekMax());
        s.push(92);
        s.push(-122);
        nums.push_back(s.pop());
        nums.push_back(s.pop());
        nums.push_back(s.peekMax());
        nums.push_back(s.popMax());
        s.push(-254);
        nums.push_back(s.pop());
        s.push(-150);
        nums.push_back(s.top());
        nums.push_back(s.pop());
        s.push(-16);
        nums.push_back(s.popMax());
        s.push(801);
        nums.push_back(s.pop());
        s.push(-184);
        nums.push_back(s.top());
        nums.push_back(s.top());
        s.push(860);
        nums.push_back(s.pop());
        s.push(-874);
        nums.push_back(s.peekMax());
        s.push(953);
        nums.push_back(s.pop());
        nums.push_back(s.top());
        nums.push_back(s.pop());
        s.push(618);
        nums.push_back(s.peekMax());
        cout << "[";
        for (auto& i : nums) {
            cout << i << ",";
        }
        cout << "]" << endl;
        cout << "[850,-928,-928,517,517,517,517,-153,-153,-17,-17,928,928,928,928,928,765,765,765,899,-587,-587,594,594,-43,-778,-778,-122,92,-778,-778,-254,-150,-150,-16,801,-184,-184,860,-184,953,-874,-874,618]" << endl;
    };
    test1();

}