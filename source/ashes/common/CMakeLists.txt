set( _PROJECT_NAME common )
project( ashes_${_PROJECT_NAME} )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	CONFIGURE_DEPENDS
		${Ashes_SOURCE_DIR}/source/ashes/${_PROJECT_NAME}/*.c
		${Ashes_SOURCE_DIR}/source/ashes/${_PROJECT_NAME}/*.cc
		${Ashes_SOURCE_DIR}/source/ashes/${_PROJECT_NAME}/*.cpp
)

file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	CONFIGURE_DEPENDS
		${Ashes_SOURCE_DIR}/include/ashes/${_PROJECT_NAME}/*.h
		${Ashes_SOURCE_DIR}/include/ashes/${_PROJECT_NAME}/*.hh
		${Ashes_SOURCE_DIR}/include/ashes/${_PROJECT_NAME}/*.hpp
		${Ashes_SOURCE_DIR}/include/ashes/${_PROJECT_NAME}/*.inl
		${Ashes_SOURCE_DIR}/include/ashes/${_PROJECT_NAME}/*.inc
		${Ashes_SOURCE_DIR}/source/ashes/${_PROJECT_NAME}/*.h
		${Ashes_SOURCE_DIR}/source/ashes/${_PROJECT_NAME}/*.hh
		${Ashes_SOURCE_DIR}/source/ashes/${_PROJECT_NAME}/*.hpp
		${Ashes_SOURCE_DIR}/source/ashes/${_PROJECT_NAME}/*.inl
		${Ashes_SOURCE_DIR}/source/ashes/${_PROJECT_NAME}/*.inc
)

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
	${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
)
add_library( ashes::${_PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_link_libraries( ${PROJECT_NAME} PUBLIC
	${CMAKE_DL_LIBS}
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${Ashes_SOURCE_DIR}/include
		${ASHES_CONFIG_DIR}/include
		${Vulkan_INCLUDE_DIR}
		${Ashes_SOURCE_DIR}/include/ashes
		${Ashes_SOURCE_DIR}/include/ashes/${_PROJECT_NAME}
)
target_compile_features( ${PROJECT_NAME} PUBLIC
	cxx_std_17
)
target_compile_definitions( ${PROJECT_NAME} PUBLIC
	${TARGET_COMPILE_DEFINITIONS}
)
target_compile_options( ${PROJECT_NAME} PUBLIC
	${TARGET_COMPILE_OPTIONS}
)
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${Ashes_BASE_DIR}/Core" )

parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes Platform "Platform" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes Win32 "Win32" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes Linux "Linux" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes MacOS "MacOS" )

target_install_dir_headers( ${PROJECT_NAME}
	${Ashes_SOURCE_DIR}/include/ashes/${_PROJECT_NAME}
	ashes/${_PROJECT_NAME}
)
