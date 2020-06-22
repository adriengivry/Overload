cmake_minimum_required(VERSION 3.13)
project(OvAudio)
set(CMAKE_CXX_STANDARD 17)

add_library(OvAudio SHARED "")

target_link_libraries(OvAudio irrKlang OvMaths OvTools OvDebug)
target_include_directories(OvAudio PUBLIC include)
set_property(TARGET OvAudio PROPERTY DEFINE_SYMBOL "OVAUDIO_EXPORT")

add_subdirectory(src/OvAudio)
add_subdirectory(include/OvAudio)