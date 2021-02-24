/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Core/GlContextImpl.hpp"

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
		X11Context( VkInstance instance
			, VkDisplaySurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
		~X11Context();

		void preInitialise( int major, int minor )override;
		void postInitialise()override;
		void enable()const override;
		void disable()const override;
		void swapBuffers()const override;
		VkExtent2D getExtent()const override;

	private:
		void doLoadSytemFunctions();
		void doInitialiseDisplay();
		void doCreateModernContext();

	private:
		VkXlibSurfaceCreateInfoKHR xlibCreateInfo{};
		VkDisplaySurfaceCreateInfoKHR displayCreateInfo{};
		Display * m_display;
		int m_screenIndex;
		Colormap m_map{ 0 };
		Window m_window{ 0 };
		GLXContext m_glxContext{ nullptr };
		int m_glxVersion{ 0 };
		int m_minor{ 0 };
		int m_major{ 0 };
		XVisualInfo * m_visualInfo{ nullptr };
		GLXFBConfig m_fbConfig{ nullptr };
		X11Context const * m_mainContext{ nullptr };
		
		using PFN_glCreateContextAttribs = GLXContext ( * )( Display *, GLXFBConfig, GLXContext, Bool, const int * );
		using PFN_glXSwapIntervalEXT = void (*)( Display *, GLXDrawable, int );
		PFN_glCreateContextAttribs glCreateContextAttribs;
		PFN_glXSwapIntervalEXT glXSwapInterval;
	};
}

#endif
