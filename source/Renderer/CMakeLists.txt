option( RENDERER_STATIC_RENDERERS "Build renderers as static libraries" FALSE )

if ( MSVC )
	add_definitions( -D_ENFORCE_MATCHING_ALLOCATORS=0 )
endif ()

add_subdirectory( Renderer )
add_subdirectory( GlRenderer )
add_subdirectory( VkRenderer )
