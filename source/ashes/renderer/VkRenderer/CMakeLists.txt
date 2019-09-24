project( VkRenderer )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}
	${Vulkan_INCLUDE_DIRS}
)

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.c
	${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.cc
	${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.cpp
)

file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.h
	${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.hh
	${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.hpp
	${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.inl
	${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.inc
)

add_definitions(
	-D${PROJECT_NAME}_EXPORTS
)
add_library( ${PROJECT_NAME} SHARED
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Ashes/Renderer" )
set_target_properties( ${PROJECT_NAME}
	PROPERTIES
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_SOVERSION}
)

target_link_libraries( ${PROJECT_NAME}
	${BinLibraries}
	common
	ashes
	${Vulkan_LIBRARY}
)

add_dependencies( ${PROJECT_NAME}
	common
	ashes
)

set( ENABLED_RENDERERS
	${ENABLED_RENDERERS}
	${PROJECT_NAME}
	PARENT_SCOPE
)

if ( WIN32 )
	#We install each .dll in <install_dir>/bin folder
	install(
		TARGETS ${PROJECT_NAME}
		COMPONENT ${PROJECT_NAME}
		CONFIGURATIONS Release
		EXPORT ${PROJECT_NAME}
		RUNTIME DESTINATION bin
	)
	install(
		TARGETS ${PROJECT_NAME}
		COMPONENT ${PROJECT_NAME}_dev
		CONFIGURATIONS RelWithDebInfo
		EXPORT ${PROJECT_NAME}
		RUNTIME DESTINATION bin/RelWithDebInfo
	)
	install(
		TARGETS ${PROJECT_NAME}
		COMPONENT ${PROJECT_NAME}_dev
		CONFIGURATIONS Debug
		EXPORT ${PROJECT_NAME}
		RUNTIME DESTINATION bin/Debug
	)
else ()
	#We install each .so in <install_dir>/lib folder
	install(
		TARGETS ${PROJECT_NAME}
		COMPONENT ${PROJECT_NAME}
		CONFIGURATIONS Release
		EXPORT ${PROJECT_NAME}
		LIBRARY DESTINATION lib
	)
	install(
		TARGETS ${PROJECT_NAME}
		COMPONENT ${PROJECT_NAME}_dev
		CONFIGURATIONS RelWithDebInfo
		EXPORT ${PROJECT_NAME}
		LIBRARY DESTINATION lib/RelWithDebInfo
	)
	install(
		TARGETS ${PROJECT_NAME}
		COMPONENT ${PROJECT_NAME}_dev
		CONFIGURATIONS Debug
		LIBRARY DESTINATION lib/Debug
	)
endif ()
