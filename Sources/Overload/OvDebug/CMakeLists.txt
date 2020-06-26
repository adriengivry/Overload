cmake_minimum_required(VERSION 3.13)
project(OvDebug)
set(CMAKE_CXX_STANDARD 17)

add_library(OvDebug SHARED "")

target_link_libraries(OvDebug OvTools)
target_include_directories(OvDebug PUBLIC include)
set_property(TARGET OvDebug PROPERTY DEFINE_SYMBOL "OVDEBUG_EXPORT")

add_subdirectory(src/OvDebug)
add_subdirectory(include/OvDebug)

if(OVERLOAD_BUILD)
	install(DIRECTORY											DESTINATION ${OVERLOAD_BUILD_DIR}/OvDebug/bin)
	install(DIRECTORY											DESTINATION ${OVERLOAD_BUILD_DIR}/OvDebug/lib)

	install(FILES "$<TARGET_FILE:OvDebug>"						DESTINATION ${OVERLOAD_BUILD_DIR}/OvDebug/bin)
	install(DIRECTORY ./include									DESTINATION ${OVERLOAD_BUILD_DIR}/OvDebug)
	install(FILES "$<TARGET_FILE_DIR:OvDebug>/OvDebug.lib"		DESTINATION ${OVERLOAD_BUILD_DIR}/OvDebug/lib)
endif()