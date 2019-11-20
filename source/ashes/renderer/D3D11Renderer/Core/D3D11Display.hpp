/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class DisplayKHR
	{
	public:
		DisplayKHR( VkExtent2D const & extent
			, VkFormat format
			, std::vector< DXGI_MODE_DESC > const & desc );
		~DisplayKHR();

		std::vector< VkDisplayModePropertiesKHR > const & getDisplayModeProperties()const;
		DXGI_MODE_DESC const & getDesc( VkDisplayModeParametersKHR const & parameters )const;

		VkExtent2D const & getExtent()const
		{
			return m_extent;
		}

	private:
		VkExtent2D m_extent;
		VkFormat m_format;
		std::vector< DXGI_MODE_DESC > m_descs;
		std::vector< VkDisplayModePropertiesKHR > m_displayModes;
	};
}
