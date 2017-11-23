/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "Connection.hpp"

#include "Renderer.hpp"

namespace renderer
{
#if defined( VK_USE_PLATFORM_WIN32_KHR )

	Connection::Connection( Renderer const & renderer
		, HINSTANCE hinstance
		, HWND hwnd )
		: m_connection{ renderer.getInstance()
			, renderer.getPhysicalDevice()
			, hinstance
			, hwnd }
	{
	}

#elif defined( VK_USE_PLATFORM_XCB_KHR )

	Connection::Connection( Renderer const & renderer
		, xcb_connection_t * connection
		, xcb_window_t handle )
		: m_connection{ renderer.getInstance()
			, renderer.getPhysicalDevice()
			, connection
			, handle }
	{
	}

#else

	Connection::Connection( Renderer const & renderer
		, Display * display
		, Window window )
		: m_connection{ renderer.getInstance()
			, renderer.getPhysicalDevice()
			, display
			, window }
	{
	}

#endif
}
