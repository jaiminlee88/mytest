import demo_pybind

print(demo_pybind.myadd(1, 2))

myClass1 = demo_pybind.myClass()
myClass1.printName()

myClass2 = demo_pybind.myClass("myClass2")
myClass2.printName()

print(myClass2.add(1, 2))

myClass2.setName("lily")
print(myClass2.sayHello("jack", 123))
