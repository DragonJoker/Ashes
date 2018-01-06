/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkConnection.hpp"

#include "VkRenderer.hpp"

#include <Renderer/PlatformWindowHandle.hpp>

namespace vk_renderer
{
#if RENDERLIB_WIN32

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

#elif RENDERLIB_XLIB

	Connection::Connection( renderer::Renderer const & renderer
		, uint32_t deviceIndex
		, renderer::WindowHandle && handle )
		: renderer::Connection{ renderer
			, deviceIndex
			, std::move( handle ) }
		, m_connection{ static_cast< Renderer const & >( renderer ).getInstance()
			, static_cast< Renderer const & >( renderer ).getPhysicalDevice( deviceIndex )
			, m_handle.getInternal< renderer::IXWindowHandle >().getDisplay()
			, m_handle.getInternal< renderer::IXWindowHandle >().getDrawable() }
	{
	}

#elif RENDERLIB_XCB

	Connection::Connection( renderer::Renderer const & renderer
		, uint32_t deviceIndex
		, renderer::WindowHandle && handle )
		: renderer::Connection{ renderer
			, deviceIndex
			, std::move( handle ) }
		, m_connection{ static_cast< Renderer const & >( renderer ).getInstance()
			, static_cast< Renderer const & >( renderer ).getPhysicalDevice( deviceIndex )
			, m_handle.getInternal< renderer::IXcbWindowHandle >().getConnection()
			, m_handle.getInternal< renderer::IXcbWindowHandle >().getHandle() }
	{
	}

#else

#endif
}
