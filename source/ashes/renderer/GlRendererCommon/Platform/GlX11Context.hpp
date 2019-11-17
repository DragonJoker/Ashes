/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRendererCommon/GlContext.hpp"

#if __linux__
#include <X11/Xlib.h>
#include <GL/glx.h>

namespace ashes::gl
{
	class X11Context
		: public ContextImpl
	{
	public:
		X11Context( VkInstance instance
			, VkXlibSurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
		~X11Context();

		void preInitialise( int major, int minor )override;
		void postInitialise()override;
		void enable()const override;
		void disable()const override;
		void swapBuffers()const override;

	private:
		void doCreateModernContext();

	private:
		VkXlibSurfaceCreateInfoKHR createInfo;
		GLXContext m_glxContext{ nullptr };
		int m_glxVersion{ 0 };
		int m_minor{ 0 };
		int m_major{ 0 };
		XVisualInfo * m_visualInfo{ nullptr };
		GLXFBConfig m_fbConfig{ nullptr };
		X11Context const * m_mainContext{ nullptr };
	};
}

#endif
