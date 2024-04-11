#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

template<typename T>
class AVLTree {
/*
平衡二叉树构建
平衡二叉树（Self-Balancing Binary SearchTree或Height-Balanced Binary Search Tree），
是一种二叉排序树，其中每一个节点的左子树和右子树的高度差至多等于1。


我们可以总结出平衡二叉树的构建过程如下：

首先，按照二叉排序树的规则，逐个将数组中的元素插入到平衡二叉树中。
在插入过程中，每次插入一个节点后，需要查找从插入位置到根节点的路径上的所有节点，检查它们的平衡因子（平衡因子为左子树高度减去右子树高度）是否超出了平衡范围（-1，0，1）。
如果发现某个节点的平衡因子超出了平衡范围，就需要对最小不平衡子树进行调整。最小不平衡子树是指离插入节点最近的不平衡节点和它的子树。
当最小不平衡子树的根节点的平衡因子为正（大于1）时，进行右旋操作。右旋操作是将根节点的左孩子升级为新的根节点，根节点成为新根节点的右孩子，新根节点的右孩子成为原根节点的左孩子。
当最小不平衡子树的根节点的平衡因子为负（小于-1）时，进行左旋操作。左旋操作是将根节点的右孩子升级为新的根节点，根节点成为新根节点的左孩子，新根节点的左孩子成为原根节点的右孩子。
如果最小不平衡子树的根节点与它的子树的平衡因子符号相反，先对最小不平衡子树的根节点的子节点进行旋转操作，使得它们的平衡因子符号相同，然后再对最小不平衡子树的根节点进行相反的旋转操作。
重复以上步骤，直到整棵树都平衡。
通过这样的构建过程，我们可以将一个数组构建成一棵高度平衡的二叉排序树（即平衡二叉树），从而提高查找的效率。平衡二叉树的高度接近log(n)，因此查找的时间复杂度近似于O(log(n))，相比于普通二叉排序树的O(n)，查找效率大大提升。
*/
public:
    struct TreeNode {
        T data;
        std::shared_ptr<TreeNode> left;
        std::shared_ptr<TreeNode> right;
        std::shared_ptr<TreeNode> parent;
        int height;
    };
    std::shared_ptr<TreeNode> root;
    int max_num{0};
public:
    AVLTree() {
        cout << "establish an AVL tree" << endl;
        root = nullptr;
    }

    void clear() {
        max_num = 0;
        if (root == nullptr) {
            return;
        }
        vector<std::shared_ptr<TreeNode>> tmp;
        tmp.push_back(root);
        while (!tmp.empty()) {
            auto curnode = tmp.front();
            tmp.erase(tmp.begin());
            if (curnode->left != nullptr) {
                tmp.push_back(curnode->left);
            }
            if (curnode->right != nullptr) {
                tmp.push_back(curnode->right);
            }
            curnode->left = nullptr;
            curnode->right = nullptr;
            curnode->parent = nullptr;
        }
        root = nullptr;
    }
    void insert(T data) {
        // cout << "--->insert: " << data << endl;
        max_num += 1;
        if (root == nullptr) {
            root = std::make_shared<TreeNode>(TreeNode{data, nullptr, nullptr, nullptr, 1});
            return;
        }
        try_insert(root, data);
        // cout << "check_rotate before root.height=" << root->height << endl;
        // check_rotate(root);
        // cout << "check_rotate after  root.height=" << root->height << endl;
    }
    int try_insert(std::shared_ptr<TreeNode> curnode, T data) {
        // 返回当前节点的depth
        int depth = 0; // 即将插入位置的深度
        if (data < curnode->data) { // 应在左侧
            if (curnode->left == nullptr) {
                curnode->left = std::make_shared<TreeNode>(TreeNode{data, nullptr, nullptr, curnode, 1});
                depth = 1;
            } else {
                depth = try_insert(curnode->left, data);
            }
        } else { // 应在右侧
            if (curnode->right == nullptr) {
                curnode->right = std::make_shared<TreeNode>(TreeNode{data, nullptr, nullptr, curnode, 1});
                depth = 1;
            } else {
                depth = try_insert(curnode->right, data);
            }
        }
        if ((depth + 1) > curnode->height) {
            // 判断是否增加了子树的高度
            curnode->height = depth + 1;
        }
        curnode = check_rotate(curnode);
        return curnode == nullptr ? 0 : curnode->height;
    }
    std::shared_ptr<TreeNode> check_rotate(std::shared_ptr<TreeNode> curnode) {
        if (curnode == nullptr) {
            return curnode;
        }
        int lnode_h = curnode->left != nullptr ? curnode->left->height : 0;
        int rnode_h = curnode->right != nullptr ? curnode->right->height : 0;

        // cout << "check_rotate: " << curnode->data << " h: " << curnode->height
        //      << " lh: " << lnode_h
        //      << " rh: " << rnode_h << endl;

        if (lnode_h - rnode_h > 1) {
            // 左子树高度大于右子树, 右旋
            int llnode_h = curnode->left->left != nullptr ? curnode->left->left->height : 0;
            int lrnode_h = curnode->left->right != nullptr ? curnode->left->right->height : 0;
            if (llnode_h < lrnode_h) {
                // 先左旋再右旋
                L_rotate(curnode->left);
                curnode = R_rotate(curnode);
            } else {
                curnode = R_rotate(curnode);
            }
        } else if (rnode_h - lnode_h > 1 ) {
            // 右子树高度大于左子树, 左旋
            int rlnode_h = curnode->right->left != nullptr ? curnode->right->left->height : 0;
            int rrnode_h = curnode->right->right != nullptr ? curnode->right->right->height : 0;
            if (rlnode_h > rrnode_h) {
                // 先右旋再左旋
                R_rotate(curnode->right);
                curnode = L_rotate(curnode);
            } else {
                curnode = L_rotate(curnode);
            }
        }
        return curnode;
        // // 尝试旋转，共有四种情况，左左，左右，右左，右右
        // if (curnode->left != nullptr && curnode->right != nullptr) {
        //     cout << "check_rotate: " << curnode->data << " h: " << curnode->height
        //          << " lh: " << curnode->left->height
        //          << " rh: " << curnode->right->height << endl;
        //     // 左右子树都存在
        //     // 两边有树，需要更下一层判断
        //     if (curnode->left->height - curnode->right->height > 1) {
        //         // 左子树高度大于右子树, 右旋
        //         auto lnode = curnode->left;
        //         // if ((lnode->right != nullptr) && (lnode->right->left != nullptr)) {
        //         if (lnode->left != nullptr && lnode->right != nullptr
        //             && lnode->left->height < lnode->right->height) {
        //             // 先左旋再右旋
        //             lnode = L_rotate(lnode);
        //             curnode = R_rotate(curnode);
        //         } else {
        //             curnode = R_rotate(curnode);
        //         }
        //     } else if (curnode->right->height - curnode->left->height > 1) {
        //         // 右子树高度大于左子树, 左旋
        //         auto rnode = curnode->right;
        //         // if ((rnode->left != nullptr) && (rnode->left->right != nullptr)) {
        //         if (rnode->left != nullptr && rnode->right != nullptr
        //             && rnode->left->height > rnode->right->height) {
        //             // 先右旋再左旋
        //             rnode = R_rotate(rnode);
        //             curnode = L_rotate(curnode);
        //         } else {
        //             curnode = L_rotate(curnode);
        //         }
        //     }
        // } else if (curnode->left != nullptr && curnode->right == nullptr) {
        //     cout << "--->check_rotate: " << curnode->data << " h: " << curnode->height
        //          << " lh: " << curnode->left->height
        //          << " rh: " << 0 << endl;
        //     // 只有左子树
        //     // 只需要看到三层即可
        //     if (curnode->left->height > 1) {
        //         // 左子树高度大于右子树,
        //         cout<< "aaaaa" << endl;
        //         auto lnode = curnode->left;
        //         // if ((lnode->right != nullptr) && (lnode->right->left != nullptr)) {
        //         if (lnode->right != nullptr) {
        //             // 先左旋再右旋x
        //             lnode = L_rotate(lnode);
        //             curnode = R_rotate(curnode);
        //         } else {
        //             curnode = R_rotate(curnode);
        //         }
        //     }
        // } else if (curnode->left == nullptr && curnode->right != nullptr) {
        //     cout << "check_rotate: " << curnode->data << " h: " << curnode->height
        //          << " lh: " << 0
        //          << " rh: " << curnode->right->height << endl;
        //     // 只有右子树
        //     if (curnode->right->height > 1) {
        //         // 右子树高度大于左子树, 左旋
        //         auto rnode = curnode->right;
        //         // if ((rnode->left != nullptr) && (rnode->left->right != nullptr)) {
        //         if (rnode->left != nullptr) {
        //             // 先右旋再左旋
        //             rnode = R_rotate(rnode);
        //             curnode = L_rotate(curnode);
        //         } else {
        //             curnode = L_rotate(curnode);
        //         }
        //     }
        // }
        // return curnode;
    }

    std::shared_ptr<TreeNode> R_rotate(std::shared_ptr<TreeNode> curnode) {
        if (curnode == nullptr || curnode->left == nullptr) {
            return curnode;
        }
        auto parent = curnode->parent;
        auto lnode = curnode->left;
        curnode->left = lnode->right;
        if (curnode->left != nullptr) {
            curnode->left->parent = curnode;
        }
        int ldepth = curnode->left != nullptr ? curnode->left->height : 0;
        int rdepth = curnode->right != nullptr ? curnode->right->height : 0;
        curnode->height = ldepth > rdepth ? ldepth + 1 : rdepth + 1;

        lnode->right = curnode;
        curnode->parent = lnode;
        ldepth = lnode->left != nullptr ? lnode->left->height : 0;
        rdepth = lnode->right != nullptr ? lnode->right->height : 0;
        lnode->height = ldepth > rdepth ? ldepth + 1 : rdepth + 1;

        if (parent != nullptr) {
            if (parent->left == curnode) {
                parent->left = lnode;
            } else {
                parent->right = lnode;
            }
            lnode->parent = parent;
            // 修改深度
            if (lnode->height + 1 > parent->height) {
                parent->height = lnode->height + 1;
            }
        } else {
            root = lnode; // 更新根节点
            root->parent = nullptr;
        }

        return lnode;
    }
    std::shared_ptr<TreeNode> L_rotate(std::shared_ptr<TreeNode> curnode) {
        if (curnode == nullptr || curnode->right == nullptr) {
            return curnode;
        }
        auto parent = curnode->parent;
        auto rnode = curnode->right;
        curnode->right = rnode->left;
        if (curnode->right != nullptr) {
            curnode->right->parent = curnode;
        }
        int ldepth = curnode->left != nullptr ? curnode->left->height : 0;
        int rdepth = curnode->right != nullptr ? curnode->right->height : 0;
        curnode->height = ldepth > rdepth ? ldepth + 1 : rdepth + 1;

        rnode->left = curnode;
        curnode->parent = rnode;
        ldepth = rnode->left != nullptr ? rnode->left->height : 0;
        rdepth = rnode->right != nullptr ? rnode->right->height : 0;
        rnode->height = ldepth > rdepth ? ldepth + 1 : rdepth + 1;

        if (parent != nullptr) {
            if (parent->left == curnode) {
                parent->left = rnode;
            } else {
                parent->right = rnode;
            }
            rnode->parent = parent;
            // 修改深度
            if (rnode->height + 1 > parent->height) {
                parent->height = rnode->height + 1;
            }
        } else {
            root = rnode; // 更新根节点
            root->parent = nullptr;
        }
        return rnode;
    }

    vector<std::pair<T,int>> get_BFS() {
        /*
        * 可以参看./lint_69_levelOrder.cc:98,
        * 有三种
        * 第一种，使用单队列，记住每一层的节点数目，每次处理完一层，再处理下一层
        * 第二种，使用双队列，一个队列读，一个队列存，交替使用
        * 第三种，使用dummy node哑结点，用哑结点来分割每一层
        */
        if (root == nullptr) {
            return {};
        }
        vector<std::shared_ptr<TreeNode>> tmp;
        vector<std::pair<T,int>> output;
        int curr_depth = root->height;
        int max_cnt = max_num;
        tmp.push_back(root);
        while (!tmp.empty()) {
            auto curnode = tmp.front();
            if (max_cnt == 0) {
                break;
            } else if (curnode->data != -1) {
                max_cnt--;
            }
            output.push_back(std::pair<T,int>(curnode->data,curnode->height));
            tmp.erase(tmp.begin());
            if (curnode->left != nullptr) {
                tmp.push_back(curnode->left);
            } else {
                tmp.push_back(std::make_shared<TreeNode>(TreeNode{-1, nullptr, nullptr, nullptr, curnode->height - 1}));
            }
            if (curnode->right != nullptr) {
                tmp.push_back(curnode->right);
            } else {
                tmp.push_back(std::make_shared<TreeNode>(TreeNode{-1, nullptr, nullptr, nullptr, curnode->height - 1}));
            }
        }
        return output;
    }

    vector<vector<std::pair<T,int>>> get_BFS_1() {
        if (root == nullptr) {
            return {};
        }
        vector<std::pair<T,int>> tmp;
        vector<vector<std::pair<T,int>>> output;
        queue<std::shared_ptr<TreeNode>> q;
        q.push(root);
        bool found_new_node = false;
        while (!q.empty()) {
            int size = q.size();
            tmp.clear();
            found_new_node = false;
            for (int i = 0; i < size; ++i) {
                auto curnode = q.front();
                q.pop();
                if (curnode->left == nullptr) {
                    q.push(std::make_shared<TreeNode>(TreeNode{-1, nullptr, nullptr, nullptr, curnode->height - 1}));
                } else {
                    q.push(curnode->left);
                    found_new_node = true;
                }
                if (curnode->right == nullptr) {
                    q.push(std::make_shared<TreeNode>(TreeNode{-1, nullptr, nullptr, nullptr, curnode->height - 1}));
                } else {
                    q.push(curnode->right);
                    found_new_node = true;
                }
                if (curnode != nullptr) {
                    tmp.push_back(std::pair<T,int>(curnode->data, curnode->height));
                } else {
                    tmp.push_back(std::pair<T,int>(-1,0));
                }
            }
            output.push_back(tmp);
            if (!found_new_node) {
                break;
            }
        }
        return output;
    }

    std::unordered_map<T, int> get_all_distance_BFS() {
        std::shared_ptr<TreeNode> curnode = root;
        std::unordered_map<T, int> distance;
        std::queue<std::shared_ptr<TreeNode>> q;
        q.push(curnode);
        distance[curnode->data] = 0;
        // 单队列方法
        while (q.size()) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                auto curnode = q.front();
                q.pop();
                auto curnode_dis = distance.find(curnode->data);
                if (curnode_dis == distance.end()) {
                    continue;
                }
                if (curnode->left != nullptr && (distance.find(curnode->left->data) == distance.end())) {
                    q.push(curnode->left);
                    distance[curnode->left->data] = curnode_dis->second + 1;
                }
                if (curnode->right != nullptr && (distance.find(curnode->right->data) == distance.end())) {
                    q.push(curnode->right);
                    distance[curnode->right->data] = curnode_dis->second + 1;
                }
            }
        }
        return distance;
    }

    // 中序遍历，递归
    void get_nodes_dfs_lmr(std::vector<pair<T,T>>& vec, std::shared_ptr<TreeNode> curnode) {
        if (curnode == nullptr) {
            return;
        }
        get_nodes_dfs_lmr(vec, curnode.get()->left);
        vec.push_back(std::pair<T,T >(curnode.get()->data, curnode.get()->height));
        get_nodes_dfs_lmr(vec, curnode.get()->right);
    }

    // 中序遍历，迭代
    void get_nodes_dfs_lmr_traverse(std::vector<pair<T,T>>& vec, std::shared_ptr<TreeNode> _root) {
        // 看lint 86，有更简单的实现
        if (_root == nullptr) {
            return;
        }
        std::stack<std::shared_ptr<TreeNode>> s;
        s.push(_root);
        std::shared_ptr<TreeNode> curnode = s.top();
        while (true) {
            if (curnode != nullptr) {
                if (curnode->left != nullptr) {
                    s.push(curnode->left);
                    curnode = s.top();
                    continue;
                }
            }
            if (s.empty()) {
                break;
            }
            curnode = s.top();
            s.pop();
            if (curnode != nullptr) {
                vec.push_back(std::pair<T,T>(curnode->data, curnode->height));
                if (curnode->right != nullptr) {
                    s.push(curnode->right);
                    curnode = s.top();
                    continue;
                }
                curnode = nullptr;
            }
        }
    }

    // morris算法，本身也是中序遍历，左中右，时间复杂度O(N),空间复杂度O(1)
    /*
    莫里斯算法实现思想:
    当前节点cur, 一开始的cur来到整棵树的头结点位置:
    1. cur无左树, cur = cur.right
    2. cur有左树, 找到左树最右节点mostright
        1.1. mostright的右指针指向null的时候, mostright.right = cur, cur = cur.left
        1.2. mostright的右指针指向cur的时候, mostright.right = null, cur = cur.right
    连接过程:先连接后左移
    复原过程：先右移后斩断，若斩断位置到位，立刻执行斩断，如果位置不到位，通过while循环到达指定位置
    */
    void get_nodes_dfs_morris(std::vector<pair<T,T>>& vec, std::shared_ptr<TreeNode> _root) {

    }

    // 后序遍历，递归
    void get_nodes_dfs_lrm(std::vector<pair<T,T>>& vec, std::shared_ptr<TreeNode> curnode) {
        if (curnode == nullptr) {
            return;
        }
        get_nodes_dfs_lrm(vec, curnode.get()->left);
        get_nodes_dfs_lrm(vec, curnode.get()->right);
        vec.push_back(std::pair<T,T >(curnode.get()->data, curnode.get()->height));
    }

    /*
    第一种思路：对于任一结点P，将其入栈，然后沿其左子树一直往下搜索，直到搜索到没有左孩子的结点，
    此时该结点出现在栈顶，但是此时不能将其出栈并访问， 因此其右孩子还为被访问。所以接下来按照相同的
    规则对其右子树进行相同的处理，当访问完其右孩子时，该结点又出现在栈顶，此时可以将其出栈并访问。
    这样就 保证了正确的访问顺序。可以看出，在这个过程中，每个结点都两次出现在栈顶，只有在第二次出现
    在栈顶时，才能访问它。因此需要多设置一个变量标识该结点是 否是第一次出现在栈顶。
    */
   // 后序遍历，迭代
    void get_nodes_dfs_lrm_traverse(std::vector<pair<T,T>>& vec, std::shared_ptr<TreeNode> curnode) {
        // 最难，标记法实现
        if (curnode == nullptr) {
            return;
        }
        std::stack<std::shared_ptr<TreeNode>> s;
        std::unordered_map<std::shared_ptr<TreeNode>, bool> visited;
        while (true) {
            if (curnode != nullptr) {
                auto it = visited.find(curnode);
                if (it == visited.end()) {
                    visited[curnode] = false;
                }
                s.push(curnode);
                curnode = curnode->left;
                continue;
            }
            if (s.empty()) {
                break;
            }
            curnode = s.top();
            auto it = visited.find(curnode);
            if (it != visited.end() && it->second == false) {
                it->second = true;
                curnode = curnode->right;
                continue;
            }
            vec.push_back(std::pair<T,T >(curnode->data, curnode->height));
            s.pop();
            curnode = nullptr;
        }
    }

    /*
   第二种思路：要保证根结点在左孩子和右孩子访问之后才能访问，因此对于任一结点P，先将其入栈。
   如果P不存在左孩子和右孩子，则可以直接访问它；或者P存 在左孩子或者右孩子，但是其左孩子和
   右孩子都已被访问过了，则同样可以直接访问该结点。若非上述两种情况，则将P的右孩子和左孩子
   依次入栈，这样就保证了 每次取栈顶元素的时候，左孩子在右孩子前面被访问，左孩子和右孩子都
   在根结点前面被访问。
    */
    void get_nodes_dfs_lrm_traverse_1(std::vector<pair<T,T>>& vec, std::shared_ptr<TreeNode> curnode) {
        // 最难，标记法标记已经访问的，不用set，记住前一次退栈的节点
        if (curnode == nullptr) {
            return;
        }
        std::stack<std::shared_ptr<TreeNode>> s;
        std::shared_ptr<TreeNode> prenode = nullptr; // 记录前一次退栈的节点
        while (true) {
            if (curnode != nullptr) {
                s.push(curnode);
                curnode = curnode->left;
                continue;
            }
            if (s.empty()) {
                break;
            }
            curnode = s.top();
            if (curnode->right != nullptr && curnode->right != prenode) {
                curnode = curnode->right;
                continue;
            }
            vec.push_back(std::pair<T,T >(curnode->data, curnode->height));
            s.pop();
            prenode = curnode;
            curnode = nullptr;
        }
    }
    // 前序遍历，递归
    void get_nodes_dfs_mlr(std::vector<pair<T,T>>& vec, std::shared_ptr<TreeNode> curnode) {
        if (curnode == nullptr) {
            return;
        }
        vec.push_back(std::pair<T,T >(curnode.get()->data, curnode.get()->height));
        get_nodes_dfs_mlr(vec, curnode.get()->left);
        get_nodes_dfs_mlr(vec, curnode.get()->right);
    }

    // 前序遍历，迭代
    void get_nodes_dfs_mlr_traverse(std::vector<pair<T,T>>& vec, std::shared_ptr<TreeNode> curnode) {
        if (curnode == nullptr) {
            return;
        }
        std::stack<std::shared_ptr<TreeNode>> s;
        while (true) {
            if (curnode != nullptr) {
                vec.push_back(std::pair<T,T>(curnode->data, curnode->height));
                if (curnode->left != nullptr) {
                    s.push(curnode);
                    curnode = curnode->left;
                    continue;
                }
                curnode = curnode->right;
                continue;
            }
            if (s.empty()) {
                break;
            }
            curnode = s.top()->right;
            s.pop();
        }
    }

    std::vector<std::string> get_allpath(){
        if (root == nullptr) {
            return {};
        }
        std::vector<std::string> output;
        get_allpath(root, output);
        return output;
    }
    string get_allpath(std::shared_ptr<TreeNode> _root, std::vector<std::string>& output) {
        if (_root == nullptr) {
            return "";
        }

        string lnode_name = get_allpath(_root->left, output);
        if (lnode_name != "") {
            output.push_back(to_string(_root->data) + "->" + lnode_name);
        }
        string rnode_name = get_allpath(_root->right, output);
        if (rnode_name != "") {
            output.push_back(to_string(_root->data) + "->" + rnode_name);
        }
        return to_string(_root->data);
    }

    std::unordered_map<T,T> get_all_distance_DFS(){
        if (root == nullptr) {
            return {};
        }
        std::vector<std::string> output;
        std::vector<T> subpath;
        std::unordered_map<T,T> distance; //  记录每个节点到根节点的距离，同时避免重复计算节点
        get_all_subpath(root, subpath, output, distance);
        return distance;
    }

    std::vector<std::string> get_all_subpath(){
        if (root == nullptr) {
            return {};
        }
        std::vector<std::string> output;
        std::vector<T> subpath;
        std::unordered_map<T,T> distance; //  记录每个节点到根节点的距离，同时避免重复计算节点
        get_all_subpath(root, subpath, output, distance);
        return output;
    }

    void get_all_subpath(std::shared_ptr<TreeNode> _root, std::vector<T> &subpath, std::vector<std::string>& output, std::unordered_map<T,T>& distance) {
        if (_root == nullptr) {
            return;
        }

        auto it = distance.find(_root->data);
        if (it != distance.end()) {
            return; // 防止环
        }

        subpath.push_back(_root->data);
        distance[_root->data] = subpath.size() - 1;
        if (_root->left == nullptr && _root->right == nullptr) {
            string path = "";
            for (int i = 0; i < subpath.size(); ++i) {
                path += to_string(subpath[i]);
                if (i != subpath.size() - 1) {
                    path += "->";
                }
            }
            output.push_back(path);
            return;
        }

        if (_root->left != nullptr) {
            get_all_subpath(_root->left, subpath, output, distance);
            subpath.pop_back();
        }

        if (_root->right != nullptr) {
            get_all_subpath(_root->right, subpath, output, distance);
            subpath.pop_back();
        }
        return;
    }
};
