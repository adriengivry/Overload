cmake_minimum_required(VERSION 3.13)
project(OvAnalytics)
set(CMAKE_CXX_STANDARD 17)

add_library(OvAnalytics SHARED "")

target_include_directories(OvAnalytics PUBLIC include)
target_compile_definitions(OvAnalytics PRIVATE OVANALYTICS_EXPORT PUBLIC PLATFORM_WINDOWS)

add_subdirectory(src/OvAnalytics)
add_subdirectory(include/OvAnalytics)