cmake_minimum_required(VERSION 3.13)
project(OvMaths)
set(CMAKE_CXX_STANDARD 17)

add_library(OvMaths SHARED "")

target_link_libraries(OvMaths OvTools)
target_include_directories(OvMaths PUBLIC include)
set_property(TARGET OvMaths PROPERTY DEFINE_SYMBOL "OVMATHS_EXPORT")

add_subdirectory(src/OvMaths)
add_subdirectory(include/OvMaths)