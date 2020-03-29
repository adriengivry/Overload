set(LUA_LOCATION "${CMAKE_SOURCE_DIR}/../../Dependencies/lua")

set(LUA_LIB "${LUA_LOCATION}/lib/liblua53.a")
set(LUA_DLL "${LUA_LOCATION}/bin/lua53.dll")

add_library(lua SHARED IMPORTED)
set_property(TARGET lua PROPERTY IMPORTED_LOCATION "${LUA_DLL}")
set_property(TARGET lua PROPERTY IMPORTED_IMPLIB "${LUA_LIB}")
target_include_directories(lua INTERFACE "${LUA_LOCATION}/include")
target_compile_definitions(lua INTERFACE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)

install(FILES "${LUA_DLL}" DESTINATION "${OVERLOAD_BUILD_DIR}")
