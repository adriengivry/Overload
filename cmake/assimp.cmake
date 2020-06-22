set(ASSIMP_LOCATION "${CMAKE_SOURCE_DIR}/../../Dependencies/assimp")

find_library(ASSIMP_LIB NAMES assimp-vc140-mt PATHS "${ASSIMP_LOCATION}/lib/")
set(ASSIMP_DLL "${ASSIMP_LOCATION}/bin/assimp-vc140-mt.dll")

add_library(assimp SHARED IMPORTED)
set_property(TARGET assimp PROPERTY IMPORTED_LOCATION "${ASSIMP_DLL}")
set_property(TARGET assimp PROPERTY IMPORTED_IMPLIB "${ASSIMP_LIB}")
target_include_directories(assimp INTERFACE "${ASSIMP_LOCATION}/include")

install(FILES ${ASSIMP_DLL} DESTINATION "${OVERLOAD_BUILD_DIR}")
