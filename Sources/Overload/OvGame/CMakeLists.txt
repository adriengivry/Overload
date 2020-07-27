cmake_minimum_required(VERSION 3.13)

if(WIN32)
	add_executable(OvGame OvGame.rc)
else()
	add_executable(OvGame "")
endif()

# Build OvGame with a console when in debug.
if(WIN32)
   set_target_properties(OvGame PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   set_target_properties(OvGame PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
   set_target_properties(OvGame PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   set_target_properties(OvGame PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
   set_target_properties(OvGame PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
   set_target_properties(OvGame PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
endif()

target_link_libraries(OvGame OvAnalytics OvDebug OvCore OvAudio OvRendering OvWindowing OvTools OvMaths OvUI OvPhysics)
target_include_directories(OvGame PUBLIC include)

add_subdirectory(src/OvGame)
add_subdirectory(include/OvGame)

copy_to_output_dir_post_build(OvGame ${GLFW_DLL}					$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame ${GLEW_DLL}					$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame ${LUA_DLL}						$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame ${ASSIMP_DLL}					$<TARGET_FILE_DIR:OvGame>)
foreach(IRRKLANGDLL ${IRRKLANG_DLLS})
	copy_to_output_dir_post_build(OvGame ${IRRKLANGDLL}				$<TARGET_FILE_DIR:OvGame>)
endforeach()


add_custom_command(TARGET OvGame POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:OvEditor>/Builder")
add_custom_command(TARGET OvGame POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:OvEditor>/Builder/Development")

if(WIN32)
	add_custom_command(TARGET OvGame POST_BUILD COMMAND cmd /C "xcopy \"$<TARGET_FILE_DIR:OvGame>\\*.dll\" \"$<TARGET_FILE_DIR:OvEditor>/Builder/Development\" /y /i /c" COMMAND_EXPAND_LISTS)
	add_custom_command(TARGET OvGame POST_BUILD COMMAND cmd /C "xcopy \"$<TARGET_FILE_DIR:OvGame>\\OvGame.exe\" \"$<TARGET_FILE_DIR:OvEditor>/Builder/Development\" /y /i /c" COMMAND_EXPAND_LISTS)
endif()