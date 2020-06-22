cmake_minimum_required(VERSION 3.13)
project(OvRendering)
set(CMAKE_CXX_STANDARD 17)

add_library(OvRendering SHARED "")

target_link_libraries(OvRendering PUBLIC opengl32 glew PRIVATE stb_image assimp OvMaths OvDebug OvTools)
target_include_directories(OvRendering PUBLIC include)
set_property(TARGET OvRendering PROPERTY DEFINE_SYMBOL "OVRENDERING_EXPORT")

add_subdirectory(src/OvRendering)
add_subdirectory(include/OvRendering)