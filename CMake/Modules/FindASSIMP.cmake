# FindASSIMP
# ------------
#
# Locate assimp library
#
# This module defines
#
# ::
#
#   ASSIMP_LIBRARIES, the libraries to link against
#   ASSIMP_FOUND, if false, do not try to link to assimp
#   ASSIMP_INCLUDE_DIR, where to find headers.
#

FIND_PATH(ASSIMP_ROOT_DIR include/assimp/ai_assert.h 
	HINTS
	PATH_SUFFIXES
		assimp
	PATHS
		/usr/local
		/usr
)

FIND_PATH(ASSIMP_INCLUDE_DIR assimp/ai_assert.h 
	HINTS
	PATH_SUFFIXES
		include
	PATHS
		${ASSIMP_ROOT_DIR}
		/usr/local/include
		/usr/include
)

if (CMAKE_CL_64 OR CMAKE_GENERATOR MATCHES Win64)
	if( MSVC )
		if ( MSVC14 )
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
		FIND_PATH( ASSIMP_LIBRARY_RELEASE_DIR assimp.lib assimp-vc${VC_NUM}-mt.lib
			HINTS
			PATH_SUFFIXES
				lib/x64
				lib/assimp_release-dll_x64
				lib/x64/Release
				lib/Release/x64
				lib
			PATHS
				${ASSIMP_ROOT_DIR}
		)

		FIND_PATH(ASSIMP_LIBRARY_DEBUG_DIR assimpD.lib assimpd.lib assimp-vc${VC_NUM}-mtd.lib
			HINTS
			PATH_SUFFIXES
				lib/x64
				lib/assimp_debug-dll_x64
				lib/x64/Debug
				lib/Debug/x64
				lib
			PATHS
				${ASSIMP_ROOT_DIR}
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)

		FIND_LIBRARY(ASSIMP_LIBRARY_RELEASE
			NAMES
				assimp-vc${VC_NUM}-mt.lib
				assimp.lib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)

		FIND_LIBRARY(ASSIMP_LIBRARY_DEBUG
			NAMES
				assimp-vc${VC_NUM}-mtd.lib
				assimpD.lib
				assimpd.lib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_DEBUG_DIR}
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)
	else()
		FIND_PATH(ASSIMP_LIBRARY_RELEASE_DIR libassimp.so libassimp.lib libassimp.dylib
			HINTS
			PATH_SUFFIXES
				lib64
				lib/assimp_release-dll_win64
				lib/x64/Release
				lib/Release/x64
				lib
			PATHS
				${ASSIMP_ROOT_DIR}
		)

		FIND_PATH(ASSIMP_LIBRARY_DEBUG_DIR libassimp.so libassimp.lib libassimp.dylib
			HINTS
			PATH_SUFFIXES
				lib64
				lib/assimp_debug-dll_win64
				lib/x64/Debug
				lib/Debug/x64
				lib
			PATHS
				${ASSIMP_ROOT_DIR}
		)

		FIND_LIBRARY(ASSIMP_LIBRARY_RELEASE
			NAMES
				libassimp.so
				libassimp.dll.a
				libassimp.dylib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)

		FIND_LIBRARY(ASSIMP_LIBRARY_DEBUG
			NAMES
				libassimp.so
				libassimp.dll.a
				libassimp.dylib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_DEBUG_DIR}
		)
	endif()
else()
	if( MSVC )
		if ( MSVC14 )
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
		FIND_PATH(ASSIMP_LIBRARY_RELEASE_DIR assimp.lib assimp-vc${VC_NUM}-mt.lib
		HINTS
		PATH_SUFFIXES
			lib/x86
			lib/assimp_release-dll_win32
			lib/x86/Release
			lib/Release/x86
			lib
		PATHS
			${ASSIMP_ROOT_DIR}
		)

		FIND_PATH(ASSIMP_LIBRARY_DEBUG_DIR assimpD.lib assimpd.lib assimp-vc${VC_NUM}-mtd.lib
		HINTS
		PATH_SUFFIXES
			lib/x86
			lib/assimp_debug-dll_win32
			lib/x86/Debug
			lib/Debug/x86
			lib
		PATHS
			${ASSIMP_ROOT_DIR}
		)

		FIND_LIBRARY(ASSIMP_LIBRARY_RELEASE
			NAMES
				assimp.lib
				assimp-vc${VC_NUM}-mt.lib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)

		FIND_LIBRARY(ASSIMP_LIBRARY_DEBUG
			NAMES
				assimpD.lib
				assimpd.lib
				assimp-vc${VC_NUM}-mtd.lib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_DEBUG_DIR}
		)
	elseif( MINGW )
		FIND_PATH(ASSIMP_LIBRARY_RELEASE_DIR libassimp.dll.a
			HINTS
			PATH_SUFFIXES
				lib/mingw
				lib/x86/Debug
				lib/Debug/x86
				lib/x86
				lib
			PATHS
				${ASSIMP_ROOT_DIR}
		)

		FIND_PATH(ASSIMP_LIBRARY_DEBUG_DIR libassimp.dll.a
			HINTS
			PATH_SUFFIXES
				lib/mingw
				lib/x86/Debug
				lib/Debug/x86
				lib/x86
				lib
			PATHS
				${ASSIMP_ROOT_DIR}
		)

		FIND_LIBRARY(ASSIMP_LIBRARY_RELEASE
			NAMES
				libassimp.dll.a
			HINTS
			PATHS
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)

		FIND_LIBRARY(ASSIMP_LIBRARY_DEBUG
			NAMES
				libassimpd.dll.a
			HINTS
			PATHS
				${ASSIMP_LIBRARY_DEBUG_DIR}
		)
	else()
		FIND_PATH(ASSIMP_LIBRARY_RELEASE_DIR libassimp.so libassimp.dylib
			HINTS
			PATH_SUFFIXES
				lib/assimp_release-dll_win32
				lib/x86/Release
				lib/Release/x86
				lib
			PATHS
				${ASSIMP_ROOT_DIR}
		)

		FIND_PATH(ASSIMP_LIBRARY_DEBUG_DIR libassimp.so libassimp.dylib
			HINTS
			PATH_SUFFIXES
				lib/assimp_debug-dll_win32
				lib/x86/Debug
				lib/Debug/x86
				lib
			PATHS
				${ASSIMP_ROOT_DIR}
		)

		FIND_LIBRARY(ASSIMP_LIBRARY_RELEASE
			NAMES
				libassimp.so
				libassimp.dylib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)

		FIND_LIBRARY(ASSIMP_LIBRARY_DEBUG
			NAMES
				libassimp.so
				libassimp.dylib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_DEBUG_DIR}
		)
	endif()
endif()

MARK_AS_ADVANCED( ASSIMP_LIBRARY_RELEASE_DIR )
MARK_AS_ADVANCED( ASSIMP_LIBRARY_DEBUG_DIR )
MARK_AS_ADVANCED( ASSIMP_LIBRARY_RELEASE )
MARK_AS_ADVANCED( ASSIMP_LIBRARY_DEBUG_DIR )
find_package_handle_standard_args( ASSIMP DEFAULT_MSG ASSIMP_LIBRARY_RELEASE ASSIMP_INCLUDE_DIR )

IF ( ASSIMP_FOUND )
	IF (MSVC)
		if ( ASSIMP_LIBRARY_DEBUG )
			SET(ASSIMP_LIBRARIES optimized ${ASSIMP_LIBRARY_RELEASE} debug ${ASSIMP_LIBRARY_DEBUG} CACHE STRING "Assimp libraries")
			SET(ASSIMP_LIBRARY_DIRS ${ASSIMP_LIBRARY_RELEASE_DIR} ${ASSIMP_LIBRARY_DEBUG_DIR})
		else()
			SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY_RELEASE} CACHE STRING "Assimp libraries")
			SET(ASSIMP_LIBRARY_DIRS ${ASSIMP_LIBRARY_RELEASE_DIR})
		endif()
	ELSE ()
		if ( ASSIMP_LIBRARY_DEBUG )
			SET(ASSIMP_LIBRARIES optimized ${ASSIMP_LIBRARY_RELEASE} debug ${ASSIMP_LIBRARY_DEBUG} CACHE STRING "Assimp libraries")
			SET(ASSIMP_LIBRARY_DIRS ${ASSIMP_LIBRARY_RELEASE_DIR} ${ASSIMP_LIBRARY_DEBUG_DIR})
		else()
			SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY_RELEASE} CACHE STRING "Assimp libraries")
			SET(ASSIMP_LIBRARY_DIRS ${ASSIMP_LIBRARY_RELEASE_DIR})
		endif()
	ENDIF ()
ENDIF ()