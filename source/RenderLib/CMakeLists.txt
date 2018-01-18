include_directories(
	${CMAKE_SOURCE_DIR}/RenderLib
	${CMAKE_BINARY_DIR}/Renderer/Renderer/Src
	${CMAKE_SOURCE_DIR}/Renderer/Renderer/Src
)

add_subdirectory( RenderLib )

if ( ANDROID )
	add_subdirectory( AndroidUtils )
	add_subdirectory( RenderLibTestApp )
elseif (WIN32)
	add_subdirectory( DesktopUtils )
	add_subdirectory( RenderLibTestApp )
endif ()
