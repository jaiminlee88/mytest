#include "libkit.hh"

#include <iostream>
#include <dlfcn.h>

using namespace std;

int main() {
    void *handle = dlopen("libxtpapi.so", RTLD_NOW);
    if (!handle) {
        cout << "open libxtpapi.so failed" << endl;
        return 0;
    }
    cout << "edge main print" << endl;
}
