# MathFunctions/CMakeLists.txt
# static
# add_library(MathFunctions MathFunctions.cpp)
# dynamic
add_library(MathFunctions SHARED MathFunctions.cpp)

# INTERFACE是指消费者需要、但生产者不需要的那些东西，写上这个，只要是链接了 MathFunctions 库，就会自动包含 MathFunctions 所在目录的头文件
target_include_directories(MathFunctions
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )