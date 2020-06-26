cmake_minimum_required(VERSION 3.13)
project(OvAudio)
set(CMAKE_CXX_STANDARD 17)

add_library(OvAudio SHARED "")

target_link_libraries(OvAudio irrKlang OvMaths OvTools OvDebug)
target_include_directories(OvAudio PUBLIC include)
set_property(TARGET OvAudio PROPERTY DEFINE_SYMBOL "OVAUDIO_EXPORT")

add_subdirectory(src/OvAudio)
add_subdirectory(include/OvAudio)

if(OVERLOAD_BUILD)
	install(DIRECTORY											DESTINATION ${OVERLOAD_BUILD_DIR}/OvAudio/bin)
	install(DIRECTORY											DESTINATION ${OVERLOAD_BUILD_DIR}/OvAudio/lib)

	install(FILES "$<TARGET_FILE:OvAudio>"						DESTINATION ${OVERLOAD_BUILD_DIR}/OvAudio/bin)
	install(DIRECTORY ./include									DESTINATION ${OVERLOAD_BUILD_DIR}/OvAudio)
	install(FILES "$<TARGET_FILE_DIR:OvAudio>/OvAudio.lib"		DESTINATION ${OVERLOAD_BUILD_DIR}/OvAudio/lib)
endif()