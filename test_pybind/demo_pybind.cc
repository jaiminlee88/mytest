//example.cpp
#include <pybind11/pybind11.h>
#include "demo_src.hh"
namespace py = pybind11;

//example是包的名称，add是包中函数的名称
//PYBIND11_MODULE是一个宏，m是py::module类型
PYBIND11_MODULE(demo_pybind, m) {
    m.doc() = "pybind11 demo_pybind plugin"; // optional module docstring

    m.def("myadd", &myadd, "A function which adds two numbers");

    py::class_<myClass>(m, "myClass")
        .def(py::init<>())
        .def(py::init<std::string>())
        .def("getName", &myClass::getName)
        .def("setName", &myClass::setName)
        .def("printName", &myClass::printName)
        .def("add", &myClass::add<int>)
        .def("sayHello", &myClass::sayHello, py::arg("a")="", py::arg("b"));
}