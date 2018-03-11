# Find OpenGL ES 3
# ----------------
#
# Finds the glslang library.
#
# This module defines:
#
# ::
#
#   glslang_FOUND        - True if glslang library is found.
#   glslang_INCLUDE_DIR  - The glslang include directiories.
#   glslang_LIBRARIES    - The glslang libraries.
#

find_package( PackageHandleStandardArgs )

find_path( glslang_ROOT_DIR include/glslang/Public/ShaderLang.h
	HINTS
	PATHS
		/usr/local
		/usr
)

if ( glslang_ROOT_DIR )
	find_path( glslang_INCLUDE_DIR glslang/Public/ShaderLang.h
		HINTS
		PATH_SUFFIXES
			include
		PATHS
			${glslang_ROOT_DIR}
			/usr/local/include
			/usr/include
	)

	if (CMAKE_CL_64 OR CMAKE_GENERATOR MATCHES Win64)
		set( PLATFORM "x64" )
	else ()
		set( PLATFORM "x86" )
	endif ()

	find_path( glslang_LIBRARY_RELEASE_DIR glslang.lib libglslang.a
		HINTS
		PATH_SUFFIXES
			lib/${PLATFORM}/Release
			lib/${PLATFORM}
			lib/Release
			lib
			/usr/local/lib
			/usr/lib
		PATHS
			${glslang_ROOT_DIR}
	)

	find_path( glslang_LIBRARY_DEBUG_DIR glslangd.lib libglslangd.a
		HINTS
		PATH_SUFFIXES
			lib/${PLATFORM}
			lib
			/usr/local/lib
			/usr/lib
		PATHS
			${glslang_ROOT_DIR}
	)

	if ( NOT glslang_LIBRARY_DEBUG_DIR )
		find_path( glslang_LIBRARY_DEBUG_DIR glslang.lib libglslang.a
			HINTS
			PATH_SUFFIXES
				lib/${PLATFORM}/Debug
				lib/Debug
				/usr/local/lib
				/usr/lib
			PATHS
				${glslang_ROOT_DIR}
		)
	endif ()

	find_library( glslang_LIBRARY_RELEASE
		NAMES
			glslang
		HINTS
		PATHS
			${glslang_LIBRARY_RELEASE_DIR}
	)

	find_library( glslang_LIBRARY_DEBUG
		NAMES
			glslangd
			glslang
		HINTS
		PATHS
			${glslang_LIBRARY_DEBUG_DIR}
	)

	find_library( OSDependent_LIBRARY_RELEASE
		NAMES
			OSDependent
		HINTS
		PATHS
			${glslang_LIBRARY_RELEASE_DIR}
	)

	find_library( OSDependent_LIBRARY_DEBUG
		NAMES
			OSDependentd
			OSDependent
		HINTS
		PATHS
			${glslang_LIBRARY_DEBUG_DIR}
	)

	find_library( OGLCompiler_LIBRARY_RELEASE
		NAMES
			OGLCompiler
		HINTS
		PATHS
			${glslang_LIBRARY_RELEASE_DIR}
	)

	find_library( OGLCompiler_LIBRARY_DEBUG
		NAMES
			OGLCompilerd
			OGLCompiler
		HINTS
		PATHS
			${glslang_LIBRARY_DEBUG_DIR}
	)

	mark_as_advanced( glslang_LIBRARY_RELEASE_DIR )
	mark_as_advanced( glslang_LIBRARY_DEBUG_DIR )
	mark_as_advanced( glslang_LIBRARY_RELEASE )
	mark_as_advanced( glslang_LIBRARY_DEBUG )
	mark_as_advanced( OSDependent_LIBRARY_RELEASE )
	mark_as_advanced( OSDependent_LIBRARY_DEBUG )
	mark_as_advanced( OGLCompiler_LIBRARY_RELEASE )
	mark_as_advanced( OGLCompiler_LIBRARY_DEBUG )
	find_package_handle_standard_args( glslang DEFAULT_MSG
		glslang_LIBRARY_RELEASE
		glslang_INCLUDE_DIR )

	IF ( glslang_FOUND )
		IF (MSVC)
			if ( glslang_LIBRARY_DEBUG )
				set( glslang_LIBRARIES
					optimized ${glslang_LIBRARY_RELEASE}
					debug ${glslang_LIBRARY_DEBUG}
					optimized ${OSDependent_LIBRARY_RELEASE}
					debug ${OSDependent_LIBRARY_DEBUG}
					optimized ${OGLCompiler_LIBRARY_RELEASE}
					debug ${OGLCompiler_LIBRARY_DEBUG}
					CACHE STRING "glslang libraries"
				)
				set( glslang_LIBRARY_DIRS
					${glslang_LIBRARY_RELEASE_DIR}
					${glslang_LIBRARY_DEBUG_DIR}
				)
			else()
				set( glslang_LIBRARIES
					${glslang_LIBRARY_RELEASE}
					${OSDependent_LIBRARY_RELEASE}
					${OGLCompiler_LIBRARY_RELEASE}
					CACHE STRING "glslang libraries"
				)
				set( glslang_LIBRARY_DIRS
					${glslang_LIBRARY_RELEASE_DIR}
				)
			endif()
		ELSE ()
			if ( glslang_LIBRARY_DEBUG )
				set( glslang_LIBRARIES
					optimized ${glslang_LIBRARY_RELEASE}
					debug ${glslang_LIBRARY_DEBUG}
					optimized ${OSDependent_LIBRARY_RELEASE}
					debug ${OSDependent_LIBRARY_DEBUG}
					optimized ${OGLCompiler_LIBRARY_RELEASE}
					debug ${OGLCompiler_LIBRARY_DEBUG}
					CACHE STRING "glslang libraries"
				)
				set( glslang_LIBRARY_DIRS
					${glslang_LIBRARY_RELEASE_DIR}
					${glslang_LIBRARY_DEBUG_DIR}
				)
			else()
				set( glslang_LIBRARIES
					${glslang_LIBRARY_RELEASE}
					${OSDependent_LIBRARY_RELEASE}
					${OGLCompiler_LIBRARY_RELEASE}
					CACHE STRING "glslang libraries"
				)
				set( glslang_LIBRARY_DIRS
					${glslang_LIBRARY_RELEASE_DIR}
				)
			endif()
		ENDIF ()
	ENDIF ()
endif ()
