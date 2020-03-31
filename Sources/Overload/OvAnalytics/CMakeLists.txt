cmake_minimum_required(VERSION 3.13)
project(OvAnalytics)
set(CMAKE_CXX_STANDARD 17)

add_library(OvAnalytics SHARED "")

target_link_libraries(OvAnalytics OvTools)
target_include_directories(OvAnalytics PUBLIC include)
set_property(TARGET OvAnalytics PROPERTY DEFINE_SYMBOL "OVANALYTICS_EXPORT")

add_subdirectory(src/OvAnalytics)
add_subdirectory(include/OvAnalytics)
