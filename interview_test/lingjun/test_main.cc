#include <iostream>
#include <istream>
#include <streambuf>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <map>
#include "quick_book.hh"
// #include "cntick.hh"
// #include "event.hh"

//! 头文件
#include "H5Cpp.h"
//! 名称空间
using namespace H5;

int main() {
    std::ifstream csv_data("rb2305.csv", std::ios::in);
    std::string line;

    if (!csv_data.is_open()) {
        std::cout << "Error: opening file fail" << std::endl;
        std::exit(1);
    }

    std::istringstream sin;         //将整行字符串line读入到字符串istringstream中
    std::vector<std::string> words; //声明一个字符串向量
    std::string word;
    // 读取标题行
    std::getline(csv_data, line);
    // 读取数据
    quick_book mybook;
    while (std::getline(csv_data, line)) {
        sin.clear();
        sin.str(line);
        words.clear();
        while (std::getline(sin, word, ',')) {//将字符串流sin中的字符读到field字符串中，以逗号为分隔符
            words.push_back(word); //将每一格中的数据逐个push
            std::cout << word << ",";
            // std::cout << atol(word.c_str());
        }
        std::cout << std::endl;
        if (words.size()>0) {
            std::cout << "TradingDay=" << words[0] << ", "
                << "LastPrice=" << words[1] << ", "
                << "Volume=" << words[6] << ", "
                << "Turnover=" << words[7] << ", "
                << "OpenInterest=" << words[8] << ", "
                << "UpperLimitPrice=" << words[9] << ", "
                << "LowerLimitPrice=" << words[10] << ", "
                << "OpenPrice=" << words[11] << ", "
                << "PreSettlementPrice=" << words[12] << ", "
                << "PreClosePrice=" << words[13] << ", "
                << "PreOpenInterest=" << words[14] << ", "
                << "ActionDay=" << words[15] << ", "

                << "BPrice1=" << words[2] << ", "
                << "BVol1=" << words[3] << ", "
                << "BPrice2=" << words[16] << ", "
                << "BVol2=" << words[17] << ", "
                << "BPrice3=" << words[18] << ", "
                << "BVol3=" << words[19] << ", "
                << "BPrice4=" << words[20] << ", "
                << "BVol4=" << words[21] << ", "
                << "BPrice5=" << words[22] << ", "
                << "BVol5=" << words[23] << ", "

                << "SPrice1=" << words[4] << ", "
                << "SVol1=" << words[5] << ", "
                << "SPrice2=" << words[24] << ", "
                << "SVol2=" << words[25] << ", "
                << "SPrice3=" << words[26] << ", "
                << "SVol3=" << words[27] << ", "
                << "SPrice4=" << words[28] << ", "
                << "SVol4=" << words[29] << ", "
                << "SPrice5=" << words[30] << ", "
                << "SVol5=" << words[31] << ", "
                << "exch_time=" << words[32] << ", "
                << "time=" << words[33] << ", "
                << "HighestPrice=" << words[34] << ", "
                << "LowestPrice=" << words[35] << ", "
                << std::endl;
        }
        Snap snap;
        snap.TradingDay = words[0];
        snap.exch_time = std::stoul(words[32]);
        snap.time = std::stoul(words[33]);
        snap.LastPrice = std::stod(words[1]);
        snap.Volume = std::stoul(words[6]);
        snap.Turnover = std::stod(words[7]);
        snap.OpenInterest = std::stoul(words[8]);
        snap.UpperLimitPrice = std::stod(words[9]);
        snap.LowerLimitPrice = std::stod(words[10]);
        snap.OpenPrice = std::stod(words[11]);
        snap.HighestPrice = std::stod(words[34]);
        snap.LowestPrice= std::stod(words[35]);
        snap.PreSettlementPrice = std::stod(words[12]);
        snap.PreClosePrice = std::stod(words[13]);
        snap.PreOpenInterest = std::stoul(words[14]);
        snap.ActionDay = words[15];
        snap.bpx[0] = std::stod(words[2]);
        snap.bpx[1] = std::stod(words[16]);
        snap.bpx[2] = std::stod(words[18]);
        snap.bpx[3] = std::stod(words[20]);
        snap.bpx[4] = std::stod(words[22]);
        snap.bsz[0] = std::stoul(words[3]);
        snap.bsz[1] = std::stoul(words[17]);
        snap.bsz[2] = std::stoul(words[19]);
        snap.bsz[3] = std::stoul(words[21]);
        snap.bsz[4] = std::stoul(words[23]);
        snap.apx[0] = std::stod(words[4]);
        snap.apx[1] = std::stod(words[24]);
        snap.apx[2] = std::stod(words[26]);
        snap.apx[3] = std::stod(words[28]);
        snap.apx[4] = std::stod(words[30]);
        snap.asz[0] = std::stoul(words[5]);
        snap.asz[1] = std::stoul(words[25]);
        snap.asz[2] = std::stoul(words[27]);
        snap.asz[3] = std::stoul(words[29]);
        snap.asz[4] = std::stoul(words[31]);
        mybook.insert(&snap);
    }
    csv_data.close();
}