/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Core/Surface.hpp"

#include "AshesPP/Core/Instance.hpp"
#include "AshesCommon/PlatformWindowHandle.hpp"

namespace ashes
{
	Surface::Surface( Instance const & instance
		, PhysicalDevice const & gpu
		, WindowHandle handle )
		: m_instance{ instance }
		, m_handle{ std::move( handle ) }
		, m_gpu{ gpu }
	{
		doCreate();
	}

	Surface::~Surface()
	{
		if ( m_surface != VK_NULL_HANDLE )
		{
			m_instance.vkDestroySurfaceKHR( m_instance
				, m_surface
				, nullptr );
		}
	}

	bool Surface::getSupport( uint32_t queueFamilyIndex )const
	{
		VkBool32 result;
		auto res = m_instance.vkGetPhysicalDeviceSurfaceSupportKHR( m_gpu
			, queueFamilyIndex
			, m_surface
			, &result );
		checkError( res, "Presentation surface support check" );
		return result == VK_TRUE;
	}

	VkSurfaceCapabilitiesKHR Surface::getCapabilities()const
	{
		VkSurfaceCapabilitiesKHR caps;
		auto res = m_instance.vkGetPhysicalDeviceSurfaceCapabilitiesKHR( m_gpu
			, m_surface
			, &caps );
		checkError( res, "Surface capabilities check" );
		return caps;
	}

	std::vector < VkPresentModeKHR > Surface::getPresentModes()const
	{
		std::vector < VkPresentModeKHR > result;
		uint32_t presentModeCount{};
		auto res = m_instance.vkGetPhysicalDeviceSurfacePresentModesKHR( m_gpu
			, m_surface
			, &presentModeCount
			, nullptr );
		checkError( res, "Surface present modes enumeration" );

		if ( presentModeCount )
		{
			result.resize( presentModeCount );
			res = m_instance.vkGetPhysicalDeviceSurfacePresentModesKHR( m_gpu
				, m_surface
				, &presentModeCount
				, result.data() );
			checkError( res, "Surface present modes enumeration" );
		}

		return result;
	}

	std::vector< VkSurfaceFormatKHR > Surface::getFormats()const
	{
		std::vector< VkSurfaceFormatKHR > result;
		uint32_t formatCount{ 0u };
		auto res = m_instance.vkGetPhysicalDeviceSurfaceFormatsKHR( m_gpu
			, m_surface
			, &formatCount
			, nullptr );
		checkError( res, "Surface formats enumeration" );

		if ( formatCount )
		{
			result.resize( formatCount );
			res = m_instance.vkGetPhysicalDeviceSurfaceFormatsKHR( m_gpu
				, m_surface
				, &formatCount
				, result.data() );
			checkError( res, "Surface formats enumeration" );
		}

		return result;
	}

#if ASHES_WIN32

	void Surface::doCreate()
	{
		VkWin32SurfaceCreateInfoKHR createInfo
		{
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			m_handle.getInternal< IMswWindowHandle >().getHinstance(),
			m_handle.getInternal< IMswWindowHandle >().getHwnd(),
		};
		DEBUG_DUMP( createInfo );
		auto res = m_instance.vkCreateWin32SurfaceKHR( m_instance
			, &createInfo
			, nullptr
			, &m_surface );
		checkError( res, "Presentation surface creation" );
		m_type = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
	}

#elif ASHES_ANDROID

	void Surface::doCreate()
	{
		VkAndroidSurfaceCreateInfoKHR createInfo =
		{
			VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			m_handle.getInternal< IAndroidWindowHandle >().getWindow(),
		};
		DEBUG_DUMP( createInfo );
		auto res = m_instance.vkCreateAndroidSurfaceKHR( m_instance
			, &createInfo
			, nullptr
			, &m_surface );
		checkError( res, "Presentation surface creation" );
		m_type = VK_KHR_ANDROID_SURFACE_EXTENSION_NAME;
	}

#elif ASHES_XCB

	void Surface::doCreate()
	{
		VkXcbSurfaceCreateInfoKHR createInfo
		{
			VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			m_handle.getInternal< IXcbWindowHandle >().getConnection(),
			m_handle.getInternal< IXcbWindowHandle >().getHandle(),
		};
		auto res = m_instance.vkCreateXcbSurfaceKHR( m_instance
			, &createInfo
			, nullptr
			, &m_surface );
		checkError( res, "Presentation surface creation" );
		m_type = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
	}

#elif ASHES_MIR

	void Surface::doCreate()
	{
		VkMirSurfaceCreateInfoKHR createInfo =
		{
			VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR,
			nullptr,
			flags,
			m_handle.getInternal< IMirWindowHandle >().getConnection(),
			m_handle.getInternal< IMirWindowHandle >().getSurface(),
		};
		auto res = m_instance.vkCreateMirSurfaceKHR( m_instance
			, &createInfo
			, nullptr
			, &m_surface );
		checkError( res, "Presentation surface creation" );
		m_type = VK_KHR_MIR_SURFACE_EXTENSION_NAME;
	}

#elif ASHES_WAYLAND

	void Surface::doCreate()
	{
		VkWaylandSurfaceCreateInfoKHR createInfo =
		{
			VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			m_handle.getInternal< IWaylandWindowHandle >().getDisplay(),
			m_handle.getInternal< IWaylandWindowHandle >().getSurface(),
		};
		auto res = m_instance.vkCreateWaylandSurfaceKHR( m_instance
			, &createInfo
			, nullptr
			, &m_surface );
		checkError( res, "Presentation surface creation" );
		m_type = VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME;
	}

#elif ASHES_XLIB

	void Surface::doCreate()
	{
		VkXlibSurfaceCreateInfoKHR createInfo
		{
			VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			m_handle.getInternal< IXWindowHandle >().getDisplay(),
			m_handle.getInternal< IXWindowHandle >().getDrawable(),
		};
		auto res = m_instance.vkCreateXlibSurfaceKHR( m_instance
			, &createInfo
			, nullptr
			, &m_surface );
		checkError( res, "Presentation surface creation" );
		m_type = VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
	}

#else

#	error "Unsupported window system."

#endif
}
