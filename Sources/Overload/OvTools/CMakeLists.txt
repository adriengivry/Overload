cmake_minimum_required(VERSION 3.13)

add_library(OvTools SHARED "")

target_include_directories(OvTools PUBLIC include)
set_property(TARGET OvTools PROPERTY DEFINE_SYMBOL "OVTOOLS_EXPORT")

add_subdirectory(src/OvTools)
add_subdirectory(include/OvTools)

if(OVERLOAD_BUILD)
	install(DIRECTORY										DESTINATION ${OVERLOAD_BUILD_DIR}/OvTools/bin)
	install(DIRECTORY										DESTINATION ${OVERLOAD_BUILD_DIR}/OvTools/lib)

	install(FILES "$<TARGET_FILE:OvTools>"					DESTINATION ${OVERLOAD_BUILD_DIR}/OvTools/bin)
	install(DIRECTORY ./include								DESTINATION ${OVERLOAD_BUILD_DIR}/OvTools)
	install(FILES "$<TARGET_FILE_DIR:OvTools>/OvTools.lib"	DESTINATION ${OVERLOAD_BUILD_DIR}/OvTools/lib)
endif()