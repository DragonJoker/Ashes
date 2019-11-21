/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	class DisplayKHR
	{
	public:
		DisplayKHR( VkDisplayPropertiesKHR const & properties
			, VkFormat format
			, uint32_t screenIndex
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

		uint32_t getScreenIndex()const
		{
			return m_screenIndex;
		}

		VkExtent2D getResolution()const
		{
			return m_properties.physicalResolution;
		}

	private:
		VkDisplayPropertiesKHR m_properties;
		VkFormat m_format;
		uint32_t m_screenIndex;
		std::vector< VkDisplayModePropertiesKHR > m_displayModes;
	};
}
