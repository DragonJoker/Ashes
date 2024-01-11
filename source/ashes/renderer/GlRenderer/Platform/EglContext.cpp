/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Platform/EglContext.hpp"

#if defined( __linux__ )

#include <EGL/egl.h>

#include <map>
#include <stdexcept>

namespace ashes::gl
{
	namespace
	{
		std::string getEGLError( std::string const & text )
		{
			auto error = eglGetError();

			switch ( error )
			{
			case EGL_SUCCESS: return text;
			case EGL_NOT_INITIALIZED: return text + ": EGL is not initialized for the specified EGL display connection.";
			case EGL_BAD_ACCESS: return text + ": EGL cannot access requested resource.";
			case EGL_BAD_ALLOC: return text + ": EGL failed to allocater resource.";
			case EGL_BAD_ATTRIBUTE: return text + ": An unrecognized attribute or attribute value was passed.";
			case EGL_BAD_CONTEXT: return text + ": An EGLContext argument does not name a valid EGL rendering context";
			case EGL_BAD_CURRENT_SURFACE: return text + ": The current surface of the calling thread is no longer valid";
			case EGL_BAD_DISPLAY: return text + ": An EGLDisplay argument does not name valid EGL display connection";
			case EGL_BAD_SURFACE: return text + ": An EGLSurface argument does not name a valid surface";
			case EGL_BAD_MATCH: return text + ": Arguments are inconsistent";
			case EGL_BAD_PARAMETER: return text + ": One or more argument values are invalid";
			case EGL_BAD_NATIVE_PIXMAP: return text + ": A NativePixmapType argument does not refer to a valid native pixmap";
			case EGL_BAD_NATIVE_WINDOW: return text + ": A NativeWindowType argument does not refer to a valid native window";
			case EGL_CONTEXT_LOST: return text + ": A power management event has occured. The application must destroy all contexts and reinitalise OpenGL ES state and objects.";
			default: return text;
			}
		}
	}
	ContextEgl::ContextEgl( Display * display
		, uint64_t window
		, int reqMajor
		, int reqMinor
		, EGLContext shared )
	{
		try
		{
			EGLBoolean ok = eglBindAPI( EGL_OPENGL_API );

			if ( !ok )
			{
				throw ashes::BaseException{ getEGLError( "Couldn't bind EGL API")  };
			}

			m_display = eglGetDisplay( display );

			if ( m_display == EGL_NO_DISPLAY )
			{
				throw ashes::BaseException{ getEGLError( "Couldn't get EGL display" ) };
			}

			EGLint major = 0;
			EGLint minor = 0;
			ok = eglInitialize( m_display, &major, &minor );

			if ( !ok )
			{
				throw ashes::BaseException{ getEGLError( "Couldn't initialise EGL" ) };
			}

			const EGLint eglConfigAttribs[]
			{
				EGL_COLOR_BUFFER_TYPE,     EGL_RGB_BUFFER,
				EGL_BUFFER_SIZE,           32,
				EGL_RED_SIZE,              8,
				EGL_GREEN_SIZE,            8,
				EGL_BLUE_SIZE,             8,
				EGL_ALPHA_SIZE,            8,

				EGL_DEPTH_SIZE,            24,
				EGL_STENCIL_SIZE,          8,

				EGL_SAMPLE_BUFFERS,        0,
				EGL_SAMPLES,               0,

				EGL_SURFACE_TYPE,          EGL_WINDOW_BIT,
				EGL_RENDERABLE_TYPE,       EGL_OPENGL_BIT,

				EGL_NONE,
			};

			EGLint constexpr configsSize = 256;
			EGLConfig configs[configsSize]{};
			EGLint numConfigs{};
			ok = eglChooseConfig( m_display
				, eglConfigAttribs
				, configs
				, configsSize // num requested configs
				, &numConfigs ); // num returned configs

			if ( !ok )
			{
				throw ashes::BaseException{ getEGLError( "Couldn't choose EGL config" ) };
			}

			if ( numConfigs == 0 )
			{
				throw ashes::BaseException{ getEGLError( "Failed to find suitable EGLConfig" ) };
			}

			EGLConfig config{ nullptr };
			EGLint i = 0;
			const EGLint eglSurfaceAttribs[]
			{
				EGL_NONE,
			};

			while ( i < numConfigs && !m_surface )
			{
				config = configs[i];
				m_surface = eglCreateWindowSurface( m_display
					, config
					, window
					, eglSurfaceAttribs );
				++i;
			}

			if ( !m_surface )
			{
				throw ashes::BaseException{ getEGLError( "EGL Surface creation failed" ) };
			}

			const EGLint eglContextAttribs[]
			{
				EGL_CONTEXT_MAJOR_VERSION, reqMajor,
				EGL_CONTEXT_MINOR_VERSION, reqMinor,
				EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
				EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, EGL_TRUE,
#if !defined( NDEBUG )
				EGL_CONTEXT_OPENGL_DEBUG, EGL_TRUE,
#endif

				EGL_NONE,
			};
			m_context = eglCreateContext( m_display
				, config
				, shared
				, eglContextAttribs );

			if ( !m_context )
			{
				throw ashes::BaseException{ getEGLError( "EGL Context creation failed" ) };
			}

			ok = enable();

			if ( !ok )
			{
				throw ashes::BaseException{ getEGLError( "eglMakeCurrent() failed" ) };
			}

			eglSwapInterval( m_display, 0 );
			disable();
		}
		catch ( std::exception & )
		{
			doCleanup();
			throw;
		}
	}
	
	ContextEgl::ContextEgl( wl_display * display
		, wl_egl_window * window
		, int reqMajor
		, int reqMinor
		, EGLContext shared )
		: ContextEgl{ reinterpret_cast< Display * >( display ), uint64_t( window ), reqMajor, reqMinor, shared }
	{
	}

	ContextEgl::~ContextEgl()
	{
		doCleanup();
	}
	
	EGLBoolean ContextEgl::enable()const
	{
		return eglMakeCurrent( m_display, m_surface, m_surface, m_context );
	}

	EGLBoolean ContextEgl::disable()const
	{
		return eglMakeCurrent( m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	}

	EGLBoolean ContextEgl::swap()const
	{
		return eglSwapBuffers( m_display, m_surface );
	}

	VkExtent2D ContextEgl::getExtent()const
	{
		VkExtent2D result{};
		EGLint width = 0;
		EGLint height = 0;

		if ( eglQuerySurface( m_display, m_surface, EGL_WIDTH, &width )
			&& eglQuerySurface( m_display, m_surface, EGL_HEIGHT, &height ) )
		{
			result.width = width;
			result.height = height;
		}

		return result;
	}

	void ContextEgl::doCleanup()
	{
		if ( m_context )
		{
			eglDestroyContext( m_display, m_context );
			m_context = nullptr;
		}

		if ( m_surface )
		{
			eglDestroySurface( m_display, m_surface );
			m_surface = nullptr;
		}

		if ( m_display )
		{
			eglTerminate( m_display );
			m_display = nullptr;
		}
	}
}

#endif
