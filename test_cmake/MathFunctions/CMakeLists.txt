# MathFunctions/CMakeLists.txt
# static
# add_library(MathFunctions MathFunctions.cpp)
# dynamic

# target_compile_definitions()
# target_compile_options()
# target_include_directories()
# target_link_libraries()


# 生成动态库库文件
add_library(MathFunctions SHARED MathFunctions.cpp) # MathFunctions是库名称 SHARED表示动态库，STATIC表示静态库 也可以不写，因为默认就是静态库，MathFunctions.cpp是源文件

# 生成静态库文件
# add_liabrary(MathFunctions STATIC MathFunctions.cpp)

# 生成对象库文件
# 对象库类似于静态库，但不生成 .a 文件，只由 CMake 记住该库生成了哪些对象文件。
# 可以先用mylib.cpp生成一个对象库（用关键字 OBJECT），然后再链接：
# add_library(MathFunctions OBJECT MathFunctions.cpp)




# INTERFACE是指消费者需要、但生产者不需要的那些东西，写上这个，只要是链接了 MathFunctions 库，就会自动包含 MathFunctions 所在目录的头文件
target_include_directories(MathFunctions INTERFACE
                          ${CMAKE_CURRENT_SOURCE_DIR} # CMAKE_CURRENT_SOURCE_DIR 表示 MathFunctions 库所在目录。
                        )