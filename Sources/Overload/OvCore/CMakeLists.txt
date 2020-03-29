cmake_minimum_required(VERSION 3.13)
project(OvCore)
set(CMAKE_CXX_STANDARD 17)

add_library(OvCore SHARED "")

target_link_libraries(OvCore lua OvAnalytics OvAudio OvDebug OvMaths OvPhysics OvRendering OvTools OvUI OvWindowing)
target_include_directories(OvCore PUBLIC include)
target_compile_definitions(OvCore PRIVATE OVCORE_EXPORT _DISABLE_EXTENDED_ALIGNED_STORAGE PUBLIC PLATFORM_WINDOWS)
target_compile_options(OvCore PRIVATE /bigobj)

add_subdirectory(src/OvCore)
add_subdirectory(include/OvCore)