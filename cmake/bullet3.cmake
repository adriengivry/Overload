set(BULLET3_LOCATION "${CMAKE_SOURCE_DIR}/../../Dependencies/bullet3")

find_library(BULLET3_BULLETCOLLISION_LIBD NAMES BulletCollision PATHS "${BULLET3_LOCATION}/lib/Debug/")
find_library(BULLET3_BULLETDYNAMICS_LIBD NAMES BulletDynamics PATHS "${BULLET3_LOCATION}/lib/Debug/")
find_library(BULLET3_BULLETSOFTBODY_LIBD NAMES BulletSoftBody PATHS "${BULLET3_LOCATION}/lib/Debug/")
find_library(BULLET3_LINEARMATH_LIBD NAMES LinearMath PATHS "${BULLET3_LOCATION}/lib/Debug/")

find_library(BULLET3_BULLETCOLLISION_LIB NAMES BulletCollision PATHS "${BULLET3_LOCATION}/lib/Release/")
find_library(BULLET3_BULLETDYNAMICS_LIB NAMES BulletDynamics PATHS "${BULLET3_LOCATION}/lib/Release/")
find_library(BULLET3_BULLETSOFTBODY_LIB NAMES BulletSoftBody PATHS "${BULLET3_LOCATION}/lib/Release/")
find_library(BULLET3_LINEARMATH_LIB NAMES LinearMath PATHS "${BULLET3_LOCATION}/lib/Release/")

add_library(bullet3::BulletCollision STATIC IMPORTED)
set_property(TARGET bullet3::BulletCollision PROPERTY IMPORTED_LOCATION_DEBUG "${BULLET3_BULLETCOLLISION_LIBD}")
set_property(TARGET bullet3::BulletCollision PROPERTY IMPORTED_LOCATION_RELEASE "${BULLET3_BULLETCOLLISION_LIB}")
target_include_directories(bullet3::BulletCollision INTERFACE "${BULLET3_LOCATION}/include")

add_library(bullet3::BulletDynamics STATIC IMPORTED)
set_property(TARGET bullet3::BulletDynamics PROPERTY IMPORTED_LOCATION_DEBUG "${BULLET3_BULLETDYNAMICS_LIBD}")
set_property(TARGET bullet3::BulletDynamics PROPERTY IMPORTED_LOCATION_RELEASE "${BULLET3_BULLETDYNAMICS_LIB}")
target_include_directories(bullet3::BulletDynamics INTERFACE "${BULLET3_LOCATION}/include")

add_library(bullet3::BulletSoftBody STATIC IMPORTED)
set_property(TARGET bullet3::BulletSoftBody PROPERTY IMPORTED_LOCATION_DEBUG "${BULLET3_BULLETSOFTBODY_LIBD}")
set_property(TARGET bullet3::BulletSoftBody PROPERTY IMPORTED_LOCATION_RELEASE "${BULLET3_BULLETSOFTBODY_LIB}")
target_include_directories(bullet3::BulletSoftBody INTERFACE "${BULLET3_LOCATION}/include")

add_library(bullet3::LinearMath STATIC IMPORTED)
set_property(TARGET bullet3::LinearMath PROPERTY IMPORTED_LOCATION_DEBUG "${BULLET3_LINEARMATH_LIBD}")
set_property(TARGET bullet3::LinearMath PROPERTY IMPORTED_LOCATION_RELEASE "${BULLET3_LINEARMATH_LIB}")
target_include_directories(bullet3::LinearMath INTERFACE "${BULLET3_LOCATION}/include")
