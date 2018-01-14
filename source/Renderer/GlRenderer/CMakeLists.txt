project( GlRenderer )

find_package( OpenGL )

if ( OpenGL_FOUND )
	include_directories(
		${CMAKE_SOURCE_DIR}
		${CMAKE_BINARY_DIR}/Renderer
		${CMAKE_CURRENT_BINARY_DIR}
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_CXX_FILES
		*.cc
		*.cpp
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_C_FILES
		*.c
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
		*.h
		*.hh
		*.hpp
		*.inl
		*.inc
	)

	add_definitions(
		-DGlRenderer_EXPORTS
		-DGLEW_STATIC=1
	)

	add_library( ${PROJECT_NAME} SHARED
		${${PROJECT_NAME}_C_FILES}
		${${PROJECT_NAME}_CXX_FILES}
		${${PROJECT_NAME}_HDR_FILES}
	)

	add_target_precompiled_header( ${PROJECT_NAME}
		GlRendererPch.hpp
		GlRendererPch.cpp
		${CMAKE_CXX_FLAGS}
		${${PROJECT_NAME}_CXX_FILES}
	)

	target_link_libraries( ${PROJECT_NAME}
		${OPENGL_gl_LIBRARY}
		Renderer
	)

	add_dependencies( ${PROJECT_NAME}
		Renderer
	)

	parse_subdir_files( Commands "Commands" )
	parse_subdir_files( GL "GL" )
	set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14 )
	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Renderer" )
else ()
	if ( NOT OpenGL_FOUND )
		message( SEND_ERROR "OpenGL library not found." )
	endif ()
endif ()
