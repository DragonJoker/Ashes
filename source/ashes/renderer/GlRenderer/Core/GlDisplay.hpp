/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class DisplayKHR
		: public AutoIdIcdObject< DisplayKHR >
	{
	public:
		DisplayKHR( VkAllocationCallbacks const * allocInfo
			, VkPhysicalDevice physicalDevice
			, VkDisplayPropertiesKHR const & properties
			, uint32_t screenIndex
			, std::vector< VkDisplayModeParametersKHR > const & displayModesParams );
		~DisplayKHR()noexcept;

		std::vector< VkDisplayModePropertiesKHR > const & getDisplayModeProperties()const;

		VkExtent2D const & getExtent()const
		{
			return m_properties.physicalResolution;
		}

		char const * getName()const
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

		VkPhysicalDevice getPhysicalDevice()const
		{
			return m_physicalDevice;
		}

	private:
		VkAllocationCallbacks const * m_allocInfo;
		VkPhysicalDevice m_physicalDevice;
		VkDisplayPropertiesKHR m_properties;
		uint32_t m_screenIndex;
		std::vector< VkDisplayModePropertiesKHR > m_displayModes;
	};
}
