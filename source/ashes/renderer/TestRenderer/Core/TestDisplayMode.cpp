/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/TestDisplayMode.hpp"
#include "Core/TestDisplay.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	DisplayModeKHR::DisplayModeKHR( VkDisplayKHR display
		, VkDisplayModeCreateInfoKHR createInfo )
		: m_display{ display }
		, m_createInfo{ std::move( createInfo ) }
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
