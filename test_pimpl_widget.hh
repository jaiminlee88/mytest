#include <memory>                     //为了使用std::unique_ptr

// 不需要的头文件移动到cc文件中
class Widget {                       //仍然在“widget.h”中
public:
    Widget();
    ~Widget(); //析构函数在后面会分析
private:
    struct Impl;                    //声明一个 实现结构体, 不用考虑实现细节 不完整类型（incomplete type）
    std::unique_ptr<Impl> pImpl;                    //以及指向它的指针
};
