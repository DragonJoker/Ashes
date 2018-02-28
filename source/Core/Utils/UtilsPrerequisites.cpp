/*
This file belongs to RendererLib.
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
		case PixelFormat::eR8:
		case PixelFormat::eS8:
			return 1u;

		case PixelFormat::eR8G8:
		case PixelFormat::eD16:
			return 2u;

		case PixelFormat::eR32F:
		case PixelFormat::eD24S8:
		case PixelFormat::eD32F:
		case PixelFormat::eD32FS8:
		case PixelFormat::eR8G8B8A8:
		case PixelFormat::eB8G8R8A8:
			return 4u;

		case PixelFormat::eR8G8B8:
			return 3u;

		case PixelFormat::eRG32F:
			return 8u;

		case PixelFormat::eRGB32F:
			return 12u;

		case PixelFormat::eRGBA16F:
			return 8u;

		case PixelFormat::eRGBA32F:
			return 16u;

		default:
			assert( false && "Unsupported pixel format" );
			return 0u;
		}
	}
}
