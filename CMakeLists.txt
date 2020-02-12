cmake_minimum_required(VERSION 3.9)
project(patterns)
set(CMAKE_ROOT_)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include_directories(include)

add_subdirectory(lib/googletest)
add_subdirectory(test)

option(USE_CLANG "Build using Clang++" ON)
if (USE_CLANG)
    set(CMAKE_CXX_COMPILER clang++)
endif(USE_CLANG)
