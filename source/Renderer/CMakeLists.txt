if ( "${RENDERER_BUILD_RENDERER_PLUGINS}" STREQUAL "" )
	set( RENDERER_BUILD_RENDERER_PLUGINS TRUE )
endif ()

if ( "${RENDERER_BUILD_RENDERER_PLUGINS}" STREQUAL "TRUE" )
	option( RENDERER_BUILD_PLUGINS "Build renderers plugins" ON )
else ()
	option( RENDERER_BUILD_PLUGINS "Build renderers plugins" OFF )
endif ()

if ( MSVC )
	add_definitions( -D_ENFORCE_MATCHING_ALLOCATORS=0 )
endif ()

add_subdirectory( Renderer )

if ( RENDERER_BUILD_PLUGINS )
	if ( "${RENDERER_STATIC_RENDERER_PLUGINS}" STREQUAL "" )
		set( RENDERER_STATIC_RENDERER_PLUGINS FALSE )
	endif ()

	if ( "${RENDERER_STATIC_RENDERER_PLUGINS}" STREQUAL "TRUE" )
		option( RENDERER_STATIC_RENDERERS "Build renderers as static libraries" ON )
	else ()
		option( RENDERER_STATIC_RENDERERS "Build renderers as static libraries" OFF )
	endif ()

	add_subdirectory( GlRenderer )
	add_subdirectory( Gl3Renderer )
	add_subdirectory( TestRenderer )
	add_subdirectory( VkRenderer )
endif ()
