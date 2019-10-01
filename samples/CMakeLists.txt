find_package( wxWidgets COMPONENTS core base )
if( NOT WIN32 )
	find_package( GTK2 )
else()
	set( GTK2_FOUND TRUE )
endif()

find_package( ASSIMP )

if( wxWidgets_FOUND AND GTK2_FOUND AND ASSIMP_FOUND )
	string( COMPARE EQUAL "${wxWidgets_USE_DEBUG}" "ON" IsWxDebug )

	file( GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*-* )
	set( SAMPLE_ID 0 )
	set( BinLibraries
		${BinLibraries}
		${glslang_LIBRARIES}
		${SPIRV_LIBRARIES}
		common
		ashes
		ashespp
		util
	)

	if( NOT WIN32 )
		set( TARGET_CXX_DEFINITIONS
			${TARGET_CXX_DEFINITIONS}
			__WXGTK__
		)
	endif()

	if( IsWxDebug )
		set( TARGET_CXX_DEFINITIONS
			${TARGET_CXX_DEFINITIONS}
			__WXDEBUG__
		)
	endif()

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
				${ASSIMP_INCLUDE_DIR}
				${CMAKE_SOURCE_DIR}/external/imgui
				${CMAKE_SOURCE_DIR}/samples/00-Common/Src
			)
			target_compile_features( ${TARGET_NAME} PRIVATE
				cxx_std_17
			)
			target_compile_definitions( ${TARGET_NAME} PRIVATE
				SAMPLE_ID=${SAMPLE_ID}
				SAMPLE_NAME="${TARGET_NAME}"
				SAMPLE_FOLDER="${FOLDER_NAME}"
				_FILE_OFFSET_BITS=64
				_LARGE_FILES
				_UNICODE
				${TARGET_CXX_DEFINITIONS}
			)
			target_compile_options( ${TARGET_NAME} PRIVATE
				${TARGET_CXX_OPTIONS}
			)
			set_target_properties( ${TARGET_NAME} PROPERTIES
				CXX_STANDARD 17
				CXX_EXTENSIONS OFF
				FOLDER "Samples"
			)
			add_dependencies( ${TARGET_NAME}
				${ENABLED_RENDERERS}
			)
			math( EXPR SAMPLE_ID "${SAMPLE_ID} + 1" )
		endif ()
	endforeach ()
endif ()

