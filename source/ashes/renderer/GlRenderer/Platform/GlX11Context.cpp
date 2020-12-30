/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "GlX11Context.hpp"

#if __linux__

#include "ashesgl_api.hpp"

#include <EGL/egl.h>
#include <GL/glx.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>

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

		Rotation convert( VkSurfaceTransformFlagBitsKHR transform )
		{
			switch ( transform )
			{
			case VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR:
				return Rotation( RR_Rotate_0 );
			case VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR:
				return Rotation( RR_Rotate_90 );
			case VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR:
				return Rotation( RR_Rotate_180 );
			case VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR:
				return Rotation( RR_Rotate_270 );
			default:
				return Rotation( RR_Rotate_0 );
			}
		}

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
		, xlibCreateInfo{ std::move( createInfo ) }
		, m_mainContext{ static_cast< X11Context const * >( mainContext ) }
		, m_display{ xlibCreateInfo.dpy }
		, m_window{ xlibCreateInfo.window }
	{
	}

	X11Context::X11Context( VkInstance instance
		, VkDisplaySurfaceCreateInfoKHR createInfo
		, ContextImpl const * mainContext )
		: ContextImpl{ instance, createInfo.imageExtent }
		, displayCreateInfo{ std::move( createInfo ) }
		, m_mainContext{ static_cast< X11Context const * >( mainContext ) }
		, m_display{ XOpenDisplay( nullptr ) }
	{
	}

	X11Context::~X11Context()
	{
		glXDestroyContext( m_display, m_glxContext );

		if ( displayCreateInfo.sType )
		{
			XDestroyWindow( m_display, m_window );
			XFreeColormap( m_display, m_map );
			XCloseDisplay( m_display );
		}
	}

	void X11Context::preInitialise( int reqMajor, int reqMinor ) try
	{
		if ( xlibCreateInfo.sType )
		{
			XWindowAttributes attribs;

			if ( XGetWindowAttributes( m_display, m_window, &attribs ) )
			{
				 m_screenIndex = XScreenNumberOfScreen( attribs.screen );
			}
			else
			{
				m_screenIndex = XDefaultScreen( m_display );
			}
		}
		else
		{
			m_screenIndex = getScreenIndex( displayCreateInfo.displayMode );
		}

		int major{ 0 };
		int minor{ 0 };
		bool ok = glXQueryVersion( m_display, &major, &minor );

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
			m_visualInfo = createVisualInfoWithFBConfig( m_display, attribList, m_screenIndex, m_fbConfig );
		}
		else
		{
			m_visualInfo = createVisualInfoWithoutFBConfig( m_display, attribList, m_screenIndex );
		}

		if ( !m_visualInfo )
		{
			throw std::runtime_error{ "Could not retrieve visual info." };
		}

		m_glxContext = glXCreateContext( m_display
			, m_visualInfo
			, ( m_mainContext
				? m_mainContext->m_glxContext
				: nullptr )
			, GL_TRUE );

		if ( !m_glxContext )
		{
			throw std::runtime_error{ "Could not create a rendering context." };
		}

		doLoadSytemFunctions();
		auto & extensions = get( instance )->getExtensions();
		m_major = std::max( reqMajor, extensions.getMajor() );
		m_minor = std::max( reqMinor, extensions.getMinor() );
	}
	catch ( std::exception & exc )
	{
		disable();

		if ( m_glxContext )
		{
			glXDestroyContext( m_display, m_glxContext );
		}

		throw;
	}

	void X11Context::postInitialise()
	{
		if ( displayCreateInfo.sType )
		{
			doInitialiseDisplay();
		}

		doCreateModernContext();
	}

	void X11Context::enable()const
	{
		glXMakeCurrent( m_display, m_window, m_glxContext );
	}

	void X11Context::disable()const
	{
		glXMakeCurrent( m_display, 0, nullptr );
	}

	void X11Context::swapBuffers()const
	{
		glXSwapBuffers( m_display, m_window );
	}

	void X11Context::doLoadSytemFunctions() try
	{
		enable();

		if ( !getFunction( "glXCreateContextAttribsARB", glCreateContextAttribs ) )
		{
			throw std::runtime_error{ "Couldn't retrieve glXCreateContextAttribsARB" };
		}

		getFunction( "glXSwapIntervalEXT", glXSwapInterval );
	}
	catch ( std::exception & exc )
	{
		disable();
		glXDestroyContext( m_display, m_glxContext );
	}

	void X11Context::doInitialiseDisplay() try
	{
		auto root = XRootWindow ( m_display, m_screenIndex );
		m_map = XCreateColormap( m_display, root, m_visualInfo->visual, AllocNone );

		if ( !m_map )
		{
			throw std::runtime_error{ "Couldn't create X Colormap" };
		}

		extent = getDisplayResolution( displayCreateInfo.displayMode );
		XSetWindowAttributes swa;
		swa.colormap = m_map;
		swa.background_pixmap = 0;
		swa.border_pixel = 0;
		swa.event_mask = StructureNotifyMask;
		m_window = XCreateWindow( m_display
			, root
			, 0, 0
			, extent.width, extent.height
			, 0
			, m_visualInfo->depth
			, InputOutput
			, m_visualInfo->visual
			, CWBorderPixel | CWColormap | CWEventMask, &swa );

		if ( !m_window )
		{
			throw std::runtime_error{ "Couldn't create X Window" };
		}

		if ( !XStoreName( m_display, m_window, "DummyWindow" ) )
		{
			throw std::runtime_error{ "Couldn't set X Window name" };
		}

		auto wmState = XInternAtom( m_display, "_NET_WM_STATE", true );
		auto wmFullscreen = XInternAtom( m_display, "_NET_WM_STATE_FULLSCREEN", true );
		if ( !XChangeProperty( m_display
			, m_window
			, wmState
			, XA_ATOM
			, 32
			, PropModeReplace
			, (unsigned char *)&wmFullscreen
			, 1 ) )
		{
			throw std::runtime_error{ "Couldn't set X Window fulsscreen" };
		}

		if ( !XMapWindow( m_display, m_window ) )
		{
			throw std::runtime_error{ "Couldn't map X Window" };
		}

		XSync( m_display, False );
		// auto sc = XRRGetScreenInfo( m_display, m_window );

		// if ( !sc )
		// {
		// 	throw std::runtime_error{ "Couldn't retrieve screen config info" };
		// }

		// auto params = getDisplayModeParameters( displayCreateInfo.displayMode );
		// int nsize{ 0 };
		// auto sizes = XRRConfigSizes( sc, &nsize );
		// int sizeIndex = 0;

		// for ( int sz = 0; sz < nsize; sz++ )
		// {
		// 	auto size = sizes[sz];
		// 	if ( size.width == params.visibleRegion.width
		// 		&& size.height == params.visibleRegion.height )
		// 	{
		// 		sizeIndex = sz;
		// 		break;
		// 	}
		// }

		// Time currentTimeStamp;
		// XRRSelectInput( m_display, m_window, RRScreenChangeNotifyMask );
		// auto timestamp = XRRConfigTimes( sc, &currentTimeStamp );

		// if ( !XRRSetScreenConfig( m_display
		// 	, sc
		// 	, XDefaultRootWindow( m_display )
		// 	, sizeIndex
		// 	, convert( displayCreateInfo.transform )
		// 	, currentTimeStamp ) )
		// {
		// 	throw std::runtime_error{ "Couldn't change display mode" };
		// }

		// XRRFreeScreenConfigInfo( sc );
		// XFlush( m_display );
	}
	catch ( std::exception & exc )
	{
		disable();
		glXDestroyContext( m_display, m_glxContext );
	}

	void X11Context::doCreateModernContext() try
	{
		std::vector< int > attribList
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, m_major,
			GLX_CONTEXT_MINOR_VERSION_ARB, m_minor,
			GLX_CONTEXT_FLAGS_ARB, GL_CONTEXT_CREATION_DEFAULT_FLAGS,
			GLX_CONTEXT_PROFILE_MASK_ARB, GL_CONTEXT_CREATION_DEFAULT_MASK,
			0
		};

		enable();
		auto glxContext = glCreateContextAttribs( m_display
			, m_fbConfig
			, ( m_mainContext
				? m_mainContext->m_glxContext
				: nullptr )
			, true
			, attribList.data() );
		disable();
		glXDestroyContext( m_display, m_glxContext );
		m_glxContext = glxContext;
		auto result = m_glxContext != nullptr;

		if ( !result )
		{
			std::stringstream error;
			error << "Failed to create an OpenGL " << m_major << "." << m_minor << " context (0x" << std::hex << ::glGetError() << ").";
			throw std::runtime_error{ error.str() };
		}

		if ( glXSwapInterval )
		{
			enable();
			glXSwapInterval( m_display, m_window, 0 );
			disable();
		}

		XFree( m_visualInfo );
	}
	catch ( std::exception & exc )
	{
		disable();
		glXDestroyContext( m_display, m_glxContext );
	}
}

#endif
