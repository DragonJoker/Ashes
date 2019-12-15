project( ashes_common )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	CONFIGURE_DEPENDS
		${ASHES_SOURCE_DIR}/source/ashes/common/*.c
		${ASHES_SOURCE_DIR}/source/ashes/common/*.cc
		${ASHES_SOURCE_DIR}/source/ashes/common/*.cpp
)

file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	CONFIGURE_DEPENDS
		${ASHES_SOURCE_DIR}/include/ashes/common/*.h
		${ASHES_SOURCE_DIR}/include/ashes/common/*.hh
		${ASHES_SOURCE_DIR}/include/ashes/common/*.hpp
		${ASHES_SOURCE_DIR}/include/ashes/common/*.inl
		${ASHES_SOURCE_DIR}/include/ashes/common/*.inc
		${ASHES_SOURCE_DIR}/source/ashes/common/*.h
		${ASHES_SOURCE_DIR}/source/ashes/common/*.hh
		${ASHES_SOURCE_DIR}/source/ashes/common/*.hpp
		${ASHES_SOURCE_DIR}/source/ashes/common/*.inl
		${ASHES_SOURCE_DIR}/source/ashes/common/*.inc
)

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
	${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
)
add_library( ashes::common
	ALIAS
	${PROJECT_NAME}
)
target_link_libraries( ${PROJECT_NAME} PUBLIC
	${BinLibraries}
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${ASHES_SOURCE_DIR}/include
		${ASHES_CONFIG_DIR}/include
		${Vulkan_INCLUDE_DIRS}
		${ASHES_SOURCE_DIR}/include/ashes
		${ASHES_SOURCE_DIR}/include/ashes/common
)
target_compile_features( ${PROJECT_NAME} PUBLIC
	cxx_std_17
)
target_compile_definitions( ${PROJECT_NAME} PUBLIC
	${TARGET_CXX_DEFINITIONS}
)
target_compile_options( ${PROJECT_NAME} PRIVATE
	${TARGET_CXX_OPTIONS}
)
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${ASHES_BASE_DIR}/Core" )

parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes Platform "Platform" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes Win32 "Win32" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes Linux "Linux" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes MacOS "MacOS" )

target_install_dir_headers( ${PROJECT_NAME}
	${ASHES_SOURCE_DIR}/include/ashes/common
	ashes/common
)
