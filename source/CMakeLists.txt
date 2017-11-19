cmake_minimum_required( VERSION 2.8 )

set( MAIN_PROJECT_NAME "RenderLib" )

# Used to look for external modules
set( CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/CMake
	${CMAKE_SOURCE_DIR}/CMake/Modules
	${CMAKE_SOURCE_DIR}/CMake/Toolchains
)
set( CMAKE_TEMPLATES_DIR ${CMAKE_SOURCE_DIR}/CMake/Templates )

project( ${MAIN_PROJECT_NAME} )

include( Platform )
include( FileUtils )
include( PCHSupport )

# Organize projects into folders
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

if ( ANDROID OR CMAKE_COMPILER_IS_GNUCXX )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y" )
	add_definitions(
		-fdiagnostics-color=auto
	)
elseif ( MSVC )
  add_definitions(
		-D_CRT_SECURE_NO_WARNINGS
	)
endif ()

if ( WIN32 )
	add_definitions( -DDESKTOP )
	set( DESKTOP TRUE )
endif ()

macro( add_forced_include _FILE )
	if ( MSVC )
		add_definitions( /FI"""${_FILE}""" )
	else ()
		add_definitions( -include "${_FILE}" )
	endif ()
endmacro( add_forced_include )

macro( source_add_forced_include _FILE ) #ARGN: _FILES
	if ( MSVC )
		set_source_files_properties( ${ARGN}
			PROPERTIES
				COMPILE_FLAGS /FI${_FILE}
		)
	else ()
		set_source_files_properties( ${ARGN}
			PROPERTIES
				COMPILE_FLAGS -include ${_FILE}
		)
	endif ()
endmacro( source_add_forced_include )

if ( MSVC )
	string( REGEX REPLACE "/Zi" "/ZI" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} )
endif ()

add_subdirectory( GlLib )
add_subdirectory( RenderLib )
add_subdirectory( FreeType )
add_subdirectory( StarMapLib )

if ( ANDROID )
	add_subdirectory( AndroidUtils )
else ()
	add_subdirectory( DesktopUtils )
endif ()

add_subdirectory( RenderLibTestApp )
add_subdirectory( StarMapTestApp )