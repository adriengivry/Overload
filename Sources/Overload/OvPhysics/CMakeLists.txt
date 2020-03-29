cmake_minimum_required(VERSION 3.13)
project(OvPhysics)
set(CMAKE_CXX_STANDARD 17)

include(${OVERLOAD_CMAKE_DIR}/bullet3.cmake)

add_library(OvPhysics SHARED "")

target_link_libraries(OvPhysics OvMaths OvTools OvDebug bullet3::BulletCollision bullet3::BulletDynamics bullet3::BulletSoftBody bullet3::LinearMath)
target_include_directories(OvPhysics PUBLIC include)
target_compile_definitions(OvPhysics PRIVATE OVPHYSICS_EXPORT PUBLIC PLATFORM_WINDOWS)


add_subdirectory(src/OvPhysics)
add_subdirectory(include/OvPhysics)