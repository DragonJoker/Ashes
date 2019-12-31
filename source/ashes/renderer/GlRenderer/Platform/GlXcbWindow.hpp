/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#if VK_USE_PLATFORM_XCB_KHR

#include "EglContext.hpp"

#include <ashes/ashes.hpp>

namespace ashes::gl
{
	class RenderWindow
	{
	private:
		RenderWindow( int major
			, int minor
			, std::string const & name );
		~RenderWindow();

	public:
		VkSurfaceCreateInfoKHR getCreateInfo()const;
		static void create( int major, int minor );
		static void destroy();
		static RenderWindow const & get();

	private:
		void doCleanup();

	private:
		Display * m_display;
		xcb_connection_t * m_connection;
		xcb_window_t m_window;
		ContextEglPtr m_context;
	};
}

#endif
