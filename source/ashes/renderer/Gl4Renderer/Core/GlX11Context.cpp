#include "Core/GlX11Context.hpp"

#if ASHES_XLIB

#include "Core/GlInstance.hpp"
#include "Core/GlPhysicalDevice.hpp"
#include "Miscellaneous/GlDebug.hpp"

#include "ashesgl4_api.hpp"

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace ashes::gl4
{
	char const VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME[VK_MAX_EXTENSION_NAME_SIZE] = VK_KHR_XLIB_SURFACE_EXTENSION_NAME;

	PFN_vkVoidFunction getFunction( char const * const name )
	{
		return reinterpret_cast< PFN_vkVoidFunction >( glXGetProcAddressARB( reinterpret_cast< GLubyte const * >( name ) ) );
	}

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
	}

	X11Context::X11Context( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
		: ContextImpl{ instance, createInfo }
		, m_glxVersion( 10 )
		, m_glxContext( nullptr )
		, m_fbConfig( nullptr )
		, m_mainContext{ mainContext }
	{
	}

	X11Context::X11Context( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
		: X11Context
		{
			instance,
			get( surface )->getContext()->createInfo,
			mainContext
		}
	{
	}

	X11Context::~X11Context()
	{
		try
		{
			glXDestroyContext( createInfo.dpy, m_glxContext );
			XFree( m_fbConfig );
		}
		catch ( ... )
		{
		}
	}

	void X11Context::initialise( Context & parent )
	{
		if ( !glXChooseFBConfig )
		{
			getFunction( "glXChooseFBConfig", glXChooseFBConfig );
			getFunction( "glXGetVisualFromFBConfig", glXGetVisualFromFBConfig );
		}

		int screen = DefaultScreen( createInfo.dpy );
		int major{ 0 };
		int minor{ 0 };
		bool ok = glXQueryVersion( createInfo.dpy, &major, &minor );
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
			m_glxContext = glXCreateContext( createInfo.dpy, visualInfo, nullptr, GL_TRUE );

			if ( !m_glxContext )
			{
				throw std::runtime_error{ "Could not create a rendering context." };
			}

			enable();
			parent.onBaseContextCreated();
			loadSystemFunctions();
			disable();

			auto & extensions = get( instance )->getExtensions();

			if ( extensions.getMajor() < 4 )
			{
				glXDestroyContext( createInfo.dpy, m_glxContext );
				throw std::runtime_error{ "The supported OpenGL version is insufficient." };
			}

			if ( !doCreateGl3Context( m_mainContext ) )
			{
				glXDestroyContext( createInfo.dpy, m_glxContext );
				throw std::runtime_error{ "The supported OpenGL version is insufficient." };
			}

			XFree( visualInfo );
			enable();

			if ( m_glXSwapIntervalEXT )
			{
				m_glXSwapIntervalEXT( createInfo.dpy, createInfo.window, 0 );
			}

			disable();
		}
	}

	void X11Context::loadSystemFunctions()
	{
#define GLX_LIB_FUNCTION( fun )\
		if ( !( getFunction( "glX"#fun, m_glX##fun ) ) )\
		{\
			std::cerr << "Couldn't load function " << "gl"#fun << std::endl;\
		}
#include "Miscellaneous/OpenGLFunctionsList.inl"
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

	XVisualInfo * X11Context::doCreateVisualInfoWithFBConfig( std::vector< int > arrayAttribs, int screen )
	{
		XVisualInfo * visualInfo = nullptr;
		int nbElements = 0;
		m_fbConfig = glXChooseFBConfig( createInfo.dpy, screen, arrayAttribs.data(), &nbElements );

		if ( !m_fbConfig )
		{
			// First try failed, we try a default FBConfig
			std::cerr << "glXChooseFBConfig failed, using default FB config" << std::endl;
			int data = 0;
			m_fbConfig = glXChooseFBConfig( createInfo.dpy, screen, &data, &nbElements );

			if ( !m_fbConfig )
			{
				// Last FBConfig try failed, we try from XVisualInfo
				std::cerr << "Default glXChooseFBConfig failed" << std::endl;
				visualInfo = doCreateVisualInfoWithoutFBConfig( arrayAttribs, screen );
			}
		}

		if ( m_fbConfig )
		{
			visualInfo = glXGetVisualFromFBConfig( createInfo.dpy, m_fbConfig[0] );

			if ( !visualInfo )
			{
				std::cerr << "glXgetVisualFromFBConfig failed" << std::endl;
			}
		}

		return visualInfo;
	}

	XVisualInfo * X11Context::doCreateVisualInfoWithoutFBConfig( std::vector< int > arrayAttribs, int screen )
	{
		XVisualInfo * result = glXChooseVisual( createInfo.dpy, screen, arrayAttribs.data() );

		if ( !result )
		{
			std::cerr << "glXChooseVisual failed" << std::endl;
		}

		return result;
	}

	bool X11Context::doCreateGl3Context( Context const * mainContext )
	{
		bool result = false;
		auto & extensions = get( instance )->getExtensions();

		try
		{
			using PFNGLCREATECONTEXTATTRIBS = GLXContext ( * )( Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list );
			PFNGLCREATECONTEXTATTRIBS glCreateContextAttribs;
			std::vector< int > attribList
			{
				GLX_CONTEXT_MAJOR_VERSION_ARB, extensions.getMajor(),
				GLX_CONTEXT_MINOR_VERSION_ARB, extensions.getMinor(),
				GLX_CONTEXT_FLAGS_ARB, GL_CONTEXT_CREATION_DEFAULT_FLAGS,
				GLX_CONTEXT_PROFILE_MASK_ARB, GL_CONTEXT_CREATION_DEFAULT_MASK,
				0
			};

			enable();
			::glGetError();

			if ( !getFunction( "glXCreateContextAttribsARB", glCreateContextAttribs ) )
			{
				disable();
				throw std::runtime_error{ "Couldn't retrieve glXCreateContextAttribsARB" };
			}

			auto glxContext = glCreateContextAttribs( createInfo.dpy
				, m_fbConfig[0]
				, ( mainContext
					? static_cast< X11Context const & >( mainContext->getImpl() ).m_glxContext
					: nullptr )
				, true
				, attribList.data() );
			glXDestroyContext( createInfo.dpy, m_glxContext );
			m_glxContext = glxContext;
			result = m_glxContext != nullptr;

			if ( !result )
			{
				std::stringstream error;
				error << "Failed to create an OpenGL " << extensions.getMajor() << "." << extensions.getMinor() << " context (0x" << std::hex << ::glGetError() << ").";
				throw std::runtime_error{ error.str() };
			}
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
			result = false;
		}
		catch ( ... )
		{
			result = false;
		}

		return result;
	}
}

#endif
