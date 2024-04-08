
#include "libkit.hh"

// TzInfo TzInfo::_utc; // 会被初始化

TzInfo::TzInfo() {
    cout << "TzInfo::TzInfo:" << (void*)this << endl;
    //b.push_back(2);
}

TzInfo::~TzInfo() {
    cout << "TzInfo::~TzInfo:" << (void*)this << " b.data(): " << (void*)b.data() << endl;
}
