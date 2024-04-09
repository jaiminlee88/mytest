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
#include <sstream>
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
     * This method will be invoked first, you should design your own algorithm 
     * to serialize a binary tree which denote by a root node to a string which
     * can be easily deserialized by your own "deserialize" method later.
     * 
     * 设计一个算法，并编写代码来序列化和反序列化二叉树。
     * 将树写入一个文件被称为“序列化”，读取文件后重建同样的二叉树被称为“反序列化”。
        如何反序列化或序列化二叉树是没有限制的，你只需要确保可以将二叉树序列化为一个字符串，
        并且可以将字符串反序列化为原来的树结构。

        对二进制树进行反序列化或序列化的方式没有限制，LintCode 将您的 serialize 输出作为 deserialize 的输入，它不会检查序列化的结果。
     
     tree = {3,9,20,#,#,15,7}
     {3,9,20,#,#,15,7}
    
     tree = {1,2,3}
     {1,2,3}

        i
      /    \
     2i+1 2i+2
     用BFS
     */
    string serialize(TreeNode * root) {
        // write your code here
        if (root == nullptr) {
            return {};
        }
        queue<TreeNode*> q;
        q.push(root);
        int size = q.size();
        vector<string> ret;
        while (true) {
            size = q.size();
            bool all_null = true;
            for (int i = 0; i < size; ++i) {
                auto node = q.front();
                q.pop();
                if (node == nullptr) {
                    q.push(nullptr); // 特别注意，空节点的左子树也是空，需要占位符
                    q.push(nullptr); // 特别注意占位符
                    ret.push_back("#");
                } else {
                    all_null = false;
                    q.push(node->left);
                    q.push(node->right);
                    ret.push_back(to_string(node->val));
                }
            }
            if (all_null) {
                break;
            }
        }
        while (true) {
            if (ret.back() == "#") {
                ret.pop_back();
            } else {
                break;
            }
        }
        string str{};
        for (int i = 0; i < ret.size(); ++i) {
            str += ret[i];
            if (i != ret.size() - 1) {
                str += ",";
            }
        }
        return str;
    }

    /**
     * This method will be invoked second, the argument data is what exactly
     * you serialized at method "serialize", that means the data is not given by
     * system, it's given by your own serialize method. So the format of data is
     * designed by yourself, and deserialize it here as you serialize it in 
     * "serialize" method.
     */
    TreeNode * deserialize(string &data) {
        // write your code here
        if (data.size() < 1) {
            return nullptr;
        }
        std::stringstream ss(data);
        vector<TreeNode*> nodes;
        string tmp;
        while (std::getline(ss, tmp, ',')) {
            if (tmp == "#") {
                nodes.push_back(nullptr);
            } else {
                nodes.push_back(new TreeNode(std::stoi(tmp)));
            }
        }
        cout << "data: " << data << endl;
        int left = 0;
        int right = 0;
        for (int i = 0; i < nodes.size(); ++i) {
            if (nodes[i] == nullptr) {
                continue;
            }
            left = 2 * i + 1;
            right = 2 * i + 2;
            cout << "curr:" << nodes[i]->val << " ";
            if (left < nodes.size()) {
                cout << " " << (nodes[left] == nullptr ? "#" : to_string(nodes[left]->val)) << " ";
                nodes[i]->left = nodes[left];
            }
            if (right < nodes.size()) {
                cout << " " << (nodes[right] == nullptr ? "#" : to_string(nodes[right]->val)) << " ";
                nodes[i]->right = nodes[right];
            }
            cout << endl;
        }
        return nodes[0];
    }

    // 占位更少
    string serialize_1(TreeNode * root) {
        // write your code here
        if (root == nullptr) {
            return {};
        }
        deque<TreeNode*> q;
        q.push_back(root);
        for (int i = 0; i < q.size(); ++i) {
            auto node = q[i];
            if (node == nullptr) {
                continue;
            }
            q.push_back(node->left);
            q.push_back(node->right);
        }
        while (q.back() == nullptr) {
            q.pop_back();
        }
        string str{};
        for (int i = 0; i < q.size(); ++i) {
            if (q[i] == nullptr) {
                str += "#,";
            } else {
                if (i != q.size() - 1) {
                    str += to_string(q[i]->val) + ",";
                } else {
                    str += to_string(q[i]->val);
                }
            }
        }
        return str;
    }
    TreeNode * deserialize_1(string &data) {
        // write your code here
        if (data.size() < 1) {
            return nullptr;
        }
        std::stringstream ss(data);
        vector<TreeNode*> nodes;
        string tmp;
        cout << "data : " << data << endl;
        while (std::getline(ss, tmp, ',')) {
            if (tmp == "#") {
                nodes.push_back(nullptr);
            } else {
                nodes.push_back(new TreeNode(std::stoi(tmp)));
            }
        }
        deque<TreeNode*> q;
        q.push_back(nodes[0]);
        int index = 0;
        bool isleftchild = true;
        for (int i = 1; i < nodes.size(); ++i) {
            if (nodes[i] != nullptr) {
                if (isleftchild) {
                    q[index]->left = nodes[i];
                } else {
                    q[index]->right = nodes[i];
                }
                q.push_back(nodes[i]);
            }
            if (!isleftchild) {
                ++index;
            }
            isleftchild = !isleftchild;
        }
        return nodes[0];
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
    auto test = [&s](string str) {
        cout << "str: " << str << endl;
        // auto res = s.permute(nums);
        // auto res = s.permute_0(nums);
        // auto res = s.permute_1(nums);
        // auto res = s.permute_2(nums);
        // auto res = s.deserialize(str);
        // auto ret  = s.serialize(res);
        auto res = s.deserialize_1(str);
        auto ret  = s.serialize_1(res);
        cout << "permutaions: " << ret << endl;

    };

    str = "3,9,20,#,#,15,7";
    test(str);

    str = "1,#,2,#,3,#,4";
    test(str);

    // str = "1,#,2,#,#,#,3,#,#,#,#,#,#,#,4";// available for serialize and deserialize, but not for serialize_1 and deserialize_1
    // test(str);
}