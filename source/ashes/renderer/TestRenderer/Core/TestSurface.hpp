/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class SurfaceKHR
	{
	private:
		SurfaceKHR();

	public:
#if _WIN32
		SurfaceKHR( VkInstance instance
			, VkWin32SurfaceCreateInfoKHR const & createInfo );
#elif __linux__
		SurfaceKHR( VkInstance instance
			, VkXlibSurfaceCreateInfoKHR const & createInfo );
		SurfaceKHR( VkInstance instance
			, VkXcbSurfaceCreateInfoKHR const & createInfo );
		SurfaceKHR( VkInstance instance
			, VkWaylandSurfaceCreateInfoKHR const & createInfo );
#elif __APPLE__
		SurfaceKHR( VkInstance instance
			, VkMacOSSurfaceCreateInfoMVK const & createInfo );
#endif
#if VK_KHR_display
		SurfaceKHR( VkInstance instance
			, VkDisplaySurfaceCreateInfoKHR const & createInfo );
#endif

		bool getSupport( uint32_t queueFamilyIndex )const;

		VkSurfaceCapabilitiesKHR getCapabilities()const noexcept
		{
			return m_surfaceCapabilities;
		}

		VkPresentModeArrayKHR getPresentModes()const noexcept
		{
			return m_presentModes;
		}

		VkSurfaceFormatArrayKHR getFormats()const noexcept
		{
			return m_surfaceFormats;
		}

		VkInstance getInstance()const noexcept
		{
			return m_instance;
		}

	private:
		VkInstance m_instance{};
		VkSurfaceFormatArrayKHR m_surfaceFormats;
		VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		VkPresentModeArrayKHR m_presentModes;
	};
}
