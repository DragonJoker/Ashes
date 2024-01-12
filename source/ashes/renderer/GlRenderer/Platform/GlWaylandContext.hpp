/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Core/GlContextImpl.hpp"

#if __linux__
#include "EglContext.hpp"

struct wl_egl_window;

namespace ashes::gl
{
	class WaylandContext
		: public ContextImpl
	{
	public:
		WaylandContext( VkInstance instance
			, VkWaylandSurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
		~WaylandContext()noexcept override;

		void preInitialise( int major, int minor )override;
		void postInitialise()override;
		void enable()const override;
		void disable()const noexcept override;
		void swapBuffers()const override;
		VkExtent2D getExtent()const override;

	private:
		VkWaylandSurfaceCreateInfoKHR createInfo;
		wl_egl_window * m_window{ nullptr };
		ContextEglPtr m_context;
		WaylandContext const * m_mainContext{ nullptr };
	};
}

#endif
