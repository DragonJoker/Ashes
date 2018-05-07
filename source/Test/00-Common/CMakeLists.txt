project( Test-00-Common )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

file( GLOB SOURCE_FILES
	Src/*.cpp
)

file( GLOB HEADER_FILES
	Src/*.hpp
	Src/*.inl
)

add_library( ${PROJECT_NAME} STATIC
	${SOURCE_FILES}
	${HEADER_FILES}
)

target_link_libraries( ${PROJECT_NAME}
	Renderer
)

add_dependencies( ${PROJECT_NAME}
	GlRenderer
	Gl3Renderer
	VkRenderer
)

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Test" )

set( ASSETS_FOLDER ${CMAKE_SOURCE_DIR}/Test/Assets )

file( GLOB_RECURSE ASSETS_FILES
	${ASSETS_FOLDER}/*
)

foreach( ASSET ${ASSETS_FILES} )
	file( RELATIVE_PATH REL_ASSET ${ASSETS_FOLDER} ${ASSET} )
	get_filename_component( FOLDER ${REL_ASSET} DIRECTORY )
	add_custom_command(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory
			$<$<CONFIG:Debug>:${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/share/Assets/${FOLDER}>
			$<$<CONFIG:Release>:${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/share/Assets/${FOLDER}>
			$<$<CONFIG:RelWithDebInfo>:${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/share/Assets/${FOLDER}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ASSETS_FOLDER}/${REL_ASSET}
			$<$<CONFIG:Debug>:${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/share/Assets/${FOLDER}>
			$<$<CONFIG:Release>:${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/share/Assets/${FOLDER}>
			$<$<CONFIG:RelWithDebInfo>:${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/share/Assets/${FOLDER}>
	)
endforeach()