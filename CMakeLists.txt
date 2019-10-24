cmake_minimum_required(VERSION 3.12)
project(Hw_3_range)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)

add_executable(Hw_3_range main.cpp)


