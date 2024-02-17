#include <iostream>
#include "./test_pimpl_widget.hh"


int main() {
    // 文件编译依存关系降到最低，
    // 1、将声明和定义分离，尽量使用前向声明（前向声明不需要包含头文件），减少头文件依赖
    // 2、以指针或者引用方式传递对象，减少头文件依赖，如果用定义对象，就要包含头文件
    // 3、尽量以class前向声明 代替 class定义式
    //    void func(Date d){} // 传值，属于定义式，需要包含头文件
    // 4、以纯虚函数的abstract class代替class声明式
    // 注意，用此类方式，在运行中可能有寻址等各种开销（virtual 表），所以不要滥用

    Widget w1;
    std::cout << "w1" << std::endl;
}