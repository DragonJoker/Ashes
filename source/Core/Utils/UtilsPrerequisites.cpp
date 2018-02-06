/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#include "PixelFormat.hpp"

#include <cassert>

namespace utils
{
	uint32_t pixelSize( PixelFormat format )noexcept
	{
		switch ( format )
		{
		case PixelFormat::eL8:
		case PixelFormat::eS8:
			return 1u;

		case PixelFormat::eL8A8:
		case PixelFormat::eRGB565:
		case PixelFormat::eRGBA5551:
		case PixelFormat::eRGBA4444:
		case PixelFormat::eD16:
			return 2u;

		case PixelFormat::eRGBA32F:
			return 128u;

		case PixelFormat::eD24S8:
		case PixelFormat::eD32F:
			return 4u;

		case PixelFormat::eR8G8B8:
			return 3u;

		case PixelFormat::eR8G8B8A8:
			return 4u;

		default:
			assert( false && "Unsupported pixel format" );
			return 0u;
		}
	}
}
