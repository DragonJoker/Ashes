project( Renderer )

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
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

set( RENDERLIB_WIN32 0 )
set( RENDERLIB_XCB 0 )
set( RENDERLIB_XLIB 0 )
set( RENDERLIB_ANDROID 0 )

if ( WIN32 )
	set( RENDERLIB_WIN32 1 )
elseif ( ANDROID )
	set( RENDERLIB_ANDROID 1 )
else ()
	set( RENDERLIB_XLIB 1 )
endif ()

set( ${PROJECT_NAME}_CONFIG_HEADER
	${CMAKE_CURRENT_BINARY_DIR}/Config.hpp
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/Config.hpp.in
	${${PROJECT_NAME}_CONFIG_HEADER}
	NEWLINE_STYLE LF
)

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_CONFIG_HEADER}
)

add_target_precompiled_header( ${PROJECT_NAME}
	RendererPch.hpp
	RendererPch.cpp
	${CMAKE_CXX_FLAGS}
	${${PROJECT_NAME}_SRC_FILES}
)

add_dependencies( ${PROJECT_NAME}
	Utils
)

parse_subdir_files( Buffer "Buffer" )
parse_subdir_files( Command "Command" )
parse_subdir_files( Core "Core" )
parse_subdir_files( Descriptor "Descriptor" )
parse_subdir_files( Enum "Enum" )
parse_subdir_files( Image "Image" )
parse_subdir_files( Pipeline "Pipeline" )
parse_subdir_files( RenderPass "RenderPass" )
parse_subdir_files( Shader "Shader" )
parse_subdir_files( Sync "Sync" )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Renderer" )
