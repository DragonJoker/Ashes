/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class DisplayModeKHR
		: public AutoIdIcdObject< DisplayModeKHR >
	{
	public:
		DisplayModeKHR( VkAllocationCallbacks const * allocInfo
			, VkDisplayKHR display
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
