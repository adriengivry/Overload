cmake_minimum_required(VERSION 3.13)
project(OvMaths)
set(CMAKE_CXX_STANDARD 17)

add_library(OvMaths SHARED "")

target_include_directories(OvMaths PUBLIC include)
target_compile_definitions(OvMaths PRIVATE OVMATHS_EXPORT PUBLIC PLATFORM_WINDOWS)

add_subdirectory(src/OvMaths)
add_subdirectory(include/OvMaths)