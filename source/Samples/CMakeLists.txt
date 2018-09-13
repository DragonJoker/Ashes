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
	endif()

	if( IsWxDebug )
		add_definitions(
			-D__WXDEBUG__
		)
	endif()

	include_directories(
		${CMAKE_SOURCE_DIR}/Utils/Src
		${CMAKE_BINARY_DIR}/Ashes/Ashes/Src
		${CMAKE_SOURCE_DIR}/Ashes/Ashes/Src
		${wxWidgets_INCLUDE_DIRS}
		${GTK2_INCLUDE_DIRS}
		${ASSIMP_INCLUDE_DIR}
		${CMAKE_SOURCE_DIR}/external/imgui
	)

	add_subdirectory( 00-Common )
	add_subdirectory( 01-ObjectLoading )
	add_subdirectory( 02-Lighting )
	add_subdirectory( 03-NormalMapping )
	add_subdirectory( 04-DeferredRendering )
	add_subdirectory( 05-Billboards )
endif ()
