#include "PlatformUtils/SurfaceInfos.hpp"

#if ASHES_WIN32

#include <Ashes/Core/PlatformWindowHandle.hpp>

#include <Windows.h>
#include <gl/GL.h>

namespace utils
{
	namespace
	{
		ashes::Format getFormat( PIXELFORMATDESCRIPTOR const & pf )
		{
			ashes::Format result = ashes::Format::eUndefined;

			if ( !pf.iPixelType )
			{
				if ( pf.cRedBits == 8 )
				{
					if ( pf.cGreenBits == 8 )
					{
						if ( pf.cBlueBits == 8 )
						{
							if ( pf.cAlphaBits == 8 )
							{
								if ( pf.cRedShift > pf.cBlueShift )
								{
									result = ashes::Format::eR8G8B8A8_UNORM;
								}
								else
								{
									result = ashes::Format::eB8G8R8A8_UNORM;
								}
							}
							else if ( !pf.cAlphaBits )
							{
								if ( pf.cRedShift > pf.cBlueShift )
								{
									result = ashes::Format::eR8G8B8_UNORM;
								}
								else
								{
									result = ashes::Format::eB8G8R8_UNORM;
								}
							}
						}
						else if ( !pf.cAlphaBits )
						{
							result = ashes::Format::eR8G8_UNORM;
						}
					}
					else if ( !pf.cAlphaBits )
					{
						result = ashes::Format::eR8_UNORM;
					}
				}
			}

			return result;
		}

		void updateSurfaceInfos( PIXELFORMATDESCRIPTOR const & pf
			, std::vector< ashes::SurfaceFormat > & formats
			, ashes::SurfaceCapabilities & capabilities )
		{
			auto format = getFormat( pf );

			if ( format != ashes::Format::eUndefined )
			{
				if ( formats.end() == std::find_if( formats.begin()
					, formats.end()
					, [&format]( ashes::SurfaceFormat const & lookup )
					{
						return lookup.format == format;
					} ) )
				{
					formats.push_back( { format, ashes::ColorSpace::eSRGBNonLinear } );
				}
			}
		}
	}

	void getSurfaceInfos( ashes::WindowHandle const & handle
		, std::string & type
		, std::vector< ashes::SurfaceFormat > & formats
		, ashes::SurfaceCapabilities & capabilities )
	{
		type = "VK_KHR_win32_surface";
		capabilities.minImageCount = 1u;
		capabilities.maxImageCount = 1u;
		capabilities.currentExtent.width = ~( 0u );
		capabilities.currentExtent.height = ~( 0u );
		capabilities.maxImageArrayLayers = 1u;
		capabilities.minImageExtent = capabilities.currentExtent;
		capabilities.maxImageExtent = capabilities.currentExtent;
		capabilities.supportedUsageFlags = ashes::ImageUsageFlag::eUndefined;
		capabilities.supportedTransforms = ashes::SurfaceTransformFlag::eIdentity;
		capabilities.currentTransform = ashes::SurfaceTransformFlag::eIdentity;
		capabilities.supportedCompositeAlpha = ashes::CompositeAlphaFlag::eInherit;

		auto hwnd = handle.getInternal< ashes::IMswWindowHandle >().getHwnd();
		auto hdc = ::GetDC( hwnd );
		int max = ::DescribePixelFormat( hdc, 1, sizeof( PIXELFORMATDESCRIPTOR ), nullptr );

		if ( max )
		{
			for ( int index = 1; index <= max; ++index )
			{
				PIXELFORMATDESCRIPTOR pf{};

				if ( ::DescribePixelFormat( hdc, index, sizeof( PIXELFORMATDESCRIPTOR ), &pf ) )
				{
					updateSurfaceInfos( pf, formats, capabilities );
				}
			}
		}

		::ReleaseDC( hwnd, hdc );
	}
}

#endif
