set( _PROJECT_NAME VkRenderer )
project( ashes${_PROJECT_NAME} )

set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}" )

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/*.c
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/*.cc
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/*.cpp
)

file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/*.h
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/*.hh
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/*.hpp
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/*.inl
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/*.inc
)

add_library( ${PROJECT_NAME} SHARED
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
add_library( ashes::${_PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME} PRIVATE
	${Vulkan_INCLUDE_DIR}
	${TARGET_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}
)
target_link_libraries( ${PROJECT_NAME} PRIVATE
	${Ashes_BINARY_LIBRARIES}
	ashes::common
	ashes::ashes
)
target_compile_definitions( ${PROJECT_NAME} PRIVATE
	${_PROJECT_NAME}_EXPORTS
	${TARGET_CXX_DEFINITIONS}
)
target_compile_options( ${PROJECT_NAME} PRIVATE
	${TARGET_CXX_OPTIONS}
)
target_add_compilation_flags( ${PROJECT_NAME} )
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${Ashes_BASE_DIR}/Renderer"
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_SOVERSION}
)

set( ENABLED_RENDERERS
	${ENABLED_RENDERERS}
	${PROJECT_NAME}
	PARENT_SCOPE
)

if ( MSVC )
	set( ENABLED_PDBS
		${ENABLED_PDBS}
		$<TARGET_PDB_FILE:${PROJECT_NAME}>
		PARENT_SCOPE
	)
endif ()
