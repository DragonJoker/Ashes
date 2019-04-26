/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/D3D11Surface.hpp"

#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11Instance.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	namespace
	{
		std::vector< VkFormat > const & getFormatsList()
		{
			static std::vector< VkFormat > const list = []()
			{
				std::vector< VkFormat > result;

				for ( uint32_t i = uint32_t( VK_FORMAT_R4G4_UNORM_PACK8 );
					i <= uint32_t( VK_FORMAT_E5B9G9R9_UFLOAT_PACK32 );
					++i )
				{
					result.push_back( VkFormat( i ) );
				}

				return result;
			}();
			return list;
		}

		std::vector< DXGI_MODE_DESC > getDisplayModesList( IDXGIOutput * adapterOutput )
		{
			std::vector< DXGI_MODE_DESC > result;

			for ( auto & format : getFormatsList() )
			{
				auto dxgiFormat = getDxgiFormat( format );

				if ( dxgiFormat != DXGI_FORMAT_UNKNOWN )
				{
					UINT numModes;
					// Get the number of modes that fit the display format for the adapter output.
					auto hr = adapterOutput->GetDisplayModeList( dxgiFormat
						, DXGI_ENUM_MODES_INTERLACED
						, &numModes
						, nullptr );

					if ( FAILED( hr ) )
					{
						throw std::runtime_error( "GetDisplayModeList(numModes) failed" );
					}

					if ( numModes )
					{
						// Create a list to hold all the possible display modes for this monitor/video card combination, and fill it.
						std::vector< DXGI_MODE_DESC > displayModeList( numModes );
						hr = adapterOutput->GetDisplayModeList( dxgiFormat
							, DXGI_ENUM_MODES_INTERLACED
							, &numModes
							, displayModeList.data() );

						if ( FAILED( hr ) )
						{
							throw std::runtime_error( "GetDisplayModeList(displayModeList) failed" );
						}

						result.insert( result.end()
							, displayModeList.begin()
							, displayModeList.end() );
					}
				}
			}

			return result;
		}

		std::map< VkFormat, std::vector< DXGI_MODE_DESC > > updateSurfaceCapabilities( std::vector< DXGI_MODE_DESC > const & displayModeList
			, RECT const & rect
			, VkSurfaceCapabilitiesKHR & capabilities )
		{
			capabilities.minImageCount = 1u;
			capabilities.maxImageCount = 1u;
			capabilities.currentExtent.width = ~( 0u );
			capabilities.currentExtent.height = ~( 0u );
			capabilities.minImageExtent = { ~( 0u ), ~( 0u ) };
			capabilities.maxImageExtent = { 0u, 0u };
			capabilities.maxImageArrayLayers = 1u;
			capabilities.supportedTransforms = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			capabilities.currentTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			capabilities.supportedCompositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
			capabilities.supportedUsageFlags = 0u;

			std::map < VkFormat, std::vector< DXGI_MODE_DESC > > result;

			// Now go through all the display modes and find the one that matches the screen width and height.
			std::vector< DXGI_MODE_DESC > matchingDisplayModes;
			std::set< DXGI_FORMAT > supportedFormats;
			auto const width = UINT( rect.right - rect.left );
			auto const height = UINT( rect.bottom - rect.top );

			for ( auto & displayMode : displayModeList )
			{
				capabilities.minImageExtent.width = std::min( capabilities.minImageExtent.width
					, displayMode.Width );
				capabilities.minImageExtent.height = std::min( capabilities.minImageExtent.height
					, displayMode.Height );

				capabilities.maxImageExtent.width = std::max( capabilities.minImageExtent.width
					, displayMode.Width );
				capabilities.maxImageExtent.height = std::max( capabilities.minImageExtent.height
					, displayMode.Height );

				supportedFormats.insert( displayMode.Format );

				if ( displayMode.Width == width
					&& displayMode.Height == height )
				{
					matchingDisplayModes.push_back( displayMode );
					auto it = result.emplace( getVkFormat( displayMode.Format ), std::vector< DXGI_MODE_DESC >{} ).first;
					it->second.emplace_back( displayMode );
				}
			}

			if ( matchingDisplayModes.empty() )
			{
				// No display mode match exactly the requested size.
				for ( auto & displayMode : displayModeList )
				{
					if ( displayMode.Width >= width
						&& displayMode.Height >= height )
					{
						matchingDisplayModes.push_back( displayMode );
						auto it = result.emplace( getVkFormat( displayMode.Format ), std::vector< DXGI_MODE_DESC >{} ).first;
						it->second.emplace_back( displayMode );
					}
				}
			}

			if ( matchingDisplayModes.empty() )
			{
				// Size is really too large.
				DXGI_MODE_DESC maxWidth{};
				DXGI_MODE_DESC maxHeight{};

				for ( auto & displayMode : displayModeList )
				{
					if ( displayMode.Width >= maxWidth.Width )
					{
						maxWidth = displayMode;
					}

					if ( displayMode.Height >= maxHeight.Height )
					{
						maxHeight = displayMode;
					}
				}

				if ( width > height )
				{
					// Choose the display mode with highest width.
					matchingDisplayModes.push_back( maxWidth );
					auto it = result.emplace( getVkFormat( maxWidth.Format ), std::vector< DXGI_MODE_DESC >{} ).first;
					it->second.emplace_back( maxWidth );
				}
				else
				{
					// Choose the display mode with highest height.
					matchingDisplayModes.push_back( maxHeight );
					auto it = result.emplace( getVkFormat( maxHeight.Format ), std::vector< DXGI_MODE_DESC >{} ).first;
					it->second.emplace_back( maxHeight );
				}
			}

			if ( !matchingDisplayModes.empty() )
			{
				auto & displayMode = matchingDisplayModes.back();
				capabilities.currentExtent.width = displayMode.Width;
				capabilities.currentExtent.height = displayMode.Height;
			}

			return result;
		}

		std::vector< VkSurfaceFormatKHR > getSurfaceFormats( std::vector< DXGI_MODE_DESC > const & displayModeList )
		{
			std::vector< VkSurfaceFormatKHR > result;
			std::set< VkFormat > uniqueFormats;

			for ( auto & displayMode : displayModeList )
			{
				auto fmt = getVkFormat( displayMode.Format );

				if ( uniqueFormats.find( fmt ) == uniqueFormats.end() )
				{
					result.push_back( { fmt, VK_COLORSPACE_SRGB_NONLINEAR_KHR } );
					uniqueFormats.insert( fmt );
				}
			}

			return result;
		}
	}

	SurfaceKHR::SurfaceKHR( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo )
		: m_createInfo{ std::move( createInfo ) }
	{
		m_type = "VK_KHR_win32_surface";
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
	}

	SurfaceKHR::~SurfaceKHR()
	{
	}

	void SurfaceKHR::getSurfaceInfos( VkPhysicalDevice physicalDevice )
	{
		m_displayModes = getDisplayModesList( get( physicalDevice )->getOutput() );

		m_surfaceFormats = getSurfaceFormats( m_displayModes );

		auto hWnd = m_createInfo.hwnd;
		RECT rect{};
		::GetWindowRect( hWnd, &rect );
		m_descs = updateSurfaceCapabilities( m_displayModes
			, rect
			, m_surfaceCapabilities );
	}

	bool SurfaceKHR::getSupport( uint32_t queueFamilyIndex )const
	{
		return true;
	}
}
