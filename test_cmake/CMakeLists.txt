# 示例 https://zhuanlan.zhihu.com/p/500002865
# https://zhuanlan.zhihu.com/p/631234923
cmake_minimum_required(VERSION 3.15) # CMake version check 必须

# set the project name
project(Tutorial VERSION 1.0.1) # 项目名称和版本号 必须 Tutorial（名称）VERSION 1.0.1（版本号，可不写）

set(SRC tutorial.cc) # 设置源文件，第一个SRC是变量名，后面可以跟多个源文件
set(CMAKE_CXX_STANDARD 20) # CMAKE_CXX_STANDARD表示C++标准，CMAKE_CXX_STANDARD_REQUIRED表示是否强制使用CMAKE_CXX_STANDARD指定的标准
set(CMAKE_CXX_STANDARD_REQUIRED True) # CMAKE_CXX_STANDARD_REQUIRED表示是否强制使用CMAKE_CXX_STANDARD指定的标准

# USE_MYMATH可以在cc文件中访问到
option(USE_MYMATH "Use tutorial provided math implementation" ON) # option添加选项USE_MYMATH，表示是否使用自定义的数学库，ON表示使用，OFF表示不使用，

string(TIMESTAMP COMPILE_TIME "%Y%m%d %H:%M:%S") #时间戳以指定格式 %Y%m%d %H:%M:%S 保存到 COMPILE_TIME 变量中

# TutorialConfig.hh.in表示模板文件，TutorialConfig.hh表示按照模板生成的文件
configure_file(TutorialConfig.hh.in TutorialConfig.hh) # 将TutorialConfig.hh.in中的@<name>@替换成对应的值，生成TutorialConfig.hh

if(USE_MYMATH)
    add_subdirectory(MathFunctions) # 要使用该库，先添加子目录路径，子目录中必须有CMakeLists.txt！！！（库生成方式在子目录该文件中定义）
    list(APPEND EXTRA_LIBS MathFunctions) # 追加元素 MathFunctions 到 EXTRA_LIBS，EXTRA_LIBS是变量名，MathFunctions是库文件名
    # list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR}/MathFunctions) # 追加列表元素 ${PROJECT_SOURCE_DIR}/MathFunctions 到 EXTRA_INCLUDES，EXTRA_INCLUDES是变量名，${PROJECT_SOURCE_DIR}/MathFunctions是头文件目录
endif()

# add the executable,生成可执行文件
add_executable(${PROJECT_NAME} ${SRC}) # PROJECT_NAME已经project为Tutorial，获取源文件SRC

# INTERFACE表示消费者需要生产者不需要，PRIVATE表示消费者不需要生产者需要，PUBLIC 表示消费者和生产者都需要
# target_compile_definitions()
# target_compile_options()
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS}) # 链接对象库，PUBLIC表示消费者和生产者都需要
target_include_directories(${PROJECT_NAME} PUBLIC # 添加头文件目录 PROJECT_NAME，添加头文件目录，PUBLIC表示消费者和生产者都需要
                           ${PROJECT_BINARY_DIR} # PROJECT_BINARY_DIR，表示编译目录，即build
                        #    ${EXTRA_INCLUDES}
                           )
# # 添加编译选项 -Wall
# target_compile_options(hello PUBLIC -Wall)

message("%[${COMPILE_TIME}] build ${PROJECT_NAME} done. ${PROJECT_BINARY_DIR} ${CMAKE_CXX_STANDARD_REQUIRED}")