//example.cpp
#include <pybind11/pybind11.h>
namespace py = pybind11;
 
int add(int i, int j) {
    return i + j;
}
//example是包的名称，add是包中函数的名称
//PYBIND11_MODULE是一个宏，m是py::module类型
PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function which adds two numbers");
}