#include <vector>
#include <limits.h>
#include <iostream>
#include <algorithm>

using namespace std;
// template <typename T>
// class segment_tree {
// public:
//     struct node{
//         T l = -1;
//         T r = -1;
//         T val = 0;
//         bool cv = false; //线段树的域
//     };
//     std::vector<node> tree;
//     segment_tree(T size) {
//         tree.resize(size << 2);
//         tree[0].l = 1;
//     }
//     /*
//     * parameter rt: 当前节点
//     * parameter l: 当前节点的左边界
//     * parameter r: 当前节点的右边界
//     * parameter val: 当前节点的值
//     * 递归建立线段树
//     */
//     void build(T rt, T l, T r) { // 快速建立线段树
//         tree[rt].l = l;
//         tree[rt].r = r;
//         if (l == r) {
//             return;
//         }
//         T mid = (l + r) >> 1;
//         build(rt << 1, l, mid);
//         build(rt << 1 | 1, mid + 1, r);
//     }

//     /*
//     * parameter rt: 当前节点
//     * parameter l: 区间左边界
//     * parameter r: 区间右边界
//     * parameter val: 区间增加的值
//     * 区间更新
//     */
//     void insert(T rt, T l, T r, T val) {
//         if (r < tree[rt].l || l > tree[rt].r) {
//             return;
//         }
//         if (r >= tree[rt].r || l <= tree[rt].l) {
//             tree[rt].val += val;
//             return;
//         }
//         insert(rt << 1, l, r, val);
//         insert(rt << 1 | 1, l, r, val);
//     }
//     // void insert(T rt, T l, T r, T val) {
//     //     if (tree[rt].l >= l && tree[rt].r <= r) {
//     //         tree[rt].val += val;
//     //         tree[rt].cv = true;
//     //         return;
//     //     }
//     //     push_down(rt);
//     //     T mid = (tree[rt].l + tree[rt].r) >> 1;
//     //     if (l <= mid) {
//     //         insert(rt << 1, l, r, val);
//     //     }
//     //     if (r > mid) {
//     //         insert(rt << 1 | 1, l, r, val);
//     //     }
//     //     push_up(rt);
//     // }

//     // void push_up(T rt) { // 
//     //     tree[rt].sum = tree[rt << 1].sum + tree[rt << 1 | 1].sum;
//     // }
//     // void push_down(T rt) {
//     //     if (tree[rt].cv) {
//     //         tree[rt << 1].val += tree[rt].val;
//     //         tree[rt << 1].cv = true;
//     //         tree[rt << 1 | 1].val += tree[rt].val;
//     //         tree[rt << 1 | 1].cv = true;
//     //         tree[rt].cv = false;
//     //     }
//     // }
// };


// 用来解决区间统计问题，比如来了一堆整数，想知道区间内的最大值，最小值，或者求和
// 区间到底是什么，就是一个左右边界，比如[1, 3]，[2, 4]，[3, 5]，[4, 6]，[5, 7]
// 线段树的区间是什么，就是一个左右边界，比如[1, 7]，[1, 3]，[4, 7]，[2, 4]，[3, 5]，[4, 6]，[5, 7] 
// 如果是整数，那么线段树的区间是[1, 2, 3, 4, 5, 6, 7]
// 如果是连续的区间，怎么划分区间，来区分那些边界点
class segment_tree_sum_int {
public:
    // 也可以用functional来实现，定制函数决定val更新规则
    struct node {
        int l = -1;
        int r = -1;
        int val = 0; // sum max or min...
        bool cv = false;
    };
    std::vector<node> tree;
public:
    segment_tree_sum_int(int down_limit, int up_limit) {
        tree.resize((up_limit - down_limit + 1) << 2);
        build(0, down_limit, up_limit);
    }
    void build(int pos, int l, int r) {
        if (l > r) {
            return;
        }
        // cout << pos << " " << l << " " << r << endl;
        tree[pos].l = l;
        tree[pos].r = r;
        if (l == r) {
            tree[pos].val = 0; // default 0
            return;
        }
        int mid = (l + r) >> 1; // 除以2
        build(pos << 1 | 1, l, mid); // 2 * pos + 1
        build((pos << 1) + 2, mid + 1, r); // 2 * pos + 2
        tree[pos].val = tree[pos << 1].val + tree[pos << 1 | 1].val;
    }

     // 确保只修改叶节点
    void insert(int num) {
        insert(0, num);
    }
    void insert(int pos, int num) {
        if (pos < 0 || pos > tree.size()) {
            return;
        }
        if (num < tree[pos].l || num > tree[pos].r) {
            return;
        }
        tree[pos].val += num;
        if (tree[pos].l == tree[pos].r) {
            return;
        }
        int mid = (tree[pos].l + tree[pos].r) >> 1;
        if (num <= mid) {
            insert(pos << 1 | 1, num);
        } else {
            insert((pos << 1) + 2, num);
        }
    }

    int query(int pos, int l, int r) {
        if (r < tree[pos].l || l > tree[pos].r) {
            return 0;
        }
        if (l <= tree[pos].l && tree[pos].r <= r) {
            return tree[pos].val;
        }
        //  [l,................. , r]
        //  [l.....mid] [mid+1.....r]
        //     [s,......r]
        int mid = (tree[pos].l + tree[pos].r) >> 1;
        // cout << mid << endl;
        int sum = 0;
        if (l <= mid) {
            sum += query(pos << 1 | 1, l, r);
        }
        if (r > mid) {
            sum += query((pos << 1) + 2, l, r);
        }
        return sum;
    }
};

// 按数组下标区间来求和，l和r是指数组下标，不是数组内的值的区间
// 可以对任何区间的所有元素加减
// 可以区间统计
// [1,2,4,5,6]
// [1,2], [2,4], [4,5], [5,6]
// 这个只能处理子区间问题
// 如果想处理任意区间，可能就要根据区间的值来建立线段树
class segment_tree_inverval {
public:
    // 也可以用functional来实现，定制函数决定val更新规则
    struct node {
        int l = -1;
        int r = -1;
        int val = 0; // sum max or min...
        bool cv = false;
    };
    std::vector<node> _tree;
    vector<int> _nums;
public:
    segment_tree_inverval(vector<int> nums) {
        _tree.resize(nums.size() << 2); // 从1开始，不用0了
        _nums = nums;
        sort(_nums.begin(), _nums.end());
        build(1, 0, nums.size() - 1);
    }
    void build(int pos, int l_idx, int r_idx) {
        if (pos > _tree.size()
            || r_idx - l_idx < 1
            || l_idx >= r_idx
            || l_idx < 0 || r_idx > _nums.size() - 1
            || _nums[l_idx] >= _nums[r_idx]) {
            return;
        }
        _tree[pos].l = _nums[l_idx];
        _tree[pos].r = _nums[r_idx];
        if (r_idx - l_idx == 1) {
            return;
        }
        int m_idx = (l_idx + r_idx) >> 1; // 除以2
        build(pos << 1 , l_idx, m_idx); // 2 * pos
        build(pos << 1 | 1, m_idx, r_idx); // 2 * pos + 1
    }

    void change(int l, int r, int val) {
        if (l > r) {
            return;
        }
        change(1, l, r, val);
    }
    // 确保只修改叶节点
    // 想给某个区间，可能覆盖多个叶节点，叶节点也需要加
    void change(int pos, int l, int r, int val) {
        // cout << pos << " " << l << " " << r << " " << val << endl;
        if (pos < 0 || pos >= _tree.size()) {
            return;
        }
        if (r <= _tree[pos].l || l >= _tree[pos].r) {
            return;
        }

        if (l <= _tree[pos].l && r >= _tree[pos].r) {
            _tree[pos].val += val;
        }
        change(pos << 1, l, r, val);
        change(pos << 1 | 1, l, r, val);
    }

    int query(int l, int r) {
        return query(1, l, r);
    }
    int query(int pos, int l, int r) {
        if (pos < 0 || pos >= _tree.size()) {
            return 0;
        }
        if (_tree[pos].l == l && _tree[pos].r == r) {
            return _tree[pos].val;
        }
        if (r <= _tree[pos << 1].r) {
            return query(pos << 1, l, r);
        }
        if (l >= _tree[pos << 1 | 1].l) {
            return query(pos << 1 | 1, l, r);
        }
        return 0;
    }

};