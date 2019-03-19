/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/VkSurface.hpp"

#include "Core/VkPhysicalDevice.hpp"
#include "Core/VkInstance.hpp"

#include <Ashes/Core/PlatformWindowHandle.hpp>

namespace vk_renderer
{
	Surface::Surface( Instance const & instance
		, ashes::PhysicalDevice const & gpu
		, ashes::WindowHandle handle )
		: ashes::Surface{ instance, gpu, std::move( handle ) }
		, m_instance{ instance }
		, m_gpu{ static_cast< PhysicalDevice const & >( gpu ) }
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

	ashes::SurfaceCapabilities Surface::getCapabilities()const
	{
		VkSurfaceCapabilitiesKHR caps;
		auto res = m_instance.vkGetPhysicalDeviceSurfaceCapabilitiesKHR( m_gpu
			, m_surface
			, &caps );
		checkError( res, "Surface capabilities check" );
		return convert( caps );
	}

	std::vector < ashes::PresentMode > Surface::getPresentModes()const
	{
		std::vector < ashes::PresentMode > result;
		uint32_t presentModeCount{};
		auto res = m_instance.vkGetPhysicalDeviceSurfacePresentModesKHR( m_gpu
			, m_surface
			, &presentModeCount
			, nullptr );
		checkError( res, "Surface present modes enumeration" );

		if ( presentModeCount )
		{
			std::vector< VkPresentModeKHR > presentModes{ presentModeCount };
			res = m_instance.vkGetPhysicalDeviceSurfacePresentModesKHR( m_gpu
				, m_surface
				, &presentModeCount
				, presentModes.data() );
			checkError( res, "Surface present modes enumeration" );
			result = convert< ashes::PresentMode >( presentModes );
		}

		return result;
	}

	std::vector< ashes::SurfaceFormat > Surface::getFormats()const
	{
		std::vector< ashes::SurfaceFormat > result;
		uint32_t formatCount{ 0u };
		auto res = m_instance.vkGetPhysicalDeviceSurfaceFormatsKHR( m_gpu
			, m_surface
			, &formatCount
			, nullptr );
		checkError( res, "Surface formats enumeration" );

		if ( formatCount )
		{
			std::vector< VkSurfaceFormatKHR > surfFormats{ formatCount };
			res = m_instance.vkGetPhysicalDeviceSurfaceFormatsKHR( m_gpu
				, m_surface
				, &formatCount
				, surfFormats.data() );
			checkError( res, "Surface formats enumeration" );
			result = convert< ashes::SurfaceFormat >( surfFormats );
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
			m_handle.getInternal< ashes::IMswWindowHandle >().getHinstance(),
			m_handle.getInternal< ashes::IMswWindowHandle >().getHwnd(),
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
			m_handle.getInternal< ashes::IAndroidWindowHandle >().getWindow(),
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
			m_handle.getInternal< ashes::IXcbWindowHandle >().getConnection(),
			m_handle.getInternal< ashes::IXcbWindowHandle >().getHandle(),
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
			m_handle.getInternal< ashes::IMirWindowHandle >().getConnection(),
			m_handle.getInternal< ashes::IMirWindowHandle >().getSurface(),
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
			m_handle.getInternal< ashes::IWaylandWindowHandle >().getDisplay(),
			m_handle.getInternal< ashes::IWaylandWindowHandle >().getSurface(),
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
			m_handle.getInternal< ashes::IXWindowHandle >().getDisplay(),
			m_handle.getInternal< ashes::IXWindowHandle >().getDrawable(),
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
