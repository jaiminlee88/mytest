#include <iostream>
#include <vector>
#include <exception>
using namespace std;

class RNumber;

class RNumber {
private:
    long long numerator;
    long long denominator;
public:
    RNumber():RNumber(0,1){}
    RNumber(long num, long deno) : numerator(num), denominator(deno) {
        if (deno == 0) {
            throw invalid_argument("denominator cant be zeor.");
        }
        normalize(); // 分子分母会很大，所以要化简公约数，否则会溢出
    }
    RNumber(const RNumber& rn) {
        if (rn.denominator == 0) {
            throw invalid_argument("denominator cant be zeor.");
        }
        numerator = rn.numerator;
        denominator = rn.denominator;
        normalize(); // 分子分母会很大，所以要化简公约数，否则会溢出
    }
    ~RNumber(){}

    // 求任意两个数的最大公约数可以使用欧几里得算法（也称为辗转相除法）
    // 该算法基于以下原理：两个整数a和b的最大公约数等于a除以b的余数c，
    // 再对b和c进行同样的操作，直到余数为0为止，此时b即为最大公约数。
    long long gcd(long long a, long long b){
        return (a % b == 0) ? b : gcd(b, a % b);
    }
    void normalize() {
        if (numerator == 0) return;
        long long _numerator = abs(numerator);
        long long _denominator = abs(denominator);
        long long g = gcd(_numerator, _denominator);
        numerator /= g;
        denominator /= g;
    }
    friend ostream& operator<<(ostream& out, RNumber&);
    long long get_denominator() { return denominator;}
    long long get_numerator() { return numerator;}

    RNumber operator=(const RNumber& other) {
        numerator = other.numerator;
        denominator = other.denominator;
        normalize();
        return *this;
    }
    RNumber operator+(const RNumber& other) {
        // a/b + c/d = (a*d + b*c) / (b*d)
        if (other.denominator == 0) {
            return *this;
        }
        RNumber result;
        result.numerator = numerator * other.denominator + other.numerator * denominator;
        result.denominator = denominator * other.denominator;
        return result;
    }
    RNumber operator-(const RNumber& other) {
        // a/b - c/d = (a*d - b*c) / (b*d)
        if (other.denominator == 0) {
            return *this;
        }
        RNumber result;
        result.numerator = numerator * other.denominator - other.numerator * denominator;
        result.denominator = denominator * other.denominator;
        return result;
    }
    RNumber operator*(const RNumber& other) {
        // a/b * c/d = (a*c) / (b*d)
        if (other.denominator == 0) {
            return *this;
        }
        RNumber result;
        result.numerator = numerator * other.numerator;
        result.denominator = denominator * other.denominator;
        return result;
    }
    RNumber operator/(const RNumber& other) {
        // a/b / c/d = (a*d) / (b*c)
        if (other.denominator == 0) {
            return *this;
        }
        RNumber result;
        result.numerator = numerator * other.denominator;
        result.denominator = denominator * other.numerator;
    }
};

ostream& operator<<(ostream& out, RNumber& rn) {
    out << rn.numerator << "/" << rn.denominator;
    return out;
}

int main() {
    vector<RNumber> rn;
    for (int i = 1; i<100; ++i) {
        rn.push_back(RNumber(1,i));
    }
    for (auto& it:rn) {
        cout << it << "+";
    }
    cout << endl;

    RNumber tmp;
    for(auto& it : rn) {
        tmp = tmp + it;
        cout << "[" << it <<  ","<< tmp << "] " << endl;
    }
    cout << endl;
    cout << tmp << endl;

    RNumber tmp_mult(1,1);
    for(auto& it : rn) {
        tmp_mult = tmp_mult * it;
        cout << "["<< it <<  ","<< tmp_mult << "] " << endl;
    }
    cout << endl;
    cout << tmp_mult << endl;
}