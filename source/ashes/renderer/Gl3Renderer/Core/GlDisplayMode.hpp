/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	class DisplayModeKHR
	{
	public:
		DisplayModeKHR( VkDisplayKHR display
			, VkDisplayModeCreateInfoKHR createInfo );
		~DisplayModeKHR();

		VkDisplayPlaneCapabilitiesKHR getDisplayPlaneCapabilities( uint32_t planeIndex );

		VkDisplayKHR getDisplay()const
		{
			return m_display;
		}

		uint32_t getRefreshRate()const
		{
			return m_createInfo.parameters.refreshRate;
		}

	private:
		VkDisplayKHR m_display;
		VkDisplayModeCreateInfoKHR m_createInfo;
	};
}
