# Find OpenGL ES 3
# ----------------
#
# Finds the OpenGL ES 3 library. This module defines:
#
#  OpenGLES3_FOUND      - True if OpenGL ES 3 library is found
#  OpenGLES3::OpenGLES3 - OpenGL ES 3 imported target
#
# Additionally these variables are defined for internal usage:
#
#  OPENGLES3_LIBRARY     - OpenGL ES 3 library
#  OPENGLES3_INCLUDE_DIR - Include dir
#

# In Emscripten OpenGL ES 3 is linked automatically, thus no need to find the
# library.
if ( NOT CORRADE_TARGET_EMSCRIPTEN )
	find_library( OPENGLES3_LIBRARY NAMES
		GLESv3
		# On some platforms (e.g. desktop emulation with Mesa or NVidia) ES3
		# support is provided in ES2 lib
		GLESv2
		# ANGLE (CMake doesn't search for lib prefix on Windows)
		libGLESv2
		# iOS
		OpenGLES
	)
	set(OPENGLES3_LIBRARY_NEEDED OPENGLES3_LIBRARY)
endif()

# Include dir
find_path( OPENGLES3_INCLUDE_DIR NAMES
	GLES3/gl3.h
	# iOS
	ES3/gl.h)

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( "OpenGLES3" DEFAULT_MSG
	${OPENGLES3_LIBRARY_NEEDED}
	OPENGLES3_INCLUDE_DIR )

if ( NOT TARGET OpenGLES3::OpenGLES3 )
	if ( OPENGLES3_LIBRARY_NEEDED )
		add_library( OpenGLES3::OpenGLES3 UNKNOWN IMPORTED )
		set_property( TARGET OpenGLES3::OpenGLES3 PROPERTY
			IMPORTED_LOCATION ${OPENGLES3_LIBRARY} )
	else ()
		# This won't work in CMake 2.8.12, but that affects Emscripten only so
		# I assume people building for that are not on that crap old Ubuntu
		# 14.04 LTS
		add_library( OpenGLES3::OpenGLES3 INTERFACE IMPORTED )
	endif()

	set_property( TARGET OpenGLES3::OpenGLES3 PROPERTY
		INTERFACE_INCLUDE_DIRECTORIES ${OPENGLES3_INCLUDE_DIR} )
endif()
