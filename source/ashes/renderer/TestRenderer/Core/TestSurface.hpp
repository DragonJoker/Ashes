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
	public:
		SurfaceKHR( VkInstance instance
			, VkSurfaceCreateInfoKHR createInfo );

		bool getSupport( uint32_t queueFamilyIndex )const;

		VkSurfaceCapabilitiesKHR getCapabilities()const
		{
			return m_surfaceCapabilities;
		}

		VkPresentModeArrayKHR getPresentModes()const
		{
			return m_presentModes;
		}

		VkSurfaceFormatArrayKHR getFormats()const
		{
			return m_surfaceFormats;
		}
		
		VkInstance getInstance()const
		{
			return m_instance;
		}

	private:
		VkInstance m_instance;
		VkSurfaceCreateInfoKHR m_createInfo;
		VkSurfaceFormatArrayKHR m_surfaceFormats;
		VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		VkPresentModeArrayKHR m_presentModes;
	};
}
