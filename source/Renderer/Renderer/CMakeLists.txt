project( Renderer )

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Src
	${CMAKE_CURRENT_BINARY_DIR}/Src
)

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	Src/*.c
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
	${CMAKE_CURRENT_BINARY_DIR}/Src/RendererConfig.hpp
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/Src/RendererConfig.hpp.in
	${${PROJECT_NAME}_CONFIG_HEADER}
	NEWLINE_STYLE LF
)

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_CONFIG_HEADER}
)

add_target_precompiled_header( ${PROJECT_NAME}
	Src/RendererPch.hpp
	Src/RendererPch.cpp
	${CMAKE_CXX_FLAGS}
	${${PROJECT_NAME}_SRC_FILES}
)

add_dependencies( ${PROJECT_NAME}
	Utils
)

parse_subdir_files( Src/Buffer "Buffer" )
parse_subdir_files( Src/Command "Command" )
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
