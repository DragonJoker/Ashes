#if defined( __linux__ )

#include "GlX11Context.hpp"

#include <Renderer/PlatformWindowHandle.hpp>

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace gl_renderer
{
	namespace
	{
		std::function< GLXFBConfig *( Display *, int, int const *, int * ) > glXChooseFBConfig;
		std::function< XVisualInfo *( Display *, GLXFBConfig ) > glXGetVisualFromFBConfig;

#if !defined( NDEBUG )

		static const int C3D_GL_CONTEXT_CREATION_DEFAULT_FLAGS = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT | GL_CONTEXT_FLAG_DEBUG_BIT;
		static const int C3D_GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#else

		static const int C3D_GL_CONTEXT_CREATION_DEFAULT_FLAGS = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
		static const int C3D_GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#endif
	}

	X11Context::X11Context( renderer::ConnectionPtr && connection )
		: Context{ std::move( connection ) }
		, m_display( m_connection.getHandle().getInternal< renderer::IXWindowHandle >().getDisplay() )
		, m_glxVersion( 10 )
		, m_glxContext( nullptr )
		, m_drawable( m_connection.getHandle().getInternal< renderer::IXWindowHandle >().getDrawable() )
		, m_fbConfig( nullptr )
	{
		if ( !glXChooseFBConfig )
		{
			gl_api::getFunction( cuT( "glXChooseFBConfig" ), glXChooseFBConfig );
			gl_api::getFunction( cuT( "glXGetVisualFromFBConfig" ), glXGetVisualFromFBConfig );
		}

		if ( m_drawable == 0 )
		{
			m_drawable = p_window->getHandle().getInternal< IXWindowHandle >()->getDrawable();
			m_display = p_window->getHandle().getInternal< IXWindowHandle >()->getDisplay();
		}

		auto engine = p_window->getEngine();
		auto renderSystem = static_cast< GlRenderSystem * >( engine->getRenderSystem() );
		auto mainContext = std::static_pointer_cast< GlContext >( renderSystem->getMainContext() );

		try
		{
			int screen = DefaultScreen( m_display );
			int major{ 0 };
			int minor{ 0 };
			bool ok = glXQueryVersion( m_display, &major, &minor );
			XVisualInfo * visualInfo = nullptr;

			if ( ok )
			{
				m_glxVersion = major * 10 + minor;
				Logger::logDebug( StringStream() << cuT( "GlXContext::create - glx version: " ) << m_glxVersion );
			}

			IntArray attribList
			{
				GLX_RENDER_TYPE, GLX_RGBA_BIT,
				GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
				GLX_DOUBLEBUFFER, GL_TRUE,
				GLX_RED_SIZE, 1,
				GLX_GREEN_SIZE, 1,
				GLX_BLUE_SIZE, 1,
				0
			};

			if ( p_window->isUsingStereo() )
			{
				attribList.push_back( GLX_STEREO );
				attribList.push_back( 1 );
			}

			attribList.push_back( 0 );

			if ( glXChooseFBConfig )
			{
				visualInfo = doCreateVisualInfoWithFBConfig( p_window, attribList, screen );
			}
			else
			{
				visualInfo = doCreateVisualInfoWithoutFBConfig( attribList, screen );
			}

			if ( visualInfo )
			{
				if ( mainContext )
				{
					m_glxContext = glXCreateContext( m_display, visualInfo, mainContext->getImpl().getContext(), GL_TRUE );
				}
				else
				{
					m_glxContext = glXCreateContext( m_display, visualInfo, nullptr, GL_TRUE );
				}

				if ( m_glxContext )
				{
					if ( !renderSystem->isInitialised() )
					{
						Logger::logDebug( StringStream() << cuT( "Display: " ) << std::hex << cuT( "0x" ) << m_display );
						Logger::logDebug( StringStream() << cuT( "Drawable: " ) << std::hex << cuT( "0x" ) << m_drawable );
						Logger::logDebug( StringStream() << cuT( "Context: " ) << std::hex << cuT( "0x" ) << m_glxContext );
						glXMakeCurrent( m_display, m_drawable, m_glxContext );
						getOpenGl().preInitialise( String() );
						glXMakeCurrent( m_display, 0, nullptr );
					}

					if ( getOpenGl().getVersion() >= 30 )
					{
						m_initialised = doCreateGl3Context( p_window );
					}
					else
					{
						m_initialised = true;
					}
				}

				XFree( visualInfo );
			}
		}
		catch ( ... )
		{
			m_initialised = false;
		}

		if ( m_initialised )
		{
			glTrack( getOpenGl(), "X11Context", this );
		}

		return m_initialised;
	}

	X11Context::~X11Context()
	{
		try
		{
			glUntrack( getOpenGl(), this );
			getOpenGl().DeleteContext( m_display, m_glxContext );
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

	XVisualInfo * X11Context::doCreateVisualInfoWithFBConfig( RenderWindow * p_window, IntArray p_arrayAttribs, int p_screen )
	{
		Logger::logDebug( cuT( "GlXContext::create - Using FBConfig" ) );
		XVisualInfo * visualInfo = nullptr;
		int nbElements = 0;
		m_fbConfig = glXChooseFBConfig( m_display, p_screen, p_arrayAttribs.data(), &nbElements );

		if ( !m_fbConfig )
		{

			// First try failed
			if ( p_window->isUsingStereo() )
			{
				// Maybe because of stereo ? We try in mono
				m_gpuInformations.removeFeature( GpuFeature::eStereo );
				Logger::logWarning( cuT( "GlXContext::create - Stereo glXChooseFBConfig failed, using mono FB config" ) );

				IntArray attribList
				{
					GLX_RENDER_TYPE, GLX_RGBA_BIT,
					GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
					GLX_DOUBLEBUFFER, GL_TRUE,
					GLX_RED_SIZE, 1,
					GLX_GREEN_SIZE, 1,
					GLX_BLUE_SIZE, 1,
					0
				};
				m_fbConfig = glXChooseFBConfig( m_display, p_screen, attribList.data(), &nbElements );

				if ( !m_fbConfig )
				{
					// Second try failed, we try a default FBConfig
					Logger::logWarning( cuT( "GlXContext::create - Mono glXChooseFBConfig failed, using default FB config" ) );
					int data = 0;
					m_fbConfig = glXChooseFBConfig( m_display, p_screen, &data, &nbElements );

					if ( !m_fbConfig )
					{
						// Last FBConfig try failed
						Logger::logWarning( cuT( "GlXContext::create - Default glXChooseFBConfig failed" ) );
						visualInfo = doCreateVisualInfoWithoutFBConfig( attribList, p_screen );
					}
					else
					{
						Logger::logDebug( cuT( "GlXContext::create - Default glXChooseFBConfig successful" ) );
					}
				}
				else
				{
					Logger::logDebug( cuT( "GlXContext::create - Mono glXChooseFBConfig successful with detailed attributes" ) );
				}
			}
			else
			{
				// Can't be because of stereo, we try a default FBConfig
				Logger::logWarning( cuT( "GlXContext::create - glXChooseFBConfig failed, using default FB config" ) );
				int data = 0;
				m_fbConfig = glXChooseFBConfig( m_display, p_screen, &data, &nbElements );

				if ( !m_fbConfig )
				{
					// Last FBConfig try failed, we try from XVisualInfo
					Logger::logWarning( cuT( "GlXContext::create - Default glXChooseFBConfig failed" ) );
					visualInfo = doCreateVisualInfoWithoutFBConfig( p_arrayAttribs, p_screen );
				}
				else
				{
					Logger::logDebug( cuT( "GlXContext::create - Default glXChooseFBConfig successful" ) );
				}
			}
		}
		else
		{
			if ( p_window->isUsingStereo() )
			{
				m_gpuInformations.addFeature( GpuFeature::eStereo );
				Logger::logDebug( cuT( "GlXContext::create - Stereo glXChooseFBConfig successful with detailed attributes" ) );
			}
			else
			{
				Logger::logDebug( cuT( "GlXContext::create - glXChooseFBConfig successful with detailed attributes" ) );
			}
		}

		if ( m_fbConfig )
		{
			visualInfo = glXGetVisualFromFBConfig( m_display, m_fbConfig[0] );

			if ( !visualInfo )
			{
				Logger::logError( cuT( "GlXContext::create - glXgetVisualFromFBConfig failed" ) );
			}
			else
			{
				Logger::logDebug( cuT( "GlXContext::create - GlXgetVisualFromFBConfig successful" ) );
			}
		}

		return visualInfo;
	}

	XVisualInfo * X11Context::doCreateVisualInfoWithoutFBConfig( IntArray p_arrayAttribs, int p_screen )
	{
		Logger::logInfo( cuT( "GlXContext::create - Not using FBConfig" ) );
		XVisualInfo * result = glXChooseVisual( m_display, p_screen, p_arrayAttribs.data() );

		if ( !result )
		{
			Logger::logError( cuT( "GlXContext::create - glXChooseVisual failed" ) );
		}

		return result;
	}

	bool X11Context::doCreateGl3Context( castor3d::RenderWindow * p_window )
	{
		bool result = false;
		GlRenderSystem * renderSystem = static_cast< GlRenderSystem * >( p_window->getEngine()->getRenderSystem() );
		GlContextSPtr mainContext = std::static_pointer_cast< GlContext >( renderSystem->getMainContext() );

		if ( getOpenGl().HasCreateContextAttribs() )
		{
			int major = getOpenGl().getVersion() / 10;
			int minor = getOpenGl().getVersion() % 10;
			IntArray attribList
			{
				int( GlContextAttribute::eMajorVersion ), major,
				int( GlContextAttribute::eMinorVersion ), minor,
				int( GlContextAttribute::eFlags ), C3D_GL_CONTEXT_CREATION_DEFAULT_FLAGS,
				int( GlProfileAttribute::eMask ), C3D_GL_CONTEXT_CREATION_DEFAULT_MASK,
				0
			};
			getOpenGl().DeleteContext( m_display, m_glxContext );

			if ( mainContext )
			{
				m_glxContext = getOpenGl().CreateContextAttribs( m_display, m_fbConfig[0], mainContext->getImpl().getContext(), true, attribList.data() );
			}
			else
			{
				m_glxContext = getOpenGl().CreateContextAttribs( m_display, m_fbConfig[0], nullptr, true, attribList.data() );
			}

			result = m_glxContext != nullptr;

			if ( result )
			{
				Logger::logInfo( StringStream() << cuT( "GlContext::create - " ) << major << cuT( "." ) << minor << cuT( " OpenGL context created." ) );
			}
			else
			{
				Logger::logError( StringStream() << cuT( "GlContext::create - Failed to create a " ) << major << cuT( "." ) << minor << cuT( " OpenGL context." ) );
			}
		}
		else
		{
			//It's not possible to make a GL 3.x and later context. Use the old style context (GL 2.1 and before)
			renderSystem->setOpenGlVersion( 2, 1 );
			Logger::logWarning( cuT( "GlContext::create - Can't create OpenGL >= 3.x context, since glCreateContextAttribs is not supported." ) );
			result = true;
		}

		return result;
	}
}

#endif
