/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/D3D11DisplayMode.hpp"
#include "Core/D3D11Display.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
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
