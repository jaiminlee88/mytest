#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <list>
using namespace std;

class Solution {
public:
    /**
    *给出 A, B, C, D 四个整数列表，计算有多少的tuple (i, j, k, l)满足A[i] + B[j] + C[k] + D[l]为 0。
     * @param A: a list
     * @param B: a list
     * @param C: a list
     * @param D: a list
     * @return: how many tuples (i, j, k, l) there are such that A[i] + B[j] + C[k] + D[l] is zero
    */
    int target = 0;
    int fourSumCount(vector<int> A, vector<int> B, vector<int> C, vector<int> D) {
        if (A.size() < 1 || B.size() < 1 || C.size() < 1 || D.size() < 1) {
            return 0;
        }
        int count = 0;
        sort(A.begin(), A.end());
        sort(B.begin(), B.end());
        sort(C.begin(), C.end());
        sort(D.begin(), D.end());
        for (int apos = 0; apos < A.size(); apos++) {
            for (int bpos = 0; bpos < B.size(); bpos++) {
                for (int cpos = 0; cpos < C.size(); cpos++) {
                    for (int dpos = 0; dpos < D.size(); dpos++) {
                        if (A[apos] + B[bpos] + C[cpos] + D[dpos] == target) {
                            cout << "A[" << apos << "]=" << A[apos] << " B[" << bpos << "]=" << B[bpos] << " C[" << cpos << "]=" << C[cpos] << " D[" << dpos << "]=" << D[dpos] << endl;
                            count++;
                        }
                    }
                }
            }
        }
        return count;
    }

    struct val3 {
        int count;
        struct pos_val {
            int pos_i;
            int pos_ii;
            int val_i;
            int val_ii;
        };
        std::list<pos_val> pos_val_list;
    };
    int fourSumCount_1(vector<int> A, vector<int> B, vector<int> C, vector<int> D) {
        // 空间换时间，取巧法
        if (A.size() < 1 || B.size() < 1 || C.size() < 1 || D.size() < 1) {
            return 0;
        }
        int count = 0;
        std::unordered_map<int, val3> mapAB;
        std::unordered_map<int, val3> mapCD;
        for (int apos = 0; apos < A.size(); apos++) {
            for (int bpos = 0; bpos < B.size(); bpos++) {
                mapAB[A[apos] + B[bpos]].count++;
                val3::pos_val tmp = {apos, bpos, A[apos], B[bpos]};
                mapAB[A[apos] + B[bpos]].pos_val_list.push_back(tmp);
            }
        }
        for (int cpos = 0; cpos < C.size(); cpos++) {
            for (int dpos = 0; dpos < D.size(); dpos++) {
                mapCD[C[cpos] + D[dpos]].count++;
                val3::pos_val tmp = {cpos, dpos, A[cpos], B[dpos]};
                mapCD[C[cpos] + D[dpos]].pos_val_list.push_back(tmp);
            }
        }
        for (auto& it : mapAB) {
            int new_target = target - it.first;
            if (mapCD.find(new_target) != mapCD.end()) {
                count += it.second.count * mapCD[new_target].count;
                for (auto& AB_pos_val : it.second.pos_val_list) {
                    for (auto& CD_pos_val : mapCD[new_target].pos_val_list) {
                        cout << "A[" << AB_pos_val.pos_i << "]=" << AB_pos_val.val_i << " B[" << AB_pos_val.pos_ii << "]=" << AB_pos_val.val_ii << " C[" << CD_pos_val.pos_i << "]=" << CD_pos_val.val_i << " D[" << CD_pos_val.pos_ii << "]=" << CD_pos_val.val_ii << endl;
                    }
                }
            }
        }
        return count;
    }
};
void print_result(std::vector<int>& A, std::vector<int>& B, std::vector<int>& C, std::vector<int>& D, int count) {
    cout << "===numbers:" << endl;
    cout << "A:[";
    for (auto& it : A) {
        cout << it << ", ";
    }
    cout << "]" << endl;
    cout << "B:[";
    for (auto& it : B) {
        cout << it << ", ";
    }
    cout << "]" << endl;
    cout << "C:[";
    for (auto& it : C) {
        cout << it << ", ";
    }
    cout << "]" << endl;
    cout << "D:[";
    for (auto& it : D) {
        cout << it << ", ";
    }
    cout << "]" << endl;
    cout << "count: " << count << endl;
}

int main () {
    Solution s;
    vector<int> A;
    vector<int> B;
    vector<int> C;
    vector<int> D;
    int count;

    A = { 1, 2};
    B = {-2,-1};
    C = {-1, 2};
    D = { 0, 2};
    count = s.fourSumCount_1(A, B, C, D);
    print_result(A, B, C, D, count);

    A = {0};
    B = {0};
    C = {0};
    D = {0};
    count = s.fourSumCount_1(A, B, C, D);
    print_result(A, B, C, D, count);

    return 0;
}