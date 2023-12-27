#include<iostream>
using namespace std;
int main(){
    int guest = 0;
    cout << "before " << guest << endl;
    while(guest++<10) {
        cout << "after " << guest << endl;
    }
}