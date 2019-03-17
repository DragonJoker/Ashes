/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/D3D11Surface.hpp"

#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11Instance.hpp"

#include <Ashes/Core/PlatformWindowHandle.hpp>

namespace d3d11_renderer
{
	namespace
	{
		std::vector< ashes::Format > const & getFormatsList()
		{
			static std::vector< ashes::Format > const list = []()
			{
				std::vector< ashes::Format > result;

				for ( uint32_t i = uint32_t( ashes::Format::eColour_BEGIN );
					i <= uint32_t( ashes::Format::eColour_END );
					++i )
				{
					result.push_back( ashes::Format( i ) );
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

					if ( !dxCheckError( hr, "GetDisplayModeList" ) )
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

						if ( !dxCheckError( hr, "GetDisplayModeList" ) )
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

		ashes::SurfaceCapabilities updateSurfaceCapabilities( IDXGIOutput * adapterOutput
			, RECT const & rect )
		{
			ashes::SurfaceCapabilities result;
			result.minImageCount = 1u;
			result.maxImageCount = 1u;
			result.currentExtent.width = ~( 0u );
			result.currentExtent.height = ~( 0u );
			result.minImageExtent = { ~( 0u ), ~( 0u ) };
			result.maxImageExtent = { 0u, 0u };
			result.maxImageArrayLayers = 1u;
			result.supportedTransforms = ashes::SurfaceTransformFlag::eIdentity;
			result.currentTransform = ashes::SurfaceTransformFlag::eIdentity;
			result.supportedCompositeAlpha = ashes::CompositeAlphaFlag::eInherit;
			result.supportedUsageFlags = ashes::ImageUsageFlag::eUndefined;

			auto displayModeList = getDisplayModesList( adapterOutput );

			// Now go through all the display modes and find the one that matches the screen width and height.
			std::vector< DXGI_MODE_DESC > matchingDisplayModes;
			std::set< DXGI_FORMAT > supportedFormats;
			auto const width = UINT( rect.right - rect.left );
			auto const height = UINT( rect.bottom - rect.top );

			for ( auto & displayMode : displayModeList )
			{
				result.minImageExtent.width = std::min( result.minImageExtent.width
					, displayMode.Width );
				result.minImageExtent.height = std::min( result.minImageExtent.height
					, displayMode.Height );

				result.maxImageExtent.width = std::max( result.minImageExtent.width
					, displayMode.Width );
				result.maxImageExtent.height = std::max( result.minImageExtent.height
					, displayMode.Height );

				supportedFormats.insert( displayMode.Format );

				if ( displayMode.Width == width
					&& displayMode.Height == height )
				{
					matchingDisplayModes.push_back( displayMode );
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
				}
				else
				{
					// Choose the display mode with highest height.
					matchingDisplayModes.push_back( maxHeight );
				}
			}

			if ( !matchingDisplayModes.empty() )
			{
				auto & displayMode = matchingDisplayModes.back();
				result.currentExtent.width = displayMode.Width;
				result.currentExtent.height = displayMode.Height;
			}

			return result;
		}

		std::vector< ashes::SurfaceFormat > getSurfaceFormats( IDXGIOutput * adapterOutput )
		{
			std::vector< ashes::SurfaceFormat > result;
			std::set< ashes::Format > uniqueFormats;
			auto displayModeList = getDisplayModesList( adapterOutput );

			for ( auto & displayMode : displayModeList )
			{
				auto fmt = convert( displayMode.Format );

				if ( uniqueFormats.find( fmt ) == uniqueFormats.end() )
				{
					result.push_back( { fmt, ashes::ColorSpace::eSRGBNonLinear } );
					uniqueFormats.insert( fmt );
				}
			}

			return result;
		}
	}

	Surface::Surface( Instance const & instance
		, ashes::PhysicalDevice const & gpu
		, ashes::WindowHandle handle )
		: ashes::Surface{ instance, gpu, std::move( handle ) }
	{
		m_type = "VK_KHR_win32_surface";
	}

	Surface::~Surface()
	{
	}

	bool Surface::getSupport( uint32_t queueFamilyIndex )const
	{
		return true;
	}

	std::vector< ashes::PresentMode > Surface::getPresentModes()const
	{
		static std::vector< ashes::PresentMode > const modes
		{
			ashes::PresentMode::eFifo
		};
		return modes;
	}

	ashes::SurfaceCapabilities Surface::getCapabilities()const
	{
		auto hWnd = m_handle.getInternal< ashes::IMswWindowHandle >().getHwnd();
		RECT rect{};
		::GetWindowRect( hWnd, &rect );
		return updateSurfaceCapabilities( static_cast< PhysicalDevice const & >( m_gpu ).getOutput()
			, rect );
	}

	std::vector< ashes::SurfaceFormat > Surface::getFormats()const
	{
		return getSurfaceFormats( static_cast< PhysicalDevice const & >( m_gpu ).getOutput() );
	}
}
