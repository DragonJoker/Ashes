/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	class DisplayModeKHR
	{
	public:
		DisplayModeKHR( VkDisplayKHR display
			, VkDisplayModeCreateInfoKHR const & createInfo );

		VkDisplayPlaneCapabilitiesKHR getDisplayPlaneCapabilities();

		DXGI_MODE_DESC const & getDesc()const
		{
			return m_desc;
		}

		VkDisplayKHR getDisplay()const
		{
			return m_display;
		}

	private:
		VkDisplayKHR m_display;
		DXGI_MODE_DESC m_desc;
	};
}
