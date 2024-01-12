/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Core/GlContextImpl.hpp"

#if __linux__
#	if ASHES_USE_XLIB_EGL
#	include "EglContext.hpp"

namespace ashes::gl
{
	class X11EglContext
		: public ContextImpl
	{
	public:
		X11EglContext( VkInstance instance
			, VkXlibSurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
		~X11EglContext()noexcept override;

		void preInitialise( int major, int minor )override;
		void postInitialise()override;
		void enable()const override;
		void disable()const noexcept override;
		void swapBuffers()const override;
		VkExtent2D getExtent()const override;

	private:
		VkXlibSurfaceCreateInfoKHR createInfo;
		ContextEglPtr m_context;
		X11EglContext const * m_mainContext{ nullptr };
	};
}

#	endif
#endif
