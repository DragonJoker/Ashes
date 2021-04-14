/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once
#ifndef ___Ashes_common_Format_HPP___
#define ___Ashes_common_Format_HPP___

#ifndef VK_NO_PROTOTYPES
#	define VK_NO_PROTOTYPES
#endif
#include <vulkan/vulkan.h>

#include <cstdint>
#include <string>
#include <algorithm>

#ifdef min
#	undef min
#	undef max
#	undef abs
#endif

namespace ashes
{
	/**
	*\brief
	*	The size of a pixel for specific format.
	*/
	struct BlockSize
	{
		/**
		*\brief
		*	The block extent.
		*/
		VkExtent3D extent;
		/**
		*\brief
		*	The block byte size.
		*/
		VkDeviceSize size;
	};
	/**
	*\brief
	*	Gets the name of the given element.
	*\param[in] value
	*	The element.
	*\return
	*	The name.
	*/
	std::string getName( VkFormat value );
	/**
	*\brief
	*	Computes an aligned size.
	*\param[in] size
	*	The unaligned size.
	*\param[in] align
	*	The alignment value.
	*\return
	*	The aligned size.
	*/
	inline constexpr VkDeviceSize getAlignedSize( VkDeviceSize size, VkDeviceSize align )
	{
		auto rem = size % align;
		return ( rem
			? size + ( align - rem )
			: size );
	}
	/**
	*\brief
	*	Tells if the given VkFormat is usable in depth and stencil buffers.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	\p true if it is usable in depth and stencil buffers.
	*/
	inline constexpr bool isDepthStencilFormat( VkFormat format )noexcept
	{
		return format == VK_FORMAT_D16_UNORM_S8_UINT
			|| format == VK_FORMAT_D24_UNORM_S8_UINT
			|| format == VK_FORMAT_D32_SFLOAT_S8_UINT;
	}
	/**
	*\brief
	*	Tells if the given VkFormat is usable in stencil buffers.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	\p true if it is usable in stencil buffers.
	*/
	inline constexpr bool isStencilFormat( VkFormat format )noexcept
	{
		return format == VK_FORMAT_S8_UINT;
	}
	/**
	*\param[in] format
	*	The source.
	*\return
	*	The stencil format matching the given depth and stencil one.
	*/
	inline constexpr VkFormat getStencil( VkFormat format )noexcept
	{
		switch ( format )
		{
		case VK_FORMAT_S8_UINT:
		case VK_FORMAT_D16_UNORM_S8_UINT:
		case VK_FORMAT_D24_UNORM_S8_UINT:
		case VK_FORMAT_D32_SFLOAT_S8_UINT:
			return VK_FORMAT_S8_UINT;
		default:
			return VK_FORMAT_UNDEFINED;
		}
	}
	/**
	*\brief
	*	Tells if the given VkFormat is usable in depth buffers.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	\p true if it is usable in depth buffers.
	*/
	inline constexpr bool isDepthFormat( VkFormat format )noexcept
	{
		return format == VK_FORMAT_D16_UNORM
			|| format == VK_FORMAT_X8_D24_UNORM_PACK32
			|| format == VK_FORMAT_D32_SFLOAT;
	}
	/**
	*\param[in] format
	*	The source.
	*\return
	*	The depth format matching the given depth and stencil one.
	*/
	inline constexpr VkFormat getDepth( VkFormat format )noexcept
	{
		switch ( format )
		{
		case VK_FORMAT_D16_UNORM_S8_UINT:
			return VK_FORMAT_D16_UNORM;
		case VK_FORMAT_D24_UNORM_S8_UINT:
			return VK_FORMAT_X8_D24_UNORM_PACK32;
		case VK_FORMAT_D32_SFLOAT_S8_UINT:
			return VK_FORMAT_D32_SFLOAT;
		case VK_FORMAT_D16_UNORM:
		case VK_FORMAT_X8_D24_UNORM_PACK32:
		case VK_FORMAT_D32_SFLOAT:
			return format;
		default:
			return VK_FORMAT_UNDEFINED;
		}
	}
	/**
	*\return
	*	\p true if given pixel format is a BC pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	inline constexpr bool isBCFormat( VkFormat format )noexcept
	{
		return format >= VK_FORMAT_BC1_RGB_UNORM_BLOCK
			&& format <= VK_FORMAT_BC7_SRGB_BLOCK;
	}
	/**
	*\return
	*	\p true if given pixel format is a ETC2 pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	inline constexpr bool isETC2Format( VkFormat format )noexcept
	{
		return format >= VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK
			&& format <= VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
	}
	/**
	*\return
	*	\p true if given pixel format is a EAC pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	inline constexpr bool isEACFormat( VkFormat format )noexcept
	{
		return format >= VK_FORMAT_EAC_R11_UNORM_BLOCK
			&& format <= VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
	}
	/**
	*\return
	*	\p true if given pixel format is a ASTC pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	inline constexpr bool isASTCFormat( VkFormat format )noexcept
	{
		return format >= VK_FORMAT_ASTC_4x4_UNORM_BLOCK
			&& format <= VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
	}
	/**
	*\return
	*	\p true if given pixel format is a YCBCR pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	inline constexpr bool isYCBCRFormat( VkFormat format )noexcept
	{
#ifdef VK_API_VERSION_1_1
		return format >= VK_FORMAT_G8B8G8R8_422_UNORM
			&& format <= VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM;
#else
		return false;
#endif
	}
	/**
	*\return
	*	\p true if given pixel format is a PVRTC pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	inline constexpr bool isPVRTCFormat( VkFormat format )noexcept
	{
#ifdef VK_API_VERSION_1_1
		return format >= VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG
			&& format <= VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG;
#else
		return false;
#endif
	}
	/**
	*\return
	*	\p true if given pixel format is a SRGB pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	inline constexpr bool isSRGBFormat( VkFormat format )noexcept
	{
		return format == VK_FORMAT_R8_SRGB
			|| format == VK_FORMAT_R8G8_SRGB
			|| format == VK_FORMAT_R8G8B8_SRGB
			|| format == VK_FORMAT_B8G8R8_SRGB
			|| format == VK_FORMAT_R8G8B8A8_SRGB
			|| format == VK_FORMAT_B8G8R8A8_SRGB
			|| format == VK_FORMAT_A8B8G8R8_SRGB_PACK32
			|| format == VK_FORMAT_BC1_RGB_SRGB_BLOCK
			|| format == VK_FORMAT_BC1_RGBA_SRGB_BLOCK
			|| format == VK_FORMAT_BC2_SRGB_BLOCK
			|| format == VK_FORMAT_BC3_SRGB_BLOCK
			|| format == VK_FORMAT_BC7_SRGB_BLOCK
			|| format == VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK
			|| format == VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
			|| format == VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_4x4_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_5x4_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_5x5_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_6x5_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_6x6_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_8x5_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_8x6_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_8x8_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_10x5_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_10x6_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_10x8_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_10x10_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_12x10_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_12x12_SRGB_BLOCK
#ifdef VK_API_VERSION_1_1
			|| format == VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG
			|| format == VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG
			|| format == VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG
			|| format == VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG
#endif
			;
	}
	/**
	*\return
	*	\p true if given pixel format is a compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	inline constexpr bool isCompressedFormat( VkFormat format )noexcept
	{
		return isBCFormat( format )
			|| isEACFormat( format )
			|| isETC2Format( format )
			|| isASTCFormat( format )
			|| isYCBCRFormat( format )
			|| isPVRTCFormat( format );
	}
	/**
	*\brief
	*	Tells if the given VkFormat is usable in depth and/or stencil buffers.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	\p true if it is usable in depth and/or stencil buffers.
	*/
	inline constexpr bool isDepthOrStencilFormat( VkFormat format )noexcept
	{
		return isDepthStencilFormat( format )
			|| isStencilFormat( format )
			|| isDepthFormat( format );
	}
	/**
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size of given non compressed pixel format.
	*/
	inline constexpr uint32_t getSize( VkFormat format )noexcept
	{
		switch ( format )
		{
		case VK_FORMAT_R4G4_UNORM_PACK8:
		case VK_FORMAT_R8_UNORM:
		case VK_FORMAT_R8_SNORM:
		case VK_FORMAT_R8_USCALED:
		case VK_FORMAT_R8_SSCALED:
		case VK_FORMAT_R8_UINT:
		case VK_FORMAT_R8_SINT:
		case VK_FORMAT_R8_SRGB:
		case VK_FORMAT_S8_UINT:
			return 1u;

		case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
		case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
		case VK_FORMAT_R5G6B5_UNORM_PACK16:
		case VK_FORMAT_B5G6R5_UNORM_PACK16:
		case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
		case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
		case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
		case VK_FORMAT_R8G8_UNORM:
		case VK_FORMAT_R8G8_SNORM:
		case VK_FORMAT_R8G8_USCALED:
		case VK_FORMAT_R8G8_SSCALED:
		case VK_FORMAT_R8G8_UINT:
		case VK_FORMAT_R8G8_SINT:
		case VK_FORMAT_R8G8_SRGB:
		case VK_FORMAT_R16_UNORM:
		case VK_FORMAT_R16_SNORM:
		case VK_FORMAT_R16_USCALED:
		case VK_FORMAT_R16_SSCALED:
		case VK_FORMAT_R16_UINT:
		case VK_FORMAT_R16_SINT:
		case VK_FORMAT_R16_SFLOAT:
		case VK_FORMAT_D16_UNORM:
			return 2u;

		case VK_FORMAT_R8G8B8_UNORM:
		case VK_FORMAT_R8G8B8_SNORM:
		case VK_FORMAT_R8G8B8_USCALED:
		case VK_FORMAT_R8G8B8_SSCALED:
		case VK_FORMAT_R8G8B8_UINT:
		case VK_FORMAT_R8G8B8_SINT:
		case VK_FORMAT_R8G8B8_SRGB:
		case VK_FORMAT_B8G8R8_UNORM:
		case VK_FORMAT_B8G8R8_SNORM:
		case VK_FORMAT_B8G8R8_USCALED:
		case VK_FORMAT_B8G8R8_SSCALED:
		case VK_FORMAT_B8G8R8_UINT:
		case VK_FORMAT_B8G8R8_SINT:
		case VK_FORMAT_B8G8R8_SRGB:
		case VK_FORMAT_D16_UNORM_S8_UINT:
			return 3u;

		case VK_FORMAT_R8G8B8A8_UNORM:
		case VK_FORMAT_R8G8B8A8_SNORM:
		case VK_FORMAT_R8G8B8A8_USCALED:
		case VK_FORMAT_R8G8B8A8_SSCALED:
		case VK_FORMAT_R8G8B8A8_UINT:
		case VK_FORMAT_R8G8B8A8_SINT:
		case VK_FORMAT_R8G8B8A8_SRGB:
		case VK_FORMAT_B8G8R8A8_UNORM:
		case VK_FORMAT_B8G8R8A8_SNORM:
		case VK_FORMAT_B8G8R8A8_USCALED:
		case VK_FORMAT_B8G8R8A8_SSCALED:
		case VK_FORMAT_B8G8R8A8_UINT:
		case VK_FORMAT_B8G8R8A8_SINT:
		case VK_FORMAT_B8G8R8A8_SRGB:
		case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
		case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
		case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
		case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
		case VK_FORMAT_A8B8G8R8_UINT_PACK32:
		case VK_FORMAT_A8B8G8R8_SINT_PACK32:
		case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
		case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
		case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
		case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
		case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
		case VK_FORMAT_A2R10G10B10_UINT_PACK32:
		case VK_FORMAT_A2R10G10B10_SINT_PACK32:
		case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
		case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
		case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
		case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
		case VK_FORMAT_A2B10G10R10_UINT_PACK32:
		case VK_FORMAT_A2B10G10R10_SINT_PACK32:
		case VK_FORMAT_R16G16_UNORM:
		case VK_FORMAT_R16G16_SNORM:
		case VK_FORMAT_R16G16_USCALED:
		case VK_FORMAT_R16G16_SSCALED:
		case VK_FORMAT_R16G16_UINT:
		case VK_FORMAT_R16G16_SINT:
		case VK_FORMAT_R16G16_SFLOAT:
		case VK_FORMAT_R32_UINT:
		case VK_FORMAT_R32_SINT:
		case VK_FORMAT_R32_SFLOAT:
		case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
		case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
		case VK_FORMAT_X8_D24_UNORM_PACK32:
		case VK_FORMAT_D32_SFLOAT:
		case VK_FORMAT_D24_UNORM_S8_UINT:
			return 4u;

		case VK_FORMAT_R16G16B16_UNORM:
		case VK_FORMAT_R16G16B16_SNORM:
		case VK_FORMAT_R16G16B16_USCALED:
		case VK_FORMAT_R16G16B16_SSCALED:
		case VK_FORMAT_R16G16B16_UINT:
		case VK_FORMAT_R16G16B16_SINT:
		case VK_FORMAT_R16G16B16_SFLOAT:
			return 6u;

		case VK_FORMAT_R16G16B16A16_UNORM:
		case VK_FORMAT_R16G16B16A16_SNORM:
		case VK_FORMAT_R16G16B16A16_USCALED:
		case VK_FORMAT_R16G16B16A16_SSCALED:
		case VK_FORMAT_R16G16B16A16_UINT:
		case VK_FORMAT_R16G16B16A16_SINT:
		case VK_FORMAT_R16G16B16A16_SFLOAT:
		case VK_FORMAT_R32G32_UINT:
		case VK_FORMAT_R32G32_SINT:
		case VK_FORMAT_R32G32_SFLOAT:
		case VK_FORMAT_R64_UINT:
		case VK_FORMAT_R64_SINT:
		case VK_FORMAT_R64_SFLOAT:
		case VK_FORMAT_D32_SFLOAT_S8_UINT:
			return 8u;

		case VK_FORMAT_R32G32B32_UINT:
		case VK_FORMAT_R32G32B32_SINT:
		case VK_FORMAT_R32G32B32_SFLOAT:
			return 12u;

		case VK_FORMAT_R32G32B32A32_UINT:
		case VK_FORMAT_R32G32B32A32_SINT:
		case VK_FORMAT_R32G32B32A32_SFLOAT:
		case VK_FORMAT_R64G64_UINT:
		case VK_FORMAT_R64G64_SINT:
		case VK_FORMAT_R64G64_SFLOAT:
			return 16u;

		case VK_FORMAT_R64G64B64_UINT:
		case VK_FORMAT_R64G64B64_SINT:
		case VK_FORMAT_R64G64B64_SFLOAT:
			return 24u;

		case VK_FORMAT_R64G64B64A64_UINT:
		case VK_FORMAT_R64G64B64A64_SINT:
		case VK_FORMAT_R64G64B64A64_SFLOAT:
			return 32u;

		default:
			return 0u;
		}
	}
	/**
	*\return
	*	The given format components count.
	*\param[in] format
	*	The VkFormat.
	*/
	inline constexpr uint32_t getCount( VkFormat format )noexcept
	{
		switch ( format )
		{
		case VK_FORMAT_R8_UNORM:
		case VK_FORMAT_R8_SNORM:
		case VK_FORMAT_R8_USCALED:
		case VK_FORMAT_R8_SSCALED:
		case VK_FORMAT_R8_UINT:
		case VK_FORMAT_R8_SINT:
		case VK_FORMAT_R8_SRGB:
		case VK_FORMAT_S8_UINT:
		case VK_FORMAT_R16_UNORM:
		case VK_FORMAT_R16_SNORM:
		case VK_FORMAT_R16_USCALED:
		case VK_FORMAT_R16_SSCALED:
		case VK_FORMAT_R16_UINT:
		case VK_FORMAT_R16_SINT:
		case VK_FORMAT_R16_SFLOAT:
		case VK_FORMAT_D16_UNORM:
		case VK_FORMAT_R32_UINT:
		case VK_FORMAT_R32_SINT:
		case VK_FORMAT_R32_SFLOAT:
		case VK_FORMAT_X8_D24_UNORM_PACK32:
		case VK_FORMAT_D32_SFLOAT:
		case VK_FORMAT_R64_UINT:
		case VK_FORMAT_R64_SINT:
		case VK_FORMAT_R64_SFLOAT:
		case VK_FORMAT_EAC_R11_UNORM_BLOCK:
		case VK_FORMAT_EAC_R11_SNORM_BLOCK:
#ifdef VK_API_VERSION_1_1
		case VK_FORMAT_R12X4_UNORM_PACK16:
		case VK_FORMAT_R10X6_UNORM_PACK16:
#endif
			return 1u;

		case VK_FORMAT_R4G4_UNORM_PACK8:
		case VK_FORMAT_R8G8_UNORM:
		case VK_FORMAT_R8G8_SNORM:
		case VK_FORMAT_R8G8_USCALED:
		case VK_FORMAT_R8G8_SSCALED:
		case VK_FORMAT_R8G8_UINT:
		case VK_FORMAT_R8G8_SINT:
		case VK_FORMAT_R8G8_SRGB:
		case VK_FORMAT_D16_UNORM_S8_UINT:
		case VK_FORMAT_R16G16_UNORM:
		case VK_FORMAT_R16G16_SNORM:
		case VK_FORMAT_R16G16_USCALED:
		case VK_FORMAT_R16G16_SSCALED:
		case VK_FORMAT_R16G16_UINT:
		case VK_FORMAT_R16G16_SINT:
		case VK_FORMAT_R16G16_SFLOAT:
		case VK_FORMAT_D24_UNORM_S8_UINT:
		case VK_FORMAT_D32_SFLOAT_S8_UINT:
		case VK_FORMAT_R32G32_UINT:
		case VK_FORMAT_R32G32_SINT:
		case VK_FORMAT_R32G32_SFLOAT:
		case VK_FORMAT_R64G64_UINT:
		case VK_FORMAT_R64G64_SINT:
		case VK_FORMAT_R64G64_SFLOAT:
		case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
		case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
#ifdef VK_API_VERSION_1_1
		case VK_FORMAT_R12X4G12X4_UNORM_2PACK16:
		case VK_FORMAT_R10X6G10X6_UNORM_2PACK16:
#endif
			return 2u;

		case VK_FORMAT_R5G6B5_UNORM_PACK16:
		case VK_FORMAT_B5G6R5_UNORM_PACK16:
		case VK_FORMAT_R8G8B8_UNORM:
		case VK_FORMAT_R8G8B8_SNORM:
		case VK_FORMAT_R8G8B8_USCALED:
		case VK_FORMAT_R8G8B8_SSCALED:
		case VK_FORMAT_R8G8B8_UINT:
		case VK_FORMAT_R8G8B8_SINT:
		case VK_FORMAT_R8G8B8_SRGB:
		case VK_FORMAT_B8G8R8_UNORM:
		case VK_FORMAT_B8G8R8_SNORM:
		case VK_FORMAT_B8G8R8_USCALED:
		case VK_FORMAT_B8G8R8_SSCALED:
		case VK_FORMAT_B8G8R8_UINT:
		case VK_FORMAT_B8G8R8_SINT:
		case VK_FORMAT_B8G8R8_SRGB:
		case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
		case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
		case VK_FORMAT_R16G16B16_UNORM:
		case VK_FORMAT_R16G16B16_SNORM:
		case VK_FORMAT_R16G16B16_USCALED:
		case VK_FORMAT_R16G16B16_SSCALED:
		case VK_FORMAT_R16G16B16_UINT:
		case VK_FORMAT_R16G16B16_SINT:
		case VK_FORMAT_R16G16B16_SFLOAT:
		case VK_FORMAT_R32G32B32_UINT:
		case VK_FORMAT_R32G32B32_SINT:
		case VK_FORMAT_R32G32B32_SFLOAT:
		case VK_FORMAT_R64G64B64_UINT:
		case VK_FORMAT_R64G64B64_SINT:
		case VK_FORMAT_R64G64B64_SFLOAT:
		case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
		case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
		case VK_FORMAT_BC2_UNORM_BLOCK:
		case VK_FORMAT_BC2_SRGB_BLOCK:
		case VK_FORMAT_BC3_UNORM_BLOCK:
		case VK_FORMAT_BC3_SRGB_BLOCK:
		case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
		case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
		case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
		case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
		case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
		case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
		case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
		case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
		case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
		case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
		case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
		case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
		case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
		case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
		case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
		case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
		case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
		case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
		case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
		case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
		case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
		case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
		case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
		case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
		case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
		case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
		case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
		case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
		case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
		case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
#ifdef VK_API_VERSION_1_1
		case VK_FORMAT_G8B8G8R8_422_UNORM:
		case VK_FORMAT_B8G8R8G8_422_UNORM:
		case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM:
		case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM:
		case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM:
		case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM:
		case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM:
		case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
		case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
		case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
		case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
		case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
		case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
		case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
		case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
		case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
		case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
		case VK_FORMAT_G16B16G16R16_422_UNORM:
		case VK_FORMAT_B16G16R16G16_422_UNORM:
		case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM:
		case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM:
		case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM:
		case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM:
		case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM:
		case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
		case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
		case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
		case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
		case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
		case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
		case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
		case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
#endif
			return 3u;

		case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
		case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
		case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
		case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
		case VK_FORMAT_R8G8B8A8_UNORM:
		case VK_FORMAT_R8G8B8A8_SNORM:
		case VK_FORMAT_R8G8B8A8_USCALED:
		case VK_FORMAT_R8G8B8A8_SSCALED:
		case VK_FORMAT_R8G8B8A8_UINT:
		case VK_FORMAT_R8G8B8A8_SINT:
		case VK_FORMAT_R8G8B8A8_SRGB:
		case VK_FORMAT_B8G8R8A8_UNORM:
		case VK_FORMAT_B8G8R8A8_SNORM:
		case VK_FORMAT_B8G8R8A8_USCALED:
		case VK_FORMAT_B8G8R8A8_SSCALED:
		case VK_FORMAT_B8G8R8A8_UINT:
		case VK_FORMAT_B8G8R8A8_SINT:
		case VK_FORMAT_B8G8R8A8_SRGB:
		case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
		case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
		case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
		case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
		case VK_FORMAT_A8B8G8R8_UINT_PACK32:
		case VK_FORMAT_A8B8G8R8_SINT_PACK32:
		case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
		case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
		case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
		case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
		case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
		case VK_FORMAT_A2R10G10B10_UINT_PACK32:
		case VK_FORMAT_A2R10G10B10_SINT_PACK32:
		case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
		case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
		case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
		case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
		case VK_FORMAT_A2B10G10R10_UINT_PACK32:
		case VK_FORMAT_A2B10G10R10_SINT_PACK32:
		case VK_FORMAT_R16G16B16A16_UNORM:
		case VK_FORMAT_R16G16B16A16_SNORM:
		case VK_FORMAT_R16G16B16A16_USCALED:
		case VK_FORMAT_R16G16B16A16_SSCALED:
		case VK_FORMAT_R16G16B16A16_UINT:
		case VK_FORMAT_R16G16B16A16_SINT:
		case VK_FORMAT_R16G16B16A16_SFLOAT:
		case VK_FORMAT_R32G32B32A32_UINT:
		case VK_FORMAT_R32G32B32A32_SINT:
		case VK_FORMAT_R32G32B32A32_SFLOAT:
		case VK_FORMAT_R64G64B64A64_UINT:
		case VK_FORMAT_R64G64B64A64_SINT:
		case VK_FORMAT_R64G64B64A64_SFLOAT:
		case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
		case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
		case VK_FORMAT_BC4_UNORM_BLOCK:
		case VK_FORMAT_BC4_SNORM_BLOCK:
		case VK_FORMAT_BC5_UNORM_BLOCK:
		case VK_FORMAT_BC5_SNORM_BLOCK:
		case VK_FORMAT_BC6H_UFLOAT_BLOCK:
		case VK_FORMAT_BC6H_SFLOAT_BLOCK:
		case VK_FORMAT_BC7_UNORM_BLOCK:
		case VK_FORMAT_BC7_SRGB_BLOCK:
		case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
		case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
		case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
		case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
#ifdef VK_API_VERSION_1_1
		case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16:
		case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
		case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
		case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16:
		case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
		case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
#endif
			return 4u;

		default:
			return 0u;
		}
	}
	/**
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size of given BC compressed pixel format, for a 4x4 pixels block.
	*/
	inline constexpr uint32_t getBCSize( VkFormat format )noexcept
	{
		switch ( format )
		{
		case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
		case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
		case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
		case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
			return 8u;

		case VK_FORMAT_BC2_UNORM_BLOCK:
		case VK_FORMAT_BC2_SRGB_BLOCK:
			return 16u;

		case VK_FORMAT_BC3_UNORM_BLOCK:
		case VK_FORMAT_BC3_SRGB_BLOCK:
			return 16u;

		case VK_FORMAT_BC4_UNORM_BLOCK:
		case VK_FORMAT_BC4_SNORM_BLOCK:
			return 8u;

		case VK_FORMAT_BC5_UNORM_BLOCK:
		case VK_FORMAT_BC5_SNORM_BLOCK:
			return 16u;

		case VK_FORMAT_BC6H_UFLOAT_BLOCK:
		case VK_FORMAT_BC6H_SFLOAT_BLOCK:
			return 16u;

		case VK_FORMAT_BC7_UNORM_BLOCK:
		case VK_FORMAT_BC7_SRGB_BLOCK:
			return 16u;

		default:
			return 0u;
		}
	}
	/**
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size of given ETC2 compressed pixel format, for a 4x4 pixels block.
	*/
	inline constexpr uint32_t getETC2Size( VkFormat format )noexcept
	{
		switch ( format )
		{
		case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
		case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
			return 8u;

		case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
		case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
			return 8u;

		case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
		case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
			return 16u;

		default:
			return 0u;
		}
	}
	/**
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size of given EAC compressed pixel format, for a 4x4 pixels block.
	*/
	inline constexpr uint32_t getEACSize( VkFormat format )noexcept
	{
		switch ( format )
		{
		case VK_FORMAT_EAC_R11_UNORM_BLOCK:
		case VK_FORMAT_EAC_R11_SNORM_BLOCK:
			return 8u;

		case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
		case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
			return 16u;

		default:
			return 0u;
		}
	}
	/**
	*\brief
	*	Retrieves the minimal VkExtent2D for given pixel format.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The VkExtent2D.
	*/
	inline constexpr VkExtent2D getMinimalExtent2D( VkFormat format )noexcept
	{
		VkExtent2D result{ 1, 1 };

		if ( ashes::isCompressedFormat( format ) )
		{
			if ( ashes::isBCFormat( format )
				|| ashes::isETC2Format( format )
				|| ashes::isEACFormat( format ) )
			{
				result.width = 4;
				result.height = 4;
			}
			else if ( ashes::isASTCFormat( format ) )
			{
				switch ( format )
				{
				case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
				case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
					result.width = 4;
					result.height = 4;
					break;
				case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
				case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
					result.width = 5;
					result.height = 4;
					break;
				case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
				case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
					result.width = 5;
					result.height = 5;
					break;
				case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
				case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
					result.width = 6;
					result.height = 5;
					break;
				case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
				case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
					result.width = 6;
					result.height = 6;
					break;
				case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
				case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
					result.width = 8;
					result.height = 5;
					break;
				case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
				case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
					result.width = 8;
					result.height = 6;
					break;
				case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
				case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
					result.width = 8;
					result.height = 8;
					break;
				case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
				case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
					result.width = 10;
					result.height = 5;
					break;
				case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
				case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
					result.width = 10;
					result.height = 6;
					break;
				case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
				case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
					result.width = 10;
					result.height = 8;
					break;
				case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
				case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
					result.width = 10;
					result.height = 10;
					break;
				case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
				case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
					result.width = 12;
					result.height = 10;
					break;
				case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
				case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
					result.width = 12;
					result.height = 12;
					break;
				default:
					break;
				}
			}
		}

		return result;
	}
	/**
	*\brief
	*	Checks if the given extent fits the given format.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	\p true if the extent is compatible with the format.
	*/
	inline constexpr bool checkExtent( VkFormat format, VkExtent2D const & extent )noexcept
	{
		auto minimal = getMinimalExtent2D( format );
		return extent.width >= minimal.width
			&& extent.height >= minimal.height;
	}
	/**
	*\brief
	*	Checks if the given extent fits the given format.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	\p true if the extent is compatible with the format.
	*/
	inline constexpr bool checkExtent( VkFormat format, VkExtent3D const & extent )noexcept
	{
		return checkExtent( format, VkExtent2D{ extent.width, extent.height } );
	}
	/**
	*\brief
	*	Retrieves the minimal VkExtent2D for given pixel format.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The VkExtent2D.
	*/
	inline constexpr VkExtent3D getMinimalExtent3D( VkFormat format )noexcept
	{
		auto const minimal = getMinimalExtent2D( format );
		return VkExtent3D{ minimal.width, minimal.height, 1 };
	}
	/**
	*\brief
	*	Retrieves the byte size of given pixel format, for its minimal extent.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*/
	inline constexpr VkDeviceSize getMinimalSize( VkFormat format )noexcept
	{
		if ( !ashes::isCompressedFormat( format ) )
		{
			return ashes::getSize( format );
		}

		if ( ashes::isBCFormat( format ) )
		{
			return ashes::getBCSize( format );
		}

		if ( ashes::isETC2Format( format ) )
		{
			return ashes::getETC2Size( format );
		}

		if ( ashes::isEACFormat( format ) )
		{
			return ashes::getEACSize( format );
		}

		return 1u;
	}
	/**
	*\param[in] format
	*	The pixel format.
	*\return
	*	The block size for given pixel format.
	*/
	inline constexpr BlockSize getBlockSize( VkFormat format )
	{
		BlockSize result{ getMinimalExtent3D( format ), 0u };
		result.size = getMinimalSize( format );
		return result;
	}
	/**
	*\brief
	*	Retrieves the real extent for the given mipmap level.
	*\param[in] extent
	*	The level 0 extent.
	*\param[in] mipLevel
	*	The mipmap level.
	*\return
	*	The mipmap level extent.
	*/
	template< typename T >
	inline constexpr T getSubresourceDimension( T const & extent
		, uint32_t mipLevel )noexcept
	{
		return std::max( T( 1 ), T( extent >> mipLevel ) );
	}
	/**
	*\brief
	*	Retrieves the real extent for the given mipmap level.
	*\param[in] extent
	*	The level 0 extent.
	*\param[in] mipLevel
	*	The mipmap level for which dimensions are computed.
	*\return
	*	The dimensions.
	*/
	inline constexpr VkExtent3D getSubresourceDimensions( VkExtent2D const & extent
		, uint32_t mipLevel )noexcept
	{
		return
		{
			getSubresourceDimension( extent.width, mipLevel ),
			getSubresourceDimension( extent.height, mipLevel ),
			1u
		};
	}
	/**
	*\brief
	*	Retrieves the real extent for the given mipmap level and image format.
	*\param[in] extent
	*	The level 0 extent.
	*\param[in] mipLevel
	*	The mipmap level for which dimensions are computed.
	*\param[in] format
	*	The image format.
	*\return
	*	The dimensions.
	*/
	inline constexpr VkExtent3D getSubresourceDimensions( VkExtent2D const & extent
		, uint32_t mipLevel
		, VkFormat format )noexcept
	{
		auto blockSize = getBlockSize( format );
		return
		{
			uint32_t( getAlignedSize( getSubresourceDimension( extent.width, mipLevel ), blockSize.extent.width ) ),
			uint32_t( getAlignedSize( getSubresourceDimension( extent.height, mipLevel ), blockSize.extent.height ) ),
			1u
		};
	}
	/**
	*\brief
	*	Retrieves the real extent for the given mipmap level.
	*\param[in] extent
	*	The level 0 extent.
	*\param[in] mipLevel
	*	The mipmap level.
	*\return
	*	The dimensions.
	*/
	inline constexpr VkExtent3D getSubresourceDimensions( VkExtent3D const & extent
		, uint32_t mipLevel )noexcept
	{
		return
		{
			getSubresourceDimension( extent.width, mipLevel ),
			getSubresourceDimension( extent.height, mipLevel ),
			getSubresourceDimension( extent.depth, mipLevel )
		};
	}
	/**
	*\brief
	*	Retrieves the real extent for the given mipmap level and image format.
	*\param[in] extent
	*	The level 0 extent.
	*\param[in] mipLevel
	*	The mipmap level.
	*\param[in] format
	*	The image format.
	*\return
	*	The dimensions.
	*/
	inline constexpr VkExtent3D getSubresourceDimensions( VkExtent3D const & extent
		, uint32_t mipLevel
		, VkFormat format )noexcept
	{
		auto blockSize = getBlockSize( format );
		return
		{
			uint32_t( getAlignedSize( getSubresourceDimension( extent.width, mipLevel ), blockSize.extent.width ) ),
			uint32_t( getAlignedSize( getSubresourceDimension( extent.height, mipLevel ), blockSize.extent.height ) ),
			uint32_t( getAlignedSize( getSubresourceDimension( extent.depth, mipLevel ), blockSize.extent.depth ) )
		};
	}
	/**
	*\brief
	*	Retrieves the byte size of given pixel format and dimensions.
	*\param[in] format
	*	The pixel format.
	*\param[in] extent
	*	The mip 0 dimensions.
	*\param[in] texel
	*	The texel dimensions for the format.
	*\param[in] mipLevel
	*	The wanted mipmap level.
	*\return
	*	The byte size.
	*/
	inline constexpr VkDeviceSize getSize( VkFormat format
		, VkExtent3D const & extent
		, BlockSize const & texel
		, uint32_t mipLevel
		, uint32_t alignment )noexcept
	{
		alignment = ( alignment <= 1u
			? uint32_t( getMinimalSize( format ) )
			: alignment );
		auto levelExtent = getSubresourceDimensions( extent, mipLevel, format );
		auto result = texel.size
			* getAlignedSize( levelExtent.width, texel.extent.width )
			* getAlignedSize( levelExtent.height, texel.extent.height )
			* getAlignedSize( levelExtent.depth, texel.extent.depth );
		return std::max( VkDeviceSize( alignment )
			, getAlignedSize( result / ( VkDeviceSize( texel.extent.width ) * texel.extent.height * texel.extent.depth )
				, alignment ) );
	}
	/**
	*\brief
	*	Retrieves the aspects mask matching given VkFormat.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	The aspects.
	*/
	inline constexpr VkImageAspectFlags getAspectMask( VkFormat format )noexcept
	{
		return ( isDepthStencilFormat( format )
			? VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
			: ( isDepthFormat( format )
				? VK_IMAGE_ASPECT_DEPTH_BIT
				: ( isStencilFormat( format )
					? VK_IMAGE_ASPECT_STENCIL_BIT
					: VK_IMAGE_ASPECT_COLOR_BIT ) ) );
	}
	/**
	*\param[in] extent
	*	The level 0 extent.
	*\return
	*	The mipmap levels count.
	*/
	inline constexpr uint32_t getMaxMipCount( VkExtent3D extent )
	{
		auto minExtent = extent.width;
		minExtent = extent.height > 1u
			? std::min( minExtent, extent.height )
			: minExtent;
		minExtent = extent.depth > 1u
			? std::min( minExtent, extent.depth )
			: minExtent;
		uint32_t result = 1u;

		while ( minExtent > 1 )
		{
			minExtent /= 2;
			++result;
		}

		return result;
	}
	/**
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\param[in] mipLevel
	*	The mipmap level for which dimensions are computed.
	*\return
	*	The byte size of given pixel format and dimensions.
	*/
	inline constexpr VkDeviceSize getSize( VkExtent3D const & extent
		, VkFormat format
		, uint32_t mipLevel = 0u
		, uint32_t alignment = 1u )noexcept
	{
		return getSize( format
			, extent
			, getBlockSize( format )
			, mipLevel
			, alignment );
	}
	/**
	*\brief
	*	Retrieves the byte size of given pixel format and dimensions.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\param[in] mipLevel
	*	The mipmap level for which dimensions are computed.
	*\return
	*	The byte size.
	*/
	inline constexpr VkDeviceSize getSize( VkExtent2D const & extent
		, VkFormat format
		, uint32_t mipLevel = 0u
		, uint32_t alignment = 1u )noexcept
	{
		return getSize( format
			, VkExtent3D{ extent.width, extent.height, 1u }
			, getBlockSize( format )
			, mipLevel
			, alignment );
	}
	/**
	*\brief
	*	Retrieves the byte size of given pixel format and dimensions, for given mipmap levels range.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\param[in] baseMipLevel, levelCount
	*	The mipmap levels range.
	*\return
	*	The byte size.
	*/
	inline constexpr VkDeviceSize getLevelsSize( VkExtent2D const & extent
		, VkFormat format
		, uint32_t baseMipLevel
		, uint32_t levelCount
		, uint32_t alignment )noexcept
	{
		VkDeviceSize result = 0u;
		auto blockSize = getBlockSize( format );
		auto imageExtent = VkExtent3D{ extent.width, extent.height, 1u };

		for ( auto mipLevel = baseMipLevel; mipLevel < baseMipLevel + levelCount; ++mipLevel )
		{
			result += getSize( format
				, imageExtent
				, blockSize
				, mipLevel
				, alignment );
		}

		return result;
	}
	/**
	*\brief
	*	Retrieves the byte size of given pixel format and dimensions, for given mipmap levels range.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\param[in] baseMipLevel, levelCount
	*	The mipmap levels range.
	*\return
	*	The byte size.
	*/
	inline constexpr VkDeviceSize getLevelsSize( VkExtent3D const & extent
		, VkFormat format
		, uint32_t baseMipLevel
		, uint32_t levelCount
		, uint32_t alignment )noexcept
	{
		VkDeviceSize result = 0u;
		auto blockSize = getBlockSize( format );

		for ( auto mipLevel = baseMipLevel; mipLevel < baseMipLevel + levelCount; ++mipLevel )
		{
			result += getSize( format
				, extent
				, blockSize
				, mipLevel
				, alignment );
		}

		return result;
	}

	inline constexpr VkDeviceSize getTotalSize( VkExtent2D const & extent
		, VkFormat format
		, uint32_t layerCount
		, uint32_t levelCount
		, uint32_t alignment )noexcept
	{
		return layerCount * getLevelsSize( extent, format, 0u, levelCount, alignment );
	}

	inline constexpr VkDeviceSize getTotalSize( VkExtent3D const & extent
		, VkFormat format
		, uint32_t layerCount
		, uint32_t levelCount
		, uint32_t alignment )noexcept
	{
		return layerCount * getLevelsSize( extent, format, 0u, levelCount, alignment );
	}

	inline constexpr VkFormat beginFmt()
	{
		return VK_FORMAT_UNDEFINED;
	}

	inline constexpr VkFormat endFmt()
	{
		return VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
	}
}

#endif
