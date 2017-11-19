function( copy_target_files _TARGET )# ARGN: The files
	foreach ( _FILE ${ARGN} )
		get_filename_component( _FILE ${_FILE} REALPATH )
		get_filename_component( _FILE_NAME ${_FILE} NAME )
		add_custom_command(
			TARGET ${_TARGET}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_FILE} ${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin/${_FILE_NAME}
		)
	endforeach ()
endfunction()

function( copy_target_directory _TARGET _SOURCE )
	add_custom_command(
		TARGET ${_TARGET}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${_SOURCE} ${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin
	)
endfunction()
