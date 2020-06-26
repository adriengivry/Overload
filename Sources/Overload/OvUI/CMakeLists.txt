cmake_minimum_required(VERSION 3.13)
project(OvUI)
set(CMAKE_CXX_STANDARD 17)

add_library(OvUI SHARED "")

target_link_libraries(OvUI glew glfw opengl32 OvTools OvMaths)
target_include_directories(OvUI PUBLIC include)
set_property(TARGET OvUI PROPERTY DEFINE_SYMBOL "OVUI_EXPORT")

add_subdirectory(src/OvUI)
add_subdirectory(include/OvUI)

if(OVERLOAD_BUILD)
	install(DIRECTORY									DESTINATION ${OVERLOAD_BUILD_DIR}/OvUI/bin)
	install(DIRECTORY									DESTINATION ${OVERLOAD_BUILD_DIR}/OvUI/lib)

	install(FILES "$<TARGET_FILE:OvUI>"					DESTINATION ${OVERLOAD_BUILD_DIR}/OvUI/bin)
	install(DIRECTORY ./include							DESTINATION ${OVERLOAD_BUILD_DIR}/OvUI)
	install(FILES "$<TARGET_FILE_DIR:OvUI>/OvUI.lib"	DESTINATION ${OVERLOAD_BUILD_DIR}/OvUI/lib)
endif()