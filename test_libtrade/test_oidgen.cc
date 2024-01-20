#include "oidgen.hh"
#include <iostream>
#include <vector>

int main() {
    std::cout << "test_oidgen start!" << std::endl;
    unsigned int a=1;
    unsigned int a1=1;
    for (int i=30;i>=0;--i){
        //左移31次，每次左移加1，将个位补1，最后就是32个1
        a<<=1;
        ++a;
        a1 = a1 << 1;
    }
    std::cout << "unsigned int max: " << a << std::endl;
    std::cout << "unsigned int 0b10000000000000000000000000000000: " << a1 << std::endl;
    std::cout << "unsigned int max: " << std::numeric_limits<unsigned int>::max() << std::endl;
    std::cout << endl;
    unsigned int oid2 = 86400000;
    std::cout << "oid2: " << oid2 << std::endl;
    std::cout << a <<" - " << oid2 << " = " << a - oid2 << std::endl;
    std::cout << endl;

    // _oidGen为单例模式，不需要主动释放
    trdkit::OIDGen* _oidGen = trdkit::OIDGen::factory("localtime");
    unsigned int oid = _oidGen->next32();
    std::cout << "_oid32 | 0x80000000 " << std::endl;
    std::cout << "oid: " << oid << std::endl;
    std::cout << endl;

    // _oidGen为单例模式，不需要主动释放
    vector<int> v{1,5,10,11,20,21,22,25,0};
    trdkit::OIDGen* _oidGen11{nullptr};
    for (auto& it : v) {
        cout << "trdkit::OIDGen::factory(\"localtime\", " << it << ")" << endl;
        trdkit::OIDGen* _oidGen11 = trdkit::OIDGen::factory("localtime", it);
        unsigned int oid11 = _oidGen11->next32();
        if (oid11 < a)
            std::cout << "oid11=" << oid11 << " " << oid11 << " < " << a << std::endl;
        else
            std::cout << "oid11=" << oid11 << " " << oid11 << " >= " << a << std::endl;
        // delete _oidGen11;
        cout << endl;
    }

    int prefix = 15;
    trdkit::OIDGen* _oidGen12 = trdkit::OIDGen::factory("localtime", prefix);
    int i = 500;
    cout << "gen 1000 oids: ";
    while(i) {
        i--;
        // continue;
        cout << "prefix=" << prefix << " oid=" << _oidGen12->next32() << endl;
    }
    cout << endl;

    int b = 1;
    b = b << 27;
    std::cout << "b: " << b << std::endl;
    int c = 0;
    c = c > 21 ? c % 21 : c;
    c *= 100000000;
    std::cout << "c: " << c << std::endl;
    std::cout << "c + " << oid2 << " = " << c + oid2 << std::endl;
    cout << endl;


    return 0;
}