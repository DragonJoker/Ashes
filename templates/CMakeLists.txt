
set( TARGET_INCLUDE_DIRS
	${TARGET_INCLUDE_DIRS}
	${CMAKE_BINARY_DIR}/include
	${CMAKE_SOURCE_DIR}/include
	${Vulkan_INCLUDE_DIRS}
)

add_subdirectory( GLFW )
