set( _PROJECT_NAME ashes )
project( ${_PROJECT_NAME} )

set( ${PROJECT_NAME}_DESCRIPTION "${PROJECT_NAME} library" )
set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

file( GLOB ${PROJECT_NAME}_SRC_FILES
	CONFIGURE_DEPENDS
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.c
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.cc
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.cpp
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.c
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.cc
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.cpp
)

file( GLOB ${PROJECT_NAME}_HDR_FILES
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

set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${Ashes_SOURCE_DIR}/include/${PROJECT_NAME}/ashes.h
)

# I hate to have to do that, but I found no other to force gcc to use --std=c++17
if ( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
	set( TARGET_CXX_OPTIONS --std=c++17 )
elseif ( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
	set( TARGET_CXX_OPTIONS --std=c++17 )
elseif ( CMAKE_CXX_COMPILER_ID STREQUAL "XLClang" )
	set( TARGET_CXX_OPTIONS --std=c++17 )
elseif ( CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" )
	set( TARGET_CXX_OPTIONS --std=c++17 )
endif ()

add_library( ${PROJECT_NAME} SHARED
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
add_library( ashes::${_PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${Ashes_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
		${CMAKE_DL_LIBS}
	PRIVATE
		${Vulkan_INCLUDE_DIR}
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}
		${Ashes_SOURCE_DIR}/source
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}
		${ASHES_CONFIG_DIR}/include/${_PROJECT_NAME}
		${ASHES_CONFIG_DIR}/source
		${ASHES_CONFIG_DIR}/source/${_PROJECT_NAME}
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		ashes::common
)
target_compile_definitions( ${PROJECT_NAME}
	PUBLIC
		${TARGET_COMPILE_DEFINITIONS}
	PRIVATE
		AshesC_EXPORTS
)
target_compile_features( ${PROJECT_NAME} PRIVATE
	cxx_std_17
)
target_compile_options( ${PROJECT_NAME} PRIVATE
	${TARGET_CXX_OPTIONS}
)
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${Ashes_BASE_DIR}/Core" )


if ( BUILD_ASHES )
	target_add_doc( ${PROJECT_NAME} "" "*.h *.hpp" )
endif ()

target_install_dir_headers( ${PROJECT_NAME}
	${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}
	${_PROJECT_NAME}
)

add_subdirectory( renderer )
set( ENABLED_RENDERERS ${ENABLED_RENDERERS} PARENT_SCOPE )
set( ASHES_CONFIG_DIR ${ASHES_CONFIG_DIR} PARENT_SCOPE )

