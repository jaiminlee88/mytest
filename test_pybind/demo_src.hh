#include <string>
#include <iostream>
int myadd(int a, int b);

class myClass {
public:
    myClass();
    myClass(std::string name);
    virtual ~myClass(){};
    std::string getName() const { return name; }
    void setName(std::string name) { this->name = name; }
    void printName() { std::cout << name << std::endl; }
    template <typename T>
    T add(T a, T b) {
        return a + b;
    }
    std::string sayHello(std::string a, int b);
private:
    std::string name;
};