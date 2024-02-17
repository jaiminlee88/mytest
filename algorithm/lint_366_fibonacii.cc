#include <iostream>

using namespace std;

class Solution {
public:
    /**
     * @param n: an integer
     * @return: an integer f(n)
     */
    int fibonacci(int n) {
        // write your code here
        if (n <= 0) {
            return -1;
        }
        if (n == 1) {
            return 0;
        } else if (n == 2) {
            return 1;
        }
        int i = 0;
        int i_1 = 1;
        for (int j = 3; j <= n; j++) {
            int tmp = i_1;
            i_1 = i_1 + i;
            i = tmp;
        }
        return i_1;
    }

    int fibonacci_recursion(int n) {
        if (n <= 0) {
            return -1;
        }
        if (n == 1) {
            return 0;
        } else if (n == 2) {
            return 1;
        }
        return fibonacci_recursion_core(n - 1, n - 2);
    }
    int fibonacci_recursion_core(int i_1, int i) {
        if (i_1 == 2 && i == 1) {
            return 1;
        }
        if (i_1 == 1 && i == 0) {
            return 1;
        }
        return fibonacci_recursion_core(i_1 - 1, i_1 - 2) + fibonacci_recursion_core(i - 1, i - 2);
    }

    int fibonacci_recursion_1(int n ) {
        if (n <= 2) {
            return n - 1;
        }
        return fibonacci_recursion_1(n - 1) + fibonacci_recursion_1(n - 2);
    }
};

int main () {
    Solution s;
    cout << "[0]=" << s.fibonacci(0) << endl;
    cout << "[1]=" << s.fibonacci(1) << endl;
    cout << "[2]=" << s.fibonacci(2) << endl;
    cout << "[3]=" << s.fibonacci(3) << endl;
    cout << "[4]=" << s.fibonacci(4) << endl;
    cout << "[5]=" << s.fibonacci(5) << endl;
    cout << "[6]=" << s.fibonacci(6) << endl;
    cout << "[7]=" << s.fibonacci(7) << endl;
    cout << "[8]=" << s.fibonacci(8) << endl;
    cout << "[9]=" << s.fibonacci(9) << endl;

    cout << "[0]=" << s.fibonacci_recursion(0) << endl;
    cout << "[1]=" << s.fibonacci_recursion(1) << endl;
    cout << "[2]=" << s.fibonacci_recursion(2) << endl;
    cout << "[3]=" << s.fibonacci_recursion(3) << endl;
    cout << "[4]=" << s.fibonacci_recursion(4) << endl;
    cout << "[5]=" << s.fibonacci_recursion(5) << endl;
    cout << "[6]=" << s.fibonacci_recursion(6) << endl;
    cout << "[7]=" << s.fibonacci_recursion(7) << endl;
    cout << "[8]=" << s.fibonacci_recursion(8) << endl;
    cout << "[9]=" << s.fibonacci_recursion(9) << endl;

    cout << "[0]=" << s.fibonacci_recursion_1(0) << endl;
    cout << "[1]=" << s.fibonacci_recursion_1(1) << endl;
    cout << "[2]=" << s.fibonacci_recursion_1(2) << endl;
    cout << "[3]=" << s.fibonacci_recursion_1(3) << endl;
    cout << "[4]=" << s.fibonacci_recursion_1(4) << endl;
    cout << "[5]=" << s.fibonacci_recursion_1(5) << endl;
    cout << "[6]=" << s.fibonacci_recursion_1(6) << endl;
    cout << "[7]=" << s.fibonacci_recursion_1(7) << endl;
    cout << "[8]=" << s.fibonacci_recursion_1(8) << endl;
    cout << "[9]=" << s.fibonacci_recursion_1(9) << endl;
    return 0;
}
