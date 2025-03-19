cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Gather all source files
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/*.cpp)
list(FILTER SOURCES EXCLUDE REGEX "CMakeFiles/.*CompilerIdCXX.*")

# Main executable
list(REMOVE_ITEM SOURCES ${CMAKE_SOURCE_DIR}/tests/test_main.cpp)  # Exclude test main from main program
add_executable(main ${SOURCES} main.cpp)

# Test executable
add_executable(runTests 
    tests/test_main.cpp 
    tests/test_Account.cpp 
    tests/test_Deposit.cpp 
    tests/test_Withdrawal.cpp 
    Account.cpp 
    Deposit.cpp 
    Withdrawal.cpp 
    Persona.cpp
)

# Link libraries
target_link_libraries(runTests PRIVATE gtest gtest_main pthread)
target_link_libraries(main PRIVATE gtest gtest_main pthread)

# Register tests
add_test(NAME RunAllTests COMMAND runTests)
