
set( TARGET_INCLUDE_DIRS
	${TARGET_INCLUDE_DIRS}
	${CMAKE_BINARY_DIR}/include
	${CMAKE_SOURCE_DIR}/include
	${Vulkan_INCLUDE_DIR}
)

if ( NOT ASHES_BUILD_UWP )
	add_subdirectory( GLFW )
endif ()
