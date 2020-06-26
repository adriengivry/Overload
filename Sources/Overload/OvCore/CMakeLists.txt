cmake_minimum_required(VERSION 3.13)

add_library(OvCore SHARED "")

target_link_libraries(OvCore lua OvAnalytics OvAudio OvDebug OvMaths OvPhysics OvRendering OvTools OvUI OvWindowing)
target_include_directories(OvCore PUBLIC include)
target_compile_definitions(OvCore PRIVATE _DISABLE_EXTENDED_ALIGNED_STORAGE)
set_property(TARGET OvCore PROPERTY DEFINE_SYMBOL "OVCORE_EXPORT")
target_compile_options(OvCore PRIVATE /bigobj)

add_subdirectory(src/OvCore)
add_subdirectory(include/OvCore)

if(OVERLOAD_BUILD)
	install(DIRECTORY											DESTINATION ${OVERLOAD_BUILD_DIR}/OvCore/bin)
	install(DIRECTORY											DESTINATION ${OVERLOAD_BUILD_DIR}/OvCore/lib)

	install(FILES "$<TARGET_FILE:OvCore>"						DESTINATION ${OVERLOAD_BUILD_DIR}/OvCore/bin)
	install(DIRECTORY ./include									DESTINATION ${OVERLOAD_BUILD_DIR}/OvCore)
	install(FILES "$<TARGET_FILE_DIR:OvCore>/OvCore.lib"		DESTINATION ${OVERLOAD_BUILD_DIR}/OvCore/lib)
endif()