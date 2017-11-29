#include "PixelFormat.hpp"

namespace renderer
{
	VkFormat convert( utils::PixelFormat const & format )noexcept
	{
		switch ( format )
		{
		case utils::PixelFormat::eL8:
			return VK_FORMAT_R8_UINT;

		case utils::PixelFormat::eL8A8:
			return VK_FORMAT_R8G8_UINT;

		case utils::PixelFormat::eR8G8B8:
			return VK_FORMAT_R8G8B8_UINT;

		case utils::PixelFormat::eRGB565:
			return VK_FORMAT_R5G6B5_UNORM_PACK16;

		case utils::PixelFormat::eR8G8B8A8:
			return VK_FORMAT_R8G8B8A8_UNORM;

		case utils::PixelFormat::eB8G8R8A8:
			return VK_FORMAT_B8G8R8A8_UNORM;

		case utils::PixelFormat::eRGBA5551:
			return VK_FORMAT_R5G5B5A1_UNORM_PACK16;

		case utils::PixelFormat::eRGBA4444:
			return VK_FORMAT_R4G4B4A4_UNORM_PACK16;

		case utils::PixelFormat::eD16:
			return VK_FORMAT_D16_UNORM;

		case utils::PixelFormat::eD24S8:
			return VK_FORMAT_D24_UNORM_S8_UINT;

		case utils::PixelFormat::eS8:
			return VK_FORMAT_S8_UINT;

		default:
			assert( false && "Unsupported pixel format." );
			return VK_FORMAT_UNDEFINED;
		}
	}

	utils::PixelFormat convert( VkFormat const & format )noexcept
	{
		switch ( format )
		{
		case VK_FORMAT_R8_UINT:
			return utils::PixelFormat::eL8;

		case VK_FORMAT_R8G8_UINT:
			return utils::PixelFormat::eL8A8;

		case VK_FORMAT_R8G8B8_UINT:
			return utils::PixelFormat::eR8G8B8;

		case VK_FORMAT_R5G6B5_UNORM_PACK16:
			return utils::PixelFormat::eRGB565;

		case VK_FORMAT_R8G8B8A8_UNORM:
			return utils::PixelFormat::eR8G8B8A8;

		case VK_FORMAT_B8G8R8A8_UNORM:
			return utils::PixelFormat::eB8G8R8A8;

		case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
			return utils::PixelFormat::eRGBA5551;

		case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
			return utils::PixelFormat::eRGBA4444;

		case VK_FORMAT_D16_UNORM:
			return utils::PixelFormat::eD16;

		case VK_FORMAT_D24_UNORM_S8_UINT:
			return utils::PixelFormat::eD24S8;

		case VK_FORMAT_S8_UINT:
			return utils::PixelFormat::eS8;

		default:
			assert( false && "Unsupported pixel format." );
			return utils::PixelFormat::eR8G8B8A8;
		}
	}
}
