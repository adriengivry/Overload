cmake_minimum_required(VERSION 3.13)
project(OvGame)
set(CMAKE_CXX_STANDARD 17)

add_executable(OvGame "")

# Build OvGame with a console when in debug.
if(WIN32)
   set_target_properties(OvGame PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   set_target_properties(OvGame PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
   set_target_properties(OvGame PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   set_target_properties(OvGame PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
   set_target_properties(OvGame PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
   set_target_properties(OvGame PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
endif(WIN32)

target_link_libraries(OvGame OvAnalytics OvDebug OvCore OvAudio OvRendering OvWindowing OvTools OvMaths OvUI OvPhysics)
target_include_directories(OvGame PUBLIC include)

add_subdirectory(src/OvGame)
add_subdirectory(include/OvGame)
