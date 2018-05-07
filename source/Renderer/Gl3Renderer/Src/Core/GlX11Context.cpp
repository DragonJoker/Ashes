#include "Core/GlX11Context.hpp"

#if RENDERLIB_XLIB
#include "Core/GlPhysicalDevice.hpp"

#include "Miscellaneous/GlDebug.hpp"

#include <Core/PlatformWindowHandle.hpp>

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace gl_renderer
{
	namespace
	{
	    using PFN_GLXCHOOSEFBCONFIG = GLXFBConfig *(*)( Display *, int, int const *, int * );
		using PFN_GLXGETVISUALFROMFBCONFIG = XVisualInfo *(*)( Display *, GLXFBConfig );
		PFN_GLXCHOOSEFBCONFIG glXChooseFBConfig = nullptr;
		PFN_GLXGETVISUALFROMFBCONFIG glXGetVisualFromFBConfig = nullptr;

#if !defined( NDEBUG )

		static const int GL_CONTEXT_CREATION_DEFAULT_FLAGS = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT | GL_CONTEXT_FLAG_DEBUG_BIT;
		static const int GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#else

		static const int GL_CONTEXT_CREATION_DEFAULT_FLAGS = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
		static const int GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#endif

		template< typename Func >
		bool getFunction( std::string const & name, Func & func )
		{
			func = reinterpret_cast< Func >( glXGetProcAddressARB( reinterpret_cast< GLubyte const * >( name.c_str() ) ) );
			return func != nullptr;
		}
	}

	X11Context::X11Context( PhysicalDevice const & gpu
		, renderer::ConnectionPtr && connection )
		: Context{ gpu, std::move( connection ) }
		, m_display( m_connection->getHandle().getInternal< renderer::IXWindowHandle >().getDisplay() )
		, m_glxVersion( 10 )
		, m_glxContext( nullptr )
		, m_drawable( m_connection->getHandle().getInternal< renderer::IXWindowHandle >().getDrawable() )
		, m_fbConfig( nullptr )
	{
		if ( !glXChooseFBConfig )
		{
			getFunction( "glXChooseFBConfig", glXChooseFBConfig );
			getFunction( "glXGetVisualFromFBConfig", glXGetVisualFromFBConfig );
		}

		int screen = DefaultScreen( m_display );
		int major{ 0 };
		int minor{ 0 };
		bool ok = glXQueryVersion( m_display, &major, &minor );
		XVisualInfo * visualInfo = nullptr;

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
			visualInfo = doCreateVisualInfoWithFBConfig( attribList, screen );
		}
		else
		{
			visualInfo = doCreateVisualInfoWithoutFBConfig( attribList, screen );
		}

		if ( visualInfo )
		{
			m_glxContext = glXCreateContext( m_display, visualInfo, nullptr, GL_TRUE );

			if ( !m_glxContext )
			{
				throw std::runtime_error{ "Could not create a rendering context." };
			}

			setCurrent();
			m_opengl = std::make_unique< OpenGLLibrary >();
			loadDebugFunctions();
			endCurrent();

			if ( m_gpu.getMajor() < 4 )
			{
				glXDestroyContext( m_display, m_glxContext );
				throw std::runtime_error{ "The supported OpenGL version is insufficient." };
			}

			if ( !doCreateGl3Context() )
			{
				glXDestroyContext( m_display, m_glxContext );
				throw std::runtime_error{ "The supported OpenGL version is insufficient." };
			}

			XFree( visualInfo );
		}
	}

	X11Context::~X11Context()
	{
		try
		{
			glXDestroyContext( m_display, m_glxContext );
			XFree( m_fbConfig );
		}
		catch ( ... )
		{
		}
	}

	void X11Context::setCurrent()const
	{
		glXMakeCurrent( m_display, m_drawable, m_glxContext );
	}

	void X11Context::endCurrent()const
	{
		glXMakeCurrent( m_display, 0, nullptr );
	}

	void X11Context::swapBuffers()const
	{
		glXSwapBuffers( m_display, m_drawable );
	}

	XVisualInfo * X11Context::doCreateVisualInfoWithFBConfig( std::vector< int > arrayAttribs, int screen )
	{
		XVisualInfo * visualInfo = nullptr;
		int nbElements = 0;
		m_fbConfig = glXChooseFBConfig( m_display, screen, arrayAttribs.data(), &nbElements );

		if ( !m_fbConfig )
		{
			// First try failed, we try a default FBConfig
			renderer::Logger::logWarning( "glXChooseFBConfig failed, using default FB config" );
			int data = 0;
			m_fbConfig = glXChooseFBConfig( m_display, screen, &data, &nbElements );

			if ( !m_fbConfig )
			{
				// Last FBConfig try failed, we try from XVisualInfo
				renderer::Logger::logWarning( "Default glXChooseFBConfig failed" );
				visualInfo = doCreateVisualInfoWithoutFBConfig( arrayAttribs, screen );
			}
			else
			{
				renderer::Logger::logDebug( "Default glXChooseFBConfig successful" );
			}
		}
		else
		{
			renderer::Logger::logDebug( "glXChooseFBConfig successful with detailed attributes" );
		}

		if ( m_fbConfig )
		{
			visualInfo = glXGetVisualFromFBConfig( m_display, m_fbConfig[0] );

			if ( !visualInfo )
			{
				renderer::Logger::logWarning( "glXgetVisualFromFBConfig failed" );
			}
			else
			{
				renderer::Logger::logDebug( "GlXgetVisualFromFBConfig successful" );
			}
		}

		return visualInfo;
	}

	XVisualInfo * X11Context::doCreateVisualInfoWithoutFBConfig( std::vector< int > arrayAttribs, int screen )
	{
		XVisualInfo * result = glXChooseVisual( m_display, screen, arrayAttribs.data() );

		if ( !result )
		{
			renderer::Logger::logError( "glXChooseVisual failed" );
		}

		return result;
	}

	bool X11Context::doCreateGl3Context()
	{
		using PFNGLCREATECONTEXTATTRIBS = GLXContext ( * )( Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list );
		PFNGLCREATECONTEXTATTRIBS glCreateContextAttribs;
		bool result = false;
		std::vector< int > attribList
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
			GLX_CONTEXT_MINOR_VERSION_ARB, 2,
			GLX_CONTEXT_FLAGS_ARB, GL_CONTEXT_CREATION_DEFAULT_FLAGS,
			GLX_CONTEXT_PROFILE_MASK_ARB, GL_CONTEXT_CREATION_DEFAULT_MASK,
			0
		};

		setCurrent();
		gl::GetError();

		if ( getFunction( "glXCreateContextAttribsARB", glCreateContextAttribs ) )
		{
			auto glxContext = glCreateContextAttribs( m_display, m_fbConfig[0], nullptr, true, attribList.data() );
			glXDestroyContext( m_display, m_glxContext );
			m_glxContext = glxContext;
			result = m_glxContext != nullptr;

			if ( result )
			{
				std::stringstream stream;
				stream << "OpenGL 3.2 context created.";
				renderer::Logger::logInfo( stream.str() );
			}
			else
			{
				std::stringstream stream;
				stream << "Failed to create an OpenGL 3.2 context.";
				renderer::Logger::logError( stream.str() );
			}
		}
		else
		{
			renderer::Logger::logError( "Couldn't load glXCreateContextAttribsARB function." );
		}

		return result;
	}
}

#endif
