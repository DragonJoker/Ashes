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

		template< typename FuncT >
		bool getFunction( char const * const name, FuncT & function )
		{
			function = reinterpret_cast< FuncT >( glXGetProcAddressARB( reinterpret_cast< GLubyte const * >( name ) ) );
			return function != nullptr;
		}

		template< typename Func >
		bool getFunction( std::string const & name, Func & func )
		{
			return getFunction( name.c_str(), func );
		}
	}

	X11Context::X11Context( VkInstance instance
		, VkXlibSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
		: Context{ instance }
		, m_glxVersion( 10 )
		, m_glxContext( nullptr )
		, m_fbConfig( nullptr )
	{
		m_createInfo = std::move( createInfo );

		if ( !glXChooseFBConfig )
		{
			getFunction( "glXChooseFBConfig", glXChooseFBConfig );
			getFunction( "glXGetVisualFromFBConfig", glXGetVisualFromFBConfig );
		}

		int screen = DefaultScreen( m_createInfo.dpy );
		int major{ 0 };
		int minor{ 0 };
		bool ok = glXQueryVersion( m_createInfo.dpy, &major, &minor );
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
			m_glxContext = glXCreateContext( m_createInfo.dpy, visualInfo, nullptr, GL_TRUE );

			if ( !m_glxContext )
			{
				throw std::runtime_error{ "Could not create a rendering context." };
			}

			enable();
			doLoadBaseFunctions();
			doLoadGLXFunctions();
			doLoadDebugFunctions();
			disable();

			auto & extensions = get( m_instance )->getExtensions();

			if ( extensions.getMajor() < 4 )
			{
				glXDestroyContext( m_createInfo.dpy, m_glxContext );
				throw std::runtime_error{ "The supported OpenGL version is insufficient." };
			}

			if ( !doCreateGl3Context( static_cast< X11Context const * >( mainContext ) ) )
			{
				glXDestroyContext( m_createInfo.dpy, m_glxContext );
				throw std::runtime_error{ "The supported OpenGL version is insufficient." };
			}

			XFree( visualInfo );
			enable();

			if ( m_glXSwapIntervalEXT )
			{
				m_glXSwapIntervalEXT( m_createInfo.dpy, m_createInfo.window, 0 );
			}

			disable();
		}
	}

	X11Context::X11Context( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
		: X11Context
		{
			instance,
			get( surface )->getContext()->m_createInfo,
			mainContext
		}
	{
	}

	X11Context::~X11Context()
	{
		try
		{
			glXDestroyContext( m_createInfo.dpy, m_glxContext );
			XFree( m_fbConfig );
		}
		catch ( ... )
		{
		}
	}

	void X11Context::enable()const
	{
		glXMakeCurrent( m_createInfo.dpy, m_createInfo.window, m_glxContext );
		m_enabled = true;
	}

	void X11Context::disable()const
	{
		m_enabled = false;
		glXMakeCurrent( m_createInfo.dpy, 0, nullptr );
	}

	void X11Context::swapBuffers()const
	{
		glXSwapBuffers( m_createInfo.dpy, m_createInfo.window );
	}

	void X11Context::doLoadBaseFunctions()
	{
#define GL_LIB_BASE_FUNCTION( fun )\
		m_gl##fun = &::gl##fun;
#define GL_LIB_FUNCTION( fun )\
		if ( !( getFunction( "gl"#fun, m_gl##fun ) ) )\
		{\
			throw std::runtime_error{ std::string{ "Couldn't load function " } + "gl"#fun };\
		}
#define GL_LIB_FUNCTION_OPT( fun )\
		if ( !( getFunction( "gl"#fun, m_gl##fun ) ) )\
		{\
			std::cerr << "Couldn't load function " << "gl"#fun << std::endl;\
		}
#include "Miscellaneous/OpenGLFunctionsList.inl"
	}

	void X11Context::doLoadGLXFunctions()
	{
#define GLX_LIB_FUNCTION( fun )\
		if ( !( getFunction( "glX"#fun, m_glX##fun ) ) )\
		{\
			std::cerr << "Couldn't load function " << "gl"#fun << std::endl;\
		}
#include "Miscellaneous/OpenGLFunctionsList.inl"
	}

	void X11Context::doLoadDebugFunctions()
	{
		auto & extensions = get( m_instance )->getExtensions();

		if ( extensions.find( KHR_debug ) )
		{
			if ( !getFunction( "glDebugMessageCallback", glDebugMessageCallback ) )
			{
				if ( !getFunction( "glDebugMessageCallbackKHR", glDebugMessageCallback ) )
				{
					std::cerr << "Unable to retrieve function glDebugMessageCallback" << std::endl;
				}
			}
		}
		else if ( extensions.find( ARB_debug_output ) )
		{
			if ( !getFunction( "glDebugMessageCallback", glDebugMessageCallback ) )
			{
				if ( !getFunction( "glDebugMessageCallbackARB", glDebugMessageCallback ) )
				{
					std::cerr << "Unable to retrieve function glDebugMessageCallback" << std::endl;
				}
			}
		}
		else if ( extensions.find( AMDX_debug_output ) )
		{
			if ( !getFunction( "glDebugMessageCallbackAMD", glDebugMessageCallbackAMD ) )
			{
				std::cerr << "Unable to retrieve function glDebugMessageCallbackAMD" << std::endl;
			}
		}

		if ( glDebugMessageCallback )
		{
			if ( !getFunction( "glObjectLabel", glObjectLabel ) )
			{
				std::cerr << "Unable to retrieve function glObjectLabel" << std::endl;
			}

			if ( !getFunction( "glObjectPtrLabel", glObjectPtrLabel ) )
			{
				std::cerr << "Unable to retrieve function glObjectPtrLabel" << std::endl;
			}

			for ( auto & callback : get( m_instance )->getDebugCallbacks() )
			{
				glDebugMessageCallback( callback.callback, callback.userParam );
				::glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
			}
		}
		else if ( glDebugMessageCallbackAMD )
		{
			for ( auto & callback : get( m_instance )->getDebugAMDCallbacks() )
			{
				glDebugMessageCallbackAMD( callback.callback, callback.userParam );
				::glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
			}
		}
	}

	XVisualInfo * X11Context::doCreateVisualInfoWithFBConfig( std::vector< int > arrayAttribs, int screen )
	{
		XVisualInfo * visualInfo = nullptr;
		int nbElements = 0;
		m_fbConfig = glXChooseFBConfig( m_createInfo.dpy, screen, arrayAttribs.data(), &nbElements );

		if ( !m_fbConfig )
		{
			// First try failed, we try a default FBConfig
			std::cerr << "glXChooseFBConfig failed, using default FB config" << std::endl;
			int data = 0;
			m_fbConfig = glXChooseFBConfig( m_createInfo.dpy, screen, &data, &nbElements );

			if ( !m_fbConfig )
			{
				// Last FBConfig try failed, we try from XVisualInfo
				std::cerr << "Default glXChooseFBConfig failed" << std::endl;
				visualInfo = doCreateVisualInfoWithoutFBConfig( arrayAttribs, screen );
			}
		}

		if ( m_fbConfig )
		{
			visualInfo = glXGetVisualFromFBConfig( m_createInfo.dpy, m_fbConfig[0] );

			if ( !visualInfo )
			{
				std::cerr << "glXgetVisualFromFBConfig failed" << std::endl;
			}
		}

		return visualInfo;
	}

	XVisualInfo * X11Context::doCreateVisualInfoWithoutFBConfig( std::vector< int > arrayAttribs, int screen )
	{
		XVisualInfo * result = glXChooseVisual( m_createInfo.dpy, screen, arrayAttribs.data() );

		if ( !result )
		{
			std::cerr << "glXChooseVisual failed" << std::endl;
		}

		return result;
	}

	bool X11Context::doCreateGl3Context( X11Context const * mainContext )
	{
		auto & extensions = get( m_instance )->getExtensions();
		using PFNGLCREATECONTEXTATTRIBS = GLXContext ( * )( Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list );
		PFNGLCREATECONTEXTATTRIBS glCreateContextAttribs;
		bool result = false;
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

		if ( getFunction( "glXCreateContextAttribsARB", glCreateContextAttribs ) )
		{
			auto glxContext = glCreateContextAttribs( m_createInfo.dpy
				, m_fbConfig[0]
				, ( mainContext
					? static_cast< X11Context const * >( mainContext )->m_glxContext
					: nullptr )
				, true
				, attribList.data() );
			glXDestroyContext( m_createInfo.dpy, m_glxContext );
			m_glxContext = glxContext;
			result = m_glxContext != nullptr;

			if ( !result )
			{
				std::cerr << "Failed to create an OpenGL " << extensions.getMajor() << "." << extensions.getMinor() << " context." << std::endl;
			}
		}
		else
		{
			std::cerr << "Couldn't load glXCreateContextAttribsARB function." << std::endl;
		}

		return result;
	}
}

#endif
