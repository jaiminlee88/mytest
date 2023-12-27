#include<iostream>

int main() {
    std::cout << "hello" << std::endl;
    std::string a = "abc\nefg\\n";
    std::cout << "a=" << a << std::endl;
    std::string raw = R"(
        hello \n 
        hahahaha
    )";
    std::cout << "raw\n" << raw << std::endl;
}