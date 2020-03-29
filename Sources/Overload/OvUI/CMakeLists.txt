cmake_minimum_required(VERSION 3.13)
project(OvUI)
set(CMAKE_CXX_STANDARD 17)

add_library(OvUI SHARED "")

target_link_libraries(OvUI glew glfw opengl32 OvTools OvMaths)
target_include_directories(OvUI PUBLIC include)
target_compile_definitions(OvUI PRIVATE OVUI_EXPORT PUBLIC PLATFORM_WINDOWS)

add_subdirectory(src/OvUI)
add_subdirectory(include/OvUI)