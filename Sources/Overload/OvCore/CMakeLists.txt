cmake_minimum_required(VERSION 3.13)
project(OvCore)
set(CMAKE_CXX_STANDARD 17)

add_library(OvCore SHARED "")

target_link_libraries(OvCore lua OvAnalytics OvAudio OvDebug OvMaths OvPhysics OvRendering OvTools OvUI OvWindowing)
target_include_directories(OvCore PUBLIC include)
target_compile_definitions(OvCore PRIVATE _DISABLE_EXTENDED_ALIGNED_STORAGE)
set_property(TARGET OvCore PROPERTY DEFINE_SYMBOL "OVCORE_EXPORT")
target_compile_options(OvCore PRIVATE /bigobj)

add_subdirectory(src/OvCore)
add_subdirectory(include/OvCore)