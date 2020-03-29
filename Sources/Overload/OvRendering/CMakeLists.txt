cmake_minimum_required(VERSION 3.13)
project(OvRendering)
set(CMAKE_CXX_STANDARD 17)

add_library(OvRendering SHARED "")

target_link_libraries(OvRendering PUBLIC opengl32 glew PRIVATE stb_image assimp OvMaths OvDebug)
target_include_directories(OvRendering PUBLIC include)
target_compile_definitions(OvRendering PRIVATE OVRENDERING_EXPORT PUBLIC PLATFORM_WINDOWS)

add_subdirectory(src/OvRendering)
add_subdirectory(include/OvRendering)