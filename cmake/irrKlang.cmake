set(IRRKLANG_LOCATION "${CMAKE_SOURCE_DIR}/../../Dependencies/irrklang")

find_library(IRRKLANG_LIB NAMES irrklang PATHS "${IRRKLANG_LOCATION}/lib/")
set(IRRKLANG_DLL "${IRRKLANG_LOCATION}/bin/irrklang.dll")
file(GLOB IRRKLANG_DLLS "${IRRKLANG_LOCATION}/bin/*.dll")

add_library(irrKlang SHARED IMPORTED)
set_property(TARGET irrKlang PROPERTY IMPORTED_LOCATION ${IRRKLANG_DLLS})
set_property(TARGET irrKlang PROPERTY IMPORTED_IMPLIB "${IRRKLANG_LIB}")
target_include_directories(irrKlang INTERFACE "${IRRKLANG_LOCATION}/include")

install(FILES ${IRRKLANG_DLLS} DESTINATION "${OVERLOAD_BUILD_DIR}")