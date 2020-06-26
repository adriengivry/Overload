cmake_minimum_required(VERSION 3.13)

add_library(OvRendering SHARED "")

target_link_libraries(OvRendering PUBLIC opengl32 glew PRIVATE stb_image assimp OvMaths OvDebug OvTools)
target_include_directories(OvRendering PUBLIC include)
set_property(TARGET OvRendering PROPERTY DEFINE_SYMBOL "OVRENDERING_EXPORT")

add_subdirectory(src/OvRendering)
add_subdirectory(include/OvRendering)

if(OVERLOAD_BUILD)
	install(DIRECTORY												DESTINATION ${OVERLOAD_BUILD_DIR}/OvRendering/bin)
	install(DIRECTORY												DESTINATION ${OVERLOAD_BUILD_DIR}/OvRendering/lib)

	install(FILES "$<TARGET_FILE:OvRendering>"						DESTINATION ${OVERLOAD_BUILD_DIR}/OvRendering/bin)
	install(DIRECTORY ./include										DESTINATION ${OVERLOAD_BUILD_DIR}/OvRendering)
	install(FILES "$<TARGET_FILE_DIR:OvRendering>/OvRendering.lib"	DESTINATION ${OVERLOAD_BUILD_DIR}/OvRendering/lib)
endif()