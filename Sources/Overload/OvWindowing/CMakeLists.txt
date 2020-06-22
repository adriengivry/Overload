cmake_minimum_required(VERSION 3.13)
project(OvWindowing)
set(CMAKE_CXX_STANDARD 17)

add_library(OvWindowing SHARED "")

target_link_libraries(OvWindowing stb_image glfw OvTools)
target_include_directories(OvWindowing PUBLIC include)
set_property(TARGET OvWindowing PROPERTY DEFINE_SYMBOL "OVWINDOWING_EXPORT")

add_subdirectory(src/OvWindowing)
add_subdirectory(include/OvWindowing)