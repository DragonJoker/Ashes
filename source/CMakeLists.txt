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

find_package( VkLib )

include_directories(
	${VkLib_INCLUDE_DIR}
)

add_subdirectory( FreeType )
add_subdirectory( Utils )
add_subdirectory( Renderer )
add_subdirectory( GlRenderer )
add_subdirectory( VkRenderer )
add_subdirectory( RenderLib )

if ( ANDROID )
	add_subdirectory( AndroidUtils )
	add_subdirectory( RenderLibTestApp )
elseif (WIN32)
	add_subdirectory( DesktopUtils )
	add_subdirectory( RenderLibTestApp )
endif ()

add_subdirectory( Test )
