#include "RendererPrerequisites.hpp"

namespace renderer
{
	VkFormat convert( PixelFormat format )
	{
		switch ( format )
		{
		case PixelFormat::eL8:
			return VK_FORMAT_R8_UINT;

		case PixelFormat::eL8A8:
			return VK_FORMAT_R8G8_UINT;

		case PixelFormat::eR8G8B8:
			return VK_FORMAT_R8G8B8_UINT;

		case PixelFormat::eRGB565:
			return VK_FORMAT_R5G6B5_UNORM_PACK16;

		case PixelFormat::eR8G8B8A8:
			return VK_FORMAT_R8G8B8A8_UNORM;

		case PixelFormat::eRGBA5551:
			return VK_FORMAT_R5G5B5A1_UNORM_PACK16;

		case PixelFormat::eRGBA4444:
			return VK_FORMAT_R4G4B4A4_UNORM_PACK16;

		case PixelFormat::eD16:
			return VK_FORMAT_D16_UNORM;

		case PixelFormat::eS8:
			return VK_FORMAT_S8_UINT;
		}

		assert( false && "Unsupported pixel format." );
		return VK_FORMAT_UNDEFINED;
	}
}
