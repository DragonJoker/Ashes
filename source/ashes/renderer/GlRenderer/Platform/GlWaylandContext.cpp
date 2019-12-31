/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "GlWaylandContext.hpp"

#if __linux__

#include <EGL/egl.h>
#include <wayland-egl.h>

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace ashes::gl
{
	WaylandContext::WaylandContext( VkInstance instance
		, VkWaylandSurfaceCreateInfoKHR createInfo
		, ContextImpl const * mainContext )
		: ContextImpl{ instance }
		, createInfo{ std::move( createInfo ) }
		, m_mainContext{ static_cast< WaylandContext const * >( mainContext ) }
	{
	}

	WaylandContext::~WaylandContext()
	{
		wl_egl_window_destroy( m_window );
	}

	void WaylandContext::preInitialise( int major, int minor )
	{
		m_window = wl_egl_window_create (createInfo.surface, 0, 0);
		m_context = std::make_unique< ContextEgl >( createInfo.display
			, m_window
			, major
			, minor
			, ( m_mainContext
				? m_mainContext->m_context->getContext()
				: EGL_NO_CONTEXT ) );
	}

	void WaylandContext::postInitialise()
	{
	}

	void WaylandContext::enable()const
	{
		m_context->enable();
	}

	void WaylandContext::disable()const
	{
		m_context->disable();
	}

	void WaylandContext::swapBuffers()const
	{
		m_context->swap();
	}
}

#endif
