/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "GlX11Context.hpp"

#if __linux__
#include <EGL/egl.h>
#include <GL/glx.h>

#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace ashes::gl
{
	namespace
	{
#if !defined( NDEBUG )

		static const int GL_CONTEXT_CREATION_DEFAULT_FLAGS = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT | GL_CONTEXT_FLAG_DEBUG_BIT;
		static const int GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#else

		static const int GL_CONTEXT_CREATION_DEFAULT_FLAGS = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
		static const int GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#endif

		XVisualInfo * createVisualInfoWithoutFBConfig( Display * dpy
			, std::vector< int > arrayAttribs
			, int screen )
		{
			XVisualInfo * result = glXChooseVisual( dpy, screen, arrayAttribs.data() );

			if ( !result )
			{
				std::cerr << "glXChooseVisual failed" << std::endl;
			}

			return result;
		}

		XVisualInfo * createVisualInfoWithFBConfig( Display * dpy
			, std::vector< int > arrayAttribs
			, int screen
			, GLXFBConfig & fbConfig )
		{
			XVisualInfo * visualInfo = nullptr;
			int nbElements = 0;
			auto fbConfigs = glXChooseFBConfig( dpy, screen, arrayAttribs.data(), &nbElements );

			if ( !fbConfigs )
			{
				// First try failed, we try a default FBConfig
				std::cerr << "glXChooseFBConfig failed, using default FB config" << std::endl;
				int data = 0;
				fbConfigs = glXChooseFBConfig( dpy, screen, &data, &nbElements );

				if ( !fbConfigs )
				{
					// Last FBConfig try failed, we try from XVisualInfo
					std::cerr << "Default glXChooseFBConfig failed" << std::endl;
					visualInfo = createVisualInfoWithoutFBConfig( dpy, arrayAttribs, screen );
				}
			}

			if ( fbConfigs )
			{
				int i = 0;

				while ( i < nbElements && !visualInfo )
				{
					fbConfig = fbConfigs[i];
					visualInfo = glXGetVisualFromFBConfig( dpy, fbConfig );
					++i;
				}

				if ( !visualInfo )
				{
					std::cerr << "glXgetVisualFromFBConfig failed" << std::endl;
				}
			}

			return visualInfo;
		}
	}

	X11Context::X11Context( VkInstance instance
		, VkXlibSurfaceCreateInfoKHR createInfo
		, ContextImpl const * mainContext )
		: ContextImpl{ instance }
		, createInfo{ std::move( createInfo ) }
		, m_mainContext{ static_cast< X11Context const * >( mainContext ) }
	{
	}

	X11Context::~X11Context()
	{
		glXDestroyContext( createInfo.dpy, m_glxContext );
	}

	void X11Context::preInitialise( int reqMajor, int reqMinor )
	{
		int screen = DefaultScreen( createInfo.dpy );
		int major{ 0 };
		int minor{ 0 };
		bool ok = glXQueryVersion( createInfo.dpy, &major, &minor );

		if ( ok )
		{
			m_glxVersion = major * 10 + minor;
		}

		std::vector< int > attribList
		{
			GLX_RENDER_TYPE, GLX_RGBA_BIT,
			GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
			GLX_DOUBLEBUFFER, GL_TRUE,
			GLX_RED_SIZE, 1,
			GLX_GREEN_SIZE, 1,
			GLX_BLUE_SIZE, 1,
			0
		};

		attribList.push_back( 0 );

		if ( glXChooseFBConfig )
		{
			m_visualInfo = createVisualInfoWithFBConfig( createInfo.dpy, attribList, screen, m_fbConfig );
		}
		else
		{
			m_visualInfo = createVisualInfoWithoutFBConfig( createInfo.dpy, attribList, screen );
		}

		if ( m_visualInfo )
		{
			m_glxContext = glXCreateContext( createInfo.dpy, m_visualInfo, nullptr, GL_TRUE );

			if ( !m_glxContext )
			{
				throw std::runtime_error{ "Could not create a rendering context." };
			}

			enable();
			disable();
			m_major = reqMajor;
			m_minor = reqMinor;
		}
	}

	void X11Context::postInitialise()
	{
		using PFN_glCreateContextAttribs = GLXContext ( * )( Display *, GLXFBConfig, GLXContext, Bool, const int * );
		using PFN_glXSwapIntervalEXT = void (*)( Display *, GLXDrawable, int );
		PFN_glCreateContextAttribs glCreateContextAttribs;
		PFN_glXSwapIntervalEXT glXSwapInterval;
		std::vector< int > attribList
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, m_major,
			GLX_CONTEXT_MINOR_VERSION_ARB, m_minor,
			GLX_CONTEXT_FLAGS_ARB, GL_CONTEXT_CREATION_DEFAULT_FLAGS,
			GLX_CONTEXT_PROFILE_MASK_ARB, GL_CONTEXT_CREATION_DEFAULT_MASK,
			0
		};

		enable();
		::glGetError();

		if ( !getFunction( "glXCreateContextAttribsARB", glCreateContextAttribs ) )
		{
			disable();
			glXDestroyContext( createInfo.dpy, m_glxContext );
			throw std::runtime_error{ "Couldn't retrieve glXCreateContextAttribsARB" };
		}

		auto glxContext = glCreateContextAttribs( createInfo.dpy
			, m_fbConfig
			, ( m_mainContext
				? m_mainContext->m_glxContext
				: nullptr )
			, true
			, attribList.data() );
		glXDestroyContext( createInfo.dpy, m_glxContext );
		m_glxContext = glxContext;
		auto result = m_glxContext != nullptr;

		if ( !result )
		{
			std::stringstream error;
			error << "Failed to create an OpenGL " << m_major << "." << m_minor << " context (0x" << std::hex << ::glGetError() << ").";
			throw std::runtime_error{ error.str() };
		}

		enable();
		if ( getFunction( "glXSwapIntervalEXT", glXSwapInterval ) )
		{
			glXSwapInterval( createInfo.dpy, createInfo.window, 0 );
		}
		disable();

		XFree( m_visualInfo );
	}

	void X11Context::enable()const
	{
		glXMakeCurrent( createInfo.dpy, createInfo.window, m_glxContext );
	}

	void X11Context::disable()const
	{
		glXMakeCurrent( createInfo.dpy, 0, nullptr );
	}

	void X11Context::swapBuffers()const
	{
		glXSwapBuffers( createInfo.dpy, createInfo.window );
	}
}

#endif
