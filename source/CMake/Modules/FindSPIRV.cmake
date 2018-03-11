# Find OpenGL ES 3
# ----------------
#
# Finds the SPIRV library.
#
# This module defines:
#
# ::
#
#   SPIRV_FOUND        - True if SPIRV library is found.
#   SPIRV_INCLUDE_DIR  - The SPIRV include directiories.
#   SPIRV_LIBRARIES    - The SPIRV libraries.
#

find_package( PackageHandleStandardArgs )

find_path( SPIRV_ROOT_DIR include/SPIRV/spirv.hpp
	HINTS
	PATHS
		/usr/local
		/usr
)


if ( SPIRV_ROOT_DIR )
	find_path( SPIRV_INCLUDE_DIR SPIRV/spirv.hpp
		HINTS
		PATH_SUFFIXES
			include
		PATHS
			${SPIRV_ROOT_DIR}
			/usr/local/include
			/usr/include
	)

	if (CMAKE_CL_64 OR CMAKE_GENERATOR MATCHES Win64)
		set( PLATFORM "x64" )
	else ()
		set( PLATFORM "x86" )
	endif ()

	find_path( SPIRV_LIBRARY_RELEASE_DIR SPIRV.lib libSPIRV.a
		HINTS
		PATH_SUFFIXES
			lib/${PLATFORM}/Release
			lib/${PLATFORM}
			lib/Release
			lib
			/usr/local/lib
			/usr/lib
		PATHS
			${SPIRV_ROOT_DIR}
	)

	find_path( SPIRV_LIBRARY_DEBUG_DIR SPIRVd.lib libSPIRVd.a
		HINTS
		PATH_SUFFIXES
			lib/${PLATFORM}/Debug
			lib/${PLATFORM}
			lib/Debug
			lib
			/usr/local/lib
			/usr/lib
		PATHS
			${SPIRV_ROOT_DIR}
	)

	if ( NOT SPIRV_LIBRARY_DEBUG_DIR )
		find_path( SPIRV_LIBRARY_DEBUG_DIR SPIRV.lib libSPIRV.a
			HINTS
			PATH_SUFFIXES
				lib/${PLATFORM}/Debug
				lib/${PLATFORM}
				/usr/local/lib
				/usr/lib
			PATHS
				${SPIRV_ROOT_DIR}
		)
	endif ()

	find_library( SPIRV_LIBRARY_RELEASE
		NAMES
			SPIRV
		HINTS
		PATHS
			${SPIRV_LIBRARY_RELEASE_DIR}
	)

	find_library( SPIRV_LIBRARY_DEBUG
		NAMES
			SPIRVd
			SPIRV
		HINTS
		PATHS
			${SPIRV_LIBRARY_DEBUG_DIR}
	)

	find_library( SPIRV_TOOLS_LIBRARY_RELEASE
		NAMES
			SPIRV-Tools
		HINTS
		PATHS
			${SPIRV_LIBRARY_RELEASE_DIR}
	)

	find_library( SPIRV_TOOLS_LIBRARY_DEBUG
		NAMES
			SPIRV-Toolsd
			SPIRV-Tools
		HINTS
		PATHS
			${SPIRV_LIBRARY_DEBUG_DIR}
	)

	find_library( SPIRV_TOOLS_OPT_LIBRARY_RELEASE
		NAMES
			SPIRV-Tools-opt
		HINTS
		PATHS
			${SPIRV_LIBRARY_RELEASE_DIR}
	)

	find_library( SPIRV_TOOLS_OPT_LIBRARY_DEBUG
		NAMES
			SPIRV-Tools-optd
			SPIRV-Tools-opt
		HINTS
		PATHS
			${SPIRV_LIBRARY_DEBUG_DIR}
	)

	find_library( SPIRV_REMAPPER_LIBRARY_RELEASE
		NAMES
			SPVRemapper
		HINTS
		PATHS
			${SPIRV_LIBRARY_RELEASE_DIR}
	)

	find_library( SPIRV_REMAPPER_LIBRARY_DEBUG
		NAMES
			SPVRemapperd
			SPVRemapper
		HINTS
		PATHS
			${SPIRV_LIBRARY_DEBUG_DIR}
	)

	mark_as_advanced( SPIRV_LIBRARY_RELEASE_DIR )
	mark_as_advanced( SPIRV_LIBRARY_DEBUG_DIR )
	mark_as_advanced( SPIRV_LIBRARY_RELEASE )
	mark_as_advanced( SPIRV_LIBRARY_DEBUG )
	mark_as_advanced( SPIRVTOOLS_LIBRARY_RELEASE )
	mark_as_advanced( SPIRVTOOLS_LIBRARY_DEBUG )
	find_package_handle_standard_args( SPIRV DEFAULT_MSG SPIRV_LIBRARY_RELEASE SPIRV_INCLUDE_DIR )

	IF ( SPIRV_FOUND )
		if ( MSVC )
			if ( SPIRV_LIBRARY_DEBUG )
				set( SPIRV_LIBRARIES
					optimized ${SPIRV_LIBRARY_RELEASE}
					debug ${SPIRV_LIBRARY_DEBUG}
					optimized ${SPIRV_TOOLS_LIBRARY_RELEASE}
					debug ${SPIRV_TOOLS_LIBRARY_DEBUG}
					optimized ${SPIRV_TOOLS_OPT_LIBRARY_RELEASE}
					debug ${SPIRV_TOOLS_OPT_LIBRARY_DEBUG}
					optimized ${SPIRV_REMAPPER_LIBRARY_RELEASE}
					debug ${SPIRV_REMAPPER_LIBRARY_DEBUG}
					CACHE STRING "SPIRV libraries" )
				set( SPIRV_LIBRARY_DIRS ${SPIRV_LIBRARY_RELEASE_DIR} ${SPIRV_LIBRARY_DEBUG_DIR} )
			else()
				set( SPIRV_LIBRARIES
					${SPIRV_LIBRARY_RELEASE}
					${SPIRV_TOOLS_LIBRARY_RELEASE}
					${SPIRV_TOOLS_OPT_LIBRARY_RELEASE}
					CACHE STRING "SPIRV libraries" )
				set( SPIRV_LIBRARY_DIRS ${SPIRV_LIBRARY_RELEASE_DIR} )
			endif()
		else ()
			set( SPIRV_LIBRARIES
				${SPIRV_LIBRARY_RELEASE}
				${SPIRV_TOOLS_LIBRARY_RELEASE}
				${SPIRV_TOOLS_OPT_LIBRARY_RELEASE}
				CACHE STRING "SPIRV libraries" )
			set( SPIRV_LIBRARY_DIRS ${SPIRV_LIBRARY_RELEASE_DIR} )
		endif ()
	ENDIF ()
endif ()
