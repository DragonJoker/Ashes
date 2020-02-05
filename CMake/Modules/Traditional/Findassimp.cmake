# FindASSIMP
# ------------
#
# Locate ASSIMP library
#
# This module defines
#
# ::
#
#   ASSIMP_LIBRARIES, the libraries to link against
#   ASSIMP_FOUND, if false, do not try to link to ASSIMP
#   ASSIMP_INCLUDE_DIR, where to find headers.
#

find_path( ASSIMP_DIR include/assimp/ai_assert.h 
	HINTS
	PATH_SUFFIXES
		ASSIMP
	PATHS
		/usr/local
		/usr
)

find_path( ASSIMP_INCLUDE_DIR assimp/ai_assert.h 
	HINTS
	PATH_SUFFIXES
		include
	PATHS
		${ASSIMP_DIR}
		/usr/local/include
		/usr/include
)

if( MSVC )
	set_property( CACHE ASSIMP_VC_NUM PROPERTY STRINGS 142 141 140 120 110 100 )
	if ( CMAKE_GENERATOR STREQUAL "Visual Studio 16 2019" )
		set( ASSIMP_VC_NUM 142 CACHE STRING "Assimp MSVC library version" )
	elseif ( CMAKE_GENERATOR STREQUAL "Visual Studio 15 2017" )
		set( ASSIMP_VC_NUM 141 CACHE STRING "Assimp MSVC library version" )
	elseif ( MSVC14 )
		set( ASSIMP_VC_NUM 140 CACHE STRING "Assimp MSVC library version" )
	elseif ( MSVC12 )
		set( ASSIMP_VC_NUM 120 CACHE STRING "Assimp MSVC library version" )
	elseif ( MSVC11 )
		set( ASSIMP_VC_NUM 110 CACHE STRING "Assimp MSVC library version" )
	elseif ( MSVC10 )
		set( ASSIMP_VC_NUM 100 CACHE STRING "Assimp MSVC library version" )
	else ()
		message( SEND_ERROR "Unsupported MSVC version" )
	endif ()
endif ()

if (CMAKE_CL_64 OR CMAKE_GENERATOR MATCHES Win64)
	if( MSVC )
		find_path( ASSIMP_LIBRARY_RELEASE_DIR assimp.lib assimp-vc${ASSIMP_VC_NUM}-mt.lib
			HINTS
			PATH_SUFFIXES
				lib/x64
				lib/ASSIMP_release-dll_x64
				lib/x64/Release
				lib/Release/x64
				lib
			PATHS
				${ASSIMP_DIR}
		)
		find_path( ASSIMP_LIBRARY_DEBUG_DIR ASSIMPD.lib ASSIMPd.lib assimp-vc${ASSIMP_VC_NUM}-mtd.lib
			HINTS
			PATH_SUFFIXES
				lib/x64
				lib/ASSIMP_debug-dll_x64
				lib/x64/Debug
				lib/Debug/x64
				lib
			PATHS
				${ASSIMP_DIR}
				${ASSIMP_DIR}/debug
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)
		find_library( ASSIMP_LIBRARY_RELEASE
			NAMES
				assimp-vc${ASSIMP_VC_NUM}-mt.lib
				assimp.lib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)
		find_library( ASSIMP_LIBRARY_DEBUG
			NAMES
				assimp-vc${ASSIMP_VC_NUM}-mtd.lib
				ASSIMPD.lib
				ASSIMPd.lib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_DEBUG_DIR}
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)
	else()
		find_path( ASSIMP_LIBRARY_RELEASE_DIR libASSIMP.so libASSIMP.lib libASSIMP.dylib
			HINTS
			PATH_SUFFIXES
				lib64
				lib/ASSIMP_release-dll_win64
				lib/x64/Release
				lib/Release/x64
				lib
			PATHS
				${ASSIMP_DIR}
		)
		find_path( ASSIMP_LIBRARY_DEBUG_DIR libASSIMP.so libASSIMP.lib libASSIMP.dylib
			HINTS
			PATH_SUFFIXES
				lib64
				lib/ASSIMP_debug-dll_win64
				lib/x64/Debug
				lib/Debug/x64
				lib
			PATHS
				${ASSIMP_DIR}
				${ASSIMP_DIR}/debug
		)
		find_library( ASSIMP_LIBRARY_RELEASE
			NAMES
				libASSIMP.so
				libASSIMP.dll.a
				libASSIMP.dylib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)
		find_library( ASSIMP_LIBRARY_DEBUG
			NAMES
				libASSIMP.so
				libASSIMP.dll.a
				libASSIMP.dylib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_DEBUG_DIR}
		)
	endif()
else()
	if( MSVC )
		find_path( ASSIMP_LIBRARY_RELEASE_DIR assimp.lib assimp-vc${ASSIMP_VC_NUM}-mt.lib
		HINTS
		PATH_SUFFIXES
			lib/x86
			lib/ASSIMP_release-dll_win32
			lib/x86/Release
			lib/Release/x86
			lib
		PATHS
			${ASSIMP_DIR}
		)
		find_path( ASSIMP_LIBRARY_DEBUG_DIR ASSIMPD.lib ASSIMPd.lib assimp-vc${ASSIMP_VC_NUM}-mtd.lib
		HINTS
		PATH_SUFFIXES
			lib/x86
			lib/ASSIMP_debug-dll_win32
			lib/x86/Debug
			lib/Debug/x86
			lib
		PATHS
			${ASSIMP_DIR}
			${ASSIMP_DIR}/debug
		)
		find_library( ASSIMP_LIBRARY_RELEASE
			NAMES
				assimp.lib
				assimp-vc${ASSIMP_VC_NUM}-mt.lib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)
		find_library( ASSIMP_LIBRARY_DEBUG
			NAMES
				ASSIMPD.lib
				ASSIMPd.lib
				assimp-vc${ASSIMP_VC_NUM}-mtd.lib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_DEBUG_DIR}
		)
	elseif( MINGW )
		find_path( ASSIMP_LIBRARY_RELEASE_DIR libASSIMP.dll.a
			HINTS
			PATH_SUFFIXES
				lib/mingw
				lib/x86/Debug
				lib/Debug/x86
				lib/x86
				lib
			PATHS
				${ASSIMP_DIR}
		)
		find_path( ASSIMP_LIBRARY_DEBUG_DIR libASSIMP.dll.a
			HINTS
			PATH_SUFFIXES
				lib/mingw
				lib/x86/Debug
				lib/Debug/x86
				lib/x86
				lib
			PATHS
				${ASSIMP_DIR}
				${ASSIMP_DIR}/debug
		)
		find_library( ASSIMP_LIBRARY_RELEASE
			NAMES
				libASSIMP.dll.a
			HINTS
			PATHS
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)
		find_library( ASSIMP_LIBRARY_DEBUG
			NAMES
				libASSIMPd.dll.a
			HINTS
			PATHS
				${ASSIMP_LIBRARY_DEBUG_DIR}
		)
	else()
		find_path( ASSIMP_LIBRARY_RELEASE_DIR libASSIMP.so libASSIMP.dylib
			HINTS
			PATH_SUFFIXES
				lib/ASSIMP_release-dll_win32
				lib/x86/Release
				lib/Release/x86
				lib
			PATHS
				${ASSIMP_DIR}
		)
		find_path( ASSIMP_LIBRARY_DEBUG_DIR libASSIMP.so libASSIMP.dylib
			HINTS
			PATH_SUFFIXES
				lib/ASSIMP_debug-dll_win32
				lib/x86/Debug
				lib/Debug/x86
				lib
			PATHS
				${ASSIMP_DIR}
				${ASSIMP_DIR}/debug
		)
		find_library( ASSIMP_LIBRARY_RELEASE
			NAMES
				libASSIMP.so
				libASSIMP.dylib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_RELEASE_DIR}
		)
		find_library( ASSIMP_LIBRARY_DEBUG
			NAMES
				libASSIMP.so
				libASSIMP.dylib
			HINTS
			PATHS
				${ASSIMP_LIBRARY_DEBUG_DIR}
		)
	endif()
endif()

mark_as_advanced( ASSIMP_LIBRARY_RELEASE_DIR )
mark_as_advanced( ASSIMP_LIBRARY_DEBUG_DIR )
mark_as_advanced( ASSIMP_LIBRARY_RELEASE )
mark_as_advanced( ASSIMP_LIBRARY_DEBUG_DIR )
find_package_handle_standard_args( ASSIMP DEFAULT_MSG ASSIMP_LIBRARY_RELEASE ASSIMP_INCLUDE_DIR )

if ( ASSIMP_FOUND )
	if ( ASSIMP_LIBRARY_DEBUG )
		add_library( assimp::assimp STATIC IMPORTED )
		set_target_properties( assimp::assimp PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES ${ASSIMP_INCLUDE_DIR}
			IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
			IMPORTED_LOCATION_DEBUG ${ASSIMP_LIBRARY_DEBUG}
			IMPORTED_LOCATION_RELEASE ${ASSIMP_LIBRARY_RELEASE}
		)
		set( ASSIMP_LIBRARIES optimized ${ASSIMP_LIBRARY_RELEASE} debug ${ASSIMP_LIBRARY_DEBUG} CACHE STRING "Assimp libraries" FORCE )
	else ()
		add_library( assimp::assimp STATIC IMPORTED )
		set_target_properties( assimp::assimp PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES ${ASSIMP_INCLUDE_DIR}
			IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
			IMPORTED_LOCATION ${ASSIMP_LIBRARY_RELEASE}
		)
		set( ASSIMP_LIBRARIES ${ASSIMP_LIBRARY_RELEASE} CACHE STRING "Assimp libraries" FORCE )
	endif ()
ENDIF ()