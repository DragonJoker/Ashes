project( GlRenderer )

find_package( OpenGL )
find_package( GLEW )

if ( OpenGL_FOUND AND GLEW_FOUND )
	include_directories(
		${CMAKE_SOURCE_DIR}
		${CMAKE_BINARY_DIR}/Renderer
		${CMAKE_CURRENT_BINARY_DIR}
		${GLEW_INCLUDE_DIR}
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
		*.c
		*.cc
		*.cpp
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
		*.h
		*.hh
		*.hpp
		*.inl
		*.inc
	)

	add_definitions( -DGlRenderer_EXPORTS )

	add_library( ${PROJECT_NAME} SHARED
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_HDR_FILES}
	)

	add_target_precompiled_header( ${PROJECT_NAME}
		GlRendererPch.hpp
		GlRendererPch.cpp
		${CMAKE_CXX_FLAGS}
		${${PROJECT_NAME}_SRC_FILES}
	)

	target_link_libraries( ${PROJECT_NAME}
		${OPENGL_gl_LIBRARY}
		optimized ${GLEW_LIBRARY_RELEASE}
		debug ${GLEW_LIBRARY_DEBUG}
		Renderer
	)

	add_dependencies( ${PROJECT_NAME}
		Renderer
	)

	parse_subdir_files( Commands "Commands" )
	set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Renderer" )
else ()
	if ( NOT OpenGL_FOUND )
		message( SEND_ERROR "OpenGL library not found." )
	endif ()
	if ( NOT GLEW_FOUND )
		message( SEND_ERROR "GLEW library not found." )
	endif ()
endif ()
