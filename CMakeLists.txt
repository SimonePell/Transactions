cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(third_party/googletest)

add_executable(test_example tests/test_example.cpp)

target_link_libraries(test_example PRIVATE gtest gtest_main)

add_test(NAME ExampleTest COMMAND test_example)
