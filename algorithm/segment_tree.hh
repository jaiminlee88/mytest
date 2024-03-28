#include <vector>


template <typename T>
class segment_tree {
public:
    struct node{
        T l = -1;
        T r = -1;
        T val = 0;
        bool cv = false; //线段树的域
    };
    std::vector<node> tree;
    segment_tree(T size) {
        tree.resize(size << 2);
        tree[0].l = 1;
    }
    /*
    * parameter rt: 当前节点
    * parameter l: 当前节点的左边界
    * parameter r: 当前节点的右边界
    * parameter val: 当前节点的值
    * 递归建立线段树
    */
    void build(T rt, T l, T r) { // 快速建立线段树
        tree[rt].l = l;
        tree[rt].r = r;
        if (l == r) {
            return;
        }
        T mid = (l + r) >> 1;
        build(rt << 1, l, mid);
        build(rt << 1 | 1, mid + 1, r);
    }

    /*
    * parameter rt: 当前节点
    * parameter l: 区间左边界
    * parameter r: 区间右边界
    * parameter val: 区间增加的值
    * 区间更新
    */
    void insert(T rt, T l, T r, T val) {
        if (r < tree[rt].l || l > tree[rt].r) {
            return;
        }
        if (r >= tree[rt].r || l <= tree[rt].l) {
            tree[rt].val += val;
            return;
        }
        insert(rt << 1, l, r, val);
        insert(rt << 1 | 1, l, r, val);
    }
    // void insert(T rt, T l, T r, T val) {
    //     if (tree[rt].l >= l && tree[rt].r <= r) {
    //         tree[rt].val += val;
    //         tree[rt].cv = true;
    //         return;
    //     }
    //     push_down(rt);
    //     T mid = (tree[rt].l + tree[rt].r) >> 1;
    //     if (l <= mid) {
    //         insert(rt << 1, l, r, val);
    //     }
    //     if (r > mid) {
    //         insert(rt << 1 | 1, l, r, val);
    //     }
    //     push_up(rt);
    // }

    // void push_up(T rt) { // 
    //     tree[rt].sum = tree[rt << 1].sum + tree[rt << 1 | 1].sum;
    // }
    // void push_down(T rt) {
    //     if (tree[rt].cv) {
    //         tree[rt << 1].val += tree[rt].val;
    //         tree[rt << 1].cv = true;
    //         tree[rt << 1 | 1].val += tree[rt].val;
    //         tree[rt << 1 | 1].cv = true;
    //         tree[rt].cv = false;
    //     }
    // }
};