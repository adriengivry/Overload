cmake_minimum_required(VERSION 3.13)

if(WIN32)
	add_executable(OvEditor OvEditor.rc)
else()
	add_executable(OvEditor "")
endif()


target_link_libraries(OvEditor OvAnalytics OvDebug OvCore OvAudio OvRendering OvWindowing OvTools OvMaths OvUI OvPhysics)
target_include_directories(OvEditor PUBLIC include)

add_subdirectory(src/OvEditor)
add_subdirectory(include/OvEditor)

function(copy_to_output_dir_post_build target what where)
	add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${what} ${where})
endfunction()

add_custom_command(TARGET OvEditor POST_BUILD COMMAND cmd /C "del \"$<TARGET_FILE_DIR:OvEditor>\\*.dll\"" COMMAND_EXPAND_LISTS)

add_custom_command(TARGET OvEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:OvEditor>/Config")
copy_to_output_dir_post_build(OvEditor "${CMAKE_CURRENT_LIST_DIR}/layout.ini" "$<TARGET_FILE_DIR:OvEditor>/Config/layout.ini")
add_custom_command(TARGET OvEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${OVERLOAD_RESOURCES_DIR}" "$<TARGET_FILE_DIR:OvEditor>/Data")
copy_to_output_dir_post_build(OvEditor ${GLFW_DLL}					$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor ${GLEW_DLL}					$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor ${IRRKLANG_DLL}				$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor ${LUA_DLL}					$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor ${ASSIMP_DLL}				$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor $<TARGET_FILE:OvAnalytics>	$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor $<TARGET_FILE:OvAudio>		$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor $<TARGET_FILE:OvCore>		$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor $<TARGET_FILE:OvDebug>		$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor $<TARGET_FILE:OvMaths>		$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor $<TARGET_FILE:OvPhysics>		$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor $<TARGET_FILE:OvRendering>	$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor $<TARGET_FILE:OvTools>		$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor $<TARGET_FILE:OvUI>			$<TARGET_FILE_DIR:OvEditor>)
copy_to_output_dir_post_build(OvEditor $<TARGET_FILE:OvWindowing>	$<TARGET_FILE_DIR:OvEditor>)

install(DIRECTORY								DESTINATION ${OVERLOAD_BUILD_DIR}/OvEditor)
install(TARGETS OvAnalytics OvAudio OvCore OvDebug OvEditor OvMaths OvPhysics OvRendering OvTools OvUI OvWindowing RUNTIME DESTINATION ${OVERLOAD_BUILD_DIR}/OvEditor)

install(FILES ${GLEW_DLL}						DESTINATION ${OVERLOAD_BUILD_DIR}/OvEditor)
install(FILES ${ASSIMP_DLL}						DESTINATION ${OVERLOAD_BUILD_DIR}/OvEditor)
install(FILES ${LUA_DLL}						DESTINATION ${OVERLOAD_BUILD_DIR}/OvEditor)
install(FILES ${IRRKLANG_DLLS}					DESTINATION ${OVERLOAD_BUILD_DIR}/OvEditor)
install(FILES ${GLFW_DLL}						DESTINATION ${OVERLOAD_BUILD_DIR}/OvEditor)

install(FILES layout.ini						DESTINATION ${OVERLOAD_BUILD_DIR}/OvEditor/Config)
install(DIRECTORY ${OVERLOAD_RESOURCES_DIR}		DESTINATION ${OVERLOAD_BUILD_DIR}/OvEditor/Data)