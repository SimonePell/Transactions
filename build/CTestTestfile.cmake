# CMake generated Testfile for 
# Source directory: /home/simone/Documents/Università/LabProgrammazione/Transactions
# Build directory: /home/simone/Documents/Università/LabProgrammazione/Transactions/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(ExampleTest "/home/simone/Documents/Università/LabProgrammazione/Transactions/build/test_example")
set_tests_properties(ExampleTest PROPERTIES  _BACKTRACE_TRIPLES "/home/simone/Documents/Università/LabProgrammazione/Transactions/CMakeLists.txt;27;add_test;/home/simone/Documents/Università/LabProgrammazione/Transactions/CMakeLists.txt;0;")
subdirs("third_party/googletest")
