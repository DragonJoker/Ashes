#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkFormat convert( renderer::PixelFormat const & format )noexcept
	{
		switch ( format )
		{
		case renderer::PixelFormat::eR8:
			return VK_FORMAT_R8_UNORM;

		case renderer::PixelFormat::eR32F:
			return VK_FORMAT_R32_SFLOAT;

		case renderer::PixelFormat::eR8G8:
			return VK_FORMAT_R8G8_UNORM;

		case renderer::PixelFormat::eRG32F:
			return VK_FORMAT_R32G32_SFLOAT;

		case renderer::PixelFormat::eR8G8B8:
			return VK_FORMAT_R8G8B8_UNORM;

		case renderer::PixelFormat::eRGB32F:
			return VK_FORMAT_R32G32B32_SFLOAT;

		case renderer::PixelFormat::eR8G8B8A8:
			return VK_FORMAT_R8G8B8A8_UNORM;

		case renderer::PixelFormat::eB8G8R8A8:
			return VK_FORMAT_B8G8R8A8_UNORM;

		case renderer::PixelFormat::eRGBA32F:
			return VK_FORMAT_R32G32B32A32_SFLOAT;

		case renderer::PixelFormat::eD16:
			return VK_FORMAT_D16_UNORM;

		case renderer::PixelFormat::eD24S8:
			return VK_FORMAT_D24_UNORM_S8_UINT;

		case renderer::PixelFormat::eD32F:
			return VK_FORMAT_D32_SFLOAT;

		case renderer::PixelFormat::eD32FS8:
			return VK_FORMAT_D32_SFLOAT_S8_UINT;

		case renderer::PixelFormat::eS8:
			return VK_FORMAT_S8_UINT;

		case renderer::PixelFormat::eBC1_RGB:
			return VK_FORMAT_BC1_RGB_UNORM_BLOCK;

		case renderer::PixelFormat::eBC1_RGBA:
			return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;

		case renderer::PixelFormat::eBC1_SRGB:
			return VK_FORMAT_BC1_RGB_SRGB_BLOCK;

		case renderer::PixelFormat::eBC1_SRGBA:
			return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;

		case renderer::PixelFormat::eBC2_RGBA:
			return VK_FORMAT_BC2_UNORM_BLOCK;

		case renderer::PixelFormat::eBC2_SRGBA:
			return VK_FORMAT_BC2_SRGB_BLOCK;

		case renderer::PixelFormat::eBC3_RGBA:
			return VK_FORMAT_BC3_UNORM_BLOCK;

		case renderer::PixelFormat::eBC3_SRGBA:
			return VK_FORMAT_BC3_SRGB_BLOCK;

		case renderer::PixelFormat::eBC4:
			return VK_FORMAT_BC4_UNORM_BLOCK;

		case renderer::PixelFormat::eBC5_RG:
			return VK_FORMAT_BC5_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_4x4_RGBA:
			return VK_FORMAT_ASTC_4x4_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_4x4_SRGBA:
			return VK_FORMAT_ASTC_4x4_SRGB_BLOCK;

		case renderer::PixelFormat::eASTC_5x4_RGBA:
			return VK_FORMAT_ASTC_5x4_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_5x4_SRGBA:
			return VK_FORMAT_ASTC_5x4_SRGB_BLOCK;

		case renderer::PixelFormat::eASTC_5x5_RGBA:
			return VK_FORMAT_ASTC_5x5_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_5x5_SRGBA:
			return VK_FORMAT_ASTC_5x5_SRGB_BLOCK;

		case renderer::PixelFormat::eASTC_6x5_RGBA:
			return VK_FORMAT_ASTC_6x5_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_6x5_SRGBA:
			return VK_FORMAT_ASTC_6x5_SRGB_BLOCK;

		case renderer::PixelFormat::eASTC_6x6_RGBA:
			return VK_FORMAT_ASTC_6x6_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_6x6_SRGBA:
			return VK_FORMAT_ASTC_6x6_SRGB_BLOCK;

		case renderer::PixelFormat::eASTC_8x5_RGBA:
			return VK_FORMAT_ASTC_8x5_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_8x5_SRGBA:
			return VK_FORMAT_ASTC_8x5_SRGB_BLOCK;

		case renderer::PixelFormat::eASTC_8x6_RGBA:
			return VK_FORMAT_ASTC_8x6_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_8x6_SRGBA:
			return VK_FORMAT_ASTC_8x6_SRGB_BLOCK;

		case renderer::PixelFormat::eASTC_8x8_RGBA:
			return VK_FORMAT_ASTC_8x8_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_8x8_SRGBA:
			return VK_FORMAT_ASTC_8x8_SRGB_BLOCK;

		case renderer::PixelFormat::eASTC_10x5_RGBA:
			return VK_FORMAT_ASTC_10x5_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_10x5_SRGBA:
			return VK_FORMAT_ASTC_10x5_SRGB_BLOCK;

		case renderer::PixelFormat::eASTC_10x6_RGBA:
			return VK_FORMAT_ASTC_10x6_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_10x6_SRGBA:
			return VK_FORMAT_ASTC_10x6_SRGB_BLOCK;

		case renderer::PixelFormat::eASTC_10x8_RGBA:
			return VK_FORMAT_ASTC_10x8_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_10x8_SRGBA:
			return VK_FORMAT_ASTC_10x8_SRGB_BLOCK;

		case renderer::PixelFormat::eASTC_10x10_RGBA:
			return VK_FORMAT_ASTC_10x10_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_10x10_SRGBA:
			return VK_FORMAT_ASTC_10x10_SRGB_BLOCK;

		case renderer::PixelFormat::eASTC_12x10_RGBA:
			return VK_FORMAT_ASTC_12x10_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_12x10_SRGBA:
			return VK_FORMAT_ASTC_12x10_SRGB_BLOCK;

		case renderer::PixelFormat::eASTC_12x12_RGBA:
			return VK_FORMAT_ASTC_12x12_UNORM_BLOCK;

		case renderer::PixelFormat::eASTC_12x12_SRGBA:
			return VK_FORMAT_ASTC_12x12_SRGB_BLOCK;

		case renderer::PixelFormat::eETC2_R8G8B8:
			return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;

		case renderer::PixelFormat::eETC2_R8G8B8_SRGB:
			return VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;

		case renderer::PixelFormat::eETC2_R8G8B8A1:
			return VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;

		case renderer::PixelFormat::eETC2_R8G8B8A1_SRGB:
			return VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;

		case renderer::PixelFormat::eETC2_R8G8B8A8:
			return VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;

		case renderer::PixelFormat::eETC2_R8G8B8A8_SRGB:
			return VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;

		case renderer::PixelFormat::eEAC_R11U:
			return VK_FORMAT_EAC_R11_UNORM_BLOCK;

		case renderer::PixelFormat::eEAC_R11S:
			return VK_FORMAT_EAC_R11_SNORM_BLOCK;

		case renderer::PixelFormat::eEAC_R11G11U:
			return VK_FORMAT_EAC_R11G11_UNORM_BLOCK;

		case renderer::PixelFormat::eEAC_R11G11S:
			return VK_FORMAT_EAC_R11G11_SNORM_BLOCK;

		default:
			assert( false && "Unsupported pixel format." );
			return VK_FORMAT_UNDEFINED;
		}
	}

	renderer::PixelFormat convert( VkFormat const & format )noexcept
	{
		switch ( format )
		{
		case VK_FORMAT_R8_UINT:
			return renderer::PixelFormat::eR8;

		case VK_FORMAT_R32_SFLOAT:
			return renderer::PixelFormat::eR32F;

		case VK_FORMAT_R8G8_UINT:
			return renderer::PixelFormat::eR8G8;

		case VK_FORMAT_R8G8B8_UINT:
			return renderer::PixelFormat::eR8G8B8;

		case VK_FORMAT_R8G8B8A8_UNORM:
			return renderer::PixelFormat::eR8G8B8A8;

		case VK_FORMAT_B8G8R8A8_UNORM:
			return renderer::PixelFormat::eB8G8R8A8;

		case VK_FORMAT_R32G32B32A32_SFLOAT:
			return renderer::PixelFormat::eRGBA32F;

		case VK_FORMAT_D16_UNORM:
			return renderer::PixelFormat::eD16;

		case VK_FORMAT_D24_UNORM_S8_UINT:
			return renderer::PixelFormat::eD24S8;

		case VK_FORMAT_D32_SFLOAT:
			return renderer::PixelFormat::eD32F;

		case VK_FORMAT_D32_SFLOAT_S8_UINT:
			return renderer::PixelFormat::eD32FS8;

		case VK_FORMAT_S8_UINT:
			return renderer::PixelFormat::eS8;

		case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
			return renderer::PixelFormat::eBC1_RGB;

		case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
			return renderer::PixelFormat::eBC1_RGBA;

		case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
			return renderer::PixelFormat::eBC1_SRGB;

		case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
			return renderer::PixelFormat::eBC1_SRGBA;

		case VK_FORMAT_BC2_UNORM_BLOCK:
			return renderer::PixelFormat::eBC2_RGBA;

		case VK_FORMAT_BC2_SRGB_BLOCK:
			return renderer::PixelFormat::eBC2_SRGBA;

		case VK_FORMAT_BC3_UNORM_BLOCK:
			return renderer::PixelFormat::eBC3_RGBA;

		case VK_FORMAT_BC3_SRGB_BLOCK:
			return renderer::PixelFormat::eBC3_SRGBA;

		case VK_FORMAT_BC4_UNORM_BLOCK:
			return renderer::PixelFormat::eBC4;

		case VK_FORMAT_BC5_UNORM_BLOCK:
			return renderer::PixelFormat::eBC5_RG;

		case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
			return renderer::PixelFormat::eASTC_4x4_RGBA;

		case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
			return renderer::PixelFormat::eASTC_4x4_SRGBA;

		case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
			return renderer::PixelFormat::eASTC_5x4_RGBA;

		case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
			return renderer::PixelFormat::eASTC_5x4_SRGBA;

		case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
			return renderer::PixelFormat::eASTC_5x5_RGBA;

		case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
			return renderer::PixelFormat::eASTC_5x5_SRGBA;

		case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
			return renderer::PixelFormat::eASTC_6x5_RGBA;

		case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
			return renderer::PixelFormat::eASTC_6x5_SRGBA;

		case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
			return renderer::PixelFormat::eASTC_8x5_RGBA;

		case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
			return renderer::PixelFormat::eASTC_8x5_SRGBA;

		case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
			return renderer::PixelFormat::eASTC_8x6_RGBA;

		case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
			return renderer::PixelFormat::eASTC_8x6_SRGBA;

		case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
			return renderer::PixelFormat::eASTC_8x8_RGBA;

		case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
			return renderer::PixelFormat::eASTC_8x8_SRGBA;

		case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
			return renderer::PixelFormat::eASTC_10x5_RGBA;

		case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
			return renderer::PixelFormat::eASTC_10x5_SRGBA;

		case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
			return renderer::PixelFormat::eASTC_10x6_RGBA;

		case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
			return renderer::PixelFormat::eASTC_10x6_SRGBA;

		case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
			return renderer::PixelFormat::eASTC_10x8_RGBA;

		case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
			return renderer::PixelFormat::eASTC_10x8_SRGBA;

		case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
			return renderer::PixelFormat::eASTC_10x10_RGBA;

		case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
			return renderer::PixelFormat::eASTC_10x10_SRGBA;

		case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
			return renderer::PixelFormat::eASTC_12x10_RGBA;

		case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
			return renderer::PixelFormat::eASTC_12x10_SRGBA;

		case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
			return renderer::PixelFormat::eASTC_12x12_RGBA;

		case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
			return renderer::PixelFormat::eASTC_12x12_SRGBA;

		case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
			return renderer::PixelFormat::eETC2_R8G8B8;

		case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
			return renderer::PixelFormat::eETC2_R8G8B8_SRGB;

		case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
			return renderer::PixelFormat::eETC2_R8G8B8A1;

		case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
			return renderer::PixelFormat::eETC2_R8G8B8A1_SRGB;

		case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
			return renderer::PixelFormat::eETC2_R8G8B8A8;

		case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
			return renderer::PixelFormat::eETC2_R8G8B8A8_SRGB;

		case VK_FORMAT_EAC_R11_UNORM_BLOCK:
			return renderer::PixelFormat::eEAC_R11U;

		case VK_FORMAT_EAC_R11_SNORM_BLOCK:
			return renderer::PixelFormat::eEAC_R11S;

		case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
			return renderer::PixelFormat::eEAC_R11G11U;

		case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
			return renderer::PixelFormat::eEAC_R11G11S;

		default:
			assert( false && "Unsupported pixel format." );
			return renderer::PixelFormat::eR8G8B8A8;
		}
	}
}
