set(GLFW_LOCATION "${CMAKE_SOURCE_DIR}/../../Dependencies/glfw")

find_library(GLFW_LIB NAMES glfw3dll PATHS "${GLFW_LOCATION}/lib/")
set(GLFW_DLL "${GLFW_LOCATION}/bin/glfw3.dll")

add_library(glfw SHARED IMPORTED)
set_property(TARGET glfw PROPERTY IMPORTED_LOCATION "${GLFW_DLL}")
set_property(TARGET glfw PROPERTY IMPORTED_IMPLIB "${GLFW_LIB}")
target_include_directories(glfw INTERFACE "${GLFW_LOCATION}/include")

