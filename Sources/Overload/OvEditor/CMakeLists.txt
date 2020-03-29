cmake_minimum_required(VERSION 3.13)
project(OvEditor)
set(CMAKE_CXX_STANDARD 17)

add_executable(OvEditor "")

target_link_libraries(OvEditor OvAnalytics OvDebug OvCore OvAudio OvRendering OvWindowing OvTools OvMaths OvUI OvPhysics)
target_include_directories(OvEditor PUBLIC include)

add_subdirectory(src/OvEditor)
add_subdirectory(include/OvEditor)

install(FILES layout.ini DESTINATION "${OVERLOAD_BUILD_DIR}/Config")
install(DIRECTORY "${OVERLOAD_RESOURCES_DIR}/" DESTINATION "${OVERLOAD_BUILD_DIR}/Data")
