cmake_minimum_required(VERSION 3.13)

add_library(OvPhysics SHARED "")

target_link_libraries(OvPhysics OvMaths OvTools OvDebug bullet3::BulletCollision bullet3::BulletDynamics bullet3::BulletSoftBody bullet3::LinearMath)
target_include_directories(OvPhysics PUBLIC include)
set_property(TARGET OvPhysics PROPERTY DEFINE_SYMBOL "OVPHYSICS_EXPORT")

add_subdirectory(src/OvPhysics)
add_subdirectory(include/OvPhysics)

if(OVERLOAD_BUILD)
	install(DIRECTORY												DESTINATION ${OVERLOAD_BUILD_DIR}/OvPhysics/bin)
	install(DIRECTORY												DESTINATION ${OVERLOAD_BUILD_DIR}/OvPhysics/lib)

	install(FILES "$<TARGET_FILE:OvPhysics>"						DESTINATION ${OVERLOAD_BUILD_DIR}/OvPhysics/bin)
	install(DIRECTORY ./include										DESTINATION ${OVERLOAD_BUILD_DIR}/OvPhysics)
	install(FILES "$<TARGET_FILE_DIR:OvPhysics>/OvPhysics.lib"		DESTINATION ${OVERLOAD_BUILD_DIR}/OvPhysics/lib)
endif()