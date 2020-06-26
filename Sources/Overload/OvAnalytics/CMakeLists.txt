cmake_minimum_required(VERSION 3.13)

add_library(OvAnalytics SHARED "")

target_link_libraries(OvAnalytics OvTools)
target_include_directories(OvAnalytics PUBLIC include)
set_property(TARGET OvAnalytics PROPERTY DEFINE_SYMBOL "OVANALYTICS_EXPORT")

add_subdirectory(src/OvAnalytics)
add_subdirectory(include/OvAnalytics)

if(OVERLOAD_BUILD)
	install(DIRECTORY												DESTINATION ${OVERLOAD_BUILD_DIR}/OvAnalytics/bin)
	install(DIRECTORY												DESTINATION ${OVERLOAD_BUILD_DIR}/OvAnalytics/lib)

	install(FILES "$<TARGET_FILE:OvAnalytics>"						DESTINATION ${OVERLOAD_BUILD_DIR}/OvAnalytics/bin)
	install(DIRECTORY ./include										DESTINATION ${OVERLOAD_BUILD_DIR}/OvAnalytics)
	install(FILES "$<TARGET_FILE_DIR:OvAnalytics>/OvAnalytics.lib"	DESTINATION ${OVERLOAD_BUILD_DIR}/OvAnalytics/lib)
endif()