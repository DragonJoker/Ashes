cmake_minimum_required( VERSION 2.8 )

set( MAIN_PROJECT_NAME "Ashes" )
set( BUILD_ASHES 1 )

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
include( Project )
include( Doxygen )

option( ASHES_BUILD_TEMPLATES "Build Ashes template applications" TRUE )
option( ASHES_BUILD_TESTS "Build Ashes test applications" TRUE )
option( ASHES_BUILD_SAMPLES "Build Ashes sample applications" TRUE )

# Organize projects into folders
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

if ( ANDROID OR CMAKE_COMPILER_IS_GNUCXX )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y" )
	add_definitions(
		-fdiagnostics-color=auto
		-fPIC
	)
elseif ( MSVC )
  add_definitions(
		-D_CRT_SECURE_NO_WARNINGS
		-D_SCL_SECURE_NO_WARNINGS
		-D_ENFORCE_MATCHING_ALLOCATORS=0
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

# Small macro to add subdirectory files to current target source and header files
macro( parse_subdir_files _FOLDER _GROUP )
	file( GLOB _FOLDER_HEADER_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/${_FOLDER}/*.h
		${CMAKE_CURRENT_SOURCE_DIR}/${_FOLDER}/*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/${_FOLDER}/*.inl
	)
	file( GLOB _FOLDER_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/${_FOLDER}/*.c
		${CMAKE_CURRENT_SOURCE_DIR}/${_FOLDER}/*.cpp
	)
	source_group( "Header Files\\${_GROUP}" FILES ${_FOLDER_HEADER_FILES} )
	source_group( "Source Files\\${_GROUP}" FILES ${_FOLDER_SOURCE_FILES} )
endmacro()

if ( MSVC )
	string( REGEX REPLACE "/Zi" "/ZI" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} )
endif ()

include_directories(
	${CMAKE_BINARY_DIR}/Ashes/Ashes
	${CMAKE_SOURCE_DIR}/Ashes
)

if ( WIN32 )
	set( BinLibraries
		Dbghelp
	)
else ()
	set( BinLibraries
		dl
	)
endif ()

add_subdirectory( Ashes )

if ( ASHES_BUILD_TESTS OR ASHES_BUILD_SAMPLES OR ASHES_BUILD_TEMPLATES )
	add_subdirectory( Utils )
endif ()

if ( ASHES_BUILD_TEMPLATES )
	add_subdirectory( Templates )
endif ()

if ( ASHES_BUILD_TESTS )
	add_subdirectory( Test )
endif ()

if ( ASHES_BUILD_SAMPLES )
	add_subdirectory( Samples )
endif ()
