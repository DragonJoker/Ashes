project( Tutorial07 )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

find_package( wxWidgets COMPONENTS core base )
if( NOT WIN32 )
	find_package( GTK2 )
else()
	set( GTK2_FOUND TRUE )
endif()

if( wxWidgets_FOUND AND GTK2_FOUND )
	string( COMPARE EQUAL "${wxWidgets_USE_DEBUG}" "ON" IsWxDebug )

	add_definitions(
		-D_FILE_OFFSET_BITS=64
		-D_LARGE_FILES
		-D_UNICODE
	)

	if( NOT WIN32 )
		add_definitions(
			-D__WXGTK__
		)
		set( BinaryLibraries
			${BinaryLibraries}
			dl
		)
	endif()

	if( IsWxDebug )
		add_definitions(
			-D__WXDEBUG__
		)
	endif()

	include_directories(
		${CMAKE_BINARY_DIR}/Renderer
		${CMAKE_SOURCE_DIR}
		${wxWidgets_INCLUDE_DIRS}
		${GTK2_INCLUDE_DIRS}
	)

	file( GLOB SOURCE_FILES
		Src/*.cpp
	)

	file( GLOB HEADER_FILES
		Src/*.hpp
		Src/*.inl
	)

	add_executable( ${PROJECT_NAME} WIN32
		${SOURCE_FILES}
		${HEADER_FILES}
	)

	target_link_libraries( ${PROJECT_NAME}
		${VkLib_LIBRARIES}
		Utils
		Renderer
		GlRenderer
		VkRenderer
		${BinaryLibraries}
		${wxWidgets_LIBRARIES}
		${GTK2_LIBRARIES}
	)

	add_dependencies( ${PROJECT_NAME}
		GlRenderer
		VkRenderer
	)

	set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Main" )

	file( GLOB SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.* )

	foreach( SHADER ${SHADERS} )
		add_custom_command(
			TARGET ${PROJECT_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory
				$<$<CONFIG:Debug>:${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/share/${PROJECT_NAME}/Shaders>
				$<$<CONFIG:Release>:${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/share/${PROJECT_NAME}/Shaders>
				$<$<CONFIG:RelWithDebInfo>:${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/share/${PROJECT_NAME}/Shaders>
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER}
				$<$<CONFIG:Debug>:${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/share/${PROJECT_NAME}/Shaders>
				$<$<CONFIG:Release>:${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/share/${PROJECT_NAME}/Shaders>
				$<$<CONFIG:RelWithDebInfo>:${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/share/${PROJECT_NAME}/Shaders>
		)
	endforeach()
endif ()
