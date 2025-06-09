if ( NOT DEFINED ASHES_BUILD_RENDERER_PLUGINS )
	option( ASHES_BUILD_RENDERER_PLUGINS "Build renderer plugins" ON )
endif ()

macro( ash_update_icd_target TARGET_NAME )
	if ( WIN32 )
		set( PATH ".\\\\" )
		set( NAME "${TARGET_NAME}" )
		set( NAMEDEBUG "${TARGET_NAME}d" )
		set( EXT "dll" )
		set( CMAKE_ICD_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE} )
		set( CMAKE_ICD_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO} )
		set( CMAKE_ICD_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG} )
	else ()
		if ( APPLE )
			set( PATH "./" )
			set( NAME "lib${TARGET_NAME}" )
			set( NAMEDEBUG "lib${TARGET_NAME}d" )
			set( EXT "dylib" )
		else ()
			set( PATH "./" )
			set( NAME "lib${TARGET_NAME}" )
			set( NAMEDEBUG "lib${TARGET_NAME}d" )
			set( EXT "so.1" )
		endif ()
		set( CMAKE_ICD_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE} )
		set( CMAKE_ICD_DIRECTORY_RELWITHDEBINFO ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO} )
		set( CMAKE_ICD_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG} )
	endif ()
	set( IN_ICD_FILE ${Ashes_SOURCE_DIR}/source/ashes/ashes_icd.json.in )
	set( OUT_ICD_FILE_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_icd.json.release )
	set( OUT_ICD_FILE_RELWITHDEBINFO ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_icd.json.relwithdebinfo )
	set( OUT_ICD_FILE_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_icd.json.debug )
	configure_file( ${IN_ICD_FILE}
		${OUT_ICD_FILE_RELEASE}
		NEWLINE_STYLE LF
	)
	configure_file( ${IN_ICD_FILE}
		${OUT_ICD_FILE_RELWITHDEBINFO}
		NEWLINE_STYLE LF
	)
	set( NAME ${NAMEDEBUG} )
	configure_file( ${IN_ICD_FILE}
		${OUT_ICD_FILE_DEBUG}
		NEWLINE_STYLE LF
	)
	add_custom_command( TARGET ${TARGET_NAME}
		PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E
			copy_if_different
				$<$<CONFIG:Release>:${OUT_ICD_FILE_RELEASE}>
				$<$<CONFIG:Release>:${CMAKE_ICD_DIRECTORY_RELEASE}${TARGET_NAME}_icd.json>
				$<$<CONFIG:RelWithDebInfo>:${OUT_ICD_FILE_RELWITHDEBINFO}>
				$<$<CONFIG:RelWithDebInfo>:${CMAKE_ICD_DIRECTORY_RELWITHDEBINFO}${TARGET_NAME}_icd.json>
				$<$<CONFIG:Debug>:${OUT_ICD_FILE_DEBUG}>
				$<$<CONFIG:Debug>:${CMAKE_ICD_DIRECTORY_DEBUG}${TARGET_NAME}d_icd.json>
	)
	if ( WIN32 )
		install(
			FILES
				$<$<CONFIG:Release>:${CMAKE_ICD_DIRECTORY_RELEASE}${TARGET_NAME}_icd.json>
				$<$<CONFIG:RelWithDebInfo>:${CMAKE_ICD_DIRECTORY_RELWITHDEBINFO}${TARGET_NAME}_icd.json>
				$<$<CONFIG:Debug>:${CMAKE_ICD_DIRECTORY_DEBUG}${TARGET_NAME}d_icd.json>
			COMPONENT AshesRenderers
			DESTINATION bin
		)
	else ()
		install(
			FILES
				$<$<CONFIG:Release>:${CMAKE_ICD_DIRECTORY_RELEASE}${TARGET_NAME}_icd.json>
				$<$<CONFIG:RelWithDebInfo>:${CMAKE_ICD_DIRECTORY_RELWITHDEBINFO}${TARGET_NAME}_icd.json>
				$<$<CONFIG:Debug>:${CMAKE_ICD_DIRECTORY_DEBUG}${TARGET_NAME}d_icd.json>
			COMPONENT AshesRenderers
			DESTINATION lib
		)
	endif ()
endmacro( ash_update_icd_target )

if ( ASHES_BUILD_RENDERER_PLUGINS )
	set( TARGET_INCLUDE_DIRS
		${TARGET_INCLUDE_DIRS}
		${Ashes_SOURCE_DIR}/include
	)
	set( ASHES_BUILD_PLUGINS ON )

	if ( "${CMAKE_SYSTEM_NAME}" STREQUAL "WindowsStore" )
		set( ASHES_BUILD_VKN OFF )
		set( ASHES_BUILD_OGL OFF )

		if ( NOT DEFINED ASHES_BUILD_RENDERER_XBOX )
			option( ASHES_BUILD_RENDERER_XBOX "Build XBox renderer plugin" ON )
		endif ()
		if ( ASHES_BUILD_RENDERER_XBOX )
			set( ASHES_BUILD_XBOX ON  )
		else ()
			set( ASHES_BUILD_XBOX OFF )
		endif ()
	else ()
		set( ASHES_BUILD_XBOX OFF )

		if ( NOT DEFINED ASHES_BUILD_RENDERER_VKN )
			option( ASHES_BUILD_RENDERER_VKN "Build Vulkan renderer plugin" ON )
		endif ()
		if ( ASHES_BUILD_RENDERER_VKN )
			set( ASHES_BUILD_VKN ON  )
		else ()
			set( ASHES_BUILD_VKN OFF )
		endif ()

		if ( NOT DEFINED ASHES_BUILD_RENDERER_OGL )
			option( ASHES_BUILD_RENDERER_OGL "Build OpenGL renderer plugin" ON )
		endif ()
		if ( ASHES_BUILD_RENDERER_OGL )
			set( ASHES_BUILD_OGL ON )
		else ()
			set( ASHES_BUILD_OGL OFF )
		endif ()
	endif ()

	if ( WIN32 )
		if ( NOT DEFINED ASHES_BUILD_RENDERER_D3D11 )
			option( ASHES_BUILD_RENDERER_D3D11 "Build Direct3D 11 renderer plugin" ON )
		endif ()
		if ( ASHES_BUILD_RENDERER_D3D11 )
			set( ASHES_BUILD_D3D11 ON  )
		else ()
			set( ASHES_BUILD_D3D11 OFF )
		endif ()
	else ()
		set( ASHES_BUILD_D3D11 OFF )
	endif ()

	if ( NOT DEFINED ASHES_BUILD_RENDERER_TEST )
		option( ASHES_BUILD_RENDERER_TEST "Build Test renderer plugin" OFF )
	endif ()
	if ( ASHES_BUILD_RENDERER_TEST )
		set( ASHES_BUILD_TEST ON  )
	else ()
		set( ASHES_BUILD_TEST OFF )
	endif ()
else ()
	set( ASHES_BUILD_PLUGINS OFF )
endif ()

if ( ASHES_BUILD_PLUGINS )
	set( RendererDependencies
		ashes::common
		ashes::ashes
	)
	if ( ASHES_BUILD_OGL OR ASHES_BUILD_D3D11 OR ASHES_BUILD_TEST )
		add_subdirectory( RendererCommon )
		set( RendererDependencies
			${RendererDependencies}
			ashes::RendererCommon
		)
	endif ()

	if ( ASHES_BUILD_OGL )
		if ( ANDROID )
			add_definitions( -DASHES_ANDROID )
		elseif ( APPLE )
			add_definitions( -DASHES_APPLE )
		elseif ( NOT WIN32 )
			add_definitions( -DASHES_LINUX )
			add_definitions( -DASHES_VENDOR_GL )
			option( ASHES_USE_XLIB_EGL "Use EGL with XLIB" OFF )
		endif ()
		add_subdirectory( GlRenderer )
	endif ()
	if ( ASHES_BUILD_VKN )
		add_subdirectory( VkRenderer )
	endif ()
	if ( ASHES_BUILD_D3D11 )
		set( D3D11_XBOX OFF )
		add_subdirectory( D3D11Renderer )
	endif ()
	if ( ASHES_BUILD_XBOX )
		set( D3D11_XBOX ON )
		add_subdirectory( D3D11Renderer XBoxRenderer )
	endif ()
	if ( ASHES_BUILD_TEST )
		add_subdirectory( TestRenderer )
	endif ()

	install(
		TARGETS ${ENABLED_RENDERERS}
		COMPONENT AshesRenderers
		EXPORT AshesRenderers
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
	)
	install(
		FILES ${ENABLED_PDBS}
		DESTINATION bin
		OPTIONAL
	)
	install( EXPORT AshesRenderers
		COMPONENT AshesRenderers
		FILE AshesRenderersConfig.cmake
		NAMESPACE ashes::
		DESTINATION lib/cmake/ashes
	)

	set( ENABLED_RENDERERS
		${ENABLED_RENDERERS}
		PARENT_SCOPE
	)
endif ()
