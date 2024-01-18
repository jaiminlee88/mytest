#include <iostream>
#include <cmath>

double squareRoot(double x) {
    // 处理边界情况和负数
    if (x < 0) {
        std::cerr << "Error: cannot calculate square root of a negative number." << std::endl;
        return 0.0;
    }
    if (x == 0) {
        return 0.0;
    }

    double guess = x;  // 初始猜测值
    double epsilon = 0.000001;  // 收敛条件

    while (true) {
        double nextGuess = 0.5 * (guess + x / guess);  // 计算新的猜测值

        // 检查收敛条件
        if (std::abs(nextGuess - guess) < epsilon) {
            return nextGuess;  // 返回最终的估计值
        }

        guess = nextGuess;  // 更新猜测值
    }
}

int main() {
    double number;
    std::cout << "Enter a number: ";
    std::cin >> number;

    double result = squareRoot(number);
    std::cout << "Square root of " << number << " is " << result << std::endl;

    std::cout << "pow("<< result << ",2)=" << std::pow(result, 2) << std::endl;
    return 0;
}
