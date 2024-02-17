#include <iostream>
#include <vector>

using namespace std;

class ArrayReader {
public:
    std::vector<int> nums;
    ArrayReader() {}
    ArrayReader(initializer_list<int> l) : nums(l) {
        if (nums.size() == 0) {
            cout << "ArrayReader(initializer_list<int> l)-> l: empty" << endl;
            return;
        }
        cout << "ArrayReader(initializer_list<int> l)-> l: ";
        int cnt = 0;
        for (auto i : l) {
            cout << i << "[" << cnt << "] ";
            ++cnt;
        }
        cout << endl;
    }
    int get(int k) {
        if (k < nums.size() && k >= 0) {
            return nums[k];
        }
        return -1;
    }
};
class Solution {
    /**
     * @param reader: An instance of ArrayReader.
     * @param target: An integer
     * @return: An integer which is the first index of target.
     * 给一个按照升序排序的非负整数数组。这个数组很大以至于你只能通过固定的接口 ArrayReader.get(k) 来访问第k个数(或者C++里是ArrayReader->get(k))，并且你也没有办法得知这个数组有多大。
     * 找到给出的整数target第一次出现的位置。你的算法需要在O(logk)的时间复杂度内完成，k为target第一次出现的位置的下标。
    * 如果找不到target，返回-1。
     */
public:
    int searchBigSortedArray(ArrayReader& reader, int target) {
        int lpos = 0;
        int rpos = 2;
        while (lpos + 1 < rpos) {
            // cout << "==== rpos " << rpos << " reader.get(rpos)=" << reader.get(rpos) << endl;
            if (reader.get(rpos) == -1) { // 越界
                rpos = (lpos + rpos) / 2;
            } else if (reader.get(rpos) < target) {
                lpos = rpos;
                rpos *= 2;
            } else if (reader.get(rpos) >= target) {
                break;
            }
        }
        while (lpos + 1 < rpos) {
            // int mpos = (lpos + rpos) / 2; // 容易溢出
            int mpos = lpos + (rpos - lpos) / 2;
            if (reader.get(mpos) < target) {
                lpos = mpos + 1;
            } else {
                rpos = mpos;
            }
        }
        // cout << lpos << " " << rpos << endl;
        if (reader.get(lpos) == target) {
            return lpos;
        } else if (reader.get(rpos) == target) {
            return rpos;
        }
        return -1;
    }
};

int main() {
    Solution s;
    ArrayReader reader;
    int target;
    int target_pos;

    // reader = {1, 3, 6, 9, 21, 45, 66, 100, 100, 100, 1000, 10000};
    // target = 3;
    // target_pos = s.searchBigSortedArray(reader, target);
    // cout << "target: " << target << " target_pos: " << target_pos << endl;

    // target = 100;
    // target_pos = s.searchBigSortedArray(reader, target);
    // cout << "target: " << target << " target_pos: " << target_pos << endl;

    // target = 2;
    // target_pos = s.searchBigSortedArray(reader, target);
    // cout << "target: " << target << " target_pos: " << target_pos << endl;

    // reader = initializer_list<int>{};
    // target = 100;
    // target_pos = s.searchBigSortedArray(reader, target);
    // cout << "target: " << target << " target_pos: " << target_pos << endl;

    reader = {100};
    target = 100;
    target_pos = s.searchBigSortedArray(reader, target);
    cout << "target: " << target << " target_pos: " << target_pos << endl;

    reader = {100,100};
    target = 100;
    target_pos = s.searchBigSortedArray(reader, target);
    cout << "target: " << target << " target_pos: " << target_pos << endl;

    reader = {100,100,100};
    target = 100;
    target_pos = s.searchBigSortedArray(reader, target);
    cout << "target: " << target << " target_pos: " << target_pos << endl;

    reader = {100,100,100,100};
    target = 100;
    target_pos = s.searchBigSortedArray(reader, target);
    cout << "target: " << target << " target_pos: " << target_pos << endl;

    reader = {50,100,100,100,100};
    target = 100;
    target_pos = s.searchBigSortedArray(reader, target);
    cout << "target: " << target << " target_pos: " << target_pos << endl;

    reader = {50,50,100,100,100,100};
    target = 100;
    target_pos = s.searchBigSortedArray(reader, target);
    cout << "target: " << target << " target_pos: " << target_pos << endl;

    reader = {50,50,50,100,100,100,100};
    target = 100;
    target_pos = s.searchBigSortedArray(reader, target);
    cout << "target: " << target << " target_pos: " << target_pos << endl;

    reader = {50,50,50,50,100};
    target = 100;
    target_pos = s.searchBigSortedArray(reader, target);
    cout << "target: " << target << " target_pos: " << target_pos << endl;

    reader = {50,50,50,50,50};
    target = 100;
    target_pos = s.searchBigSortedArray(reader, target);
    cout << "target: " << target << " target_pos: " << target_pos << endl;

    reader = { 1 , 4 ,  5 ,  7 ,  8 ,  10 , 15 , 20 ,  30 ,  50 ,  80 ,  90 , 100,  200,  300, 400};
    target  = 20;
    target_pos = s.searchBigSortedArray(reader, target);
    cout << "target: " << target << " target_pos: " << target_pos << endl;
    return 0;
}

