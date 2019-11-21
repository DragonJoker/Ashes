/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
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

		VkDisplayModeParametersKHR const & getParameters()
		{
			return m_createInfo.parameters;
		}

	private:
		VkDisplayKHR m_display;
		VkDisplayModeCreateInfoKHR m_createInfo;
	};
}
