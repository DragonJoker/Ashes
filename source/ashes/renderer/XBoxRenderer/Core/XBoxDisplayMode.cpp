/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/XBoxDisplayMode.hpp"
#include "Core/XBoxDisplay.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	DisplayModeKHR::DisplayModeKHR( VkDisplayKHR display
		, VkDisplayModeCreateInfoKHR const & createInfo )
		: m_display{ display }
		, m_desc{ get( m_display )->getDesc( createInfo.parameters ) }
	{
	}

	VkDisplayPlaneCapabilitiesKHR DisplayModeKHR::getDisplayPlaneCapabilities()
	{
		VkExtent2D defaultExt{ get( m_display )->getExtent() };
		VkOffset2D defaultMinPos{};
		VkOffset2D defaultMaxPos{};
		VkDisplayPlaneCapabilitiesKHR const result
		{
			VK_DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR,
			defaultMinPos,
			defaultMaxPos,
			defaultExt,
			defaultExt,
			defaultMinPos,
			defaultMaxPos,
			defaultExt,
			defaultExt,
		};
		return result;
	}
}
