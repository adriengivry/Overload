cmake_minimum_required(VERSION 3.13)
project(OvGame)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_executable(OvGame "")
else()
	add_executable(OvGame WIN32 "")
endif()

target_link_libraries(OvGame OvAnalytics OvDebug OvCore OvAudio OvRendering OvWindowing OvTools OvMaths OvUI OvPhysics)
target_include_directories(OvGame PUBLIC include)
target_compile_definitions(OvGame PRIVATE OVGAME_EXPORT PUBLIC PLATFORM_WINDOWS)

add_subdirectory(src/OvGame)
add_subdirectory(include/OvGame)
