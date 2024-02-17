#include <iostream>
#include <vector>

using namespace std;

int main() {
    std::pair<int, int> p1;
    p1.first = 1;
    p1.second = 2;
    cout << "p1.first: " << p1.first << " p1.second: " << p1.second << endl;
    std::pair<int, int> p2(3, 4);
    cout << "p2.first: " << p2.first << " p2.second: " << p2.second << endl;

    std::vector<std::pair<int, int>> vec;
    vec.push_back(p1);
    vec.push_back(p2);
    for (auto& i : vec) {
        cout << "vec.first: " << i.first << " vec.second: " << i.second << endl;
    }
}