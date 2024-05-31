#include <algorithm>
#include <vector>
#include <string>
#include <tuple>
#include <functional>
#include <iostream>

/*
The C++ program in the attachedproblem4.
cpphas a variableproble4Inputthat is a list of 
unique characters in the form of a string. 
It generates all possible permutations of the list 
and a unique hash value of each permutation, 
and prints to screen.The program is not working correctly.  
Specifically, the list of permutations is not complete 
andthe hash values being printed are all identical. 
It is also too slow with the possibility of memoryexceptions. 
Please find the 4 bugs in the program and add a comment to each change
you make.
*/
using namespace std;

class table_generator {
    using table_t = std::vector<std::tuple<std::string, std::size_t>>;
private:
    const std::tuple<std::string, std::size_t>& add_word() {
        // table_.reserve(table_.size() + 1);
        table_.emplace_back(word_, hash_word(word_));
        return table_.back();
    }

    bool next_word() {
        if (!first_) {
            first_ = true;
            return true;
        }
        // cout << "*******before: " << word_ << endl;
        // bool ret = std::next_permutation(word_.begin(), word_.end());
        // if (ret == false) {
        // cout << "======after: " << word_ << endl;
        // }
        // return ret;
        return std::next_permutation(word_.begin(), word_.end());
    }

    size_t factorial(int n) {
        size_t maz_size = 1;
        for (int i = 1; i <= n; ++i) {
            maz_size *= i;
        }
        return maz_size;
    }

    static std::size_t hash_word(std::string const & word) {
        return std::hash<std::string>{}(word);
    }

public:
    table_generator(std::string alphabet)
        : word_(std::move(alphabet)) {
            std::sort(word_.begin(), word_.end());
            table_.reserve(factorial(word_.size()));
        }

    ~table_generator() {
        // for (auto & [word, hash] : table_) {
        //     std::cout << word << " " << std::hex << hash << "\n";
        // }
    }

    bool operator()(std::string & word, std::size_t& hash) {
        // if (table_.empty()) {
        //     std::sort(word_.begin(), word_.end());
        // }

        if (!next_word()) {
            return false;
        }

        std::tie(word, hash) = add_word();
        return true;
    }

private:
    table_t     table_;
    std::string word_;
    bool        first_ = false;
};

int main() {
    vector<string> res;
    std::string word_ = "jqinv";
    std::sort(word_.begin(), word_.end());
    std::cout << word_ << " *** " << std::hash<std::string>{}(word_) << std::endl;
    res.push_back(word_);
    int cnt0 = 1;
    while (std::next_permutation(word_.begin(), word_.end())) {
        cnt0++;
        // std::cout << word_ << " " << std::hash<std::string>{}(word_) << std::endl;
        res.push_back(word_);
    }
    for (auto& i : res) {
        std::cout << i << " ";
    }
    cout << endl;
    cout << "Total count: " << cnt0 << " res.size(): " << res.size() << endl;

    // // table_generator proble4Input("abbbc");
    // // table_generator proble4Input("aabb");
    // table_generator proble4Input("abcde");
    table_generator proble4Input("jqinv");
    std::string word;
    std::size_t hash;
    std::vector<string> res2;
    int cnt = 0;
    while (proble4Input(word, hash)) {
        cnt++;
        res2.push_back(word);
        std::cout << word << " " << std::hex << hash << "\n";
    }
    int cnt2 = 0;
    for (int i = 0; i < res.size(); i++) {
        cnt2++;
        std::cout << res[i] << " " << res2[i] << endl;
    }
    std::cout << "Total count2: " << cnt2 << std::endl;
    cout << endl;
    std::cout << "Total count: " << cnt << std::endl;

    cout << "res.size(): " << res.size() << " res2.size(): " << res2.size() << endl;
}
