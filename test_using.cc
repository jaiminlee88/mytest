#include <iostream>
#include <unordered_map>
#include <memory>
using namespace std;


namespace jack {
    double fetch;
}
namespace jill{
    double fetch;
}
namespace {
    double abc; // internal linkage, static storage
}

int main() {
	// jack::fetch = 8.0; // valid
    // jill::fetch = 9.0;// valid
    // double fetch = 10.0;// valid
    // cout << "fetch=" << fetch << " jack::fetch=" << jack::fetch << " jill::fetch=" << jill::fetch << endl;

    // using jack::fetch; // make local
    // // using jill::fetch; // invalid, 会有冲突
    // // double fetch; // invalid, 会有冲突

    // double fetch = 9;
    // {
    //     using jack::fetch;
    //     jack::fetch = 8.0; // 会覆盖global fetch
    //     cout << "fetch=" << fetch << endl;
    // }

    using namespace jack;
    double fetch = 9; // hide jack::fetch
    jack::fetch = 10;
    // ::fetch = 11; // invalid
    abc = 3;
    cout << fetch << " " << jack::fetch << " abc=" << abc << endl;

    using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;

    typedef void (*FP)(int, const std::string&);    //typedef
    using FP = void (*)(int, const std::string&);   //别名声明
    return 0;
}