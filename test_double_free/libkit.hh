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
    static TzInfo _utc; // 会被初始化，必须在cc中初始化，放在头文件中，导致多次初始化，因为头文件会被多个cc引用
};

// static TzInfo _utc;