# CMake generated Testfile for 
# Source directory: /home/simone/Documents/Università/LabProgrammazione/Transactions
# Build directory: /home/simone/Documents/Università/LabProgrammazione/Transactions/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(RunAllTests "/home/simone/Documents/Università/LabProgrammazione/Transactions/build/runTests")
set_tests_properties(RunAllTests PROPERTIES  _BACKTRACE_TRIPLES "/home/simone/Documents/Università/LabProgrammazione/Transactions/CMakeLists.txt;46;add_test;/home/simone/Documents/Università/LabProgrammazione/Transactions/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
