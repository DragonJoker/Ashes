# FindGLFW
# ------------
#
# Locate GLFW library
#
# This module defines
#
# ::
#
#   GLFW_LIBRARIES, the libraries to link against
#   GLFW_FOUND, if false, do not try to link to assimp
#   GLFW_INCLUDE_DIR, where to find headers.
#

find_path( GLFW_ROOT_DIR include/GLFW/glfw3.h 
	HINTS
	PATH_SUFFIXES
		GLFW
	PATHS
		/usr/local
		/usr
)

find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h 
	HINTS
	PATH_SUFFIXES
		include
	PATHS
		${GLFW_ROOT_DIR}
		/usr/local/include
		/usr/include
)

if( WIN32 )
	if ( MINGW )
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
		message( SEND_ERROR "Unsupported compiler" )
	endif ()

	find_path( GLFW_LIBRARY_DIR glfw3.lib
		HINTS
		PATH_SUFFIXES
			${SUBFOLDER}
		PATHS
			${GLFW_ROOT_DIR}
	)

	find_library(GLFW_LIBRARY
		NAMES
			glfw3.lib
		HINTS
		PATHS
			${GLFW_LIBRARY_DIR}
	)
else()
	find_path( GLFW_LIBRARY_DIR libglfw3.so
		HINTS
		PATH_SUFFIXES
			lib
		PATHS
			${GLFW_ROOT_DIR}
	)

	find_library( GLFW_LIBRARY
		NAMES
			libglfw3.so
		HINTS
		PATHS
			${GLFW_LIBRARY_DIR}
	)
endif()

mark_as_advanced( GLFW_LIBRARY_DIR )
mark_as_advanced( GLFW_LIBRARY )
find_package_handle_standard_args( GLFW DEFAULT_MSG GLFW_LIBRARY GLFW_INCLUDE_DIR )

if ( GLFW_FOUND )
	set( GLFW_LIBRARIES ${GLFW_LIBRARY} CACHE STRING "GLFW libraries" )
endif ()