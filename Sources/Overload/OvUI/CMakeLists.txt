cmake_minimum_required(VERSION 3.13)

add_library(OvUI SHARED "")

target_link_libraries(OvUI glew glfw opengl32 OvTools OvMaths)
target_include_directories(OvUI PUBLIC include)
set_property(TARGET OvUI PROPERTY DEFINE_SYMBOL "OVUI_EXPORT")

add_subdirectory(src/OvUI)
add_subdirectory(include/OvUI)