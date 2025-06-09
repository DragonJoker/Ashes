/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::D3D11_NAMESPACE
{
	class DisplayKHR
		: public NonCopyable
	{
	public:
		DisplayKHR( VkPhysicalDevice physicalDevice
			, VkExtent2D const & extent
			, VkFormat format
			, std::vector< DXGI_MODE_DESC > const & desc );
		~DisplayKHR()noexcept;

		std::vector< VkDisplayModePropertiesKHR > const & getDisplayModeProperties()const;
		DXGI_MODE_DESC const & getDesc( VkDisplayModeParametersKHR const & parameters )const;

		VkExtent2D const & getExtent()const
		{
			return m_extent;
		}

		VkPhysicalDevice getPhysicalDevice()const
		{
			return m_physicalDevice;
		}

	private:
		VkPhysicalDevice m_physicalDevice;
		VkExtent2D m_extent;
		VkFormat m_format;
		std::vector< DXGI_MODE_DESC > m_descs;
		std::vector< VkDisplayModePropertiesKHR > m_displayModes;
	};
}
