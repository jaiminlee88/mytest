#include <vector>
#include <iostream>

using namespace std;


class Solution {
public:
    /**
     * @param a: Given an integer array
     * @return: nothing
     * 给出一个整数数组，把它变成一个最小堆数组，这称为堆化操作。
       对于堆数组A，A[0]是堆的根，并对于每个A[i]，
       A [i * 2 + 1]是A[i]的左儿子并且A[i * 2 + 2]是A[i]的右儿子。

        堆是一种数据结构，它通常有三种方法：push， pop 和 top。
        其中，“push”添加新的元素进入堆，“pop”删除堆中最小/最大元素，“top”返回堆中最小/最大元素。
        把一个无序整数数组变成一个堆数组。如果是最小堆，每个元素A[i]，
        我们将得到A[i * 2 + 1] >= A[i]和A[i * 2 + 2] >= A[i]
        返回其中任何一个。

        输入 : [3,2,1,4,5]
        输出 : [1,2,3,4,5]
        解释 : 返回任何一个合法的堆数组，因此 [1,3,2,4,5] 也是一个正确的结果
     */
    void heapify(vector<int> &a) {
        // write your code here
        if (a.size() == 0 || a.size() == 1) {
            return;
        }
        // heapity_core(a, 0, a.size() - 1);
        heapity_core_1(a);
    }
    void heapity_core(vector<int>& nums, int start, int end) {
        // 直接排序法，效率稍微低点
        if (start >= end) {
            return;
        }
        int lpos = start;
        int rpos = end;
        int minpos = lpos + (rpos - lpos) / 2;
        int pivot = nums[minpos];
        while (lpos <= rpos) {
            if (pivot > nums[lpos]) {
                ++lpos;
                continue;
            }
            if (pivot < nums[rpos]) {
                --rpos;
                continue;
            }
            if (lpos <= rpos) {
                swap(nums[lpos], nums[rpos]);
                ++lpos;
                --rpos;
            }
        }
        heapity_core(nums, start, rpos);
        heapity_core(nums, lpos, end);
    }

    void heapity_core_1(vector<int> &a) {
        // 从最后一个非叶子节点开始，依次向前调整，每一个节点都要走完一次
        for (int i = a.size() - 1; i > 0; i--) {
            bool changed = true;
            while (changed) {
                int j = i;
                changed = false;
                while (j > 0) {
                    int parent = j % 2== 0 ? (j - 2) / 2 : (j - 1) / 2;
                    if (a[j] < a[parent]) {
                        // cout << "swap a["<<parent<<"]=" << a[parent] << " a["<<j<<"]=" << a[j] << endl;
                        swap(a[j], a[parent]);
                        changed = true;
                    }
                    j = parent;
                }
            }
        }
    }
    //  // 快速排序 标准写法
    //     if (start >= end) {
    //         return;
    //     }
    //     int lpos = start;
    //     int rpos = end;
    //     int m = (lpos + rpos) / 2;
    //     int pivot = vec[m];
    //     while (lpos <= rpos) { // 确保lpos>rpos最终
    //         while((lpos <= rpos) && (vec[lpos] < pivot)) {
    //             ++lpos;
    //         }
    //         while((rpos >= lpos) && (vec[rpos] > pivot)) {
    //             --rpos;
    //         }
    //         if (lpos <= rpos) {
    //             std::swap(vec[lpos], vec[rpos]);
    //             ++lpos;
    //             --rpos;
    //         }
    //     }
    //     quick_sort_core_1(vec, start, rpos);
    //     quick_sort_core_1(vec, lpos, end);
};

int main (){
    Solution s;
    auto test = [&s](vector<int> nums) {
        cout << "nums: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
        s.heapify(nums);
        cout << "heapify: ";
        for (auto& i : nums) {
            cout << i << " ";
        }
        cout << endl;
    };

    test({3,2,1,4,5});
    test({3,2,1,2,4,5});
    test({3,2,1,2,2,4,5});
    test({4,2,5,7,1,3,6});
}