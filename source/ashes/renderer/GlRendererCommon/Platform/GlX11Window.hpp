/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#if __linux__

#include "renderer/GlRendererCommon/GlCommonPrerequisites.hpp"

#if ASHES_USE_XLIB_EGL
#	include "EglContext.hpp"
#else
typedef struct __GLXFBConfigRec * GLXFBConfig;
typedef struct __GLXcontextRec * GLXContext;
#endif
typedef XID Colormap;

namespace ashes::gl
{
	class RenderWindow
	{
	public:
		RenderWindow( int major, int minor );
		~RenderWindow();
		VkXlibSurfaceCreateInfoKHR getCreateInfo()const;

	private:
		void doCleanup();

	private:
		Colormap m_map{ 0 };
		Display * m_display{ nullptr };
		Window m_window{ 0 };
#if ASHES_USE_XLIB_EGL
		EglContextPtr m_context;
#else
		GLXFBConfig m_fbConfig{ nullptr };
		GLXContext m_glxContext{ nullptr };
#endif
	};
}

#endif
