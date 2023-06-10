/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#if __linux__

#include "GlRendererPrerequisites.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-identifier"

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
		RenderWindow( int major
			, int minor
			, std::string const & name );
		~RenderWindow();
		VkXlibSurfaceCreateInfoKHR getCreateInfo()const;

	private:
		void doCleanup();

	private:
		Colormap m_map{ 0 };
		Display * m_display{ nullptr };
		Window m_window{ 0 };
#if ASHES_USE_XLIB_EGL
		ContextEglPtr m_context;
#else
		GLXFBConfig m_fbConfig{ nullptr };
		GLXContext m_glxContext{ nullptr };
#endif
	};
}

#pragma clang diagnostic pop

#endif
