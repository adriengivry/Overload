cmake_minimum_required(VERSION 3.13)
project(OvDebug)
set(CMAKE_CXX_STANDARD 17)

add_library(OvDebug "")

target_link_libraries(OvDebug OvTools::OvTools)

target_include_directories(OvDebug PUBLIC include)

add_subdirectory(src/OvDebug)
add_subdirectory(include/OvDebug)