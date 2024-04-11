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
 * Definition of TreeNode:
 * class TreeNode {
 * public:
 *     int val;
 *     TreeNode *left, *right;
 *     TreeNode(int val) {
 *         this->val = val;
 *         this->left = this->right = NULL;
 *     }
 * }
 */
class TreeNode {
public:
    int val;
    TreeNode *left, *right;
    TreeNode(int val) {
        this->val = val;
        this->left = this->right = NULL;
    }
};

class Solution {
public:
    /**
     * @param root: given BST
     * @param minimum: the lower limit
     * @param maximum: the upper limit
     * @return: the root of the new tree 
     * 
     * 给定一个有根的二叉搜索树和两个数字min和max，
     * 修整这个树使得所有的数字在这个新的树种都是在min和max之间（包括min和max）。
     * 然后这个所得的树仍然是合法的二叉搜索树。
     * 
     * 输入：
        {8,3,10,1,6,#,14,#,#,4,7,13}
        5
        13
        输出： {8, 6, 10, #, 7, #, 13}
        说明：树的图片在题面描述里已经给出

       输入：
        {1,0,2}
        1
        2
        输出： {1,#,2}
        说明:
        输入是
         1
        / \
        0  2
        输出是
        1
         \
          2

        可以中序遍历 lmr，如果m < min, 则删除m，如果m > max, 则删除m
     */
    TreeNode* trimBST(TreeNode *root, int minimum, int maximum) {
        // write your code here
        return trimBST_0(root, minimum, maximum);
    }

    TreeNode* trimBST_0(TreeNode *root, int minimum, int maximum) {
        if (root == nullptr) {
            return nullptr;
        }
        vector<int> nums;
        lmr(root, nums);
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] < minimum || nums[i] > maximum) {
                root = removeNode(root, nums[i]);
            }
        }
        return root;
    }

    void lmr(TreeNode* root, vector<int> &nums) {
        if (root == nullptr) {
            return;
        }
        lmr(root->left, nums);
        nums.push_back(root->val);
        lmr(root->right, nums);
    }

    TreeNode* removeNode(TreeNode* root, int val) {
        TreeNode* dummy_node = new TreeNode(0);
        dummy_node->left = root;
        TreeNode* parent = find_node(dummy_node, root, val);
        TreeNode* node = nullptr;
        if (parent->left != nullptr && parent->left->val == val) {
            node = parent->left;
        } else if (parent->right != nullptr && parent->right->val == val) {
            node = parent->right;
        } else {
            return dummy_node->left;
        }
        deleteNode(parent, node);
        return dummy_node->left;
    }

    TreeNode* find_node(TreeNode* parent, TreeNode* &node, int val) {
        // return parent
        if (node == nullptr) {
            return parent;
        }
        if (node->val == val) {
            return parent;
        }
        if (val < node->val) {
            return find_node(node, node->left, val);
        } else {
            return find_node(node, node->right, val);
        }
    }

    void deleteNode(TreeNode* parent, TreeNode* node) {
        if (node->right == nullptr) {
            if (parent->left == node) {
                parent->left = node->left;
            } else {
                parent->right = node->left;
            }
            delete node;
            return;
        }
        TreeNode* tmp = node->right;
        TreeNode* father = node;
        while (tmp->left != nullptr) {
            father = tmp;
            tmp = tmp->left;
        } // find most left node
        if (father->left == tmp) {
            father->left = tmp->right; // tmp->left is nullptr, tmp->right is unknown
        } else {
            father->right = tmp->right;
        }
        if (parent->left == node) {
            parent->left = tmp;
        } else {
            parent->right = tmp;
        }
        tmp->left = node->left;
        tmp->right = node->right;
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