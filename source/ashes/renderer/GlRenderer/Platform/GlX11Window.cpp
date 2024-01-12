/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Platform/GlX11Window.hpp"

#if __linux__

#include <GL/glx.h>

#if ASHES_USE_XLIB__EGL
#include <EGL/egl.h>
#endif

namespace ashes::gl
{
	RenderWindow::RenderWindow( int reqMajor
		, int reqMinor
		, std::string const & name )
	{
		std::string fullName = "DummyWindow" + name;
		try
		{
			m_display = XOpenDisplay( nullptr );

			if ( !m_display )
			{
				throw ashes::BaseException{ "Couldn't open X Display" };
			}

			int attributes[] =
			{
				GLX_X_RENDERABLE, True,
				GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
				GLX_RENDER_TYPE, GLX_RGBA_BIT,
				GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
				GLX_DOUBLEBUFFER, GL_TRUE,
				GLX_RED_SIZE, 1,
				GLX_GREEN_SIZE, 1,
				GLX_BLUE_SIZE, 1,
				0
			};

			int fbcount = 0;
			GLXFBConfig * configs = glXChooseFBConfig( m_display, DefaultScreen( m_display ), attributes, &fbcount );
			int bestFbcIndex = -1;
			int worstFbcIndex = -1;
			int bestNumSamp = -1;
			int worstNumSamp = 999;

			for ( int i = 0; i < fbcount; ++i )
			{
				XVisualInfo * vi = glXGetVisualFromFBConfig( m_display, configs[i] );

				if ( vi )
				{
					int sampleBuffers;
					int samples;
					glXGetFBConfigAttrib( m_display, configs[i], GLX_SAMPLE_BUFFERS, &sampleBuffers );
					glXGetFBConfigAttrib( m_display, configs[i], GLX_SAMPLES, &samples );

					if ( bestFbcIndex < 0 || ( sampleBuffers && samples > bestNumSamp ) )
					{
						bestFbcIndex = i;
						bestNumSamp = samples;
					}

					if ( worstFbcIndex < 0 || !sampleBuffers || samples < worstNumSamp )
					{
						worstFbcIndex = i;
						worstNumSamp = samples;
					}
				}

				XFree( vi );
			}

			if ( bestFbcIndex == -1 )
			{
				throw ashes::BaseException{ "Couldn't find appropriate GLXFBConfig" };
			}

			auto fbConfig = configs[bestFbcIndex];
			XVisualInfo * vi = glXGetVisualFromFBConfig( m_display, fbConfig );

			if ( !vi )
			{
				throw ashes::BaseException{ "Couldn't find get XVisualInfo" };
			}

			Window root = RootWindow( m_display, vi->screen );
			m_map = XCreateColormap( m_display, root, vi->visual, AllocNone );

			if ( !m_map )
			{
				throw ashes::BaseException{ "Couldn't create X Colormap" };
			}

			XSetWindowAttributes swa;
			swa.colormap = m_map;
			swa.background_pixmap = 0;
			swa.border_pixel = 0;
			swa.event_mask = StructureNotifyMask;
			m_window = XCreateWindow( m_display
				, root
				, 0, 0
				, 640, 480
				, 0
				, vi->depth
				, InputOutput
				, vi->visual
				, CWBorderPixel | CWColormap | CWEventMask, &swa );

			if ( !m_window )
			{
				throw ashes::BaseException{ "Couldn't create X Window" };
			}

			XStoreName( m_display, m_window, fullName.c_str() );
			XSync( m_display, False );

#if ASHES_USE_XLIB_EGL

			m_context = std::make_unique< ContextEgl >( m_display
				, m_window
				, reqMajor
				, reqMinor
				, EGL_NO_CONTEXT );
			XFree( vi );
			XFree( fbConfig );

#else

			m_fbConfig = fbConfig;
			int major{ 0 };
			int minor{ 0 };
			bool ok = glXQueryVersion( m_display, &major, &minor );

			if ( !ok )
			{
				throw ashes::BaseException{ "Could not query GLX version." };
			}

			m_glxContext = glXCreateContext( m_display, vi, nullptr, GL_TRUE );

			if ( !m_glxContext )
			{
				throw ashes::BaseException{ "Could not create a rendering context." };
			}

			glXMakeCurrent( m_display, m_window, m_glxContext );
			XFree( vi );

#endif
		}
		catch ( std::exception & )
		{
			doCleanup();
			throw;
		}
	}

	RenderWindow::~RenderWindow()noexcept
	{
		doCleanup();
	}

	void RenderWindow::doCleanup()noexcept
	{
#if ASHES_USE_XLIB_EGL

		m_context.reset();

#else

		if ( m_glxContext )
		{
			glXMakeCurrent( m_display, 0, nullptr );
			glXDestroyContext( m_display, m_glxContext );
		}

#endif

		if ( m_window )
		{
			XDestroyWindow( m_display, m_window );
			m_window = 0;
		}

		if ( m_map )
		{
			XFreeColormap( m_display, m_map );
			m_map = 0;
		}

		if ( m_display )
		{
			XCloseDisplay( m_display );
			m_display = nullptr;
		}
	}

	VkXlibSurfaceCreateInfoKHR RenderWindow::getCreateInfo()const
	{
		return 
		{
			VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0u,
			m_display,
			m_window,
		};
	}
}

#endif
