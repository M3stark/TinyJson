cmake_minimum_required(VERSION 2.6)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall")
include_directories(../src)

add_library(json ../src/json.cpp)
add_library(parse ../src/parse.cpp)
add_library(json_val ../src/json_val.cpp)
enable_testing()
add_executable(Test test.cpp)
target_link_libraries(Test json parse json_val /usr/lib/libgtest.so /usr/lib/libgtest_main.so -pthread)
add_test(NAME gtest COMMAND gtest)

add_executable(jsonchecker jsonchecker.cpp)
target_link_libraries(jsonchecker json parse json_val)
