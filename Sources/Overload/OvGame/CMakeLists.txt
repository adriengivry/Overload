cmake_minimum_required(VERSION 3.13)
project(OvGame)
set(CMAKE_CXX_STANDARD 17)

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
endif(WIN32)

target_link_libraries(OvGame OvAnalytics OvDebug OvCore OvAudio OvRendering OvWindowing OvTools OvMaths OvUI OvPhysics)
target_include_directories(OvGame PUBLIC include)

add_subdirectory(src/OvGame)
add_subdirectory(include/OvGame)

file(GLOB FILES_TO_REMOVE "$<TARGET_FILE_DIR:OvGame>/*.dll")
file(REMOVE FILES_TO_REMOVE)
copy_to_output_dir_post_build(OvGame ${GLFW_DLL}					$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame ${GLEW_DLL}					$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame ${IRRKLANG_DLL}				$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame ${LUA_DLL}						$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame ${ASSIMP_DLL}					$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame $<TARGET_FILE:OvAnalytics>		$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame $<TARGET_FILE:OvAudio>			$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame $<TARGET_FILE:OvCore>			$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame $<TARGET_FILE:OvDebug>			$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame $<TARGET_FILE:OvMaths>			$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame $<TARGET_FILE:OvPhysics>		$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame $<TARGET_FILE:OvRendering>		$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame $<TARGET_FILE:OvTools>			$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame $<TARGET_FILE:OvUI>			$<TARGET_FILE_DIR:OvGame>)
copy_to_output_dir_post_build(OvGame $<TARGET_FILE:OvWindowing>		$<TARGET_FILE_DIR:OvGame>)

# Coocking directory temporarly created here. Probably should be moved to OvEditor.
add_custom_command(TARGET OvGame POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:OvEditor>/Builder")
# Super hacky, copies the _entire_ build directory.
add_custom_command(TARGET OvGame POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "$<TARGET_FILE_DIR:OvGame>" "$<TARGET_FILE_DIR:OvEditor>/Builder/Development")