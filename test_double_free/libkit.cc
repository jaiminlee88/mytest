
#include "libkit.hh"

TzInfo TzInfo::_utc;

TzInfo::TzInfo() {
    cout << "TzInfo::TzInfo:" << (void*)this << endl;
    //b.push_back(2);
}

TzInfo::~TzInfo() {
    cout << "TzInfo::~TzInfo:" << (void*)this << endl;
    cout << "b.data(): " << (void*)b.data() << endl;
}
