find_package( wxWidgets COMPONENTS core base )
if( NOT WIN32 )
	find_package( GTK2 )
else()
	set( GTK2_FOUND TRUE )
endif()

find_package( ASSIMP )

if( wxWidgets_FOUND AND GTK2_FOUND AND ASSIMP_FOUND )
	string( COMPARE EQUAL "${wxWidgets_USE_DEBUG}" "ON" IsWxDebug )

	include_directories(
		${CMAKE_BINARY_DIR}/include
		${CMAKE_SOURCE_DIR}/include
		${wxWidgets_INCLUDE_DIRS}
		${GTK2_INCLUDE_DIRS}
		${ASSIMP_INCLUDE_DIR}
		${CMAKE_SOURCE_DIR}/external/imgui
	)

	set( BinLibraries
		${BinLibraries}
		${glslang_LIBRARIES}
		${SPIRV_LIBRARIES}
		common
		ashes
		ashespp
		util
	)

	file( GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*-* )
	set( SAMPLE_ID 0 )

	foreach ( child ${children} )
		if ( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child} )
			set( TARGET_NAME Sample-${child} )
			add_subdirectory( ${child} )
			target_compile_definitions( ${TARGET_NAME} PRIVATE
				SAMPLE_ID=${SAMPLE_ID}
				_FILE_OFFSET_BITS=64
				_LARGE_FILES
				_UNICODE
			)
			set_property( TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17 )
			set_property( TARGET ${TARGET_NAME} PROPERTY FOLDER "Samples" )

			if( NOT WIN32 )
				target_compile_definitions( ${TARGET_NAME} PRIVATE
					__WXGTK__
				)
			endif()

			if( IsWxDebug )
				target_compile_definitions( ${TARGET_NAME} PRIVATE
					__WXDEBUG__
				)
			endif()

			math( EXPR SAMPLE_ID "${SAMPLE_ID} + 1" )
		endif ()
	endforeach ()
endif ()
