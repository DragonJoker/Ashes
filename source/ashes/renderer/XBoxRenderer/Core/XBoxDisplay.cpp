/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/XBoxDisplay.hpp"

#include "Core/XBoxDisplayMode.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	DisplayKHR::DisplayKHR( VkPhysicalDevice physicalDevice
		, VkExtent2D const & extent
		, VkFormat format
		, std::vector< DXGI_MODE_DESC > const & descs )
		: m_physicalDevice{ physicalDevice }
		, m_extent{ extent }
		, m_format{ format }
		, m_descs{ descs }
	{
		for ( auto const & desc : m_descs )
		{
			VkDisplayModeParametersKHR parameters
			{
				extent,
				uint32_t( ( 1000.0f * float( desc.RefreshRate.Numerator ) ) / float( desc.RefreshRate.Denominator ) ),
			};
			m_displayModes.push_back(
				{
					nullptr,
					parameters,
				} );
		}

		for ( auto & displayMode : m_displayModes )
		{
			VkDisplayModeCreateInfoKHR createInfo
			{
				VK_STRUCTURE_TYPE_DISPLAY_MODE_CREATE_INFO_KHR,
				nullptr,
				0u,
				displayMode.parameters,
			};
			allocate( displayMode.displayMode
				, nullptr
				, get( this )
				, createInfo );
		}
	}

	DisplayKHR::~DisplayKHR()noexcept
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

	DXGI_MODE_DESC const & DisplayKHR::getDesc( VkDisplayModeParametersKHR const & parameters )const
	{
		auto it = std::find_if( m_displayModes.begin()
			, m_displayModes.end()
			, [&parameters]( VkDisplayModePropertiesKHR const & lookup )
			{
				return lookup.parameters.refreshRate == parameters.refreshRate
					&& lookup.parameters.visibleRegion.width == parameters.visibleRegion.width
					&& lookup.parameters.visibleRegion.height == parameters.visibleRegion.height;
			} );
		assert( it != m_displayModes.end() );
		return *( m_descs.begin() + std::distance( m_displayModes.begin(), it ) );
	}
}
