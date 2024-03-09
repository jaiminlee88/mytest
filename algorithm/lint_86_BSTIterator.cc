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
 * Example of iterate a tree:
 * BSTIterator iterator = BSTIterator(root);
 * while (iterator.hasNext()) {
 *    TreeNode * node = iterator.next();
 *    do something for node
 */


class BSTIterator {
public:
    /*
    * @param root: The root of binary tree.
    */
    TreeNode * curnode{nullptr};
    std::stack<TreeNode*> s;
    BSTIterator(TreeNode * root) {
        // do intialization if necessary
        curnode = root;
    }

    /*
     * @return: True if there has next node, or false
     */
    bool hasNext() {
        // write your code here
        if (curnode == nullptr && s.empty()) {
            return false;
        }
        return true;
    }

    /*
     * @return: return next node
     */
    TreeNode * next() {
        // write your code here
        while (true) {
            if (curnode != nullptr) {
                s.push(curnode);
                curnode = curnode->left;
                continue;
            }
            if (s.empty()) {
                return curnode;
            }
            curnode = s.top();
            s.pop();
            TreeNode* ret = curnode;
            if (curnode->right != nullptr) {
                curnode = curnode->right;
            } else {
                curnode = nullptr;
            }
            return ret;
        }
    }
};