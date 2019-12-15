project( ashes )

set( ${PROJECT_NAME}_DESCRIPTION "${PROJECT_NAME} library" )
set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

file( GLOB ${PROJECT_NAME}_SRC_FILES
	CONFIGURE_DEPENDS
		${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.c
		${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.cc
		${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.cpp
		${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.c
		${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.cc
		${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.cpp
)

file( GLOB ${PROJECT_NAME}_HDR_FILES
	CONFIGURE_DEPENDS
		${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.h
		${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.hh
		${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.hpp
		${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.inl
		${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.inc
		${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.h
		${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.hh
		${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.hpp
		${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.inl
		${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.inc
)

set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/ashes.h
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
add_library( ashes::${PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
message( STATUS "ASHES_SOURCE_DIR: ${ASHES_SOURCE_DIR}" )
target_include_directories( ${PROJECT_NAME}
	PUBLIC
		${ASHES_SOURCE_DIR}/include
		${ASHES_CONFIG_DIR}/include
	PRIVATE
		${Vulkan_INCLUDE_DIRS}
		${ASHES_SOURCE_DIR}/source
		${ASHES_CONFIG_DIR}/source
		${ASHES_CONFIG_DIR}/include/${PROJECT_NAME}
		${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}
		${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}
		${ASHES_CONFIG_DIR}/source/${PROJECT_NAME}
)
target_link_libraries( ${PROJECT_NAME} PRIVATE
	ashes::common
)
target_compile_definitions( ${PROJECT_NAME} PRIVATE
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
	FOLDER "${ASHES_BASE_DIR}/Core" )


if ( BUILD_ASHES )
	target_add_doc( ${PROJECT_NAME} "" "*.h *.hpp" )
endif ()

target_install_dir_headers( ${PROJECT_NAME}
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}
	${PROJECT_NAME}
)

add_subdirectory( renderer )
set( ENABLED_RENDERERS ${ENABLED_RENDERERS} PARENT_SCOPE )
set( ASHES_CONFIG_DIR ${ASHES_CONFIG_DIR} PARENT_SCOPE )

