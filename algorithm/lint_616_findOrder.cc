#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
using namespace std;


struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int a, int b) : x(a), y(b) {}
};

class Solution {
public:
    /**
     * @param numCourses: a total of n courses
     * @param prerequisites: a list of prerequisite pairs
     * @return: the course order
     * 你需要去上n门九章的课才能获得offer，这些课被标号为 0 到 n-1 。
        有一些课程需要“前置课程”，比如如果你要上课程0，你需要先学课程1，我们用一个匹配来表示他们： [0,1]
        给你课程的总数量和一些前置课程的需求，返回你为了学完所有课程所安排的学习顺序。
        可能会有多个正确的顺序，你只要返回一种就可以了。如果不可能完成所有课程，返回一个空数组。
     */


    vector<int> findOrder(int numCourses, vector<vector<int>> &prerequisites) {
        // write your code here
        if (numCourses == 0) {
            return {};
        }
        if (numCourses == 1) {
            return {0};
        }

        std::unordered_map<int, vector<int>> nodes_map;
        std::unordered_map<int, int> indegree;
        std::queue<int> q;
        std::vector<int> order;
        for (int i = 0; i < numCourses; ++i) {
            nodes_map[i] = vector<int>{}; // 初始化
            indegree[i] = 0;
        }
        for (auto& conn : prerequisites) {
            // pre->post
            int pre = conn[1];
            int post = conn[0];
            auto& pre_node = nodes_map[pre];
            pre_node.push_back(post);

            auto& post_degree = indegree[post];
            ++post_degree;
        }
        for (auto& degree : indegree) {
            if (degree.second == 0) {
                q.push(degree.first);
            }
        }

        while (q.empty() == false) {
            int cur = q.front();
            q.pop();
            order.push_back(cur);
            auto& curnode = nodes_map[cur];
            for (auto& node : curnode) {
                auto& degree = indegree[node];
                --degree;
                if (degree == 0) {
                    q.push(node);
                }
            }
        }
        return order.size() == numCourses ? order : vector<int>{};
    }


    // 下面的错了，把n看做是需要上课的数量，实际上是一共的课程数， 要注意读题
    // void new_reliance(int pre, int post, std::unordered_map<int, vector<int>>& nodes_map, std::unordered_map<int, int>& indegree) {
    //     auto pre_node = nodes_map.find(pre);
    //     if (pre_node == nodes_map.end()) {
    //         nodes_map[pre] = vector<int>{post};
    //     } else {
    //         pre_node->second.push_back(post);
    //     }
    //     auto pre_degree = indegree.find(pre);
    //     if (pre_degree == indegree.end()) {
    //         indegree[pre] = 0;
    //     }

    //     auto post_node = nodes_map.find(post);
    //     if (post_node == nodes_map.end()) {
    //         nodes_map[post] = vector<int>{};
    //     }
    //     auto post_degree = indegree.find(post);
    //     if (post_degree == indegree.end()) {
    //         indegree[post] = 1;
    //     } else {
    //         ++(post_degree->second);
    //     }
    // }

    // vector<int> findOrder(int numCourses, vector<vector<int>> &prerequisites) {
    //     // write your code here
    //     if (prerequisites.size() == 0 && numCourses == 1) {
    //         return {0};
    //     }
    //     if (prerequisites.size() == 0 || numCourses == 0) {
    //         return {};
    //     }
    //     std::unordered_map<int, vector<int>> nodes_map;
    //     std::unordered_map<int, int> indegree;
    //     std::vector<int> order;
    //     for (auto& conn : prerequisites) {
    //         // pre->post
    //         int pre = conn[1];
    //         int post = conn[0];
    //         new_reliance(pre, post, nodes_map, indegree);
    //     }
    //     while (true) {
    //         if (order.size() >= numCourses) {
    //             return order;
    //         }
    //         cout << "-----------" << endl;
    //         for (auto& it : indegree) {
    //             cout << it.first << " indegree:" << it.second << endl;
    //         }
    //         bool found = false;
    //         for (auto& it : indegree) {
    //             // cout << it.first << " indegree:" << it.second << endl;
    //             if (it.second == 0) {
    //                 found = true;
    //                 order.push_back(it.first);
    //                 auto& node = nodes_map[it.first];
    //                 for (auto& n : node) {
    //                     indegree[n]--;
    //                 }
    //                 indegree.erase(it.first);
    //                 break;
    //             }
    //         }
    //         if (!found) {
    //             return {};
    //         }
    //     }
    //     return {};
    // }

    // vector<int> findOrder_1(int numCourses, vector<vector<int>> &prerequisites) {
    //     // write your code here
    //     if (prerequisites.size() == 0 && numCourses == 1) {
    //         return {0};
    //     }
    //     if (prerequisites.size() == 0 || numCourses == 0) {
    //         return {};
    //     }
    //     std::unordered_map<int, vector<int>> nodes_map;
    //     std::unordered_map<int, int> indegree;
    //     std::queue<int> q;
    //     std::vector<int> order;
    //     for (auto& conn : prerequisites) {
    //         // pre->post
    //         int pre = conn[1];
    //         int post = conn[0];
    //         new_reliance(pre, post, nodes_map, indegree);
    //     }

    //     // cout << "----1-------" << endl;
    //     // for (auto& it : indegree) {
    //     //     cout << it.first << " indegree:" << it.second << endl;
    //     // }
    //     // for (auto& it : nodes_map) {
    //     //     cout << it.first << "->[";
    //     //     for (auto& n : it.second) {
    //     //         cout << n << " ";
    //     //     }
    //     //     cout << "]" << endl;
    //     // }
    //     for (auto& degree : indegree) {
    //         if (degree.second == 0) {
    //             q.push(degree.first);
    //         }
    //     }
    //     // cout << "----2-------" << endl;
    //     // cout << "q.size()=" << q.size() << endl;
    //     while (true) {
    //         if (order.size() >= numCourses) {
    //             return order;
    //         }
    //         if (q.empty() == true) {
    //             return {};
    //         }
    //         int cur = q.front();
    //         q.pop();
    //         order.push_back(cur);
    //         auto curnode = nodes_map.find(cur);
    //         for (auto& node : curnode->second) {
    //             auto degree = indegree.find(node);
    //             --(degree->second);
    //             if (degree->second == 0) {
    //                 q.push(node);
    //             }
    //         }
    //     }
    //     return {};
    // }
};

int main() {
    Solution s;
    vector<vector<int>> grid;
    int required_course;
    auto test = [&s] (vector<vector<int>>& grid, int n) {
        cout << "============" << endl;
        cout << "grid:" << endl;
        for (auto row : grid) {
            for (auto col : row) {
                cout << col << " ";
            }
            cout << endl;
        }
        vector<int> order = s.findOrder(n, grid);
        cout << "n: " << n << " order: ";
        if (order.size() == 0) {
            cout << " not found" << endl;
            return;
        }
        for (int i = 0; i < order.size(); ++i) {
            cout << order[i];
            if (i != order.size() - 1) {
                cout << "->";
            }
        }
        cout << endl;
    };

    grid = {};
    required_course = 1;
    test(grid, required_course); // 2

    grid = {{1,0}};
    required_course = 2;
    test(grid, required_course); // 2

    grid = {{1,0},
            {2,0},
            {3,1},
            {3,2}};
    required_course = 4;
    test(grid, required_course); //  0 1 3 2

    grid = {{1,2},
            {4,3}};
    required_course = 5;
    test(grid, required_course); // 3 4 2 1 0

    grid = {{2,1},
            {3,2},
            {1,3}};
    required_course = 2;
    test(grid, required_course); // 3 4 2 1 0
    return 0;
}