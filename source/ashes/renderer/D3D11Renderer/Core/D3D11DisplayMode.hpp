/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class DisplayModeKHR
	{
	public:
		DisplayModeKHR( VkDisplayKHR display
			, VkDisplayModeCreateInfoKHR createInfo );
		~DisplayModeKHR();

		VkDisplayPlaneCapabilitiesKHR getDisplayPlaneCapabilities( uint32_t planeIndex );

		DXGI_MODE_DESC const & getDesc()const
		{
			return m_desc;
		}

	private:
		VkDisplayKHR m_display;
		DXGI_MODE_DESC m_desc;
	};
}
