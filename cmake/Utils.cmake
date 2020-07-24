function(copy_to_output_dir_post_build target what where)
	add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${what} ${where})
endfunction()
