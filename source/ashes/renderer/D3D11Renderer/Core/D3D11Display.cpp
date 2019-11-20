/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/D3D11Display.hpp"

#include "Core/D3D11DisplayMode.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	DisplayKHR::DisplayKHR( VkExtent2D const & extent
		, VkFormat format
		, std::vector< DXGI_MODE_DESC > const & descs )
		: m_extent{ extent }
		, m_format{ format }
		, m_descs{ descs }
	{
		for ( auto & desc : m_descs )
		{
			VkDisplayModeParametersKHR parameters
			{
				extent,
				uint32_t( ( 1000.0f * desc.RefreshRate.Numerator ) / desc.RefreshRate.Denominator ),
			};
			m_displayModes.push_back(
				{
					VK_NULL_HANDLE,
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
