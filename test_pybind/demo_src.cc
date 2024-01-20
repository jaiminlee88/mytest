#include "demo_src.hh"

int myadd(int i, int j) {
    return i + j;
}

myClass::myClass() : name("default") {

}

myClass::myClass(std::string name) : name(name) {

}

std::string myClass::sayHello(std::string a, int b) {
    std::string ret = name + " says hello to " + a + " " + std::to_string(b);
    return ret;
}