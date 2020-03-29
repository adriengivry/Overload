set(GLEW_LOCATION "${CMAKE_SOURCE_DIR}/../../Dependencies/glew")

find_library(GLEW_LIB NAMES glew32 PATHS "${GLEW_LOCATION}/lib/")
set(GLEW_DLL "${GLEW_LOCATION}/bin/glew32.dll")

add_library(glew SHARED IMPORTED)
set_property(TARGET glew PROPERTY IMPORTED_LOCATION "${GLEW_DLL}")
set_property(TARGET glew PROPERTY IMPORTED_IMPLIB "${GLEW_LIB}")
target_include_directories(glew INTERFACE "${GLEW_LOCATION}/include")
