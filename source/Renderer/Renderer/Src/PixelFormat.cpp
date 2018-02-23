#include "RendererPrerequisites.hpp"

namespace renderer
{
	bool isDepthStencilFormat( PixelFormat format )noexcept
	{
		return format == PixelFormat::eD24S8
			|| format == PixelFormat::eD32FS8;
	}

	bool isStencilFormat( PixelFormat format )noexcept
	{
		return format == PixelFormat::eS8;
	}

	bool isDepthFormat( PixelFormat format )noexcept
	{
		return format == PixelFormat::eD16
			|| format == PixelFormat::eD32F;
	}

	bool isCompressedFormat( PixelFormat format )noexcept
	{
		return format > PixelFormat::eS8;
	}

	ImageAspectFlags getAspectMask( PixelFormat format )noexcept
	{
		return isDepthStencilFormat( format )
			? ImageAspectFlags( ImageAspectFlag::eDepth | ImageAspectFlag::eStencil )
			: isDepthFormat( format )
				? ImageAspectFlags( ImageAspectFlag::eDepth )
				: isStencilFormat( format )
					? ImageAspectFlags( ImageAspectFlag::eStencil )
					: ImageAspectFlags( ImageAspectFlag::eColour );
	}

	std::string getName( PixelFormat value )
	{
		switch ( value )
		{
		case PixelFormat::eR8:
			return "R8";
		case PixelFormat::eR32F:
			return "R32F";
		case PixelFormat::eR8G8:
			return "R8G8";
		case PixelFormat::eRG32F:
			return "RG32F";
		case PixelFormat::eR8G8B8:
			return "R8G8B8";
		case PixelFormat::eRGB32F:
			return "RGB32F";
		case PixelFormat::eR8G8B8A8:
			return "R8G8B8A8";
		case PixelFormat::eB8G8R8A8:
			return "B8G8R8A8";
		case PixelFormat::eRGBA32F:
			return "RGBA32F";
		case PixelFormat::eD16:
			return "D16";
		case PixelFormat::eD24S8:
			return "D24S8";
		case PixelFormat::eD32F:
			return "D32F";
		case PixelFormat::eD32FS8:
			return "D32FS8";
		case PixelFormat::eS8:
			return "S8";
		case PixelFormat::eBC1_RGB:
			return "BC1_RGB";
		case PixelFormat::eBC1_RGBA:
			return "BC1_RGBA";
		case PixelFormat::eBC1_SRGB:
			return "BC1_SRGB";
		case PixelFormat::eBC1_SRGBA:
			return "BC1_SRGBA";
		case PixelFormat::eBC2_RGBA:
			return "BC2_RGBA";
		case PixelFormat::eBC2_SRGBA:
			return "BC2_SRGBA";
		case PixelFormat::eBC3_RGBA:
			return "BC3_RGBA";
		case PixelFormat::eBC3_SRGBA:
			return "BC3_SRGBA";
		case PixelFormat::eBC4:
			return "BC4";
		case PixelFormat::eBC5_RG:
			return "BC5_RG";
		case PixelFormat::eASTC_4x4_RGBA:
			return "ASTC_4x4_RGBA";
		case PixelFormat::eASTC_4x4_SRGBA:
			return "ASTC_4x4_SRGBA";
		case PixelFormat::eASTC_5x4_RGBA:
			return "ASTC_5x4_RGBA";
		case PixelFormat::eASTC_5x4_SRGBA:
			return "ASTC_5x4_SRGBA";
		case PixelFormat::eASTC_5x5_RGBA:
			return "ASTC_5x5_RGBA";
		case PixelFormat::eASTC_5x5_SRGBA:
			return "ASTC_5x5_SRGBA";
		case PixelFormat::eASTC_6x5_RGBA:
			return "ASTC_6x5_RGBA";
		case PixelFormat::eASTC_6x5_SRGBA:
			return "ASTC_6x5_SRGBA";
		case PixelFormat::eASTC_6x6_RGBA:
			return "ASTC_6x6_RGBA";
		case PixelFormat::eASTC_6x6_SRGBA:
			return "ASTC_6x6_SRGBA";
		case PixelFormat::eASTC_8x5_RGBA:
			return "ASTC_8x5_RGBA";
		case PixelFormat::eASTC_8x5_SRGBA:
			return "ASTC_8x5_SRGBA";
		case PixelFormat::eASTC_8x6_RGBA:
			return "ASTC_8x6_RGBA";
		case PixelFormat::eASTC_8x6_SRGBA:
			return "ASTC_8x6_SRGBA";
		case PixelFormat::eASTC_8x8_RGBA:
			return "ASTC_8x8_RGBA";
		case PixelFormat::eASTC_8x8_SRGBA:
			return "ASTC_8x8_SRGBA";
		case PixelFormat::eASTC_10x5_RGBA:
			return "ASTC_10x5_RGBA";
		case PixelFormat::eASTC_10x5_SRGBA:
			return "ASTC_10x5_SRGBA";
		case PixelFormat::eASTC_10x6_RGBA:
			return "ASTC_10x6_RGBA";
		case PixelFormat::eASTC_10x6_SRGBA:
			return "ASTC_10x6_SRGBA";
		case PixelFormat::eASTC_10x8_RGBA:
			return "ASTC_10x8_RGBA";
		case PixelFormat::eASTC_10x8_SRGBA:
			return "ASTC_10x8_SRGBA";
		case PixelFormat::eASTC_10x10_RGBA:
			return "ASTC_10x10_RGBA";
		case PixelFormat::eASTC_10x10_SRGBA:
			return "ASTC_10x10_SRGBA";
		case PixelFormat::eASTC_12x10_RGBA:
			return "ASTC_12x10_RGBA";
		case PixelFormat::eASTC_12x10_SRGBA:
			return "ASTC_12x10_SRGBA";
		case PixelFormat::eASTC_12x12_RGBA:
			return "ASTC_12x12_RGBA";
		case PixelFormat::eASTC_12x12_SRGBA:
			return "ASTC_12x12_SRGBA";
		case PixelFormat::eETC2_R8G8B8:
			return "ETC2_R8G8B8";
		case PixelFormat::eETC2_R8G8B8_SRGB:
			return "ETC2_R8G8B8_SRGB";
		case PixelFormat::eETC2_R8G8B8A1:
			return "ETC2_R8G8B8A1";
		case PixelFormat::eETC2_R8G8B8A1_SRGB:
			return "ETC2_R8G8B8A1_SRGB";
		case PixelFormat::eETC2_R8G8B8A8:
			return "ETC2_R8G8B8A8";
		case PixelFormat::eETC2_R8G8B8A8_SRGB:
			return "ETC2_R8G8B8A8_SRGB";
		case PixelFormat::eEAC_R11U:
			return "EAC_R11U";
		case PixelFormat::eEAC_R11S:
			return "EAC_R11S";
		case PixelFormat::eEAC_R11G11U:
			return "EAC_R11G11U";
		case PixelFormat::eEAC_R11G11S:
			return "EAC_R11G11S";

		default:
			assert( false && "Unsupported PixelFormat." );
			throw std::runtime_error{ "Unsupported PixelFormat" };
		}

		return std::string{};
	}
}
