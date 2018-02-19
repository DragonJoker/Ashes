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
		Utils
		Renderer
		${GLFW_LIBRARIES}
	)

	set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14 )
	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Templates" )

endif ()
