# Find OpenGL ES 3
# ----------------
#
# Finds the VkLib library.
#
# This module defines:
#
# ::
#
#   VkLib_FOUND        - True if VkLib library is found.
#   VkLib_INCLUDE_DIRS - The VkLib include directiories.
#   VkLib_LIBRARIES    - The VkLib libraries.
#

find_package( PackageHandleStandardArgs )

find_path( VkLib_ROOT_DIR include/VkLib/VkLibPrerequisites.hpp
	HINTS
	PATHS
		/usr/local
		/usr
)

if ( ${VkLib_ROOT_DIR} )
	find_path( VkLib_INCLUDE_DIR VkLib/VkLibPrerequisites.hpp
		HINTS
		PATH_SUFFIXES
			include
		PATHS
			${VkLib_ROOT_DIR}
			/usr/local/include
			/usr/include
	)

	if (CMAKE_CL_64 OR CMAKE_GENERATOR MATCHES Win64)
		set( PLATFORM "x64" )
	else ()
		set( PLATFORM "x86" )
	endif ()

	find_path( VkLib_LIBRARY_RELEASE_DIR VkLib.lib VkLib.a
		HINTS
		PATH_SUFFIXES
			lib/${PLATFORM}
			/usr/local/lib
			/usr/lib
		PATHS
			${VkLib_ROOT_DIR}
	)

	find_path( VkLib_LIBRARY_DEBUG_DIR VkLib.lib VkLib.a
		HINTS
		PATH_SUFFIXES
			lib/${PLATFORM}/Debug
			/usr/local/lib
			/usr/lib
		PATHS
			${VkLib_ROOT_DIR}
	)

	find_library( VkLib_LIBRARY_RELEASE
		NAMES
			VkLib.lib
			VkLib.a
		HINTS
		PATHS
			${VkLib_LIBRARY_RELEASE_DIR}
	)

	find_library( VkLib_LIBRARY_DEBUG
		NAMES
			VkLib.lib
			VkLib.a
		HINTS
		PATHS
			${VkLib_LIBRARY_DEBUG_DIR}
	)

	mark_as_advanced( VkLib_LIBRARY_RELEASE_DIR )
	mark_as_advanced( VkLib_LIBRARY_DEBUG_DIR )
	mark_as_advanced( VkLib_LIBRARY_RELEASE )
	mark_as_advanced( VkLib_LIBRARY_DEBUG_DIR )
	find_package_handle_standard_args( VkLib DEFAULT_MSG VkLib_LIBRARY_RELEASE VkLib_INCLUDE_DIR )

	IF ( VkLib_FOUND )
		IF (MSVC)
			if ( VkLib_LIBRARY_DEBUG )
				set( VkLib_LIBRARIES optimized ${VkLib_LIBRARY_RELEASE} debug ${VkLib_LIBRARY_DEBUG} CACHE STRING "VkLib libraries" )
				set( VkLib_LIBRARY_DIRS ${VkLib_LIBRARY_RELEASE_DIR} ${VkLib_LIBRARY_DEBUG_DIR} )
			else()
				set( VkLib_LIBRARIES ${VkLib_LIBRARY_RELEASE} CACHE STRING "VkLib libraries" )
				set( VkLib_LIBRARY_DIRS ${VkLib_LIBRARY_RELEASE_DIR} )
			endif()
		ELSE ()
			if ( VkLib_LIBRARY_DEBUG )
				set( VkLib_LIBRARIES optimized ${VkLib_LIBRARY_RELEASE} debug ${VkLib_LIBRARY_DEBUG} CACHE STRING "VkLib libraries" )
				set( VkLib_LIBRARY_DIRS ${VkLib_LIBRARY_RELEASE_DIR} ${VkLib_LIBRARY_DEBUG_DIR} )
			else()
				set( VkLib_LIBRARIES ${VkLib_LIBRARY_RELEASE} CACHE STRING "VkLib libraries" )
				set( VkLib_LIBRARY_DIRS ${VkLib_LIBRARY_RELEASE_DIR} )
			endif()
		ENDIF ()
	ENDIF ()
endif ()
