
set( TARGET_INCLUDE_DIRS
	${TARGET_INCLUDE_DIRS}
	${CMAKE_BINARY_DIR}/include
	${CMAKE_SOURCE_DIR}/include
)

add_subdirectory( GLFW )
