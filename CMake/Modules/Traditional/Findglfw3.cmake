# FindGLFW
# ------------
#
# Locate glfw3 library
#
# This module defines
#
# ::
#
#   glfw3_LIBRARIES, the libraries to link against
#   glfw3_FOUND, if false, do not try to link to assimp
#   glfw3_INCLUDE_DIR, where to find headers.
#

find_path( glfw3_DIR include/GLFW/glfw3.h 
	HINTS
	PATH_SUFFIXES
		GLFW
	PATHS
		/usr/local
		/usr
)

find_path(glfw3_INCLUDE_DIR GLFW/glfw3.h 
	HINTS
	PATH_SUFFIXES
		include
	PATHS
		${glfw3_DIR}
)
if( APPLE )
	set( SUBFOLDER "lib-macos" )
elseif ( MINGW )
	if ( CMAKE_CL_64 OR CMAKE_GENERATOR MATCHES Win64 )
		set( SUBFOLDER "lib-mingw-w64" )
	else ()
		set( SUBFOLDER "lib-mingw" )
	endif ()
elseif ( MSVC14 )
	set( SUBFOLDER "lib-vc2015" )
elseif ( MSVC12 )
	set( SUBFOLDER "lib-vc2013" )
elseif ( MSVC11 )
	set( SUBFOLDER "lib-vc2012" )
else ()
	set( SUBFOLDER "lib-x86_64-linux-gnu" )
endif ()

find_path( glfw3_LIBRARY_DIR
	NAMES
		glfw3.lib
		glfw3dll.lib
		libglfw.3.dylib
		libglfw.so
	HINTS
	PATH_SUFFIXES
		${SUBFOLDER}
		lib
	PATHS
		${glfw3_DIR}
)

find_library(glfw3_LIBRARY
	NAMES
		glfw3.lib
		glfw3dll.lib
		libglfw.3.dylib
		libglfw.so
	HINTS
	PATHS
		${glfw3_LIBRARY_DIR}
)

find_package_handle_standard_args( glfw3 DEFAULT_MSG glfw3_LIBRARY glfw3_INCLUDE_DIR )

if ( glfw3_FOUND )
	add_library( glfw STATIC IMPORTED )
	set_target_properties( glfw PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES ${glfw3_INCLUDE_DIR}
	)
	set_target_properties( glfw PROPERTIES
		IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
		IMPORTED_LOCATION ${glfw3_LIBRARY}
	)
	unset( glfw3_INCLUDE_DIR CACHE )
	unset( glfw3_LIBRARY_DIR CACHE )
	unset( glfw3_LIBRARY CACHE )
else ()
	mark_as_advanced( glfw3_INCLUDE_DIR )
	mark_as_advanced( glfw3_LIBRARY_DIR )
	mark_as_advanced( glfw3_LIBRARY )
endif ()