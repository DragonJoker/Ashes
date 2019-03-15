/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/VkConnection.hpp"

#include "Core/VkPhysicalDevice.hpp"
#include "Core/VkInstance.hpp"

#include <Ashes/Core/PlatformWindowHandle.hpp>

namespace vk_renderer
{
	Connection::Connection( Instance const & instance
		, ashes::PhysicalDevice const & gpu
		, ashes::WindowHandle handle )
		: ashes::Connection{ instance, gpu, std::move( handle ) }
		, m_instance{ instance }
		, m_gpu{ static_cast< PhysicalDevice const & >( gpu ) }
	{
		doCreatePresentSurface();
		updateSurfaceCapabilities();
	}

	Connection::~Connection()
	{
		if ( m_presentSurface != VK_NULL_HANDLE )
		{
			m_instance.vkDestroySurfaceKHR( m_instance
				, m_presentSurface
				, nullptr );
		}
	}

	bool Connection::getSurfaceSupport( uint32_t queueFamilyIndex )const
	{
		VkBool32 result;
		auto res = m_instance.vkGetPhysicalDeviceSurfaceSupportKHR( m_gpu
			, queueFamilyIndex
			, m_presentSurface
			, &result );
		checkError( res, "Presentation surface support check" );
		return result == VK_TRUE;
	}

	void Connection::updateSurfaceCapabilities()
	{
		doRetrieveSurfaceCapabilities();
		doRetrieveSurfaceFormats();
		doRetrievePresentModes();
	}

#if ASHES_WIN32

	void Connection::doCreatePresentSurface()
	{
		VkWin32SurfaceCreateInfoKHR createInfo
		{
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			m_handle.getInternal< ashes::IMswWindowHandle >().getHinstance(),
			m_handle.getInternal< ashes::IMswWindowHandle >().getHwnd(),
		};
		DEBUG_DUMP( createInfo );
		auto res = m_instance.vkCreateWin32SurfaceKHR( m_instance
			, &createInfo
			, nullptr
			, &m_presentSurface );
		checkError( res, "Presentation surface creation" );
		m_surfaceType = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
	}

#elif ASHES_ANDROID

	void Connection::doCreatePresentSurface()
	{
		VkAndroidSurfaceCreateInfoKHR createInfo =
		{
			VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			m_handle.getInternal< ashes::IAndroidWindowHandle >().getWindow(),
		};
		DEBUG_DUMP( createInfo );
		auto res = m_instance.vkCreateAndroidSurfaceKHR( m_instance
			, &createInfo
			, nullptr
			, &m_presentSurface );
		checkError( res, "Presentation surface creation" );
		m_surfaceType = VK_KHR_ANDROID_SURFACE_EXTENSION_NAME;
	}

#elif ASHES_XCB

	void Connection::doCreatePresentSurface()
	{
		VkXcbSurfaceCreateInfoKHR createInfo
		{
			VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			m_handle.getInternal< ashes::IXcbWindowHandle >().getConnection(),
			m_handle.getInternal< ashes::IXcbWindowHandle >().getHandle(),
		};
		auto res = m_instance.vkCreateXcbSurfaceKHR( m_instance
			, &createInfo
			, nullptr
			, &m_presentSurface );
		checkError( res, "Presentation surface creation" );
		m_surfaceType = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
	}

#elif ASHES_MIR

	void Connection::doCreatePresentSurface()
	{
		VkMirSurfaceCreateInfoKHR createInfo =
		{
			VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR,
			nullptr,
			flags,
			m_handle.getInternal< ashes::IMirWindowHandle >().getConnection(),
			m_handle.getInternal< ashes::IMirWindowHandle >().getSurface(),
		};
		auto res = m_instance.vkCreateMirSurfaceKHR( m_instance
			, &createInfo
			, nullptr
			, &m_presentSurface );
		checkError( res, "Presentation surface creation" );
		m_surfaceType = VK_KHR_MIR_SURFACE_EXTENSION_NAME;
	}

#elif ASHES_WAYLAND

	void Connection::doCreatePresentSurface()
	{
		VkWaylandSurfaceCreateInfoKHR createInfo =
		{
			VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			m_handle.getInternal< ashes::IWaylandWindowHandle >().getDisplay(),
			m_handle.getInternal< ashes::IWaylandWindowHandle >().getSurface(),
		};
		auto res = m_instance.vkCreateWaylandSurfaceKHR( m_instance
			, &createInfo
			, nullptr
			, &m_presentSurface );
		checkError( res, "Presentation surface creation" );
		m_surfaceType = VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME;
	}

#elif ASHES_XLIB

	void Connection::doCreatePresentSurface()
	{
		VkXlibSurfaceCreateInfoKHR createInfo
		{
			VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			m_handle.getInternal< ashes::IXWindowHandle >().getDisplay(),
			m_handle.getInternal< ashes::IXWindowHandle >().getDrawable(),
		};
		auto res = m_instance.vkCreateXlibSurfaceKHR( m_instance
			, &createInfo
			, nullptr
			, &m_presentSurface );
		checkError( res, "Presentation surface creation" );
		m_surfaceType = VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
	}

#else

#	error "Unsupported window system."

#endif

	void Connection::doRetrieveSurfaceCapabilities()
	{
		// On récupère les capacités de la surface.
		VkSurfaceCapabilitiesKHR caps;
		auto res = m_instance.vkGetPhysicalDeviceSurfaceCapabilitiesKHR( m_gpu
			, m_presentSurface
			, &caps );
		checkError( res, "Surface capabilities check" );
		m_surfaceCapabilities = convert( caps );
	}

	void Connection::doRetrievePresentModes()
	{
		// On récupère la liste de VkPresentModeKHR supportés par la surface.
		uint32_t presentModeCount{};
		auto res = m_instance.vkGetPhysicalDeviceSurfacePresentModesKHR( m_gpu
			, m_presentSurface
			, &presentModeCount
			, nullptr );
		checkError( res, "Surface present modes enumeration" );

		if ( presentModeCount )
		{
			std::vector< VkPresentModeKHR > presentModes{ presentModeCount };
			res = m_instance.vkGetPhysicalDeviceSurfacePresentModesKHR( m_gpu
				, m_presentSurface
				, &presentModeCount
				, presentModes.data() );
			checkError( res, "Surface present modes enumeration" );
			m_presentModes = convert< ashes::PresentMode >( presentModes );
		}
	}

	void Connection::doRetrieveSurfaceFormats()
	{
		// On récupère la liste de VkFormat supportés par la surface.
		uint32_t formatCount{ 0u };
		auto res = m_instance.vkGetPhysicalDeviceSurfaceFormatsKHR( m_gpu
			, m_presentSurface
			, &formatCount
			, nullptr );
		checkError( res, "Surface formats enumeration" );

		if ( formatCount )
		{
			std::vector< VkSurfaceFormatKHR > surfFormats{ formatCount };
			res = m_instance.vkGetPhysicalDeviceSurfaceFormatsKHR( m_gpu
				, m_presentSurface
				, &formatCount
				, surfFormats.data() );
			checkError( res, "Surface formats enumeration" );
			m_surfaceFormats = convert< ashes::SurfaceFormat >( surfFormats );
		}
	}
}
