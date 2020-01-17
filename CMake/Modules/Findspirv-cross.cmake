# Find spirv-cross
# ----------------
#
# Finds the spirv-cross libraries.
#
# This module defines:
#
# ::
#
#   spirv-cross_FOUND        - True if spirv-cross library is found.
#   spirv-cross_INCLUDE_DIR  - The spirv-cross include directiories.
#   spirv-cross_LIBRARIES    - The spirv-cross libraries.
#
# COMPONENTS directive is supported, the supported components are:
#
#   c
#   core
#   cpp
#   glsl
#   hlsl
#   msl
#   reflect
#   util
#

find_package( PackageHandleStandardArgs )

if ( NOT spirv-cross_FIND_COMPONENTS )
	set( spirv-cross_FIND_COMPONENTS core glsl reflect )
endif ()

set( PLATFORM "x86" )
set( SDK_PLATFORM "32" )
if ( MSVC )
	if( (CMAKE_CL_64 OR CMAKE_GENERATOR MATCHES Win64) )
		set( PLATFORM "x64" )
		set( SDK_PLATFORM "" )
	endif()
else()
	if( (${CMAKE_SIZEOF_VOID_P} EQUAL 8) AND NOT MINGW )
		set( PLATFORM "x64" )
		set( SDK_PLATFORM "" )
	endif()
endif()

find_path( spirv-cross_DIR
		include/spirv-cross/spirv.hpp
	HINTS
	PATHS
		/usr/local
		/usr
)

if ( spirv-cross_DIR )
	set( spirv-cross_FOUND ON )
	find_path( spirv-cross_INCLUDE_DIR
			spirv_cross/spirv.hpp
		HINTS
		PATH_SUFFIXES
			include
		PATHS
			${spirv-cross_DIR}
			/usr/local/include
			/usr/include
	)

	foreach( COMPONENT ${spirv-cross_FIND_COMPONENTS} )
		find_path( spirv-cross_${COMPONENT}_LIBRARY_RELEASE_DIR
				spirv-cross-${COMPONENT}.lib
				spirv-cross-${COMPONENT}.a
				libspirv-cross-${COMPONENT}.a
			HINTS
			PATH_SUFFIXES
				lib/${PLATFORM}/Release
				lib/${PLATFORM}
				lib/Release
				lib
				Lib${SDK_PLATFORM}
				/usr/local/lib
				/usr/lib
			PATHS
				${spirv-cross_DIR}
		)
		find_path( spirv-cross_${COMPONENT}_LIBRARY_DEBUG_DIR
				spirv-cross-${COMPONENT}.lib
				spirv-cross-${COMPONENT}.a
				libspirv-cross-${COMPONENT}.a
			HINTS
			PATH_SUFFIXES
				lib/${PLATFORM}/Debug
				lib/${PLATFORM}
				lib/Debug
				lib
				Lib${SDK_PLATFORM}
				/usr/local/lib
				/usr/lib
			PATHS
				${spirv-cross_DIR}/debug
				${spirv-cross_DIR}
		)
		find_library( spirv-cross_${COMPONENT}_LIBRARY_RELEASE
			NAMES
				spirv-cross-${COMPONENT}
			HINTS
			PATHS
				${spirv-cross_${COMPONENT}_LIBRARY_RELEASE_DIR}
		)
		find_library( spirv-cross_${COMPONENT}_LIBRARY_DEBUG
			NAMES
				spirv-cross-${COMPONENT}
			HINTS
			PATHS
				${spirv-cross_${COMPONENT}_LIBRARY_DEBUG_DIR}
		)
		find_package_handle_standard_args( spirv-cross-${COMPONENT} DEFAULT_MSG
			spirv-cross_${COMPONENT}_LIBRARY_RELEASE
			spirv-cross_${COMPONENT}_LIBRARY_DEBUG
		)

		if ( spirv-cross-${COMPONENT}_FOUND )
			add_library( spirv-cross-${COMPONENT} STATIC IMPORTED )
			set_target_properties( spirv-cross-${COMPONENT} PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES ${spirv-cross_INCLUDE_DIR}
			)

			if ( spirv-cross_${COMPONENT}_LIBRARY_DEBUG )
				set_property( TARGET spirv-cross-${COMPONENT} APPEND
					PROPERTY
						IMPORTED_CONFIGURATIONS DEBUG
				)
				set_target_properties( spirv-cross-${COMPONENT} PROPERTIES
					IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
					IMPORTED_LOCATION_DEBUG ${spirv-cross_${COMPONENT}_LIBRARY_DEBUG}
				)
			else ()
				set_property( TARGET spirv-cross-${COMPONENT} APPEND
					PROPERTY
						IMPORTED_CONFIGURATIONS RELEASE
				)
				set_target_properties( spirv-cross-${COMPONENT} PROPERTIES
					IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
					IMPORTED_LOCATION_DEBUG ${spirv-cross_${COMPONENT}_LIBRARY_RELEASE}
				)
			endif ()

			unset( spirv-cross_${COMPONENT}_LIBRARY_RELEASE_DIR CACHE )
			unset( spirv-cross_${COMPONENT}_LIBRARY_DEBUG_DIR CACHE )
			unset( spirv-cross_${COMPONENT}_LIBRARY_RELEASE CACHE )
			unset( spirv-cross_${COMPONENT}_LIBRARY_DEBUG CACHE )
		else ()
			mark_as_advanced( spirv-cross_${COMPONENT}_LIBRARY_RELEASE_DIR )
			mark_as_advanced( spirv-cross_${COMPONENT}_LIBRARY_DEBUG_DIR )
			mark_as_advanced( spirv-cross_${COMPONENT}_LIBRARY_RELEASE )
			mark_as_advanced( spirv-cross_${COMPONENT}_LIBRARY_DEBUG )
			set( spirv-cross_FOUND OFF )
		endif ()
	endforeach( COMPONENT ${spirv-cross_FIND_COMPONENTS} )

	mark_as_advanced( spirv-cross_DIR )

	if ( spirv-cross_FOUND )
		unset( spirv-cross_INCLUDE_DIR CACHE )
	endif ()
else ()
	set( spirv-cross_FOUND OFF )
endif ()

