/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "GlXcbContext.hpp"

#if __linux__

#include "ashesgl_api.hpp"

#include <EGL/egl.h>
#include <GL/glx.h>

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace ashes::gl
{
	PFN_vkVoidFunction getFunction( char const * const name )
	{
		return reinterpret_cast< PFN_vkVoidFunction >( glXGetProcAddressARB( reinterpret_cast< GLubyte const * >( name ) ) );
	}

	namespace
	{
#if !defined( NDEBUG )

		static const int GL_CONTEXT_CREATION_DEFAULT_FLAGS = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT | GL_CONTEXT_FLAG_DEBUG_BIT;
		static const int GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#else

		static const int GL_CONTEXT_CREATION_DEFAULT_FLAGS = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
		static const int GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#endif
	}

	XcbContext::XcbContext( VkInstance instance
		, VkXcbSurfaceCreateInfoKHR createInfo
		, ContextImpl const * mainContext )
		: ContextImpl{ instance }
		, createInfo{ std::move( createInfo ) }
		, m_mainContext{ static_cast< XcbContext const * >( mainContext ) }
	{
	}

	XcbContext::~XcbContext()
	{
		XCloseDisplay( m_xdisplay );
	}

	void XcbContext::preInitialise( int reqMajor, int reqMinor )
	{
		m_xdisplay = XOpenDisplay( nullptr );

		if ( !m_xdisplay )
		{
			throw std::runtime_error{ "Couldn't open display" };
		}

		auto & extensions = get( instance )->getExtensions();
		m_context = std::make_unique< ContextEgl >( m_xdisplay
			, createInfo.window
			, std::max( reqMajor, extensions.getMajor() )
			, std::max( reqMinor, extensions.getMinor() )
			, ( m_mainContext
				? m_mainContext->m_context->getContext()
				: EGL_NO_CONTEXT ) );
	}

	void XcbContext::postInitialise()
	{
	}

	void XcbContext::enable()const
	{
		m_context->enable();
	}

	void XcbContext::disable()const
	{
		m_context->disable();
	}

	void XcbContext::swapBuffers()const
	{
		m_context->swap();
	}

	VkExtent2D CoreContext::getExtent()const
	{
		return m_context->getExtent();
	}
}

#endif
