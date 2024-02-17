#include<iostream>
#include<exception>

void abc() {
    std::cout << "run abc..." << std::endl;
    throw "error xxxxabcabc.";
}

void def() noexcept { // 推荐使用noexcept关键字来声明不抛出异常，这样编译器可以优化代码，而且是极尽所能优化
// void def() noexcept(false) { // 关闭异常抛出，编译器可以优化代码。
    std::cout << "run def..." << std::endl;
    throw "error xxxxdefdef.";
}

void opq() throw() { // 不抛出异常，该规范已经被C++11废弃，不建议使用。
    std::cout << "run opq..." << std::endl;
    throw "error opq."; // valid,程序会调用std::terminate()函数来终止程序。
}


class bad_msg{
public:
    bad_msg(int num, std::string msg) : err_num(num), err_msg(msg) {}
    friend std::ostream& operator<<(std::ostream& out, bad_msg& it);

public:
    int err_num;
    std::string err_msg;
};

std::ostream& operator<<(std::ostream& out, bad_msg& it) {
    return out << "num=" << it.err_num << " msg:" << it.err_msg;
}

void ghi() {
    std::cout << "run ghi..." << std::endl;
    throw bad_msg(1, "hahahahaha");
}

void jkl() {
    std::cout << "run jkl..." << std::endl;
    throw 5;
}

class bad_msg_exception: public std::exception {
public:
    bad_msg_exception(int num, std::string msg) : err_num(num), err_msg(msg += " [exception]") {}
    friend std::ostream& operator<<(std::ostream& out, bad_msg& it);
    const char* what() {
        std::string ret = std::string("err_num:") + std::to_string(err_num) + " err_msg:" + err_msg;
        return "hahaaaaaaaaaaaaaaaaaaaaa";
        // return ret.c_str();
    }
public:
    int err_num;
    std::string err_msg;
};

void lmn() {
    std::cout << "run lmn..." << std::endl;
    throw bad_msg_exception(100000, "hey hey hey");
}

void rst() noexcept(false) {
    std::cout << "run rst..." << std::endl;
    abc(); // 会抛出异常，noexcept并不会阻止异常的抛出。
}

void rst1() noexcept {
    std::cout << "run rst1..." << std::endl;
    abc(); // 会抛出异常，noexcept并不会阻止异常的抛出。
}

int main() {
    try{
        // abc();
        // ghi();
        // def();
        // jkl();
        // lmn();
        // rst();
        rst1();
    } catch (const char* msg){
        std::cout << "catch exception: " << msg << std::endl;
    } catch (bad_msg msg){
        std::cout << "catch exception: " << msg << std::endl;
    } catch (bad_msg_exception& e){
        std::cout << "catch exception: " << e.err_num << " err_msg:" << e.err_msg <<  e.what() << std::endl;
    } catch (...) {
        std::cout << "catch default exception." << std::endl;
    }
    // opq();
}

// #include <iostream>
// #include <exception>
// using namespace std;
 
// struct MyException : public exception
// {
//   const char * what () const throw () // 表示不抛出异常
//   {
//     return "C++ Exception";
//   }
// };
 
// int main()
// {
//   try
//   {
//     throw MyException();
//   }
//   catch(MyException& e)
//   {
//     std::cout << "MyException caught" << std::endl;
//     std::cout << e.what() << std::endl;
//   }
//   catch(std::exception& e)
//   {
//     //其他的错误
//   }
// }
