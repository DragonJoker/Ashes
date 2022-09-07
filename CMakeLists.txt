cmake_minimum_required( VERSION 3.13 )
cmake_policy( VERSION 3.13 )

if ( VCPKG_TARGET_TRIPLET )
	set( VCPKG_TOOLCHAIN ON )
endif ()

set( MAIN_PROJECT_NAME "Ashes" )
set( BUILD_ASHES 1 )

# Used to look for external modules
if ( NOT CMAKE_MODULE_PATH )
	if ( VCPKG_TOOLCHAIN )
		set( CMAKE_MODULE_PATH
			${CMAKE_SOURCE_DIR}/CMake
			${CMAKE_SOURCE_DIR}/CMake/Modules
			${CMAKE_SOURCE_DIR}/CMake/Toolchains
		)
	else ()
		set( CMAKE_MODULE_PATH
			${CMAKE_SOURCE_DIR}/CMake
			${CMAKE_SOURCE_DIR}/CMake/Modules
			${CMAKE_SOURCE_DIR}/CMake/Modules/Traditional
			${CMAKE_SOURCE_DIR}/CMake/Toolchains
		)
	endif ()
endif ()

set( CMAKE_TEMPLATES_DIR ${CMAKE_SOURCE_DIR}/CMake/Templates )
set( CMAKE_POSITION_INDEPENDENT_CODE ON )

project( ${MAIN_PROJECT_NAME} )

include( FileUtils )
include( PCHSupport )
include( Project )
include( Doxygen )

option( ASHES_BUILD_TEMPLATES "Build Ashes template applications" ON )
option( ASHES_BUILD_TESTS "Build Ashes test applications" ON )
option( ASHES_BUILD_SAMPLES "Build Ashes sample applications" ON )

# Organize projects into folders
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

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
macro( parse_subdir_files_ex _BASE_DIR _PROJGROUP _FOLDER _GROUP )
	set( __PROJGROUP "${_PROJGROUP}" )
	if ( NOT ${__PROJGROUP} STREQUAL "" )
		set( __PROJGROUP "${__PROJGROUP}/" )
	endif ()
	file( GLOB _FOLDER_HEADER_FILES
		${_BASE_DIR}/include/${__PROJGROUP}${_PROJECT_NAME}/${_FOLDER}/*.h
		${_BASE_DIR}/include/${__PROJGROUP}${_PROJECT_NAME}/${_FOLDER}/*.hpp
		${_BASE_DIR}/include/${__PROJGROUP}${_PROJECT_NAME}/${_FOLDER}/*.inl
		${_BASE_DIR}/source/${__PROJGROUP}${_PROJECT_NAME}/${_FOLDER}/*.h
		${_BASE_DIR}/source/${__PROJGROUP}${_PROJECT_NAME}/${_FOLDER}/*.hpp
		${_BASE_DIR}/source/${__PROJGROUP}${_PROJECT_NAME}/${_FOLDER}/*.inl
	)
	file( GLOB _FOLDER_SOURCE_FILES
		${_BASE_DIR}/include/${__PROJGROUP}${_PROJECT_NAME}/${_FOLDER}/*.c
		${_BASE_DIR}/include/${__PROJGROUP}${_PROJECT_NAME}/${_FOLDER}/*.cpp
		${_BASE_DIR}/source/${__PROJGROUP}${_PROJECT_NAME}/${_FOLDER}/*.c
		${_BASE_DIR}/source/${__PROJGROUP}${_PROJECT_NAME}/${_FOLDER}/*.cpp
	)
	source_group( "Header Files\\${_GROUP}" FILES ${_FOLDER_HEADER_FILES} )
	source_group( "Source Files\\${_GROUP}" FILES ${_FOLDER_SOURCE_FILES} )
endmacro()

macro( parse_subdir_files _BASE_DIR _FOLDER _GROUP )
	parse_subdir_files_ex( ${_BASE_DIR} "" ${_FOLDER} ${_GROUP} )
endmacro()

if ( VCPKG_TOOLCHAIN )
	set( CMAKE_MAP_IMPORTED_CONFIG_MINSIZEREL Release )
	set( CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release ) 
endif ()

if ( MSVC )
	option( ASHES_ENABLE_ASAN "Enable address sanitizer on Ashes projects" OFF )
	if ( ASHES_ENABLE_ASAN )
		string( REGEX REPLACE "/ZI" "/Zi" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} )
	else ()
		string( REGEX REPLACE "/Zi" "/ZI" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} )
	endif ()
endif ()

if ( NOT EXISTS ${CMAKE_SOURCE_DIR}/external/Vulkan-Headers/include/ )
	if ( VCPKG_PACKAGE_BUILD )
		set( Vulkan_INCLUDE_DIR ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include )
	elseif ( VCPKG_TOOLCHAIN )
		find_path( Vulkan_INCLUDE_DIR "vulkan/vulkan.h" )
	else ()
		find_package( Vulkan REQUIRED )
	endif ()
else ()
	set( Vulkan_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/Vulkan-Headers/include )
endif ()

set(CMAKE_DEBUG_POSTFIX "d")

set( Ashes_BASE_DIR "Ashes" )
set( Ashes_SOURCE_DIR ${CMAKE_SOURCE_DIR} )
set( Ashes_BINARY_DIR ${CMAKE_BINARY_DIR}/source )

if ( WIN32 )
	set( Ashes_BINARY_DEFINITIONS VK_USE_PLATFORM_WIN32_KHR=1 )
elseif ( ANDROID )
	set( Ashes_BINARY_DEFINITIONS VK_USE_PLATFORM_ANDROID_KHR=1 )
elseif ( APPLE )
	set( Ashes_BINARY_DEFINITIONS VK_USE_PLATFORM_MACOS_MVK=1 )
else ()
	set( Ashes_BINARY_DEFINITIONS VK_USE_PLATFORM_XLIB_KHR=1 )
endif ()

add_subdirectory( source )

if ( ASHES_BUILD_TEMPLATES )
	add_subdirectory( templates )
endif ()

add_subdirectory( test )

if ( ASHES_BUILD_SAMPLES )
	add_subdirectory( samples )
endif ()
