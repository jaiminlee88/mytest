#include <memory>                     //为了使用std::unique_ptr
#include <ostream>
// 不需要的头文件移动到cc文件中
class abc;          //前向声明一个类, 不用考虑实现细节 不完整类型（incomplete type）
class Widget {                       //仍然在“widget.h”中
public:
    Widget();
    ~Widget(); //析构函数在后面会分析
private:
    void func1(abc* g); // 使用指针，不需要包含头文件，只需要前向声明
    void func2(abc& g); // 使用引用，不需要包含头文件，只需要前向声明
    // void func3(abc g){
    //     std::cout << "Widget::func2(abc g) g->a=" << g.a << std::endl;
    // }       // 使用了定义式，需要包含头文件，不包含则报错
    struct Impl;                    //前向声明一个 实现结构体, 不用考虑实现细节 不完整类型（incomplete type）
    std::unique_ptr<Impl> pImpl;    //以及指向它的指针
};
