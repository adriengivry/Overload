cmake_minimum_required(VERSION 3.13)
project(OvDebug)
set(CMAKE_CXX_STANDARD 17)

add_library(OvDebug SHARED "")

target_link_libraries(OvDebug OvTools)
target_include_directories(OvDebug PUBLIC include)
target_compile_definitions(OvDebug PRIVATE OVDEBUG_EXPORT PUBLIC PLATFORM_WINDOWS)

add_subdirectory(src/OvDebug)
add_subdirectory(include/OvDebug)