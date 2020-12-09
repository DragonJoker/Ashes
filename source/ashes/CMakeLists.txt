option( ASHES_ICD "Build Ashes as an ICD" ON )

if ( ASHES_ICD )
	project( ashes )
else ()
	project( vulkan )
endif ()

set( ${PROJECT_NAME}_DESCRIPTION "${PROJECT_NAME} library" )
set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( ${PROJECT_NAME}_SRC_FILES
	ashes.cpp
)

if ( ASHES_ICD )
	if ( WIN32 )
		set( NAME "ashes-1" )
		set( NAMEDEBUG "ashesd-1" )
		set( EXT "dll" )
	elseif ( APPLE )
		set( NAME "libashes" )
		set( NAMEDEBUG "libashesd" )
		set( EXT "dylib" )
	else ()
		set( NAME "libashes" )
		set( NAMEDEBUG "libashesd" )
		set( EXT "so.1" )
	endif ()
	set( IN_ICD_FILE ${Ashes_SOURCE_DIR}/source/ashes/ashes_icd.json.in )
	set( OUT_ICD_FILE_RELEASE ${ASHES_CONFIG_DIR}/ashes/ashes_icd.json.release )
	set( OUT_ICD_FILE_RELWITHDEBUGINFO ${ASHES_CONFIG_DIR}/ashes/ashes_icd.json.relwithdebuginfo )
	set( OUT_ICD_FILE_DEBUG ${ASHES_CONFIG_DIR}/ashes/ashes_icd.json.debug )
	configure_file( ${IN_ICD_FILE}
		${OUT_ICD_FILE_RELEASE}
		NEWLINE_STYLE LF
	)
	configure_file( ${IN_ICD_FILE}
		${OUT_ICD_FILE_RELWITHDEBUGINFO}
		NEWLINE_STYLE LF
	)
	set( NAME ${NAMEDEBUG} )
	configure_file( ${IN_ICD_FILE}
		${OUT_ICD_FILE_DEBUG}
		NEWLINE_STYLE LF
	)
endif ()

set( ${PROJECT_NAME}_HDR_FILES
	../../include/ashes/ashes.h
	../../include/ashes/ashes.hpp
	../../include/ashes/ashes_functions_list.hpp
	ashes_plugin.hpp
)

# I hate to have to do that, but I found no other to force gcc to use --std=c++17
if ( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
	set( TARGET_CXX_OPTIONS --std=c++17 )
elseif ( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
	set( TARGET_CXX_OPTIONS --std=c++17 )
elseif ( CMAKE_CXX_COMPILER_ID STREQUAL "XLClang" )
	set( TARGET_CXX_OPTIONS --std=c++17 )
elseif ( CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" )
	set( TARGET_CXX_OPTIONS --std=c++17 )
endif ()

add_library( ${PROJECT_NAME} SHARED
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
add_library( ashes::ashes
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${Ashes_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${Vulkan_INCLUDE_DIR}
		${Ashes_SOURCE_DIR}/include/ashes
		${Ashes_SOURCE_DIR}/source
		${Ashes_SOURCE_DIR}/source/ashes
		${ASHES_CONFIG_DIR}/include/ashes
		${ASHES_CONFIG_DIR}/source
		${ASHES_CONFIG_DIR}/source/ashes
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		ashes::common
	PUBLIC
		${CMAKE_DL_LIBS}
)
target_compile_definitions( ${PROJECT_NAME}
	PUBLIC
		${TARGET_COMPILE_DEFINITIONS}
		VK_ENABLE_BETA_EXTENSIONS
	PRIVATE
		AshesC_EXPORTS
)

if ( ASHES_ICD )
	target_compile_definitions( ${PROJECT_NAME}
		PUBLIC
			ASHES_ICD
	)
endif ()
target_compile_options( ${PROJECT_NAME} PRIVATE
	${TARGET_CXX_OPTIONS}
	${TARGET_COMPILE_OPTIONS}
)

if ( ASHES_ICD )
	add_custom_command( TARGET ${PROJECT_NAME}
		PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E
			copy_if_different
				${OUT_ICD_FILE}
				$<$<CONFIG:Release>:${OUT_ICD_FILE_RELEASE}>
				$<$<CONFIG:RelWithDebugInfo>:${OUT_ICD_FILE_RELWITHDEBINFO}>
				$<$<CONFIG:Debug>:${OUT_ICD_FILE_DEBUG}>
				$<$<CONFIG:Release>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}ashes_icd.json>
				$<$<CONFIG:RelWithDebugInfo>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO}ashes_icd.json>
				$<$<CONFIG:Debug>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}ashes_icd.json>
	)
endif ()

if ( WIN32 )
	set_target_properties( ${PROJECT_NAME} PROPERTIES
		CXX_STANDARD 17
		CXX_EXTENSIONS OFF
		FOLDER "${Ashes_BASE_DIR}/Core"
		DEBUG_POSTFIX "d-${${PROJECT_NAME}_VERSION_MAJOR}"
		RELEASE_POSTFIX "-${${PROJECT_NAME}_VERSION_MAJOR}"
		RELWITHDEBINFO_POSTFIX "-${${PROJECT_NAME}_VERSION_MAJOR}"
	)
else ()
	set_target_properties( ${PROJECT_NAME} PROPERTIES
		CXX_STANDARD 17
		CXX_EXTENSIONS OFF
		FOLDER "${Ashes_BASE_DIR}/Core"
		VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}"
		SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}"
	)
endif ()

if ( BUILD_ASHES )
	target_add_doc( ${PROJECT_NAME} "" "*.h *.hpp" )
endif ()

target_install_dir_headers( ${PROJECT_NAME}
	${Ashes_SOURCE_DIR}/include/ashes
	ashes
)

if ( ASHES_ICD )
	install(
		FILES ${OUT_ICD_FILE}
		COMPONENT ashes
		DESTINATION share/ashes
	)
endif ()

add_subdirectory( renderer )

set( ENABLED_RENDERERS ${ENABLED_RENDERERS} PARENT_SCOPE )
set( ASHES_CONFIG_DIR ${ASHES_CONFIG_DIR} PARENT_SCOPE )

