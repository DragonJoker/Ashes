#include"RenderLibPrerequisites.h"

#if defined( PLATFORM_ANDROID )
#	include <EGL/egl.h>
#	include <GLES3/gl3.h>
#elif defined( PLATFORM_IPHONE ) || defined( PLATFORM_IPHONE_SIMULATOR )
#	include <OpenGLES/gltypes.h>
#	include <OpenGLES/EAGL.h>
#	include <OpenGLES/EAGLDrawable.h>
#	include <OpenGLES/ES3/gl.h>
#	include <OpenGLES/ES3/glext.h>
#elif defined( PLATFORM_WINDOWS )
#	ifndef GLEW_STATIC
#		define GLEW_STATIC
#	endif
#	include <GlLib/GL/glew.h>
#endif
