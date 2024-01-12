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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
		return reinterpret_cast< PFN_vkVoidFunction >( glXGetProcAddressARB( reinterpret_cast< GLubyte const * >( name ) ) );
#pragma GCC diagnostic pop
	}

	XcbContext::XcbContext( VkInstance instance
		, VkXcbSurfaceCreateInfoKHR createInfo
		, ContextImpl const * mainContext )
		: ContextImpl{ instance }
		, createInfo{ std::move( createInfo ) }
		, m_mainContext{ static_cast< XcbContext const * >( mainContext ) }
	{
	}

	XcbContext::~XcbContext()noexcept
	{
		XCloseDisplay( m_xdisplay );
	}

	void XcbContext::preInitialise( int reqMajor, int reqMinor )
	{
		m_xdisplay = XOpenDisplay( nullptr );

		if ( !m_xdisplay )
		{
			throw ashes::BaseException{ "Couldn't open display" };
		}

		auto & extensions = get( instance )->getExtensions();
		m_context = std::make_unique< ContextEgl >( m_xdisplay
			, createInfo.window
			, std::max( reqMajor, int( extensions.getMajor() ) )
			, std::max( reqMinor, int( extensions.getMinor() ) )
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

	void XcbContext::disable()const noexcept
	{
		m_context->disable();
	}

	void XcbContext::swapBuffers()const
	{
		m_context->swap();
	}

	VkExtent2D XcbContext::getExtent()const
	{
		return m_context->getExtent();
	}
}

#endif
