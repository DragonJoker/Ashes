/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "CGlContext.hpp"

#include "CGlView.hh"

#if defined( __APPLE__ )
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

void * CGLGetProcAddress( const char * name );
void linkContextToView( CGLContextObj context
	, const void * view );

namespace ashes::gl
{
	namespace
	{
		std::string getName( CGLError error )
		{
			switch ( error )
			{
			case kCGLNoError:
				return "No error";
			case kCGLBadAttribute:
				return "Invalid pixel format attribute";
			case kCGLBadProperty:
				return "Invalid renderer property";
			case kCGLBadPixelFormat:
				return "Invalid pixel format";
			case kCGLBadRendererInfo:
				return "Invalid renderer info";
			case kCGLBadContext:
				return "Invalid context";
			case kCGLBadDrawable:
				return "Invalid drawable";
			case kCGLBadDisplay:
				return "Invalid graphics device";
			case kCGLBadState:
				return "Invalid context state";
			case kCGLBadValue:
				return "Invalid numerical value";
			case kCGLBadMatch:
				return "Invalid share context";
			case kCGLBadEnumeration:
				return "Invalid enumerant";
			case kCGLBadOffScreen:
				return "Invalid offscreen drawable";
			case kCGLBadFullScreen:
				return "Invalid offscreen drawable";
			case kCGLBadWindow:
				return "Invalid window";
			case kCGLBadAddress:
				return "Invalid pointer";
			case kCGLBadCodeModule:
				return "Invalid code module";
			case kCGLBadAlloc:
				return "Invalid memory allocation";
			case kCGLBadConnection:
				return "Invalid CoreGraphics connection";
			default:
				return "Unknown error";
			}
		}
	}

	PFN_vkVoidFunction getFunction( char const * const name )
	{
		return reinterpret_cast< PFN_vkVoidFunction >( CGLGetProcAddress( name ) );
	}

	void checkCGLErrorCode( CGLError error, std::string const & name )
	{
		if (error != kCGLNoError)
		{
			throw std::runtime_error{ name + ": " + getName( error ) };
		}
	}

	CoreContext::CoreContext( VkInstance instance
		, VkMacOSSurfaceCreateInfoMVK createInfo
		, ContextImpl const * mainContext )
		: ContextImpl{ instance }
		, surfaceCreateInfo{ std::move( createInfo ) }
		, m_mainContext{ static_cast< CoreContext const * >( mainContext ) }
	{
	}

	CoreContext::CoreContext( VkInstance instance
		, VkDisplaySurfaceCreateInfoKHR createInfo
		, ContextImpl const * mainContext )
		: ContextImpl{ instance, createInfo.imageExtent }
		, displayCreateInfo{ std::move( createInfo ) }
		, m_mainContext{ static_cast< CoreContext const * >( mainContext ) }
	{
	}

	CoreContext::~CoreContext()
	{
		if (surfaceCreateInfo.pView)
		{
			freeGLView( m_glView );
			m_glView = {};
		}
		else
		{
			CGLDestroyContext( m_cglContext );
		}
	}

	void CoreContext::preInitialise( int reqMajor, int reqMinor ) try
	{
		if (surfaceCreateInfo.pView)
		{
			m_glView = makeGLView( surfaceCreateInfo.pView );
			m_cglContext = m_glView.context;
		}
		else
		{
			std::vector< CGLPixelFormatAttribute > attributes
			{
				kCGLPFAAccelerated,
				kCGLPFAOpenGLProfile, CGLPixelFormatAttribute( kCGLOGLPVersion_GL4_Core ),
				CGLPixelFormatAttribute( 0 ),
			};

			CGLPixelFormatObj pix;
			int num; // stores the number of possible pixel formats

			auto errorCode = CGLChoosePixelFormat( attributes.data(), &pix, &num );
			checkCGLErrorCode( errorCode, "CGLChoosePixelFormat" );

			errorCode = CGLCreateContext( pix
				, (m_mainContext  
					? m_mainContext->m_cglContext
					: nullptr)
				, &m_cglContext ); // second parameter can be another context for object sharing
			checkCGLErrorCode( errorCode, "CGLCreateContext" );

			CGLDestroyPixelFormat( pix );
		}

		m_major = reqMajor;
		m_minor = reqMinor;
	}
	catch ( std::exception & exc )
	{
		if ( m_cglContext )
		{
			CGLDestroyContext( m_cglContext );
		}

		throw;
	}

	void CoreContext::postInitialise()
	{
		enable();
		int sync = 0;
		auto errorCode = CGLSetParameter( m_cglContext, kCGLCPSwapInterval, &sync );
		disable();
		checkCGLErrorCode( errorCode, "CGLSetParameter - kCGLCPSwapInterval" );
	}

	void CoreContext::enable()const
	{
		auto errorCode = CGLSetCurrentContext( m_cglContext );
		checkCGLErrorCode( errorCode, "CGLSetCurrentContext - m_cglContext" );
	}

	void CoreContext::disable()const
	{
		auto errorCode = CGLSetCurrentContext( nullptr );
		checkCGLErrorCode( errorCode, "CGLSetCurrentContext - nullptr" );
	}

	void CoreContext::swapBuffers()const
	{
		auto errorCode = CGLFlushDrawable( m_cglContext );
		checkCGLErrorCode( errorCode, "CGLFlushDrawable" );
	}
}

#endif
