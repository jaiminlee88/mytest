#include <vector>
#include <iostream>
#include <queue>

using namespace std;

// template <typename T>
// class segment_Tree {
// public:
//     struct node{
//         T l = -1;
//         T r = -1;
//         T val = 0;
//         bool cv = false; //线段树的域
//     };
//     std::vector<node> tree;
//     segment_Tree(T size) {
//         tree.resize(size << 2); //存储线段树的数据，注意要开四倍区间，防止越界，空间浪费对于线段树是不可避免的。
//     }
//     inline unsigned int ls(int i) { return i << 1; }	//访问左孩子： 2*i
//     inline unsigned int rs(int i) { return i << 1 | 1; } //访问右孩子：2*i+1
//     void push_up(int i) {
//         tree[i].val = min(tree[ls(i)].val, tree[rs(i)].val);	//给子树的根节点值等于孩子节点的最小值
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
//         push_up(rt);
//     }

//     /*
//     * parameter rt: 当前节点
//     * parameter l: 区间左边界
//     * parameter r: 区间右边界
//     * parameter val: 区间增加的值
//     * 区间更新
//     */
//     void insert(T rt, T l, T r, T val) {
//         if( r < tree[rt].l || l > tree[rt].r ) {
//             return ;
//         }
//         if( l <= tree[rt].l && r >= tree[rt].r )
//         {
//             tree[rt].cv = 1;
//             return ;
//         }
//         insert( 2 * rt , l , r , val);
//         insert( 2 * rt + 1 , l , r , val);
//     }
//     void print() {
//         cout << "==========================" << endl;
//         std::queue<int> q;
//         q.push(1);
//         while (true) {
//             int size = q.size();
//             if (size == 0) {
//                 break;
//             }
//             bool found = false;
//             for (int i = 0; i < size; ++i) {
//                 int front = q.front();
//                 q.pop();
//                 std::cout << "[" << tree[front].l << ", " << tree[front].r << ", " << tree[front].val << "] ";
//                 if ((tree[front].l != -1 || tree[front].r != -1) && (tree[front].l != tree[front].r) ) {
//                     found = true;
//                 }
//                 q.push(front << 1);
//                 q.push(front << 1 | 1);
//             }
//             std::cout << std::endl;
//             if (!found) {
//                 break;
//             }
//         }
//         cout << "==========================" << endl;
//     }
// };


template<typename T>
class SegmentTree {
private:
    struct node{
        T left = -1;
        T right = -1;
        T val = 0;
        bool cv = false; //线段树的域
    };
    vector<struct node> tree;
    int n;
    T max_val;
    T min_val;

    void build(int node, T left, T right) {
        tree[node].left = left;
        tree[node].right = right;
        tree[node].val = 0; // 初始值为0，可以根据实际情况调整
        if (left == right) {
            return;
        }
        T mid = (left + right) / 2;
        build(node * 2, left, mid);
        build(node * 2 + 1, mid + 1, right);
    }

    void update(int node, T left, T right, T l, T r, T val) {
        if (r < left || right < l)
            return;
        if (l <= left && right <= r) {
            tree[node].val += val;
            return;
        }
        T mid = (left + right) / 2;
        update(node * 2, left, mid, l, r, val);
        update(node * 2 + 1, mid + 1, right, l, r, val);
    }

    T query(int node, T left, T right, T l, T r) {
        if (r < left || right < l)
            return 0; // 修改为支持自定义类型的初始值
        if (l <= left && right <= r)
            return tree[node].val;
        T mid = (left + right) / 2;
        return query(node * 2, left, mid, l, r) + query(node * 2 + 1, mid + 1, right, l, r); // 修改为支持自定义类型的加法
    }


public:
    SegmentTree(T min_val, T max_val) : min_val(min_val), max_val(max_val) {
        n = max_val - min_val + 1;
        tree.resize(4 * n);
        build(1, min_val, max_val);
    }

    void insert(T l, T r, T val) {
        update(1, min_val, max_val, l, r, val);
    }

    T query(T l, T r) {
        return query(1, min_val, max_val, l, r);
    }
    void print() {
        cout << "==========================" << endl;
        std::queue<int> q;
        q.push(1);
        while (true) {
            int size = q.size();
            if (size == 0) {
                break;
            }
            bool found = false;
            for (int i = 0; i < size; ++i) {
                int front = q.front();
                q.pop();
                std::cout << "[" << tree[front].left << ", " << tree[front].right << ", " << tree[front].val << "] ";
                // if ((tree[front].left != -1 || tree[front].right != -1) && (tree[front].left != tree[front].right) ) {
                //     found = true;
                // }
                q.push(front << 1);
                q.push(front << 1 | 1);
            }
            std::cout << std::endl;
            if (!found) {
                break;
            }
        }
        cout << "==========================" << endl;
    }
};

int main() {
    SegmentTree<int> st(1, 10); // 最小值1，最大值100
    st.insert(1, 5, 10); // 区间 [1, 5] 加上10
    st.insert(6, 10, 5); // 区间 [6, 10] 加上5

    // 查询区间 [1, 10] 的和
    int sum = st.query(1, 10);
    printf("Sum of [1, 10] is %d\n", sum);
    st.print();
    return 0;
}