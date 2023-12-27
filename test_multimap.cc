#include<map>
#include<set>
#include<vector>
#include<iostream>
#include<iterator>
#include<algorithm>
#include<ctype.h>
using namespace std;

void print_map(multimap<int,string>& m, int key){
    // for (auto& ref_it : m) {
    //     cout << "ref_it=" << ref_it << endl;
    // }
    // 
    pair<multimap<int,string>::iterator, multimap<int,string>::iterator> range = m.equal_range(key);
    for (auto it = range.first; it != range.second; it++) {
        cout << "it->first=" << it->first << " it->second=" << it->second << endl;
    }
    // for (multimap<int,string>::iterator it = range.first; it != range.second; it++) {
    //     cout << "it->first=" << it->first << " it->second=" << it->second << endl;
    // }
}
int main(){
    multimap<int,string> a;
    a.insert(make_pair(1,"hello"));
    a.insert(make_pair(2,"world"));
    a.insert(make_pair(3,"apple"));
    a.insert(make_pair(4,"banana"));
    a.insert(make_pair(1,"lambda"));
    pair<int, string> item(6,"item");
    a.insert(item);
    print_map(a,3);
    print_map(a,1);
    print_map(a,6);
    // a[5] = string("function");
    // copy(a.begin(), a.end(), ostream_iterator<pair<int,string>>(cout, ", ")); // 1,hello, 1,lambda, 2,world, 3,apple, 4,banana, 5,function,

    vector<string> words{"app","APP","apple","orange","grape"};
    set<string> wordset;
    cout << (char)tolower('A') << endl;
    auto _toLower = [](std::string &s) -> std::string {
        transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {return std::tolower(c);});
        return s;
    };
    transform(words.begin(), words.end(), insert_iterator<set<string>>(wordset, wordset.begin()), _toLower);
    map<string, int> wordmap;
    set<string>::iterator si;
    for (si=wordset.begin(); si!=wordset.end(); si++) {
        wordmap[*si] = count(words.begin(), words.end(), *si);
        // wordmap.insert(pair<string,int>(*si, count(words.begin(),words.end(), *si)));
    }
    for (auto& it : wordmap) {
        cout << it.first << ":" << it.second << endl;
    }
}

