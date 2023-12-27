#include<iostream>

using namespace std;

struct things1 {
    char ch;   // 4
    int a;     // 4
    double x;  // 8
};

struct things2 {
    int a;    // 8
    double x; // 8
    char c;   // 8
};

int main(){
    cout << "alignof=================" << endl;
    cout << "sizeof(things1)=" << sizeof(things1) << endl;
    cout << "sizeof(thing2)=" << sizeof(things2) << endl;
    cout << "alignof(things1)=" << alignof(things1) << endl;
    cout << "alignof(things2)=" << alignof(things2) << endl;
}
