cmake_minimum_required(VERSION 3.13)

add_library(OvAnalytics SHARED "")

target_link_libraries(OvAnalytics OvTools)
target_include_directories(OvAnalytics PUBLIC include)
set_property(TARGET OvAnalytics PROPERTY DEFINE_SYMBOL "OVANALYTICS_EXPORT")

add_subdirectory(src/OvAnalytics)
add_subdirectory(include/OvAnalytics)