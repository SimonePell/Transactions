cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

# Include directories for headers
include_directories(${CMAKE_SOURCE_DIR})

# Gather all source files **except** `CMakeFiles`-generated files
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/*.cpp)
list(FILTER SOURCES EXCLUDE REGEX "CMakeFiles/.*CompilerIdCXX.*")

# Ensure GoogleTest is fetched
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# Ensure GoogleTest include directories are available
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# **🔥 Create the main executable (without test files)**
list(REMOVE_ITEM SOURCES ${CMAKE_SOURCE_DIR}/tests/test_main.cpp)  # Exclude test_main.cpp
add_executable(main ${SOURCES} main.cpp)  # Now, main.cpp is only for `main`

# **🔥 Create the test executable separately**
add_executable(runTests tests/test_main.cpp tests/test_Account.cpp tests/test_Deposit.cpp tests/test_Withdrawal.cpp)
target_link_libraries(runTests PRIVATE gtest gtest_main pthread)

# Register test with CTest
add_test(NAME RunAllTests COMMAND runTests)
