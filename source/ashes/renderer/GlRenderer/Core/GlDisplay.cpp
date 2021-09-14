/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "GlDisplay.hpp"
#include "GlDisplayMode.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	DisplayKHR::DisplayKHR( VkAllocationCallbacks const * allocInfo
		, VkPhysicalDevice physicalDevice
		, VkDisplayPropertiesKHR const & properties
		, uint32_t screenIndex
		, std::vector< VkDisplayModeParametersKHR > const & displayModesParams )
		: m_allocInfo{ allocInfo }
		, m_physicalDevice{ physicalDevice }
		, m_properties{ properties }
		, m_screenIndex{ screenIndex }
	{
		for ( auto & parameters : displayModesParams )
		{
			VkDisplayModeCreateInfoKHR createInfo
			{
				VK_STRUCTURE_TYPE_DISPLAY_MODE_CREATE_INFO_KHR,
				nullptr,
				0u,
				parameters,
			};
			VkDisplayModeKHR displayMode;
			allocate( displayMode
				, m_allocInfo
				, get( this )
				, createInfo );
			m_displayModes.push_back( { displayMode, parameters } );
		}
	}

	DisplayKHR::~DisplayKHR()
	{
		for ( auto & displayMode : m_displayModes )
		{
			deallocate( displayMode.displayMode, m_allocInfo );
		}
	}

	std::vector< VkDisplayModePropertiesKHR > const & DisplayKHR::getDisplayModeProperties()const
	{
		return m_displayModes;
	}
}
