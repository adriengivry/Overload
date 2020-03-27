cmake_minimum_required(VERSION 3.13)
project(OvTools)
set(CMAKE_CXX_STANDARD 17)

add_library(OvTools "")

target_include_directories(OvTools PUBLIC include)

add_subdirectory(src/OvTools)
add_subdirectory(include/OvTools)