option( ASHES_TEST_ICD "Test ICD mode for ashes" ON )

if ( ASHES_BUILD_TESTS OR ASHES_BUILD_INFO )
	set( TARGET_INCLUDE_DIRS
		${Ashes_SOURCE_DIR}/include
		${Ashes_BINARY_DIR}
	)
	if ( ASHES_TEST_ICD )
		set( TARGET_INCLUDE_DIRS
			${TARGET_INCLUDE_DIRS}
			${Vulkan_INCLUDE_DIR}
		)
	endif ()
	find_package( Vulkan QUIET REQUIRED )
	if ( NOT ASHES_TEST_ICD )
		set( Vulkan_LIBRARY
			ashes::ashes
		)
	endif ()
endif ()

if ( ASHES_BUILD_TESTS )
	set( wxWidgets_IGNORE_VCPKG ON )
	find_package( wxWidgets COMPONENTS core base )
	if( APPLE )
		set( GTK_FOUND ON )
		set( Ashes_BINARY_LIBRARIES
			${Ashes_BINARY_LIBRARIES}
			"-framework AppKit"
			"-framework QuartzCore"
		)
	elseif( WIN32 )
		set( GTK_FOUND ON )
	else ()
		find_package( GTK3 )
		if ( GTK3_FOUND )
			set( GTK_FOUND ${GTK3_FOUND} )
			set( GTK_INCLUDE_DIRS ${GTK3_INCLUDE_DIRS} )
			set( GTK_LIBRARIES ${GTK3_LIBRARIES} )
		else ()
			set( GTK_FOUND OFF )
		endif ()
	endif()

	if( wxWidgets_FOUND AND GTK_FOUND )
		file( GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*-* )
		set( TEST_ID 0 )

		foreach ( FOLDER_NAME ${children} )
			if ( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER_NAME} )
				string( REGEX REPLACE "[^-]*-([a-zA-Z0-9]*)" "\\1" TEST_NAME ${FOLDER_NAME} )
				set( TARGET_NAME Test-${FOLDER_NAME} )
				add_subdirectory( ${FOLDER_NAME} )
				target_compile_definitions( ${TARGET_NAME}
					PRIVATE
						TEST_ID=${TEST_ID}
						TEST_NAME="${TEST_NAME}"
				)
				set_target_properties( ${TARGET_NAME}
					PROPERTIES
						CXX_STANDARD 17
						CXX_EXTENSIONS OFF
						FOLDER "${Ashes_BASE_DIR}/Tests"
				)
				math( EXPR TEST_ID
					"${TEST_ID} + 1"
				)
			endif ()
		endforeach ()
	endif ()
endif ()
