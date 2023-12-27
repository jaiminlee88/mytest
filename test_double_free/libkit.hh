#pragma once

#include <vector>
#include <iostream>
using namespace std;

class TzInfo {
public:
    TzInfo();
    ~TzInfo();
    int a{};
    vector<int> b{};
    char c = 't';
    static TzInfo _utc;
};

// static TzInfo _utc;