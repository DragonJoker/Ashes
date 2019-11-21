/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	class DisplayKHR
	{
	public:
		DisplayKHR( VkDisplayPropertiesKHR const & properties
			, VkFormat format
			, std::vector< VkDisplayModeParametersKHR > const & displayModesParams );
		~DisplayKHR();

		std::vector< VkDisplayModePropertiesKHR > const & getDisplayModeProperties()const;

		VkExtent2D const & getExtent()const
		{
			return m_properties.physicalResolution;
		}

		char const * const getName()const
		{
			return m_properties.displayName;
		}

	private:
		VkDisplayPropertiesKHR m_properties;
		VkFormat m_format;
		std::vector< VkDisplayModePropertiesKHR > m_displayModes;
	};
}
