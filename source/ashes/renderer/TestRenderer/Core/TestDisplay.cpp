/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "TestDisplay.hpp"
#include "TestDisplayMode.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	DisplayKHR::DisplayKHR( VkDisplayPropertiesKHR const & properties
		, VkFormat format
		, uint32_t screenIndex
		, std::vector< VkDisplayModeParametersKHR > const & displayModesParams )
		: m_properties{ properties }
		, m_format{ format }
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
				, nullptr
				, get( this )
				, createInfo );
			m_displayModes.push_back( { displayMode, parameters } );
		}
	}

	DisplayKHR::~DisplayKHR()
	{
		for ( auto & displayMode : m_displayModes )
		{
			deallocate( displayMode.displayMode, nullptr );
		}
	}

	std::vector< VkDisplayModePropertiesKHR > const & DisplayKHR::getDisplayModeProperties()const
	{
		return m_displayModes;
	}
}
