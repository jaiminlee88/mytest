#include <ostream>
#include <iostream>
#include <cmath>
using namespace std;

class Solution {
public:
    /**
     * @param source: A source string
     * @param target: A target string
     * @return: An integer as index
     */
    // 运算
    // 1.模p加法：(a + b) % p = (a%p + b%p) % p

    // 2.模p减法：(a - b) % p = (a%p - b%p) % p

    // 3.模p乘法：(a * b) % p = ((a % p)*(b % p)) % p

    // 4.幂模p ：(a^b) % p = ((a % p)^b) % p

    // 5.模运算满足结合律、交换律和分配律。

    // 同余的定义
    // a≡b (mod n) 表示a和b模n同余，即a和b除以n的余数相等。

    // 同余的性质
    // 1.反身性：a≡a (mod m)

    // 2.对称性：若a≡b(mod m)，则b≡a (mod m)

    // 3.传递性：若a≡b(mod m)，b≡c(mod m)，则a≡c(mod m)

    // 4.同余式相加减：若a≡b(mod m)，c≡d(mod m)，则a±c≡b±d(mod m)

    // 5.同余式相乘：若a≡b(mod m)，c≡d(mod m)，则ac≡bd(mod m)

    int strStr2(string &source, string &target) {
        // write your code here
        if (source.size() < target.size()) {
            return -1;
        }
        if (target.size() == 0) {
            return 0;
        }

        // long long hash_mod = pow(10, target.size() -1);
        long long hash_mod = 101;
        long long hash_mul = 256;
        int max_hash_mul = 1;
        for (int i = 0; i < target.size() - 1; i++) { // 256^(m-1) % 101，免得后面再算
            max_hash_mul = (max_hash_mul * hash_mul) % hash_mod;
        }
        long long target_hash = 0;
        long long source_hash = 0;
        for (int i = 0; i < target.size(); i++) {
            target_hash = (target_hash * hash_mul + target[i]) % hash_mod;
            source_hash = (source_hash * hash_mul + source[i]) % hash_mod;
        }
        for (int lpos = 0; lpos <= (source.size() - target.size()); lpos++) {
            // cout << "before target_hash: " << target_hash << " source_hash: " << source_hash << endl;
            if (target_hash == source_hash) {
                bool found_flag = true;
                for (int i = 0; i < target.size(); i++) {
                    if(source[lpos + i] != target[i]) {
                        found_flag = false;
                        break;
                    }
                }
                if (found_flag) {
                    return lpos;
                }
            }
            source_hash = ((source_hash - source[lpos] * max_hash_mul) * hash_mul
                            + source[lpos + target.size()]) % hash_mod;
            if (source_hash < 0) {
                source_hash += hash_mod;
            }
            // cout << "after  target_hash: " << target_hash << " source_hash: " << source_hash << endl;
        }
        return -1;
    }
};

#include <cstring>
#define BASE 256
#define MODULUS 101
void RabinKarp(char t[], char p[])
{
    cout << "origin:" << t << " target:" << p << endl;
    int t_len = strlen(t);
    int p_len = strlen(p); // 目标串长度

    // 哈希滚动之用, BASE^(m-1), 如m^31
    int h = 1;
    for (int i = 0; i < p_len - 1; i++) {
        h = (h * BASE) % MODULUS; // 最大256， a*x^(m-1), max(a) = 256, max(x) = 256, max(a*x^(m-1)) = 255*256^30 < 256^31
    }

    int t_hash = 0;
    int p_hash = 0;
    for (int i = 0; i < p_len; i++) {
        // 起始的哈希值
        t_hash = (BASE * t_hash + t[i]) % MODULUS;
        p_hash = (BASE * p_hash + p[i]) % MODULUS;
    }

    int i = 0;
    while (i <= t_len - p_len)
    {
        cout << "i: " << i << " t_hash: " << t_hash << " p_hash: " << p_hash << endl;
         // 考虑到哈希碰撞的可能性，还需要用 memcmp 再比对一下
        if (t_hash == p_hash && memcmp(p, t + i, p_len) == 0)
            cout << p << " is found at index " << i << endl;

        // 哈希滚动
        t_hash = (BASE * (t_hash - t[i] * h) + t[i + p_len]) % MODULUS;

        // 防止出现负数
        if (t_hash < 0)
            t_hash = t_hash + MODULUS;

        i++;
    }
}

int main() {
    // 大字符串中找子串
    // 可以KMP算法，也可以暴力算法，也可Rabin-Karp算法（hash func）
    Solution s;
    string source = "abcdabcdefg";
    string target = "bcd";
    int    pos = s.strStr2(source, target);
    std::cout << "===src: " << source << " tar: " << target << " pos: " << pos << std::endl;

    source = "";
    target = "";
    pos = s.strStr2(source, target);
    std::cout << "===src: " << source << " tar: " << target << " pos: " << pos << std::endl;

    source = "abcabc";
    target = "cab";
    pos = s.strStr2(source, target);
    std::cout << "===src: " << source << " tar: " << target << " pos: " << pos << std::endl;

    source = "abcabc";
    target = "b";
    pos = s.strStr2(source, target);
    std::cout << "===src: " << source << " tar: " << target << " pos: " << pos << std::endl;

    source = "abcabc";
    target = "d";
    pos = s.strStr2(source, target);
    std::cout << "===src: " << source << " tar: " << target << " pos: " << pos << std::endl;

    source = "abcabc";
    target = "abcac";
    pos = s.strStr2(source, target);
    std::cout << "===src: " << source << " tar: " << target << " pos: " << pos << std::endl;

    source = "abcabc";
    target = "";
    pos = s.strStr2(source, target);
    std::cout << "===src: " << source << " tar: " << target << " pos: " << pos << std::endl;

    source = "abcabc";
    target = "abcabcabc";
    pos = s.strStr2(source, target);
    std::cout << "===src: " << source << " tar: " << target << " pos: " << pos << std::endl;

    source = "abcccabc";
    target = "cc";
    pos = s.strStr2(source, target);
    std::cout << "===src: " << source << " tar: " << target << " pos: " << pos << std::endl;

    source = "hriuwheujwlhadfhsaljfhjahwjehjwhiuehyuwiehyiuwahdjsahjfhajshfjwhuhejwhjehwjehjwhejwhejwhejwhejwhejhwjeh";
    target = "riuwheujwlhadfhsaljfhjahwjehjwhiuehyuwiehyiuwahdjsahjfhajshfjwhuhejwhjehwjehjwhejwhej";
    pos = s.strStr2(source, target);
    std::cout << "===src: " << source << " tar: " << target << " pos: " << pos << std::endl;

    // source = "abcabc";
    // target = "b";
    // const int len = source.size();
    // char sc[len + 1];
    // strcpy(sc, source.c_str());
    // char tc[len + 1];
    // strcpy(tc, target.c_str());
    // RabinKarp(sc, tc);


    return 0;
}