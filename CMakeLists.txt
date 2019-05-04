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
macro( parse_subdir_files _BASE_DIR _FOLDER _GROUP )
	file( GLOB _FOLDER_HEADER_FILES
		${_BASE_DIR}/include/${PROJECT_NAME}/${_FOLDER}/*.h
		${_BASE_DIR}/include/${PROJECT_NAME}/${_FOLDER}/*.hpp
		${_BASE_DIR}/include/${PROJECT_NAME}/${_FOLDER}/*.inl
	)
	file( GLOB _FOLDER_SOURCE_FILES
		${_BASE_DIR}/source/${PROJECT_NAME}/${_FOLDER}/*.c
		${_BASE_DIR}/source/${PROJECT_NAME}/${_FOLDER}/*.cpp
	)
	source_group( "Header Files\\${_GROUP}" FILES ${_FOLDER_HEADER_FILES} )
	source_group( "Source Files\\${_GROUP}" FILES ${_FOLDER_SOURCE_FILES} )
endmacro()

if ( MSVC )
	string( REGEX REPLACE "/Zi" "/ZI" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} )
endif ()

if ( WIN32 )
	set( BinLibraries
		Dbghelp
	)
else ()
	set( BinLibraries
		dl
	)
endif ()

add_subdirectory( source )

if ( ASHES_BUILD_TESTS OR ASHES_BUILD_SAMPLES OR ASHES_BUILD_TEMPLATES )
	set( ENABLE_GLSLANG_BINARIES FALSE CACHE BOOL "Builds glslangValidator and spirv-remap" FORCE )
	set( ENABLE_AMD_EXTENSIONS FALSE CACHE BOOL "Enables support of AMD-specific extensions" FORCE )
	set( ENABLE_NV_EXTENSIONS FALSE CACHE BOOL "Enables support of Nvidia-specific extensions" FORCE )
	set( ENABLE_HLSL FALSE CACHE BOOL "Enables HLSL input support" FORCE )
	set( BUILD_TESTING OFF )
	set( SKIP_GLSLANG_INSTALL ON )
	add_subdirectory( external/glslang )

	target_install_subdir_headers( glslang external/glslang/glslang/ Public "" )
	target_install_subdir_headers( glslang external/glslang/glslang/ Include "" )
	target_install_subdir_headers( glslang external/glslang/glslang/ MachineIndependent "" )
	target_install_subdir_headers( SPIRV external/glslang/SPIRV/ "" "" )

	if ( MSVC )
		install(
			FILES
				${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/lib/glslang.lib
				${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/lib/OGLCompiler.lib
				${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/lib/OSDependent.lib
				${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/lib/SPIRV.lib
				${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/lib/SpvRemapper.lib
			COMPONENT glslang
			CONFIGURATIONS Release
			EXPORT glslang
			ARCHIVE DESTINATION lib
		)
		install(
			FILES
				${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/lib/glslang.lib
				${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/lib/OGLCompiler.lib
				${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/lib/OSDependent.lib
				${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/lib/SPIRV.lib
				${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/lib/SpvRemapper.lib
			COMPONENT glslang_dev
			CONFIGURATIONS RelWithDebInfo
			ARCHIVE DESTINATION lib/RelWithDebInfo
		)
		install(
			FILES
				${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/lib/glslangd.lib
				${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/lib/OGLCompilerd.lib
				${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/lib/OSDependentd.lib
				${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/lib/SPIRVd.lib
				${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/lib/SpvRemapperd.lib
			COMPONENT glslang_dev
			CONFIGURATIONS Debug
			ARCHIVE DESTINATION lib/Debug
		)
	else ()
		install(
			FILES
				${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/lib/libglslang.a
				${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/lib/libOGLCompiler.a
				${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/lib/libOSDependent.a
				${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/lib/libSPIRV.a
				${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/lib/libSpvRemapper.a
			COMPONENT glslang
			CONFIGURATIONS Release
			EXPORT glslang
			ARCHIVE DESTINATION lib
		)
		install(
			FILES
				${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/lib/libglslang.a
				${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/lib/libOGLCompiler.a
				${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/lib/libOSDependent.a
				${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/lib/libSPIRV.a
				${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/lib/libSpvRemapper.a
			COMPONENT glslang_dev
			CONFIGURATIONS RelWithDebInfo
			ARCHIVE DESTINATION lib/RelWithDebInfo
		)
		install(
			FILES
				${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/lib/libglslangd.a
				${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/lib/libOGLCompilerd.a
				${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/lib/libOSDependentd.a
				${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/lib/libSPIRVd.a
				${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/lib/libSpvRemapperd.a
			COMPONENT glslang_dev
			CONFIGURATIONS Debug
			ARCHIVE DESTINATION lib/Debug
		)
	endif ()
endif ()

if ( ASHES_BUILD_TEMPLATES )
	add_subdirectory( templates )
endif ()

if ( ASHES_BUILD_TESTS )
	add_subdirectory( test )
endif ()

if ( ASHES_BUILD_SAMPLES )
	add_subdirectory( samples )
endif ()
