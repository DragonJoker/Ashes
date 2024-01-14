/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class DisplayKHR
		: public NonCopyable
	{
	public:
		DisplayKHR( VkDisplayPropertiesKHR const & properties
			, VkFormat format
			, uint32_t screenIndex
			, std::vector< VkDisplayModeParametersKHR > const & displayModesParams );
		~DisplayKHR()noexcept;

		std::vector< VkDisplayModePropertiesKHR > const & getDisplayModeProperties()const;

		VkExtent2D const & getExtent()const noexcept
		{
			return m_properties.physicalResolution;
		}

		char const * getName()const noexcept
		{
			return m_properties.displayName;
		}

		uint32_t getScreenIndex()const noexcept
		{
			return m_screenIndex;
		}

		VkExtent2D getResolution()const noexcept
		{
			return m_properties.physicalResolution;
		}

	private:
		VkDisplayPropertiesKHR m_properties;
		uint32_t m_screenIndex;
		std::vector< VkDisplayModePropertiesKHR > m_displayModes;
	};
}
