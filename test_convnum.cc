
#include<string>
#include<vector>
#include<iostream>
using namespace std;

void convertRMB(long long num) {
    std::string rmb[4] = {"", "万","亿","兆"};
    std::string Num[10] = {"零", "壹", "贰","叁","肆","伍","陆","柒","捌","玖"};
    std::string throusand[4] = {"","拾","佰","仟"};
    std::vector<std::string> out;
    int k = 0;
    while (num) {
        long long temp = num % 10000;
        if (temp == 0) {
            out.push_back(Num[0]);
            num =(long long)(num/10000);
            k++;
            continue;
        }
        out.push_back(rmb[k++]);
        bool is_zero = false;
        bool got_num = false;
        int j = 0;
        int l = 0;
        while (true) {
            int a = temp % 10; // 逐个来
            if (a) {
                got_num = true;
                out.push_back(throusand[j]);
                out.push_back(Num[a]);
                if (is_zero) {
                    is_zero = false;
                }
            } else {
                if (!is_zero && got_num && (int)(temp/10)) {
                    out.push_back(Num[0]);
                    is_zero = true;
                    got_num = false;
                }
            }
            temp = (int)(temp/10);
            j++;
            if (temp==0) {
                if (j<4 && j>0) {
                    out.push_back(Num[0]);
                }
                break;
            }
        }
        num =(long long)(num/10000);
    }

    bool is_zero = false;
    for (int i = out.size()-1; i >= 0; i--) {
        if (out[i] == "") {
            continue;
        } else if (out[i] == "零") {
            if (i == 0 || i == out.size()-1) continue;
            if(!is_zero) {
                std::cout << out[i];
            }
            is_zero = true;
        } else {
            is_zero = false;
            std::cout << out[i];
        }
    }
    std::cout << endl;
    return;
}

int main() {
    while (true) {
        long long i  = 0;
        std::cin >> i;
        if (i>0) {
            convertRMB(i);
        } else if (i == 0) {
            break;
        }
    }
    return 0;
}