set( _PROJECT_NAME ashespp )
project( ${_PROJECT_NAME} )

set( ${PROJECT_NAME}_DESCRIPTION "${PROJECT_NAME} library" )
set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

if ( MSVC )
	set( LIBS
		DbgHelp )
endif ()

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	CONFIGURE_DEPENDS
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.c
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.cc
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.cpp
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.c
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.cc
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.cpp
)

file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	CONFIGURE_DEPENDS
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.h
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.hh
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.hpp
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.inl
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.inc
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.h
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.hh
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.hpp
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.inl
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.inc
)

add_library( ${_PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
add_library( ashes::${_PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_compile_definitions( ${PROJECT_NAME} PUBLIC
	${Ashes_BINARY_DEFINITIONS}
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${Vulkan_INCLUDE_DIR}
		${Ashes_SOURCE_DIR}/source
		${ASHES_CONFIG_DIR}/source
		${ASHES_CONFIG_DIR}/include/${_PROJECT_NAME}
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}
		${ASHES_CONFIG_DIR}/source/${_PROJECT_NAME}
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		ashes::common
	PUBLIC
		ashes::ashes
		${LIBS}
)
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${Ashes_BASE_DIR}/Core"
)

add_target_precompiled_header( ${PROJECT_NAME}
	${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/AshesPPPch.hpp
	${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/AshesPPPch.cpp
	"${CMAKE_CXX_FLAGS}"
	${${PROJECT_NAME}_SRC_FILES}
)

parse_subdir_files( ${Ashes_SOURCE_DIR} Buffer "Buffer" )
parse_subdir_files( ${Ashes_SOURCE_DIR} Command "Command" )
parse_subdir_files( ${Ashes_SOURCE_DIR} Core "Core" )
parse_subdir_files( ${Ashes_SOURCE_DIR} Descriptor "Descriptor" )
parse_subdir_files( ${Ashes_SOURCE_DIR} Enum "Enum" )
parse_subdir_files( ${Ashes_SOURCE_DIR} Image "Image" )
parse_subdir_files( ${Ashes_SOURCE_DIR} Miscellaneous "Miscellaneous" )
parse_subdir_files( ${Ashes_SOURCE_DIR} Pipeline "Pipeline" )
parse_subdir_files( ${Ashes_SOURCE_DIR} Core/Platform "Core\\\\\\\\Platform" )
parse_subdir_files( ${Ashes_SOURCE_DIR} RenderPass "RenderPass" )
parse_subdir_files( ${Ashes_SOURCE_DIR} Shader "Shader" )
parse_subdir_files( ${Ashes_SOURCE_DIR} Sync "Sync" )
parse_subdir_files( ${Ashes_SOURCE_DIR} Utils "Utils" )

if ( BUILD_ASHES )
	target_add_doc( ${_PROJECT_NAME} "" "*.h *.hpp" )
endif ()

#For libs, we install headers to <install_dir>/include/${PROJECT_NAME}
target_install_headers( ${PROJECT_NAME}
	${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME} )

