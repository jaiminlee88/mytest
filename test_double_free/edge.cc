#include "libkit.hh"

#include <iostream>
#include <dlfcn.h>

using namespace std;

int main() {
    void *handle = dlopen("libxtpapi.so", RTLD_NOW);
    if (!handle) {
        cout << "open [libxtpapi.so] failed" << endl;
        return 0;
    } else {
        cout << "open [libxtpapi.so] success" << endl;
    }

    void *handle1 = dlopen("libkit.so", RTLD_NOW);
    if (!handle1) {
        cout << "open [libkit.so] failed" << endl;
        return 0;
    } else {
        cout << "open [libkit.so] success" << endl;
    }

    cout << "edge main print" << endl;
}
