project( GLFW-Template )

find_package( GLFW )

if( GLFW_FOUND )
	include_directories(
		${GLFW_INCLUDE_DIR}
	)

	file( GLOB SOURCE_FILES
		*.cpp
		*.hpp
	)

	add_executable( ${PROJECT_NAME}
		${SOURCE_FILES}
	)

	target_link_libraries( ${PROJECT_NAME}
		Ashes
		${GLFW_LIBRARIES}
		${BinLibraries}
	)

	add_dependencies(
		${ENABLED_RENDERERS}
	)

	if ( UNIX )
		target_link_libraries( ${PROJECT_NAME}
			dl
		)
	endif ()
	set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Templates" )

endif ()
