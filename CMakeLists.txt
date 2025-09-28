cmake_minimum_required(VERSION 3.10)
project(Opus)

set(CMAKE_C_COMPILER gcc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")

include_directories(include include/grammar include/abstract_term include/utils)

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