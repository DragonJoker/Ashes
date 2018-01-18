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
		${CMAKE_BINARY_DIR}/Renderer/Renderer
		${CMAKE_SOURCE_DIR}/Core
		${CMAKE_SOURCE_DIR}/Renderer/
		${CMAKE_SOURCE_DIR}/Renderer/Renderer
		${wxWidgets_INCLUDE_DIRS}
		${GTK2_INCLUDE_DIRS}
	)

	add_subdirectory( 00-Common )
	add_subdirectory( 01-DeviceCreation )
	add_subdirectory( 02-SwapChainCreation )
	add_subdirectory( 03-VertexLayout )
	add_subdirectory( 04-StagingBuffer )
	add_subdirectory( 05-Texture )
	add_subdirectory( 06-DescriptorSets )
	add_subdirectory( 07-FrameBuffer )
endif ()
