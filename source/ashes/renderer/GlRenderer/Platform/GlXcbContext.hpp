/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Core/GlContextImpl.hpp"

#if __linux__
#include "EglContext.hpp"

namespace ashes::gl
{
	class XcbContext
		: public ContextImpl
	{
	public:
		XcbContext( VkInstance instance
			, VkXcbSurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
		~XcbContext();

		void preInitialise( int major, int minor )override;
		void postInitialise()override;
		void enable()const override;
		void disable()const override;
		void swapBuffers()const override;
		VkExtent2D getExtent()const override;

	private:
		VkXcbSurfaceCreateInfoKHR createInfo;
		Display * m_xdisplay{ nullptr };
		ContextEglPtr m_context;
		XcbContext const * m_mainContext{ nullptr };
	};
}

#endif
