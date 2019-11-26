# Find NVAPI
# ----------
#
# Finds the NVAPI library.
#
# This module defines:
#
# ::
#
#   NVAPI_FOUND        - True if NVAPI library is found.
#   NVAPI_INCLUDE_DIR  - The NVAPI include directiory.
#   NVAPI_LIBRARY      - The NVAPI library.
#

find_package( PackageHandleStandardArgs )

find_path( NVAPI_ROOT_DIR nvapi.h
	HINTS
	PATHS
		/usr/local
		/usr
)


if ( NVAPI_ROOT_DIR )
	find_path( NVAPI_INCLUDE_DIR nvapi.h
		HINTS
		PATH_SUFFIXES
			include
		PATHS
			${NVAPI_ROOT_DIR}
	)

	if ( CMAKE_CL_64 OR CMAKE_GENERATOR MATCHES Win64 )
		set( NVAPI_PLATFORM "amd64" )
		set( NVAPI_POSTFIX "64" )
	else ()
		set( NVAPI_PLATFORM "x86" )
	endif ()

	find_path( NVAPI_LIBRARY_DIR nvapi${NVAPI_POSTFIX}.lib
		HINTS
		PATH_SUFFIXES
			${NVAPI_PLATFORM}
		PATHS
			${NVAPI_ROOT_DIR}
	)

	find_library( NVAPI_LIBRARY
		NAMES
			nvapi${NVAPI_POSTFIX}.lib
		HINTS
		PATHS
			${NVAPI_LIBRARY_DIR}
	)

	mark_as_advanced( NVAPI_LIBRARY_DIR )
	mark_as_advanced( NVAPI_LIBRARY )
	find_package_handle_standard_args( NVAPI DEFAULT_MSG NVAPI_LIBRARY NVAPI_INCLUDE_DIR )
endif ()
