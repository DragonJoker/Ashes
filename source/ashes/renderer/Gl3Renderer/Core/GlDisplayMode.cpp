/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlDisplayMode.hpp"
#include "Core/GlDisplay.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
{
	DisplayModeKHR::DisplayModeKHR( VkDisplayKHR display
		, VkDisplayModeCreateInfoKHR createInfo )
		: m_display{ display }
		, m_createInfo{ std::move( createInfo ) }
	{
	}

	DisplayModeKHR::~DisplayModeKHR()
	{
	}

	VkDisplayPlaneCapabilitiesKHR DisplayModeKHR::getDisplayPlaneCapabilities( uint32_t planeIndex )
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
