
#include <ostream>
#include <iostream>

using namespace std;

class Solution {
public:
    /**
     * @param source: 
     * @param target: 
     * @return: return the index
     */
    int strStr(string &source, string &target) {
        // Write your code here
        if (source.size() < target.size()) {
            return -1;
        }
        if (target.size() == 0) {
            return 0;
        }
        for (unsigned int i=0; i <= (source.size() - target.size()); ++i) {
            bool is_match = true;
            for(unsigned int j=0; j < target.size(); ++j) {
                if (source[i + j] != target[j]) {
                    is_match = false;
                    break;
                }
            }
            if (is_match) {
                return i;
            }
        }
        return -1;
    }
};

int main() {
    Solution s;
    string source = "abcdabcdefg";
    string target = "bcd";
    std::cout << s.strStr(source, target) << std::endl;
    return 0;
}