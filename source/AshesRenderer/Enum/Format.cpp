#include "AshesRenderer/AshesRendererPrerequisites.hpp"

#include "AshesRenderer/Miscellaneous/Extent3D.hpp"

namespace ashes
{
	uint32_t getSize( Format format )noexcept
	{
		assert( !isCompressedFormat( format ) );

		switch ( format )
		{
		case Format::eR4G4_UNORM_PACK8:
		case Format::eR8_UNORM:
		case Format::eR8_SNORM:
		case Format::eR8_USCALED:
		case Format::eR8_SSCALED:
		case Format::eR8_UINT:
		case Format::eR8_SINT:
		case Format::eR8_SRGB:
		case Format::eS8_UINT:
			return 1u;

		case Format::eR4G4B4A4_UNORM_PACK16:
		case Format::eR5G6B5_UNORM_PACK16:
		case Format::eB5G6R5_UNORM_PACK16:
		case Format::eR5G5B5A1_UNORM_PACK16:
		case Format::eB5G5R5A1_UNORM_PACK16:
		case Format::eA1R5G5B5_UNORM_PACK16:
		case Format::eR8G8_UNORM:
		case Format::eR8G8_SNORM:
		case Format::eR8G8_USCALED:
		case Format::eR8G8_SSCALED:
		case Format::eR8G8_UINT:
		case Format::eR8G8_SINT:
		case Format::eR8G8_SRGB:
		case Format::eR16_UNORM:
		case Format::eR16_SNORM:
		case Format::eR16_USCALED:
		case Format::eR16_SSCALED:
		case Format::eR16_UINT:
		case Format::eR16_SINT:
		case Format::eR16_SFLOAT:
		case Format::eD16_UNORM:
			return 2u;

		case Format::eR8G8B8_UNORM:
		case Format::eR8G8B8_SNORM:
		case Format::eR8G8B8_USCALED:
		case Format::eR8G8B8_SSCALED:
		case Format::eR8G8B8_UINT:
		case Format::eR8G8B8_SINT:
		case Format::eR8G8B8_SRGB:
		case Format::eB8G8R8_UNORM:
		case Format::eB8G8R8_SNORM:
		case Format::eB8G8R8_USCALED:
		case Format::eB8G8R8_SSCALED:
		case Format::eB8G8R8_UINT:
		case Format::eB8G8R8_SINT:
		case Format::eB8G8R8_SRGB:
		case Format::eD16_UNORM_S8_UINT:
			return 3u;

		case Format::eR8G8B8A8_UNORM:
		case Format::eR8G8B8A8_SNORM:
		case Format::eR8G8B8A8_USCALED:
		case Format::eR8G8B8A8_SSCALED:
		case Format::eR8G8B8A8_UINT:
		case Format::eR8G8B8A8_SINT:
		case Format::eR8G8B8A8_SRGB:
		case Format::eB8G8R8A8_UNORM:
		case Format::eB8G8R8A8_SNORM:
		case Format::eB8G8R8A8_USCALED:
		case Format::eB8G8R8A8_SSCALED:
		case Format::eB8G8R8A8_UINT:
		case Format::eB8G8R8A8_SINT:
		case Format::eB8G8R8A8_SRGB:
		case Format::eA8B8G8R8_UNORM_PACK32:
		case Format::eA8B8G8R8_SNORM_PACK32:
		case Format::eA8B8G8R8_USCALED_PACK32:
		case Format::eA8B8G8R8_SSCALED_PACK32:
		case Format::eA8B8G8R8_UINT_PACK32:
		case Format::eA8B8G8R8_SINT_PACK32:
		case Format::eA8B8G8R8_SRGB_PACK32:
		case Format::eA2R10G10B10_UNORM_PACK32:
		case Format::eA2R10G10B10_SNORM_PACK32:
		case Format::eA2R10G10B10_USCALED_PACK32:
		case Format::eA2R10G10B10_SSCALED_PACK32:
		case Format::eA2R10G10B10_UINT_PACK32:
		case Format::eA2R10G10B10_SINT_PACK32:
		case Format::eA2B10G10R10_UNORM_PACK32:
		case Format::eA2B10G10R10_SNORM_PACK32:
		case Format::eA2B10G10R10_USCALED_PACK32:
		case Format::eA2B10G10R10_SSCALED_PACK32:
		case Format::eA2B10G10R10_UINT_PACK32:
		case Format::eA2B10G10R10_SINT_PACK32:
		case Format::eR16G16_UNORM:
		case Format::eR16G16_SNORM:
		case Format::eR16G16_USCALED:
		case Format::eR16G16_SSCALED:
		case Format::eR16G16_UINT:
		case Format::eR16G16_SINT:
		case Format::eR16G16_SFLOAT:
		case Format::eR32_UINT:
		case Format::eR32_SINT:
		case Format::eR32_SFLOAT:
		case Format::eB10G11R11_UFLOAT_PACK32:
		case Format::eE5B9G9R9_UFLOAT_PACK32:
		case Format::eX8_D24_UNORM_PACK32:
		case Format::eD32_SFLOAT:
		case Format::eD24_UNORM_S8_UINT:
		case Format::eD32_SFLOAT_S8_UINT:
			return 4u;

		case Format::eR16G16B16_UNORM:
		case Format::eR16G16B16_SNORM:
		case Format::eR16G16B16_USCALED:
		case Format::eR16G16B16_SSCALED:
		case Format::eR16G16B16_UINT:
		case Format::eR16G16B16_SINT:
		case Format::eR16G16B16_SFLOAT:
			return 6u;

		case Format::eR16G16B16A16_UNORM:
		case Format::eR16G16B16A16_SNORM:
		case Format::eR16G16B16A16_USCALED:
		case Format::eR16G16B16A16_SSCALED:
		case Format::eR16G16B16A16_UINT:
		case Format::eR16G16B16A16_SINT:
		case Format::eR16G16B16A16_SFLOAT:
		case Format::eR32G32_UINT:
		case Format::eR32G32_SINT:
		case Format::eR32G32_SFLOAT:
		case Format::eR64_UINT:
		case Format::eR64_SINT:
		case Format::eR64_SFLOAT:
			return 8u;

		case Format::eR32G32B32_UINT:
		case Format::eR32G32B32_SINT:
		case Format::eR32G32B32_SFLOAT:
			return 12u;

		case Format::eR32G32B32A32_UINT:
		case Format::eR32G32B32A32_SINT:
		case Format::eR32G32B32A32_SFLOAT:
		case Format::eR64G64_UINT:
		case Format::eR64G64_SINT:
		case Format::eR64G64_SFLOAT:
			return 16u;

		case Format::eR64G64B64_UINT:
		case Format::eR64G64B64_SINT:
		case Format::eR64G64B64_SFLOAT:
			return 24u;

		case Format::eR64G64B64A64_UINT:
		case Format::eR64G64B64A64_SINT:
		case Format::eR64G64B64A64_SFLOAT:
			return 32u;

		default:
			assert( false && "Unsupported pixel format" );
			return 0u;
		}
	}

	uint32_t getBCCompressedSize( ashes::Format format )noexcept
	{
		assert( isBCCompressedFormat( format ) );

		switch ( format )
		{
		case ashes::Format::eBC1_RGBA_SRGB_BLOCK:
		case ashes::Format::eBC1_RGBA_UNORM_BLOCK:
		case ashes::Format::eBC1_RGB_SRGB_BLOCK:
		case ashes::Format::eBC1_RGB_UNORM_BLOCK:
			return 8u;

		case ashes::Format::eBC2_UNORM_BLOCK:
		case ashes::Format::eBC2_SRGB_BLOCK:
			return 16u;

		case ashes::Format::eBC3_UNORM_BLOCK:
		case ashes::Format::eBC3_SRGB_BLOCK:
			return 16u;

		case ashes::Format::eBC4_UNORM_BLOCK:
		case ashes::Format::eBC4_SNORM_BLOCK:
			return 8u;

		case ashes::Format::eBC5_UNORM_BLOCK:
		case ashes::Format::eBC5_SNORM_BLOCK:
			return 16u;

		case ashes::Format::eBC6H_UFLOAT_BLOCK:
		case ashes::Format::eBC6H_SFLOAT_BLOCK:
			return 16u;

		case ashes::Format::eBC7_UNORM_BLOCK:
		case ashes::Format::eBC7_SRGB_BLOCK:
			return 16u;

		default:
			assert( false && "Unsupported pixel format" );
			return 0u;
		}
	}

	uint32_t getETC2CompressedSize( ashes::Format format )noexcept
	{
		assert( isETC2CompressedFormat( format ) );

		switch ( format )
		{
		case ashes::Format::eETC2_R8G8B8_UNORM_BLOCK:
		case ashes::Format::eETC2_R8G8B8_SRGB_BLOCK:
			return 8u;

		case ashes::Format::eETC2_R8G8B8A1_UNORM_BLOCK:
		case ashes::Format::eETC2_R8G8B8A1_SRGB_BLOCK:
			return 8u;

		case ashes::Format::eETC2_R8G8B8A8_UNORM_BLOCK:
		case ashes::Format::eETC2_R8G8B8A8_SRGB_BLOCK:
			return 16u;

		default:
			assert( false && "Unsupported pixel format" );
			return 0u;
		}
	}

	uint32_t getEACCompressedSize( ashes::Format format )noexcept
	{
		assert( isEACCompressedFormat( format ) );

		switch ( format )
		{
		case ashes::Format::eEAC_R11_UNORM_BLOCK:
		case ashes::Format::eEAC_R11_SNORM_BLOCK:
			return 8u;

		case ashes::Format::eEAC_R11G11_UNORM_BLOCK:
		case ashes::Format::eEAC_R11G11_SNORM_BLOCK:
			return 16u;

		default:
			assert( false && "Unsupported pixel format" );
			return 0u;
		}
	}

	bool isDepthStencilFormat( Format format )noexcept
	{
		return format == Format::eD16_UNORM_S8_UINT
			|| format == Format::eD24_UNORM_S8_UINT
			|| format == Format::eD32_SFLOAT_S8_UINT;
	}

	bool isStencilFormat( Format format )noexcept
	{
		return format == Format::eS8_UINT;
	}

	bool isDepthFormat( Format format )noexcept
	{
		return format == Format::eD16_UNORM
			|| format == Format::eX8_D24_UNORM_PACK32
			|| format == Format::eD32_SFLOAT;
	}

	bool isBCCompressedFormat( Format format )noexcept
	{
		return format >= Format::eBCCompressed_BEGIN
			&& format <= Format::eBCCompressed_END;
	}

	bool isETC2CompressedFormat( Format format )noexcept
	{
		return format >= Format::eETC2Compressed_BEGIN
			&& format <= Format::eETC2Compressed_END;
	}

	bool isEACCompressedFormat( Format format )noexcept
	{
		return format >= Format::eEACCompressed_BEGIN
			&& format <= Format::eEACCompressed_END;
	}

	bool isASTCCompressedFormat( Format format )noexcept
	{
		return format >= Format::eASTCCompressed_BEGIN
			&& format <= Format::eASTCCompressed_END;
	}

	bool isCompressedFormat( Format format )noexcept
	{
		return format >= Format::eCompressed_BEGIN
			&& format <= Format::eCompressed_END;
	}

	bool isYCBCRFormat( Format format )noexcept
	{
		return format >= Format::eYCBCR_BEGIN
			&& format <= Format::eYCBCR_END;
	}

	bool isPVRTCFormat( Format format )noexcept
	{
		return format >= Format::ePVRTC_BEGIN
			&& format <= Format::ePVRTC_END;
	}

	std::string getName( Format value )
	{
		static std::map< ashes::Format, std::string > const names
		{
			{ ashes::Format::eUndefined, "UNDEFINED" },
			{ ashes::Format::eR4G4_UNORM_PACK8, "R4G4_UNORM_PACK8" },
			{ ashes::Format::eR4G4B4A4_UNORM_PACK16, "R4G4B4A4_UNORM_PACK16" },
			{ ashes::Format::eB4G4R4A4_UNORM_PACK16, "B4G4R4A4_UNORM_PACK16" },
			{ ashes::Format::eR5G6B5_UNORM_PACK16, "R5G6B5_UNORM_PACK16" },
			{ ashes::Format::eB5G6R5_UNORM_PACK16, "B5G6R5_UNORM_PACK16" },
			{ ashes::Format::eR5G5B5A1_UNORM_PACK16, "R5G5B5A1_UNORM_PACK16" },
			{ ashes::Format::eB5G5R5A1_UNORM_PACK16, "B5G5R5A1_UNORM_PACK16" },
			{ ashes::Format::eA1R5G5B5_UNORM_PACK16, "A1R5G5B5_UNORM_PACK16" },
			{ ashes::Format::eR8_UNORM, "R8_UNORM" },
			{ ashes::Format::eR8_SNORM, "R8_SNORM" },
			{ ashes::Format::eR8_USCALED, "R8_USCALED" },
			{ ashes::Format::eR8_SSCALED, "R8_SSCALED" },
			{ ashes::Format::eR8_UINT, "R8_UINT" },
			{ ashes::Format::eR8_SINT, "R8_SINT" },
			{ ashes::Format::eR8_SRGB, "R8_SRGB" },
			{ ashes::Format::eR8G8_UNORM, "R8G8_UNORM" },
			{ ashes::Format::eR8G8_SNORM, "R8G8_SNORM" },
			{ ashes::Format::eR8G8_USCALED, "R8G8_USCALED" },
			{ ashes::Format::eR8G8_SSCALED, "R8G8_SSCALED" },
			{ ashes::Format::eR8G8_UINT, "R8G8_UINT" },
			{ ashes::Format::eR8G8_SINT, "R8G8_SINT" },
			{ ashes::Format::eR8G8_SRGB, "R8G8_SRGB" },
			{ ashes::Format::eR8G8B8_UNORM, "R8G8B8_UNORM" },
			{ ashes::Format::eR8G8B8_SNORM, "R8G8B8_SNORM" },
			{ ashes::Format::eR8G8B8_USCALED, "R8G8B8_USCALED" },
			{ ashes::Format::eR8G8B8_SSCALED, "R8G8B8_SSCALED" },
			{ ashes::Format::eR8G8B8_UINT, "R8G8B8_UINT" },
			{ ashes::Format::eR8G8B8_SINT, "R8G8B8_SINT" },
			{ ashes::Format::eR8G8B8_SRGB, "R8G8B8_SRGB" },
			{ ashes::Format::eB8G8R8_UNORM, "B8G8R8_UNORM" },
			{ ashes::Format::eB8G8R8_SNORM, "B8G8R8_SNORM" },
			{ ashes::Format::eB8G8R8_USCALED, "B8G8R8_USCALED" },
			{ ashes::Format::eB8G8R8_SSCALED, "B8G8R8_SSCALED" },
			{ ashes::Format::eB8G8R8_UINT, "B8G8R8_UINT" },
			{ ashes::Format::eB8G8R8_SINT, "B8G8R8_SINT" },
			{ ashes::Format::eB8G8R8_SRGB, "B8G8R8_SRGB" },
			{ ashes::Format::eR8G8B8A8_UNORM, "R8G8B8A8_UNORM" },
			{ ashes::Format::eR8G8B8A8_SNORM, "R8G8B8A8_SNORM" },
			{ ashes::Format::eR8G8B8A8_USCALED, "R8G8B8A8_USCALED" },
			{ ashes::Format::eR8G8B8A8_SSCALED, "R8G8B8A8_SSCALED" },
			{ ashes::Format::eR8G8B8A8_UINT, "R8G8B8A8_UINT" },
			{ ashes::Format::eR8G8B8A8_SINT, "R8G8B8A8_SINT" },
			{ ashes::Format::eR8G8B8A8_SRGB, "R8G8B8A8_SRGB" },
			{ ashes::Format::eB8G8R8A8_UNORM, "B8G8R8A8_UNORM" },
			{ ashes::Format::eB8G8R8A8_SNORM, "B8G8R8A8_SNORM" },
			{ ashes::Format::eB8G8R8A8_USCALED, "B8G8R8A8_USCALED" },
			{ ashes::Format::eB8G8R8A8_SSCALED, "B8G8R8A8_SSCALED" },
			{ ashes::Format::eB8G8R8A8_UINT, "B8G8R8A8_UINT" },
			{ ashes::Format::eB8G8R8A8_SINT, "B8G8R8A8_SINT" },
			{ ashes::Format::eB8G8R8A8_SRGB, "B8G8R8A8_SRGB" },
			{ ashes::Format::eA8B8G8R8_UNORM_PACK32, "A8B8G8R8_UNORM_PACK32" },
			{ ashes::Format::eA8B8G8R8_SNORM_PACK32, "A8B8G8R8_SNORM_PACK32" },
			{ ashes::Format::eA8B8G8R8_USCALED_PACK32, "A8B8G8R8_USCALED_PACK32" },
			{ ashes::Format::eA8B8G8R8_SSCALED_PACK32, "A8B8G8R8_SSCALED_PACK32" },
			{ ashes::Format::eA8B8G8R8_UINT_PACK32, "A8B8G8R8_UINT_PACK32" },
			{ ashes::Format::eA8B8G8R8_SINT_PACK32, "A8B8G8R8_SINT_PACK32" },
			{ ashes::Format::eA8B8G8R8_SRGB_PACK32, "A8B8G8R8_SRGB_PACK32" },
			{ ashes::Format::eA2R10G10B10_UNORM_PACK32, "A2R10G10B10_UNORM_PACK32" },
			{ ashes::Format::eA2R10G10B10_SNORM_PACK32, "A2R10G10B10_SNORM_PACK32" },
			{ ashes::Format::eA2R10G10B10_USCALED_PACK32, "A2R10G10B10_USCALED_PACK32" },
			{ ashes::Format::eA2R10G10B10_SSCALED_PACK32, "A2R10G10B10_SSCALED_PACK32" },
			{ ashes::Format::eA2R10G10B10_UINT_PACK32, "A2R10G10B10_UINT_PACK32" },
			{ ashes::Format::eA2R10G10B10_SINT_PACK32, "A2R10G10B10_SINT_PACK32" },
			{ ashes::Format::eA2B10G10R10_UNORM_PACK32, "A2B10G10R10_UNORM_PACK32" },
			{ ashes::Format::eA2B10G10R10_SNORM_PACK32, "A2B10G10R10_SNORM_PACK32" },
			{ ashes::Format::eA2B10G10R10_USCALED_PACK32, "A2B10G10R10_USCALED_PACK32" },
			{ ashes::Format::eA2B10G10R10_SSCALED_PACK32, "A2B10G10R10_SSCALED_PACK32" },
			{ ashes::Format::eA2B10G10R10_UINT_PACK32, "A2B10G10R10_UINT_PACK32" },
			{ ashes::Format::eA2B10G10R10_SINT_PACK32, "A2B10G10R10_SINT_PACK32" },
			{ ashes::Format::eR16_UNORM, "R16_UNORM" },
			{ ashes::Format::eR16_SNORM, "R16_SNORM" },
			{ ashes::Format::eR16_USCALED, "R16_USCALED" },
			{ ashes::Format::eR16_SSCALED, "R16_SSCALED" },
			{ ashes::Format::eR16_UINT, "R16_UINT" },
			{ ashes::Format::eR16_SINT, "R16_SINT" },
			{ ashes::Format::eR16_SFLOAT, "R16_SFLOAT" },
			{ ashes::Format::eR16G16_UNORM, "R16G16_UNORM" },
			{ ashes::Format::eR16G16_SNORM, "R16G16_SNORM" },
			{ ashes::Format::eR16G16_USCALED, "R16G16_USCALED" },
			{ ashes::Format::eR16G16_SSCALED, "R16G16_SSCALED" },
			{ ashes::Format::eR16G16_UINT, "R16G16_UINT" },
			{ ashes::Format::eR16G16_SINT, "R16G16_SINT" },
			{ ashes::Format::eR16G16_SFLOAT, "R16G16_SFLOAT" },
			{ ashes::Format::eR16G16B16_UNORM, "R16G16B16_UNORM" },
			{ ashes::Format::eR16G16B16_SNORM, "R16G16B16_SNORM" },
			{ ashes::Format::eR16G16B16_USCALED, "R16G16B16_USCALED" },
			{ ashes::Format::eR16G16B16_SSCALED, "R16G16B16_SSCALED" },
			{ ashes::Format::eR16G16B16_UINT, "R16G16B16_UINT" },
			{ ashes::Format::eR16G16B16_SINT, "R16G16B16_SINT" },
			{ ashes::Format::eR16G16B16_SFLOAT, "R16G16B16_SFLOAT" },
			{ ashes::Format::eR16G16B16A16_UNORM, "R16G16B16A16_UNORM" },
			{ ashes::Format::eR16G16B16A16_SNORM, "R16G16B16A16_SNORM" },
			{ ashes::Format::eR16G16B16A16_USCALED, "R16G16B16A16_USCALED" },
			{ ashes::Format::eR16G16B16A16_SSCALED, "R16G16B16A16_SSCALED" },
			{ ashes::Format::eR16G16B16A16_UINT, "R16G16B16A16_UINT" },
			{ ashes::Format::eR16G16B16A16_SINT, "R16G16B16A16_SINT" },
			{ ashes::Format::eR16G16B16A16_SFLOAT, "R16G16B16A16_SFLOAT" },
			{ ashes::Format::eR32_UINT, "R32_UINT" },
			{ ashes::Format::eR32_SINT, "R32_SINT" },
			{ ashes::Format::eR32_SFLOAT, "R32_SFLOAT" },
			{ ashes::Format::eR32G32_UINT, "R32G32_UINT" },
			{ ashes::Format::eR32G32_SINT, "R32G32_SINT" },
			{ ashes::Format::eR32G32_SFLOAT, "R32G32_SFLOAT" },
			{ ashes::Format::eR32G32B32_UINT, "R32G32B32_UINT" },
			{ ashes::Format::eR32G32B32_SINT, "R32G32B32_SINT" },
			{ ashes::Format::eR32G32B32_SFLOAT, "R32G32B32_SFLOAT" },
			{ ashes::Format::eR32G32B32A32_UINT, "R32G32B32A32_UINT" },
			{ ashes::Format::eR32G32B32A32_SINT, "R32G32B32A32_SINT" },
			{ ashes::Format::eR32G32B32A32_SFLOAT, "R32G32B32A32_SFLOAT" },
			{ ashes::Format::eR64_UINT, "R64_UINT" },
			{ ashes::Format::eR64_SINT, "R64_SINT" },
			{ ashes::Format::eR64_SFLOAT, "R64_SFLOAT" },
			{ ashes::Format::eR64G64_UINT, "R64G64_UINT" },
			{ ashes::Format::eR64G64_SINT, "R64G64_SINT" },
			{ ashes::Format::eR64G64_SFLOAT, "R64G64_SFLOAT" },
			{ ashes::Format::eR64G64B64_UINT, "R64G64B64_UINT" },
			{ ashes::Format::eR64G64B64_SINT, "R64G64B64_SINT" },
			{ ashes::Format::eR64G64B64_SFLOAT, "R64G64B64_SFLOAT" },
			{ ashes::Format::eR64G64B64A64_UINT, "R64G64B64A64_UINT" },
			{ ashes::Format::eR64G64B64A64_SINT, "R64G64B64A64_SINT" },
			{ ashes::Format::eR64G64B64A64_SFLOAT, "R64G64B64A64_SFLOAT" },
			{ ashes::Format::eB10G11R11_UFLOAT_PACK32, "B10G11R11_UFLOAT_PACK32" },
			{ ashes::Format::eE5B9G9R9_UFLOAT_PACK32, "E5B9G9R9_UFLOAT_PACK32" },
			{ ashes::Format::eD16_UNORM, "D16_UNORM" },
			{ ashes::Format::eX8_D24_UNORM_PACK32, "X8_D24_UNORM_PACK32" },
			{ ashes::Format::eD32_SFLOAT, "D32_SFLOAT" },
			{ ashes::Format::eS8_UINT, "S8_UINT" },
			{ ashes::Format::eD16_UNORM_S8_UINT, "D16_UNORM_S8_UINT" },
			{ ashes::Format::eD24_UNORM_S8_UINT, "D24_UNORM_S8_UINT" },
			{ ashes::Format::eD32_SFLOAT_S8_UINT, "D32_SFLOAT_S8_UINT" },
			{ ashes::Format::eBC1_RGB_UNORM_BLOCK, "BC1_RGB_UNORM_BLOCK" },
			{ ashes::Format::eBC1_RGB_SRGB_BLOCK, "BC1_RGB_SRGB_BLOCK" },
			{ ashes::Format::eBC1_RGBA_UNORM_BLOCK, "BC1_RGBA_UNORM_BLOCK" },
			{ ashes::Format::eBC1_RGBA_SRGB_BLOCK, "BC1_RGBA_SRGB_BLOCK" },
			{ ashes::Format::eBC2_UNORM_BLOCK, "BC2_UNORM_BLOCK" },
			{ ashes::Format::eBC2_SRGB_BLOCK, "BC2_SRGB_BLOCK" },
			{ ashes::Format::eBC3_UNORM_BLOCK, "BC3_UNORM_BLOCK" },
			{ ashes::Format::eBC3_SRGB_BLOCK, "BC3_SRGB_BLOCK" },
			{ ashes::Format::eBC4_UNORM_BLOCK, "BC4_UNORM_BLOCK" },
			{ ashes::Format::eBC4_SNORM_BLOCK, "BC4_SNORM_BLOCK" },
			{ ashes::Format::eBC5_UNORM_BLOCK, "BC5_UNORM_BLOCK" },
			{ ashes::Format::eBC5_SNORM_BLOCK, "BC5_SNORM_BLOCK" },
			{ ashes::Format::eBC6H_UFLOAT_BLOCK, "BC6H_UFLOAT_BLOCK" },
			{ ashes::Format::eBC6H_SFLOAT_BLOCK, "BC6H_SFLOAT_BLOCK" },
			{ ashes::Format::eBC7_UNORM_BLOCK, "BC7_UNORM_BLOCK" },
			{ ashes::Format::eBC7_SRGB_BLOCK, "BC7_SRGB_BLOCK" },
			{ ashes::Format::eETC2_R8G8B8_UNORM_BLOCK, "ETC2_R8G8B8_UNORM_BLOCK" },
			{ ashes::Format::eETC2_R8G8B8_SRGB_BLOCK, "ETC2_R8G8B8_SRGB_BLOCK" },
			{ ashes::Format::eETC2_R8G8B8A1_UNORM_BLOCK, "ETC2_R8G8B8A1_UNORM_BLOCK" },
			{ ashes::Format::eETC2_R8G8B8A1_SRGB_BLOCK, "ETC2_R8G8B8A1_SRGB_BLOCK" },
			{ ashes::Format::eETC2_R8G8B8A8_UNORM_BLOCK, "ETC2_R8G8B8A8_UNORM_BLOCK" },
			{ ashes::Format::eETC2_R8G8B8A8_SRGB_BLOCK, "ETC2_R8G8B8A8_SRGB_BLOCK" },
			{ ashes::Format::eEAC_R11_UNORM_BLOCK, "EAC_R11_UNORM_BLOCK" },
			{ ashes::Format::eEAC_R11_SNORM_BLOCK, "EAC_R11_SNORM_BLOCK" },
			{ ashes::Format::eEAC_R11G11_UNORM_BLOCK, "EAC_R11G11_UNORM_BLOCK" },
			{ ashes::Format::eEAC_R11G11_SNORM_BLOCK, "EAC_R11G11_SNORM_BLOCK" },
			{ ashes::Format::eASTC_4x4_UNORM_BLOCK, "ASTC_4x4_UNORM_BLOCK" },
			{ ashes::Format::eASTC_4x4_SRGB_BLOCK, "ASTC_4x4_SRGB_BLOCK" },
			{ ashes::Format::eASTC_5x4_UNORM_BLOCK, "ASTC_5x4_UNORM_BLOCK" },
			{ ashes::Format::eASTC_5x4_SRGB_BLOCK, "ASTC_5x4_SRGB_BLOCK" },
			{ ashes::Format::eASTC_5x5_UNORM_BLOCK, "ASTC_5x5_UNORM_BLOCK" },
			{ ashes::Format::eASTC_5x5_SRGB_BLOCK, "ASTC_5x5_SRGB_BLOCK" },
			{ ashes::Format::eASTC_6x5_UNORM_BLOCK, "ASTC_6x5_UNORM_BLOCK" },
			{ ashes::Format::eASTC_6x5_SRGB_BLOCK, "ASTC_6x5_SRGB_BLOCK" },
			{ ashes::Format::eASTC_6x6_UNORM_BLOCK, "ASTC_6x6_UNORM_BLOCK" },
			{ ashes::Format::eASTC_6x6_SRGB_BLOCK, "ASTC_6x6_SRGB_BLOCK" },
			{ ashes::Format::eASTC_8x5_UNORM_BLOCK, "ASTC_8x5_UNORM_BLOCK" },
			{ ashes::Format::eASTC_8x5_SRGB_BLOCK, "ASTC_8x5_SRGB_BLOCK" },
			{ ashes::Format::eASTC_8x6_UNORM_BLOCK, "ASTC_8x6_UNORM_BLOCK" },
			{ ashes::Format::eASTC_8x6_SRGB_BLOCK, "ASTC_8x6_SRGB_BLOCK" },
			{ ashes::Format::eASTC_8x8_UNORM_BLOCK, "ASTC_8x8_UNORM_BLOCK" },
			{ ashes::Format::eASTC_8x8_SRGB_BLOCK, "ASTC_8x8_SRGB_BLOCK" },
			{ ashes::Format::eASTC_10x5_UNORM_BLOCK, "ASTC_10x5_UNORM_BLOCK" },
			{ ashes::Format::eASTC_10x5_SRGB_BLOCK, "ASTC_10x5_SRGB_BLOCK" },
			{ ashes::Format::eASTC_10x6_UNORM_BLOCK, "ASTC_10x6_UNORM_BLOCK" },
			{ ashes::Format::eASTC_10x6_SRGB_BLOCK, "ASTC_10x6_SRGB_BLOCK" },
			{ ashes::Format::eASTC_10x8_UNORM_BLOCK, "ASTC_10x8_UNORM_BLOCK" },
			{ ashes::Format::eASTC_10x8_SRGB_BLOCK, "ASTC_10x8_SRGB_BLOCK" },
			{ ashes::Format::eASTC_10x10_UNORM_BLOCK, "ASTC_10x10_UNORM_BLOCK" },
			{ ashes::Format::eASTC_10x10_SRGB_BLOCK, "ASTC_10x10_SRGB_BLOCK" },
			{ ashes::Format::eASTC_12x10_UNORM_BLOCK, "ASTC_12x10_UNORM_BLOCK" },
			{ ashes::Format::eASTC_12x10_SRGB_BLOCK, "ASTC_12x10_SRGB_BLOCK" },
			{ ashes::Format::eASTC_12x12_UNORM_BLOCK, "ASTC_12x12_UNORM_BLOCK" },
			{ ashes::Format::eASTC_12x12_SRGB_BLOCK, "ASTC_12x12_SRGB_BLOCK" },
			{ ashes::Format::eG8B8G8R8_422_UNORM, "G8B8G8R8_422_UNORM" },
			{ ashes::Format::eB8G8R8G8_422_UNORM, "B8G8R8G8_422_UNORM" },
			{ ashes::Format::eG8_B8_R8_3PLANE_420_UNORM, "G8_B8_R8_3PLANE_420_UNORM" },
			{ ashes::Format::eG8_B8R8_2PLANE_420_UNORM, "G8_B8R8_2PLANE_420_UNORM" },
			{ ashes::Format::eG8_B8_R8_3PLANE_422_UNORM, "G8_B8_R8_3PLANE_422_UNORM" },
			{ ashes::Format::eG8_B8R8_2PLANE_422_UNORM, "G8_B8R8_2PLANE_422_UNORM" },
			{ ashes::Format::eG8_B8_R8_3PLANE_444_UNORM, "G8_B8_R8_3PLANE_444_UNORM" },
			{ ashes::Format::eR10X6_UNORM_PACK16, "R10X6_UNORM_PACK16" },
			{ ashes::Format::eR10X6G10X6_UNORM_2PACK16, "R10X6G10X6_UNORM_2PACK16" },
			{ ashes::Format::eR10X6G10X6B10X6A10X6_UNORM_4PACK16, "R10X6G10X6B10X6A10X6_UNORM_4PACK16" },
			{ ashes::Format::eG10X6B10X6G10X6R10X6_422_UNORM_4PACK16, "G10X6B10X6G10X6R10X6_422_UNORM_4PACK16" },
			{ ashes::Format::eB10X6G10X6R10X6G10X6_422_UNORM_4PACK16, "B10X6G10X6R10X6G10X6_422_UNORM_4PACK16" },
			{ ashes::Format::eG10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16, "G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16" },
			{ ashes::Format::eG10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16, "G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16" },
			{ ashes::Format::eG10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16, "G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16" },
			{ ashes::Format::eG10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16, "G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16" },
			{ ashes::Format::eG10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16, "G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16" },
			{ ashes::Format::eR12X4_UNORM_PACK16, "R12X4_UNORM_PACK16" },
			{ ashes::Format::eR12X4G12X4_UNORM_2PACK16, "R12X4G12X4_UNORM_2PACK16" },
			{ ashes::Format::eR12X4G12X4B12X4A12X4_UNORM_4PACK16, "R12X4G12X4B12X4A12X4_UNORM_4PACK16" },
			{ ashes::Format::eG12X4B12X4G12X4R12X4_422_UNORM_4PACK16, "G12X4B12X4G12X4R12X4_422_UNORM_4PACK16" },
			{ ashes::Format::eB12X4G12X4R12X4G12X4_422_UNORM_4PACK16, "B12X4G12X4R12X4G12X4_422_UNORM_4PACK16" },
			{ ashes::Format::eG12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16, "G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16" },
			{ ashes::Format::eG12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16, "G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16" },
			{ ashes::Format::eG12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16, "G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16" },
			{ ashes::Format::eG12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16, "G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16" },
			{ ashes::Format::eG12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16, "G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16" },
			{ ashes::Format::eG16B16G16R16_422_UNORM, "G16B16G16R16_422_UNORM" },
			{ ashes::Format::eB16G16R16G16_422_UNORM, "B16G16R16G16_422_UNORM" },
			{ ashes::Format::eG16_B16_R16_3PLANE_420_UNORM, "G16_B16_R16_3PLANE_420_UNORM" },
			{ ashes::Format::eG16_B16R16_2PLANE_420_UNORM, "G16_B16R16_2PLANE_420_UNORM" },
			{ ashes::Format::eG16_B16_R16_3PLANE_422_UNORM, "G16_B16_R16_3PLANE_422_UNORM" },
			{ ashes::Format::eG16_B16R16_2PLANE_422_UNORM, "G16_B16R16_2PLANE_422_UNORM" },
			{ ashes::Format::eG16_B16_R16_3PLANE_444_UNORM, "G16_B16_R16_3PLANE_444_UNORM" },
			{ ashes::Format::ePVRTC1_2BPP_UNORM_BLOCK_IMG, "PVRTC1_2BPP_UNORM_BLOCK_IMG" },
			{ ashes::Format::ePVRTC1_4BPP_UNORM_BLOCK_IMG, "PVRTC1_4BPP_UNORM_BLOCK_IMG" },
			{ ashes::Format::ePVRTC2_2BPP_UNORM_BLOCK_IMG, "PVRTC2_2BPP_UNORM_BLOCK_IMG" },
			{ ashes::Format::ePVRTC2_4BPP_UNORM_BLOCK_IMG, "PVRTC2_4BPP_UNORM_BLOCK_IMG" },
			{ ashes::Format::ePVRTC1_2BPP_SRGB_BLOCK_IMG, "PVRTC1_2BPP_SRGB_BLOCK_IMG" },
			{ ashes::Format::ePVRTC1_4BPP_SRGB_BLOCK_IMG, "PVRTC1_4BPP_SRGB_BLOCK_IMG" },
			{ ashes::Format::ePVRTC2_2BPP_SRGB_BLOCK_IMG, "PVRTC2_2BPP_SRGB_BLOCK_IMG" },
			{ ashes::Format::ePVRTC2_4BPP_SRGB_BLOCK_IMG, "PVRTC2_4BPP_SRGB_BLOCK_IMG" },
		};
		return names.at( value );
	}
}
