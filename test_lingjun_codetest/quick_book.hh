#pragma once
#include<iostream>
#include<list>

struct Snap {
public:
    std::string TradingDay;
    std::string ActionDay;
    unsigned long exch_time;
    unsigned long time;

    unsigned int OpenInterest;
    double UpperLimitPrice;
    double LowerLimitPrice;
    double OpenPrice;
    double HighestPrice;
    double LowestPrice;
    double LastPrice;
    unsigned int Volume;
    double Turnover;

    double PreSettlementPrice;
    double PreClosePrice;
    unsigned int PreOpenInterest;

    double bpx[5] = {0};
    unsigned int bsz[5] = {0};
    double apx[5] = {0};
    unsigned int asz[5] = {0};
    void str() {
        std::cout << " |TradingDay=" << TradingDay;
        std::cout << " |ActionDay=" << ActionDay;
        std::cout << " |exch_time=" << exch_time;
        std::cout << " |time=" << time;
        std::cout << " |OpenInterest=" << OpenInterest;
        std::cout << " |UpperLimitPrice=" << UpperLimitPrice;
        std::cout << " |LowerLimitPrice=" << LowerLimitPrice;
        std::cout << " |OpenPrice=" << OpenPrice;
        std::cout << " |HighestPrice=" << HighestPrice;
        std::cout << " |LowestPrice=" << LowestPrice;
        std::cout << " |LastPrice=" << LastPrice;
        std::cout << " |Volume=" << Volume;
        std::cout << " |Turnover=" << Turnover;
        std::cout << " |PreSettlementPrice=" << PreSettlementPrice;
        std::cout << " |PreClosePrice=" << PreClosePrice;
        std::cout << " |PreOpenInterest=" << PreOpenInterest;
        for (int i = 0; i < 5 ; i++) {
            std::cout << " |bpx["<< i << "]=" << bpx[i];
            std::cout << " |bsz["<< i << "]=" << bsz[i];
            std::cout << " |apx["<< i << "]=" << apx[i];
            std::cout << " |asz["<< i << "]=" << asz[i];
        }
        std::cout << std::endl;
    }
};

class quick_book {
public:
    quick_book(){}
    ~quick_book(){}
    void print_book() {
        // for (unsigned int i=askladders.size()-1; i >= 0; --i) {
        //     std::cout << askladders[i]._price << "|";
        // }
        // std::cout << "|||";
        // for (unsigned int i=0; i < bidladders.size(); ++i) {
        //     std::cout << bidladders[i]._price << "|";
        // }
        // std::cout << endl;
    }

    void insert(Snap* psnap) {
        if (psnap == nullptr) return;
        psnap->str();
        // for (bidladders) {

        // }
    }
private:
    // the book shall be in the form with each ladders (price, qty, num of orders),
    struct Order{
        unsigned int quantity{0};
        unsigned int queue_position{0};
        unsigned int last_modification_time{0};
    };
    struct ladder{
        double _price{0.0};
        uint32_t _qty{0};
        std::list<Order> _ords;
    };
    std::list<ladder> bidladders; // high to low
    std::list<ladder> askladders; // low to high
    bool              refresh{false};
};