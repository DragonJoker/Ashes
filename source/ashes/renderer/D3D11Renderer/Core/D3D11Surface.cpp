/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/D3D11Surface.hpp"

#include "Core/D3D11DisplayMode.hpp"
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
			}( );
			return list;
		}

		void updateSurfaceCapabilities( std::vector< DXGI_MODE_DESC > const & displayModeList
			, RECT const & rect
			, VkSurfaceCapabilitiesKHR & capabilities
			, std::map< VkFormat, std::vector< DXGI_MODE_DESC > > & descs
			, std::map< VkFormat, DXGI_MODE_DESC > & matchingDescs )
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
			capabilities.supportedCompositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			capabilities.supportedUsageFlags = 0u;

			// Now go through all the display modes and find the one that matches the screen width and height.
			std::vector< DXGI_MODE_DESC > matchingDisplayModes;
			auto width = UINT( rect.right - rect.left );
			auto height = UINT( rect.bottom - rect.top );

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
			}

			width = std::min( width, capabilities.maxImageExtent.width );
			height = std::min( height, capabilities.maxImageExtent.height );

			for ( auto & displayMode : displayModeList )
			{
				if ( displayMode.Width == width
					&& displayMode.Height == height )
				{
					matchingDisplayModes.push_back( displayMode );
					auto it = descs.emplace( getVkFormat( displayMode.Format ), std::vector< DXGI_MODE_DESC >{} ).first;
					it->second.emplace_back( displayMode );
				}
			}

			if ( matchingDisplayModes.empty() )
			{
				// No display mode match exactly the requested size.
				// We then retrieve the first display mode size greater than requested
				VkExtent2D size{};
				for ( auto & displayMode : displayModeList )
				{
					if ( displayMode.Width >= width
						&& displayMode.Height >= height )
					{
						size.width = displayMode.Width;
						size.height = displayMode.Height;
						break;
					}
				}

				if ( size.width && size.height )
				{
					for ( auto & displayMode : displayModeList )
					{
						if ( displayMode.Width == size.width
							&& displayMode.Height == size.height )
						{
							matchingDisplayModes.push_back( displayMode );
							auto it = descs.emplace( getVkFormat( displayMode.Format ), std::vector< DXGI_MODE_DESC >{} ).first;
							it->second.emplace_back( displayMode );
						}
					}
				}
			}

			if ( matchingDisplayModes.empty() )
			{
				// Requested size is really too large.
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
					auto it = descs.emplace( getVkFormat( maxWidth.Format ), std::vector< DXGI_MODE_DESC >{} ).first;
					it->second.emplace_back( maxWidth );
				}
				else
				{
					// Choose the display mode with highest height.
					matchingDisplayModes.push_back( maxHeight );
					auto it = descs.emplace( getVkFormat( maxHeight.Format ), std::vector< DXGI_MODE_DESC >{} ).first;
					it->second.emplace_back( maxHeight );
				}
			}

			if ( !matchingDisplayModes.empty() )
			{
				std::sort( matchingDisplayModes.begin()
					, matchingDisplayModes.end()
					, []( DXGI_MODE_DESC const & lhs, DXGI_MODE_DESC const & rhs )
					{
						return lhs.Width < rhs.Width
							|| ( lhs.Width == rhs.Width
								&& lhs.Height < rhs.Height );
					} );

				capabilities.currentExtent.width = width;
				capabilities.currentExtent.height = height;
			}

			for ( auto & desc : descs )
			{
				matchingDescs.emplace( desc.first, desc.second.front() );
			}
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

	std::vector< DXGI_MODE_DESC > getDisplayModesList( VkInstance instance
		, IDXGIOutput * adapterOutput )
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
					get( instance )->onReportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
						, VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT
						, uint64_t( instance )
						, 0u
						, VK_ERROR_INCOMPATIBLE_DRIVER
						, "Direct3D11"
						, "GetDisplayModeList(numModes) failed" );
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
						get( instance )->onReportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
							, VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT
							, uint64_t( instance )
							, 0u
							, VK_ERROR_INCOMPATIBLE_DRIVER
							, "Direct3D11"
							, "GetDisplayModeList(displayModeList) failed" );
					}

					result.insert( result.end()
						, displayModeList.begin()
						, displayModeList.end() );
				}
			}
		}

		return result;
	}

	SurfaceKHR::SurfaceKHR( VkInstance instance
		, VkWin32SurfaceCreateInfoKHR createInfo )
		: m_instance{ instance }
		, m_win32CreateInfo{ std::move( createInfo ) }
		, m_type{ VK_KHR_WIN32_SURFACE_EXTENSION_NAME }
	{
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
	}

	SurfaceKHR::SurfaceKHR( VkInstance instance
		, VkDisplaySurfaceCreateInfoKHR createInfo )
		: m_instance{ instance }
		, m_displayCreateInfo{ std::move( createInfo ) }
		, m_type{ VK_KHR_WIN32_SURFACE_EXTENSION_NAME }
	{
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
	}

	SurfaceKHR::~SurfaceKHR()
	{
	}

	inline VkFlags & mergeFlags( VkFlags & value
		, VkFlags const & flag )noexcept
	{
		value &= flag;
		return value;
	}

	void SurfaceKHR::doUpdate( VkPhysicalDevice physicalDevice )const
	{
		if ( m_currentPhysicalDevice != physicalDevice )
		{
			m_currentPhysicalDevice = physicalDevice;
			m_displayModes = getDisplayModesList( m_instance, get( physicalDevice )->getOutput() );
			m_surfaceFormats = getSurfaceFormats( m_displayModes );

			if ( isWin32() )
			{
				auto hWnd = m_win32CreateInfo.hwnd;
				RECT rect{};
				::GetWindowRect( hWnd, &rect );
				updateSurfaceCapabilities( m_displayModes
					, rect
					, m_surfaceCapabilities
					, m_descs
					, m_matchingDescs );
			}
			else if ( isDisplay() )
			{
				RECT rect{};
				rect.right = LONG( m_displayCreateInfo.imageExtent.width );
				rect.bottom = LONG( m_displayCreateInfo.imageExtent.height );
				updateSurfaceCapabilities( m_displayModes
					, rect
					, m_surfaceCapabilities
					, m_descs
					, m_matchingDescs );
			}

			m_surfaceCapabilities.supportedUsageFlags = VK_IMAGE_TYPE_MAX_ENUM;

			for ( auto surfaceFormat : m_surfaceFormats )
			{
				auto props = get( m_currentPhysicalDevice )->getFormatProperties( surfaceFormat.format );
				mergeFlags( m_surfaceCapabilities.supportedUsageFlags
					, getUsageFlags( props.linearTilingFeatures ) );
			}
		}
	}

	bool SurfaceKHR::getSupport( uint32_t queueFamilyIndex )const
	{
		return true;
	}

	HWND SurfaceKHR::getHwnd()const
	{
		if ( isWin32() )
		{
			return m_win32CreateInfo.hwnd;
		}

		if ( isDisplay() )
		{
			return GetActiveWindow();
		}

		return nullptr;
	}

	DXGI_MODE_DESC const & SurfaceKHR::getDisplayMode()const
	{
		assert( isDisplay() );
		return get( m_displayCreateInfo.displayMode )->getDesc();
	}
}
