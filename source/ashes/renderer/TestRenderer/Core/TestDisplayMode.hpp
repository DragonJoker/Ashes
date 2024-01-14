/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class DisplayModeKHR
	{
	public:
		DisplayModeKHR( VkDisplayKHR display
			, VkDisplayModeCreateInfoKHR createInfo );

		VkDisplayPlaneCapabilitiesKHR getDisplayPlaneCapabilities();

		VkDisplayKHR getDisplay()const noexcept
		{
			return m_display;
		}

		VkDisplayModeParametersKHR const & getParameters()const noexcept
		{
			return m_createInfo.parameters;
		}

	private:
		VkDisplayKHR m_display;
		VkDisplayModeCreateInfoKHR m_createInfo;
	};
}
