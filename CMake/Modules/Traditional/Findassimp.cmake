# FindASSIMP
# ------------
#
# Locate assimp library
#
# This module defines
#
# ::
#
#   assimp_LIBRARIES, the libraries to link against
#   assimp_FOUND, if false, do not try to link to assimp
#   assimp_INCLUDE_DIR, where to find headers.
#

find_path( assimp_DIR include/assimp/ai_assert.h 
	HINTS
	PATH_SUFFIXES
		assimp
	PATHS
		/usr/local
		/usr
)
find_path( assimp_INCLUDE_DIR assimp/ai_assert.h 
	HINTS
	PATH_SUFFIXES
		include
	PATHS
		${assimp_DIR}
		/usr/local/include
		/usr/include
)

if ( MSVC )
	if ( CMAKE_GENERATOR STREQUAL "Visual Studio 16 2019" )
		set( VC_NUM 142 140 )
	elseif ( CMAKE_GENERATOR STREQUAL "Visual Studio 15 2017" )
		set( VC_NUM 141 140 )
	elseif ( MSVC14 )
		set( VC_NUM 140 )
	elseif ( MSVC12 )
		set( VC_NUM 120 )
	elseif ( MSVC11 )
		set( VC_NUM 110 )
	elseif ( MSVC10 )
		set( VC_NUM 100 )
	else ()
		message( SEND_ERROR "Unsupported MSVC version" )
	endif ()
endif ()

if ( CMAKE_CL_64 OR CMAKE_GENERATOR MATCHES Win64 )
	if ( MSVC )
		foreach ( NUM ${VC_NUM} )
			if ( NOT assimp_LIBRARY_RELEASE )
				find_path( assimp_LIBRARY_RELEASE_DIR assimp.lib assimp-vc${NUM}-mt.lib
					HINTS
					PATH_SUFFIXES
						lib/x64
						lib/assimp_release-dll_x64
						lib/x64/Release
						lib/Release/x64
						lib
					PATHS
						${assimp_DIR}
				)
				find_path( assimp_LIBRARY_DEBUG_DIR assimpD.lib assimpd.lib assimp-vc${NUM}-mtd.lib
					HINTS
					PATH_SUFFIXES
						lib/x64
						lib/assimp_debug-dll_x64
						lib/x64/Debug
						lib/Debug/x64
						lib
					PATHS
						${assimp_DIR}
						${assimp_DIR}/debug
						${assimp_LIBRARY_RELEASE_DIR}
				)
				find_library( assimp_LIBRARY_RELEASE
					NAMES
						assimp-vc${NUM}-mt.lib
						assimp.lib
					HINTS
					PATHS
						${assimp_LIBRARY_RELEASE_DIR}
				)
				find_library( assimp_LIBRARY_DEBUG
					NAMES
						assimp-vc${NUM}-mtd.lib
						assimpD.lib
						assimpd.lib
					HINTS
					PATHS
						${assimp_LIBRARY_DEBUG_DIR}
						${assimp_LIBRARY_RELEASE_DIR}
				)
			endif ()
		endforeach ()
	else ()
		find_path( assimp_LIBRARY_RELEASE_DIR libassimp.so libassimp.lib libassimp.dylib
			HINTS
			PATH_SUFFIXES
				lib64
				lib/assimp_release-dll_win64
				lib/x64/Release
				lib/Release/x64
				lib
			PATHS
				${assimp_DIR}
		)
		find_path( assimp_LIBRARY_DEBUG_DIR libassimp.so libassimp.lib libassimp.dylib
			HINTS
			PATH_SUFFIXES
				lib64
				lib/assimp_debug-dll_win64
				lib/x64/Debug
				lib/Debug/x64
				lib
			PATHS
				${assimp_DIR}
				${assimp_DIR}/debug
		)
		find_library( assimp_LIBRARY_RELEASE
			NAMES
				libassimp.so
				libassimp.dll.a
				libassimp.dylib
			HINTS
			PATHS
				${assimp_LIBRARY_RELEASE_DIR}
		)
		find_library( assimp_LIBRARY_DEBUG
			NAMES
				libassimp.so
				libassimp.dll.a
				libassimp.dylib
			HINTS
			PATHS
				${assimp_LIBRARY_DEBUG_DIR}
		)
	endif ()
else ()
	if ( MSVC )
		foreach ( NUM ${VC_NUM} )
			if ( NOT assimp_LIBRARY_RELEASE )
				find_path( assimp_LIBRARY_RELEASE_DIR assimp.lib assimp-vc${NUM}-mt.lib
					HINTS
					PATH_SUFFIXES
						lib/x86
						lib/assimp_release-dll_win32
						lib/x86/Release
						lib/Release/x86
						lib
					PATHS
						${assimp_DIR}
				)
				find_path( assimp_LIBRARY_DEBUG_DIR assimpD.lib assimpd.lib assimp-vc${NUM}-mtd.lib
					HINTS
					PATH_SUFFIXES
						lib/x86
						lib/assimp_debug-dll_win32
						lib/x86/Debug
						lib/Debug/x86
						lib
					PATHS
						${assimp_DIR}
						${assimp_DIR}/debug
				)
				find_library( assimp_LIBRARY_RELEASE
					NAMES
						assimp.lib
						assimp-vc${NUM}-mt.lib
					HINTS
					PATHS
						${assimp_LIBRARY_RELEASE_DIR}
				)
				find_library( assimp_LIBRARY_DEBUG
					NAMES
						assimpD.lib
						assimpd.lib
						assimp-vc${NUM}-mtd.lib
					HINTS
					PATHS
						${assimp_LIBRARY_DEBUG_DIR}
				)
			endif ()
		endforeach ()
	elseif ( MINGW )
		find_path( assimp_LIBRARY_RELEASE_DIR libassimp.dll.a
			HINTS
			PATH_SUFFIXES
				lib/mingw
				lib/x86/Debug
				lib/Debug/x86
				lib/x86
				lib
			PATHS
				${assimp_DIR}
		)
		find_path( assimp_LIBRARY_DEBUG_DIR libassimp.dll.a
			HINTS
			PATH_SUFFIXES
				lib/mingw
				lib/x86/Debug
				lib/Debug/x86
				lib/x86
				lib
			PATHS
				${assimp_DIR}
				${assimp_DIR}/debug
		)
		find_library( assimp_LIBRARY_RELEASE
			NAMES
				libassimp.dll.a
			HINTS
			PATHS
				${assimp_LIBRARY_RELEASE_DIR}
		)
		find_library( assimp_LIBRARY_DEBUG
			NAMES
				libassimpd.dll.a
			HINTS
			PATHS
				${assimp_LIBRARY_DEBUG_DIR}
		)
	else ()
		find_path( assimp_LIBRARY_RELEASE_DIR libassimp.so libassimp.dylib
			HINTS
			PATH_SUFFIXES
				lib/assimp_release-dll_win32
				lib/x86/Release
				lib/Release/x86
				lib
			PATHS
				${assimp_DIR}
		)
		find_path( assimp_LIBRARY_DEBUG_DIR libassimp.so libassimp.dylib
			HINTS
			PATH_SUFFIXES
				lib/assimp_debug-dll_win32
				lib/x86/Debug
				lib/Debug/x86
				lib
			PATHS
				${assimp_DIR}
				${assimp_DIR}/debug
		)
		find_library( assimp_LIBRARY_RELEASE
			NAMES
				libassimp.so
				libassimp.dylib
			HINTS
			PATHS
				${assimp_LIBRARY_RELEASE_DIR}
		)
		find_library( assimp_LIBRARY_DEBUG
			NAMES
				libassimp.so
				libassimp.dylib
			HINTS
			PATHS
				${assimp_LIBRARY_DEBUG_DIR}
		)
	endif ()
endif ()

find_package_handle_standard_args( assimp DEFAULT_MSG assimp_LIBRARY_RELEASE assimp_INCLUDE_DIR )

if ( assimp_FOUND )
	if ( NOT TARGET assimp::assimp )
		add_library( assimp::assimp UNKNOWN IMPORTED )
		set_target_properties(assimp::assimp PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${assimp_INCLUDE_DIR}" )
		if ( assimp_LIBRARY_RELEASE AND assimp_LIBRARY_DEBUG )
			set_property( TARGET assimp::assimp APPEND PROPERTY
				IMPORTED_CONFIGURATIONS RELEASE )
			set_target_properties( assimp::assimp PROPERTIES
				IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
				IMPORTED_LOCATION_RELEASE "${assimp_LIBRARY_RELEASE}" )
			set_property( TARGET assimp::assimp APPEND PROPERTY
				IMPORTED_CONFIGURATIONS DEBUG )
			set_target_properties( assimp::assimp PROPERTIES
				IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
				IMPORTED_LOCATION_DEBUG "${assimp_LIBRARY_DEBUG}" )
		elseif ( assimp_LIBRARY_RELEASE )
			set_target_properties( assimp::assimp PROPERTIES
				IMPORTED_LINK_INTERFACE_LANGUAGES "C"
				IMPORTED_LOCATION "${assimp_LIBRARY_RELEASE}" )
		endif()
	endif()
	if ( MSVC )
		if ( assimp_LIBRARY_DEBUG )
			set( assimp_LIBRARIES optimized ${assimp_LIBRARY_RELEASE} debug ${assimp_LIBRARY_DEBUG} CACHE STRING "Assimp libraries" )
		else ()
			set( assimp_LIBRARIES ${assimp_LIBRARY_RELEASE} CACHE STRING "Assimp libraries" )
		endif ()
	else ()
		if ( assimp_LIBRARY_DEBUG )
			set( assimp_LIBRARIES optimized ${assimp_LIBRARY_RELEASE} debug ${assimp_LIBRARY_DEBUG} CACHE STRING "Assimp libraries" )
		else ()
			set( assimp_LIBRARIES ${assimp_LIBRARY_RELEASE} CACHE STRING "Assimp libraries" )
		endif ()
	endif ()
	unset( assimp_LIBRARY_RELEASE_DIR CACHE )
	unset( assimp_LIBRARY_DEBUG_DIR CACHE )
else ()
	mark_as_advanced( assimp_LIBRARY_RELEASE_DIR )
	mark_as_advanced( assimp_LIBRARY_DEBUG_DIR )
endif ()

mark_as_advanced( assimp_LIBRARY_RELEASE )
mark_as_advanced( assimp_LIBRARY_DEBUG )
