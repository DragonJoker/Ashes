cmake_minimum_required( VERSION 3.10 )
cmake_policy( VERSION 3.10 )

if ( VCPKG_TARGET_TRIPLET )
	set( VCPKG_TOOLCHAIN ON )
endif ()

set( MAIN_PROJECT_NAME "Ashes" )
set( BUILD_ASHES 1 )

# Used to look for external modules
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
set( CMAKE_TEMPLATES_DIR ${CMAKE_SOURCE_DIR}/CMake/Templates )

project( ${MAIN_PROJECT_NAME} )

include( Platform )
include( FileUtils )
include( PCHSupport )
include( Project )
include( Doxygen )

option( ASHES_BUILD_TEMPLATES "Build Ashes template applications" ON )
option( ASHES_BUILD_TESTS "Build Ashes test applications" ON )
option( ASHES_BUILD_SAMPLES "Build Ashes sample applications" ON )

if ( NOT APPLE )
	option( ASHES_BUILD_INFO "Build AshesInfo application" ON )
endif ()

if ( EXISTS ${CMAKE_SOURCE_DIR}/test/Vulkan/CMakeLists.txt )
	option( ASHES_BUILD_SW_SAMPLES "Build Sascha Willems examples." FALSE )
endif ()

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

if ( MSVC )
	string( REGEX REPLACE "/Zi" "/ZI" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} )
endif ()

if ( VCPKG_TOOLCHAIN OR NOT EXISTS ${CMAKE_SOURCE_DIR}/external/Vulkan-Headers/include/ )
	find_package( Vulkan REQUIRED )
else ()
	set( Vulkan_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/Vulkan-Headers/include )
endif ()

if ( VCPKG_TOOLCHAIN OR NOT EXISTS ${CMAKE_SOURCE_DIR}/external/spirv_cross/spirv_cross.cpp )
	set( ASHES_BUILD_SPIRV_CROSS OFF )
	if ( VCPKG_TOOLCHAIN )
		set( spirv-cross_DIR ${ASHES_INSTALLED_DIR} )
	endif ()
	find_package( spirv-cross REQUIRED COMPONENTS core cpp glsl hlsl reflect util )
	if ( spirv-cross_FOUND )
		set( ASHES_HAS_SPIRV_CROSS ON )
		set( SPIRV_CROSS_LIBRARIES
			spirv-cross-core
			spirv-cross-cpp
			spirv-cross-glsl
			spirv-cross-reflect
			spirv-cross-util
		)
		set( SPIRV_CROSS_HLSL_LIBRARIES
			spirv-cross-hlsl
		)
	else ()
		set( ASHES_HAS_SPIRV_CROSS OFF )
		message( FATAL_ERROR "Couldn't find spirv-cross" )
	endif ()
else ()
	set( ASHES_BUILD_SPIRV_CROSS ON )
	set( ASHES_HAS_SPIRV_CROSS ON )
	set( SPIRV_CROSS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external )
	set( SPIRV_CROSS_DIR ${SPIRV_CROSS_INCLUDE_DIR}/spirv_cross )
endif ()

set(CMAKE_DEBUG_POSTFIX "d")

set( Ashes_BASE_DIR "Ashes" )
set( Ashes_SOURCE_DIR ${CMAKE_SOURCE_DIR} )
set( Ashes_BINARY_DIR ${CMAKE_BINARY_DIR} )

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

if ( ASHES_BUILD_TESTS OR ASHES_BUILD_SAMPLES OR ASHES_BUILD_TEMPLATES )
	if ( NOT TARGET glslang )
		if ( VCPKG_TOOLCHAIN OR NOT EXISTS ${CMAKE_SOURCE_DIR}/external/spirv_cross/spirv_cross.cpp )
			if ( VCPKG_TOOLCHAIN )
				find_package( glslang CONFIG )
			else ()
				find_package( glslang )
			endif ()
			if ( GLSLANG_FOUND )
				set( ASHES_HAS_GLSLANG ON )
			else ()
				set( ASHES_HAS_GLSLANG OFF )
			endif ()
		else ()
			set( ASHES_HAS_GLSLANG ON )
			set( ASHES_BUILD_GLSLANG ON )
			set( ENABLE_GLSLANG_BINARIES FALSE CACHE BOOL "Builds glslangValidator and spirv-remap" FORCE )
			set( ENABLE_AMD_EXTENSIONS FALSE CACHE BOOL "Enables support of AMD-specific extensions" FORCE )
			set( ENABLE_NV_EXTENSIONS FALSE CACHE BOOL "Enables support of Nvidia-specific extensions" FORCE )
			set( ENABLE_HLSL FALSE CACHE BOOL "Enables HLSL input support" FORCE )
			set( BUILD_TESTING OFF )
			set( SKIP_GLSLANG_INSTALL ON )
			add_subdirectory( external/glslang )
		endif ()
	endif ()
endif ()

if ( ASHES_BUILD_TEMPLATES )
	add_subdirectory( templates )
endif ()

add_subdirectory( test )

if ( ASHES_BUILD_SAMPLES )
	add_subdirectory( samples )
endif ()
