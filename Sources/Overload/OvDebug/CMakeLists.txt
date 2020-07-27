cmake_minimum_required(VERSION 3.13)

add_library(OvDebug SHARED "")

target_link_libraries(OvDebug OvTools)
target_include_directories(OvDebug PUBLIC include)
set_property(TARGET OvDebug PROPERTY DEFINE_SYMBOL "OVDEBUG_EXPORT")

add_subdirectory(src/OvDebug)
add_subdirectory(include/OvDebug)