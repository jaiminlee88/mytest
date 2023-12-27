cmake_minimum_required(VERSION 3.15) # CMake version check 必须

set(PROJECT_NAME test_oidgen)
# set the project name
project(${PROJECT_NAME} VERSION 1.0.1) # 项目名称和版本号 必须 Tutorial（名称）VERSION 1.0.1（版本号，可不写）

set(SRC ${PROJECT_NAME}.cc)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_LIBTRADEKIT "Use libtrdkit" ON)

string(TIMESTAMP COMPILE_TIME %Y%m%d-%H%M%S)
configure_file(TutorialConfig.hh.in TutorialConfig.hh)

if(USE_LIBTRADEKIT)
    # add_subdirectory(/home/jmli/repo/libtradekit/ ${CMAKE_CURRENT_SOURCE_DIR})
    list(APPEND EXTRA_LIBS /home/jmli/repo/libtradekit/libtrdkit.a)
    list(APPEND EXTRA_INCLUDES /home/jmli/repo/libtradekit/)
endif()

# add the executable
add_executable(${PROJECT_NAME} ${SRC})

# INTERFACE表示消费者需要生产者不需要，PRIVATE表示消费者不需要生产者需要，PUBLIC 表示消费者和生产者都需要
# target_compile_definitions()
# target_compile_options()
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC
                           ${PROJECT_BINARY_DIR}
                           ${EXTRA_INCLUDES}
                           )

message("EXTRA_LIBS: ${EXTRA_LIBS}")
message("build ${PROJECT_NAME} done. ${PROJECT_BINARY_DIR} ${CMAKE_CXX_STANDARD_REQUIRED}")
message("1111111111111111111")