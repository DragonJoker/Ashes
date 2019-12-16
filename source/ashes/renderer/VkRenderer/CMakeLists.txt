project( VkRenderer )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.c
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.cc
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.cpp
)

file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.h
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.hh
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.hpp
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.inl
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.inc
)

add_library( ${PROJECT_NAME} SHARED
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
add_library( ashes::${PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME} PRIVATE
	${Vulkan_INCLUDE_DIR}
	${TARGET_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}
)
target_link_libraries( ${PROJECT_NAME} PRIVATE
	${Ashes_BINARY_LIBRARIES}
	ashes::common
	ashes::ashes
)
target_compile_definitions( ${PROJECT_NAME} PRIVATE
	${PROJECT_NAME}_EXPORTS
	${TARGET_CXX_DEFINITIONS}
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
	FOLDER "${Ashes_BASE_DIR}/Renderer"
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_SOVERSION}
)

set( ENABLED_RENDERERS
	${ENABLED_RENDERERS}
	${PROJECT_NAME}
	PARENT_SCOPE
)
