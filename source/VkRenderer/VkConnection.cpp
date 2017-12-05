/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkConnection.hpp"

#include "VkRenderer.hpp"

#include <Renderer/PlatformWindowHandle.hpp>

namespace vk_renderer
{
#if defined( VK_USE_PLATFORM_WIN32_KHR )

	Connection::Connection( renderer::Renderer const & renderer
		, uint32_t deviceIndex
		, renderer::WindowHandle && handle )
		: renderer::Connection{ renderer
			, deviceIndex
			, std::move( handle ) }
		, m_connection{ static_cast< Renderer const & >( renderer ).getInstance()
			, static_cast< Renderer const & >( renderer ).getPhysicalDevice( deviceIndex )
			, m_handle.getInternal< renderer::IMswWindowHandle >().getHinstance()
			, m_handle.getInternal< renderer::IMswWindowHandle >().getHwnd() }
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
