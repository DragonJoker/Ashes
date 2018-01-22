project( GlRenderer )

find_package( OpenGL )

if ( OpenGL_FOUND )
	include_directories(
		${CMAKE_SOURCE_DIR}
		${CMAKE_BINARY_DIR}/Renderer/Renderer/Src
		${CMAKE_SOURCE_DIR}/Renderer/Renderer/Src
		${CMAKE_CURRENT_SOURCE_DIR}/Src
		${CMAKE_CURRENT_BINARY_DIR}/Src
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_CXX_FILES
		Src/*.cc
		Src/*.cpp
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
		Src/*.h
		Src/*.hh
		Src/*.hpp
		Src/*.inl
		Src/*.inc
	)

	add_definitions(
		-DGlRenderer_EXPORTS
		-DGLEW_STATIC=1
	)

	add_library( ${PROJECT_NAME} SHARED
		${${PROJECT_NAME}_CXX_FILES}
		${${PROJECT_NAME}_HDR_FILES}
	)

	add_target_precompiled_header( ${PROJECT_NAME}
		Src/GlRendererPch.hpp
		Src/GlRendererPch.cpp
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

	parse_subdir_files( Src/Buffer "Buffer" )
	parse_subdir_files( Src/Command "Command" )
	parse_subdir_files( Src/Command/Commands "Command\\\\Commands" )
	parse_subdir_files( Src/Core "Core" )
	parse_subdir_files( Src/Descriptor "Descriptor" )
	parse_subdir_files( Src/Enum "Enum" )
	parse_subdir_files( Src/Image "Image" )
	parse_subdir_files( Src/Miscellaneous "Miscellaneous" )
	parse_subdir_files( Src/Pipeline "Pipeline" )
	parse_subdir_files( Src/RenderPass "RenderPass" )
	parse_subdir_files( Src/Shader "Shader" )
	parse_subdir_files( Src/Sync "Sync" )
	set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14 )
	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Renderer" )
else ()
	if ( NOT OpenGL_FOUND )
		message( SEND_ERROR "OpenGL library not found." )
	endif ()
endif ()
