project( GLFW-Template )

find_package( GLFW )

if( GLFW_FOUND )

	file( GLOB SOURCE_FILES
		*.cpp
		*.hpp
	)

	add_executable( ${PROJECT_NAME}
		${SOURCE_FILES}
	)
	target_include_directories( ${PROJECT_NAME} PRIVATE
		${TARGET_INCLUDE_DIRS}
		${GLFW_INCLUDE_DIR}
		${Vulkan_INCLUDE_DIR}
	)
	target_link_libraries( ${PROJECT_NAME} PRIVATE
		ashes::common
		ashes::ashes
		ashes::ashespp
		${GLFW_LIBRARIES}
		${Ashes_BINARY_LIBRARIES}
	)
	target_compile_definitions( ${PROJECT_NAME} PRIVATE
		${TARGET_CXX_DEFINITIONS}
		${Ashes_BINARY_DEFINITIONS}
	)
	target_compile_options( ${PROJECT_NAME} PRIVATE
		${TARGET_CXX_OPTIONS}
	)
	set_target_properties( ${PROJECT_NAME} PROPERTIES
		CXX_STANDARD 17
		CXX_EXTENSIONS OFF
		FOLDER "${Ashes_BASE_DIR}/Templates"
	)
	add_dependencies( ${PROJECT_NAME}
		${ENABLED_RENDERERS}
	)

endif ()
