/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Platform/EglContext.hpp"

#ifndef _WIN32

#include <EGL/egl.h>

#include <stdexcept>

namespace ashes::gl
{
	EglContext::EglContext( EGLNativeDisplayType display
		, EGLNativeWindowType window
		, int reqMajor
		, int reqMinor
		, EGLContext shared )
	{
		try
		{
			EGLBoolean ok = eglBindAPI( EGL_OPENGL_API );

			if ( !ok )
			{
				throw std::runtime_error{ "Couldn't bind EGL API" };
			}

			m_display = eglGetDisplay( display );

			if ( m_display == EGL_NO_DISPLAY )
			{
				throw std::runtime_error{ "Couldn't get EGL display" };
			}

			EGLint major, minor;
			ok = eglInitialize( m_display, &major, &minor );

			if ( !ok )
			{
				throw std::runtime_error{ "Couldn't initialise EGL" };
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
				throw std::runtime_error{ "Couldn't choose EGL config" };
			}

			if ( numConfigs == 0 )
			{
				throw std::runtime_error{ "Failed to find suitable EGLConfig" };
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
				throw std::runtime_error{ "EGL Surface creation failed" };
			}

			const EGLint eglContextAttribs[]
			{
				EGL_CONTEXT_MAJOR_VERSION, reqMajor,
				EGL_CONTEXT_MINOR_VERSION, reqMinor,
				EGL_NONE,
			};
			m_context = eglCreateContext( m_display
				, config
				, shared
				, eglContextAttribs );

			if ( !m_context )
			{
				throw std::runtime_error{ "EGL Context creation failed" };
			}

			ok = enable();

			if ( !ok )
			{
				throw std::runtime_error{ "eglMakeCurrent() failed" };
			}

			eglSwapInterval( m_display, 0 );
			disable();
		}
		catch ( std::exception & p_exc )
		{
			doCleanup();
			throw;
		}
	}

	EglContext::~EglContext()
	{
		doCleanup();
	}
	
	EGLBoolean EglContext::enable()const
	{
		return eglMakeCurrent( m_display, m_surface, m_surface, m_context );
	}

	EGLBoolean EglContext::disable()const
	{
		return eglMakeCurrent( m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	}

	EGLBoolean EglContext::swap()const
	{
		return eglSwapBuffers( m_display, m_surface );
	}

	void EglContext::doCleanup()
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
