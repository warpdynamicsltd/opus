cmake_minimum_required(VERSION 3.10)
project(MyCppProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(include include/grammar)

# Automatically gather all .cpp files under src/
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)

add_executable(main ${SRC_FILES})

add_library(opus_lib STATIC src/grammar/grammar.cpp)
target_include_directories(opus_lib PUBLIC include)

# Enable testing for CTest
enable_testing()

# Add the tests subdirectory
add_subdirectory(tests)
set(CTEST_OUTPUT_ON_FAILURE ON)
add_test(NAME tests COMMAND tests)