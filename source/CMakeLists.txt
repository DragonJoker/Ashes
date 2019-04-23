get_filename_component( ASHES_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY )
get_filename_component( ASHES_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY )

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${ASHES_SOURCE_DIR}/include
)

if ( WIN32 )
	set( BinLibraries
		Dbghelp
	)
else ()
	set( BinLibraries
		dl
	)
endif ()

if ( NOT DEFINED ASHES_BUILD_RENDERER_PLUGINS )
	option( ASHES_BUILD_RENDERER_PLUGINS "Build renderer plugins" ON )
endif ()

if ( ASHES_BUILD_RENDERER_PLUGINS )
	set( ASHES_BUILD_PLUGINS TRUE )

	if ( NOT DEFINED ASHES_BUILD_RENDERER_OGL3 )
		option( ASHES_BUILD_RENDERER_OGL3 "Build OpenGL 3.X renderer plugin" ON )
	endif ()
	if ( ASHES_BUILD_RENDERER_OGL3 )
		set( ASHES_BUILD_OGL3 TRUE  )
	else ()
		set( ASHES_BUILD_OGL3 FALSE )
	endif ()

	if ( NOT DEFINED ASHES_BUILD_RENDERER_OGL4 )
		option( ASHES_BUILD_RENDERER_OGL4 "Build OpenGL 4.X renderer plugin" ON )
	endif ()
	if ( ASHES_BUILD_RENDERER_OGL4 )
		set( ASHES_BUILD_OGL4 TRUE  )
	else ()
		set( ASHES_BUILD_OGL4 FALSE )
	endif ()

	if ( NOT DEFINED ASHES_BUILD_RENDERER_VKN )
		option( ASHES_BUILD_RENDERER_VKN "Build Vulkan renderer plugin" ON )
	endif ()
	if ( ASHES_BUILD_RENDERER_VKN )
		set( ASHES_BUILD_VKN TRUE  )
	else ()
		set( ASHES_BUILD_VKN FALSE )
	endif ()

	if ( WIN32 )
		if ( NOT DEFINED ASHES_BUILD_RENDERER_D3D11 )
			option( ASHES_BUILD_RENDERER_D3D11 "Build Direct3D 11 renderer plugin" ON )
		endif ()
		if ( ASHES_BUILD_RENDERER_D3D11 )
			set( ASHES_BUILD_D3D11 TRUE  )
		else ()
			set( ASHES_BUILD_D3D11 FALSE )
		endif ()
	else ()
		set( ASHES_BUILD_D3D11 FALSE )
	endif ()

	if ( NOT DEFINED ASHES_BUILD_RENDERER_TEST )
		option( ASHES_BUILD_RENDERER_TEST "Build Test renderer plugin" ON )
	endif ()
	if ( ASHES_BUILD_RENDERER_TEST )
		set( ASHES_BUILD_TEST TRUE  )
	else ()
		set( ASHES_BUILD_TEST FALSE )
	endif ()
else ()
	set( ASHES_BUILD_PLUGINS FALSE )
endif ()

if ( MSVC )
	add_definitions(
		-D_ENFORCE_MATCHING_ALLOCATORS=0
		-DNOMINMAX
		# -DWIN32_LEAN_AND_MEAN
	)
endif ()

set( SPIRV_CROSS_DIR ${ASHES_SOURCE_DIR}/external/SPIRV-Cross )

add_subdirectory( common )
add_subdirectory( ashes )
add_subdirectory( AshesPP )

if ( ASHES_BUILD_PLUGINS )
	add_subdirectory( renderer )
	set( ENABLED_RENDERERS ${ENABLED_RENDERERS} PARENT_SCOPE )
endif ()

if ( ASHES_BUILD_TESTS OR ASHES_BUILD_SAMPLES )
	add_subdirectory( util )
endif ()
