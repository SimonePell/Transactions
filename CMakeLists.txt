cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

# Include directories for headers
include_directories(${CMAKE_SOURCE_DIR})

# Gather all source files **EXCEPT** CMake auto-generated files and main.cpp
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/*.cpp)

# Exclude main.cpp (not needed for testing)
list(REMOVE_ITEM SOURCES ${CMAKE_SOURCE_DIR}/main.cpp)

# Exclude CMake-generated files (CompilerIdCXX)
list(FILTER SOURCES EXCLUDE REGEX "CMakeFiles/.*CompilerIdCXX.*")

# Ensure GoogleTest is fetched
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# Create test executable with `test_main.cpp`
add_executable(runTests ${SOURCES} tests/test_main.cpp tests/test_Account.cpp tests/test_Deposit.cpp tests/test_Withdrawal.cpp)

# Link Google Test
target_link_libraries(runTests PRIVATE gtest gtest_main pthread)

# Register test with CTest
add_test(NAME RunAllTests COMMAND runTests)
