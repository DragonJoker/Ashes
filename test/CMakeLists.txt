if ( NOT ASHES_BUILD_UWP )
	option( ASHES_TEST_ICD "Test ICD mode for ashes" ON )

	if ( ASHES_BUILD_TST OR ASHES_BUILD_INFO )
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

	if ( ASHES_BUILD_TST )
		if ( VCPKG_TOOLCHAIN AND ( MSVC OR APPLE ) )
			set( NanoSVG_DIR ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/NanoSVG )
			set( wxWidgets_DIR ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/wxwidgets )
			find_package( NanoSVG CONFIG )
			find_package( wxWidgets CONFIG COMPONENTS core base )
		else ()
			set( CMAKE_MODULE_PATH
				${CMAKE_SOURCE_DIR}/CMake
				${CMAKE_SOURCE_DIR}/CMake/Modules
				${CMAKE_SOURCE_DIR}/CMake/Modules/Traditional
				${CMAKE_SOURCE_DIR}/CMake/Toolchains
			)
			find_package( wxWidgets COMPONENTS core base )
		endif ()

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
							UNITY_BUILD "${ASHES_UNITY_BUILD}"
					)
					math( EXPR TEST_ID
						"${TEST_ID} + 1"
					)
				endif ()
			endforeach ()
		endif ()
	endif ()
endif ()
