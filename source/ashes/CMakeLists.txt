project( ashes )

set( ${PROJECT_NAME}_DESCRIPTION "${PROJECT_NAME} library" )
set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( ${PROJECT_NAME}_SRC_FILES
	ashes.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	../../include/ashes/ashes.h
	../../include/ashes/ashes.hpp
	../../include/ashes/ashes_functions_list.hpp
	ashes_plugin.hpp
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
add_library( ashes::ashes
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${Ashes_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${Vulkan_INCLUDE_DIR}
		${Ashes_SOURCE_DIR}/include/ashes
		${Ashes_SOURCE_DIR}/source
		${Ashes_SOURCE_DIR}/source/ashes
		${ASHES_CONFIG_DIR}/include/ashes
		${ASHES_CONFIG_DIR}/source
		${ASHES_CONFIG_DIR}/source/ashes
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		ashes::common
	PUBLIC
		${CMAKE_DL_LIBS}
)
target_compile_definitions( ${PROJECT_NAME}
	PUBLIC
		${TARGET_COMPILE_DEFINITIONS}
		VK_ENABLE_BETA_EXTENSIONS
	PRIVATE
		AshesC_EXPORTS
)
target_compile_options( ${PROJECT_NAME} PRIVATE
	${TARGET_CXX_OPTIONS}
	${TARGET_COMPILE_OPTIONS}
)

if ( WIN32 )
	set_target_properties( ${PROJECT_NAME} PROPERTIES
		CXX_STANDARD 17
		CXX_EXTENSIONS OFF
		FOLDER "${Ashes_BASE_DIR}/Core"
		DEBUG_POSTFIX "d-${${PROJECT_NAME}_VERSION_MAJOR}"
		RELEASE_POSTFIX "-${${PROJECT_NAME}_VERSION_MAJOR}"
		RELWITHDEBINFO_POSTFIX "-${${PROJECT_NAME}_VERSION_MAJOR}"
	)
else ()
	set_target_properties( ${PROJECT_NAME} PROPERTIES
		CXX_STANDARD 17
		CXX_EXTENSIONS OFF
		FOLDER "${Ashes_BASE_DIR}/Core"
		VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}"
		SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}"
	)
endif ()

if ( BUILD_ASHES )
	target_add_doc( ${PROJECT_NAME} "" "*.h *.hpp" )
endif ()

target_install_dir_headers( ${PROJECT_NAME}
	${Ashes_SOURCE_DIR}/include/ashes
	ashes
)

add_subdirectory( renderer )

set( ENABLED_RENDERERS ${ENABLED_RENDERERS} PARENT_SCOPE )
set( ASHES_CONFIG_DIR ${ASHES_CONFIG_DIR} PARENT_SCOPE )
set( ENABLED_PDBS
	${ENABLED_PDBS}
	$<TARGET_PDB_FILE:${PROJECT_NAME}>
	PARENT_SCOPE
)

