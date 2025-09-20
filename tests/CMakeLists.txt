# tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

find_package(GTest REQUIRED)

# Create executable for tests
add_executable(tests
    test_grammar.cpp
)

# Link the test executable against Google Test and your project library
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main opus_lib)



