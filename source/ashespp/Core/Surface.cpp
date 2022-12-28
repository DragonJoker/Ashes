/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Core/Surface.hpp"

#include "ashespp/Core/Instance.hpp"
#include "ashespp/Core/PlatformWindowHandle.hpp"

namespace ashes
{
	Surface::Surface( Instance const & instance
		, PhysicalDevice const & gpu
		, WindowHandle handle )
		: m_instance{ instance }
		, m_gpu{ gpu }
		, m_handle{ std::move( handle ) }
	{
		doCreate();
		m_handle.setSurface( *this );
	}

	Surface::~Surface()
	{
		if ( m_internal != VkSurfaceKHR{} )
		{
			m_instance.vkDestroySurfaceKHR( m_instance
				, m_internal
				, m_instance.getAllocationCallbacks() );
		}
	}

	bool Surface::getSupport( uint32_t queueFamilyIndex )const
	{
		VkBool32 result{};
		auto res = m_instance.vkGetPhysicalDeviceSurfaceSupportKHR( m_gpu
			, queueFamilyIndex
			, m_internal
			, &result );
		checkError( res, "Presentation surface support check" );
		return result == VK_TRUE;
	}

	VkSurfaceCapabilitiesKHR Surface::getCapabilities()const
	{
		VkSurfaceCapabilitiesKHR caps{};
		auto res = m_instance.vkGetPhysicalDeviceSurfaceCapabilitiesKHR( m_gpu
			, m_internal
			, &caps );
		checkError( res, "Surface capabilities check" );
		return caps;
	}

	std::vector < VkPresentModeKHR > Surface::getPresentModes()const
	{
		std::vector < VkPresentModeKHR > result;
		uint32_t presentModeCount{};
		auto res = m_instance.vkGetPhysicalDeviceSurfacePresentModesKHR( m_gpu
			, m_internal
			, &presentModeCount
			, nullptr );
		checkError( res, "Surface present modes enumeration" );

		if ( presentModeCount )
		{
			result.resize( presentModeCount );
			res = m_instance.vkGetPhysicalDeviceSurfacePresentModesKHR( m_gpu
				, m_internal
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
			, m_internal
			, &formatCount
			, nullptr );
		checkError( res, "Surface formats enumeration" );

		if ( formatCount )
		{
			result.resize( formatCount );
			res = m_instance.vkGetPhysicalDeviceSurfaceFormatsKHR( m_gpu
				, m_internal
				, &formatCount
				, result.data() );
			checkError( res, "Surface formats enumeration" );
		}

		return result;
	}

	void Surface::doCreate()
	{
#if defined( VK_USE_PLATFORM_WIN32_KHR )
		if ( m_handle.getSurfaceName() == VK_KHR_WIN32_SURFACE_EXTENSION_NAME )
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
				, m_instance.getAllocationCallbacks()
				, &m_internal );
			checkError( res, "Presentation surface creation" );
			m_type = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
			return;
		}

#endif
#if defined( VK_USE_PLATFORM_ANDROID_KHR )
		if ( m_handle.getSurfaceName() == VK_KHR_ANDROID_SURFACE_EXTENSION_NAME )
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
				, m_instance.getAllocationCallbacks()
				, &m_internal );
			checkError( res, "Presentation surface creation" );
			m_type = VK_KHR_ANDROID_SURFACE_EXTENSION_NAME;
			return;
		}
#endif
#if defined( VK_USE_PLATFORM_XCB_KHR )
		if ( m_handle.getSurfaceName() == VK_KHR_XCB_SURFACE_EXTENSION_NAME )
		{
			VkXcbSurfaceCreateInfoKHR createInfo
			{
				VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
				nullptr,
				0,
				m_handle.getInternal< IXcbWindowHandle >().getConnection(),
				m_handle.getInternal< IXcbWindowHandle >().getWindow(),
			};
			auto res = m_instance.vkCreateXcbSurfaceKHR( m_instance
				, &createInfo
				, m_instance.getAllocationCallbacks()
				, &m_internal );
			checkError( res, "Presentation surface creation" );
			m_type = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
			return;
		}
#endif
#if defined( VK_USE_PLATFORM_MIR_KHR )
		if ( m_handle.getSurfaceName() == VK_KHR_MIR_SURFACE_EXTENSION_NAME )
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
				, m_instance.getAllocationCallbacks()
				, &m_internal );
			checkError( res, "Presentation surface creation" );
			m_type = VK_KHR_MIR_SURFACE_EXTENSION_NAME;
			return;
		}
#endif
#if defined( VK_USE_PLATFORM_WAYLAND_KHR )
		if ( m_handle.getSurfaceName() == VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME )
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
				, m_instance.getAllocationCallbacks()
				, &m_internal );
			checkError( res, "Presentation surface creation" );
			m_type = VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME;
			return;
		}
#endif
#if defined( VK_USE_PLATFORM_XLIB_KHR )
		if ( m_handle.getSurfaceName() == VK_KHR_XLIB_SURFACE_EXTENSION_NAME )
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
				, m_instance.getAllocationCallbacks()
				, &m_internal );
			checkError( res, "Presentation surface creation" );
			m_type = VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
			return;
		}
#endif
#if defined( VK_USE_PLATFORM_MACOS_MVK )
		if ( m_handle.getSurfaceName() == VK_MVK_MACOS_SURFACE_EXTENSION_NAME )
		{
			VkMacOSSurfaceCreateInfoMVK createInfo
			{
				VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK,
				nullptr,
				0,
				m_handle.getInternal< IMacOsWindowHandle >().getView(),
			};
			auto res = m_instance.vkCreateMacOSSurfaceMVK( m_instance
				, &createInfo
				, m_instance.getAllocationCallbacks()
				, &m_internal );
			checkError( res, "Presentation surface creation" );
			m_type = VK_MVK_MACOS_SURFACE_EXTENSION_NAME;
			return;
		}
#endif
	}
}
