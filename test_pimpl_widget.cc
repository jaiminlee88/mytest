
#include "./test_pimpl_widget.hh"
#include "./test_pimpl_gadget.hh"

#if __cplusplus <= 201103L
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params) {
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
#endif

struct Widget::Impl {
    std::string name;           //Widget::Impl类型的定义
    std::vector<double> data;
    Gadget g1,g2,g3;
};

Widget::Widget() : pImpl(new Widget::Impl()) {
    //在“widget.cpp”中
}


// 如果不写，会默认生成一个，因为有unique_ptr，所以会默认生成一些代码，其中一段是static_assert检查是否完整类型，如果不是，就会报错 ,暗含inline
// static_assert(sizeof(_Tp)>0,invalid application of ‘sizeof’ to incomplete type ‘Widget::Impl’
Widget::~Widget() {
    //在“widget.cpp”中 ,
    // 不要使用默认析构函数，会增加一些代码，其中一段是static_assert检查是否完整类型，如果不是，就会报错 ,暗含inline
}
// Widget::~Widget() = default;        //同上述代码效果一致
