set( wxWidgets_IGNORE_VCPKG ON )
find_package( wxWidgets REQUIRED COMPONENTS core base )

if( APPLE )
	set( GTK2_FOUND TRUE )
	set( Ashes_BINARY_LIBRARIES
		${Ashes_BINARY_LIBRARIES}
		"-framework AppKit"
		"-framework QuartzCore"
	)
elseif( WIN32 )
	set( GTK2_FOUND TRUE )
else ()
	find_package( GTK2 )
endif()

if ( VCPKG_TOOLCHAIN )
	find_package( assimp CONFIG REQUIRED assimp )
	set( assimp_LIBRARIES assimp::assimp )
else ()
	find_package( assimp )
endif ()

if( wxWidgets_FOUND AND GTK2_FOUND AND ASSIMP_FOUND )
	file( GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*-* )
	set( SAMPLE_ID 0 )
	set( Ashes_BINARY_LIBRARIES
		${Ashes_BINARY_LIBRARIES}
		${wxWidgets_LIBRARIES}
		${GTK2_LIBRARIES}
		${assimp_LIBRARIES}
		ashes::ashes
		ashes::ashespp
		ashes::util
	)
	set( TARGET_CXX_DEFINITIONS
		${TARGET_CXX_DEFINITIONS}
		${wxWidgets_DEFINITIONS}
	)

	foreach ( child ${children} )
		if ( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child} )
			set( FOLDER_NAME ${child} )
			set( TARGET_NAME Sample-${FOLDER_NAME} )
			add_subdirectory( ${child} )
			target_include_directories( ${TARGET_NAME} PRIVATE
				${TARGET_INCLUDE_DIRS}
				${CMAKE_BINARY_DIR}/include
				${CMAKE_SOURCE_DIR}/include
				${wxWidgets_INCLUDE_DIRS}
				${GTK2_INCLUDE_DIRS}
				${assimp_INCLUDE_DIR}
				${CMAKE_SOURCE_DIR}/external/imgui
				${CMAKE_SOURCE_DIR}/samples/00-Common/Src
				${Vulkan_INCLUDE_DIR}
			)
			target_compile_definitions( ${TARGET_NAME} PRIVATE
				SAMPLE_ID=${SAMPLE_ID}
				SAMPLE_NAME="${TARGET_NAME}"
				SAMPLE_FOLDER="${FOLDER_NAME}"
				${TARGET_CXX_DEFINITIONS}
				${Ashes_BINARY_DEFINITIONS}
			)
			target_compile_options( ${TARGET_NAME} PRIVATE
				${TARGET_CXX_OPTIONS}
			)
			set_target_properties( ${TARGET_NAME} PROPERTIES
				CXX_STANDARD 17
				CXX_EXTENSIONS OFF
				FOLDER "${Ashes_BASE_DIR}/Samples"
			)
			math( EXPR SAMPLE_ID "${SAMPLE_ID} + 1" )
		endif ()
	endforeach ()
endif ()
