find_package( wxWidgets COMPONENTS core base )
if( NOT WIN32 )
	find_package( GTK2 )
else()
	set( GTK2_FOUND TRUE )
endif()

find_package( ASSIMP )

if( wxWidgets_FOUND AND GTK2_FOUND AND ASSIMP_FOUND )
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
		${CMAKE_SOURCE_DIR}/Core
		${CMAKE_BINARY_DIR}/Renderer/Renderer/Src
		${CMAKE_SOURCE_DIR}/Renderer/Renderer/Src
		${wxWidgets_INCLUDE_DIRS}
		${GTK2_INCLUDE_DIRS}
		${ASSIMP_INCLUDE_DIR}
	)

	add_subdirectory( 00-SamplesCommon )
	add_subdirectory( 01-ObjectLoading )
	add_subdirectory( 02-Lighting )
endif ()
