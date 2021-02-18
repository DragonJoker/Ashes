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
	*	Computes an aligned size.
	*\param[in] size
	*	The unaligned size.
	*\param[in] align
	*	The alignment value.
	*\return
	*	The aligned size.
	*/
	inline VkDeviceSize getAlignedSize( VkDeviceSize size, VkDeviceSize align )
	{
		auto rem = size % align;
		return ( rem
			? size + ( align - rem )
			: size );
	}
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
	*\param[in] format
	*	The pixel format.
	*\return
	*	The block size for given pixel format.
	*/
	BlockSize getBlockSize( VkFormat format );
	/**
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size of given non compressed pixel format.
	*/
	uint32_t getSize( VkFormat format )noexcept;
	/**
	*\return
	*	The given format components count.
	*\param[in] format
	*	The VkFormat.
	*/
	uint32_t getCount( VkFormat format )noexcept;
	/**
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size of given BC compressed pixel format, for a 4x4 pixels block.
	*/
	uint32_t getBCSize( VkFormat format )noexcept;
	/**
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size of given ETC2 compressed pixel format, for a 4x4 pixels block.
	*/
	uint32_t getETC2Size( VkFormat format )noexcept;
	/**
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size of given EAC compressed pixel format, for a 4x4 pixels block.
	*/
	uint32_t getEACSize( VkFormat format )noexcept;
	/**
	*\brief
	*	Tells if the given VkFormat is usable in depth and stencil buffers.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	\p true if it is usable in depth and stencil buffers.
	*/
	bool isDepthStencilFormat( VkFormat format )noexcept;
	/**
	*\brief
	*	Tells if the given VkFormat is usable in stencil buffers.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	\p true if it is usable in stencil buffers.
	*/
	bool isStencilFormat( VkFormat format )noexcept;
	/**
	*\param[in] format
	*	The source.
	*\return
	*	The stencil format matching the given depth and stencil one.
	*/
	VkFormat getStencil( VkFormat format )noexcept;
	/**
	*\brief
	*	Tells if the given VkFormat is usable in depth buffers.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	\p true if it is usable in depth buffers.
	*/
	bool isDepthFormat( VkFormat format )noexcept;
	/**
	*\param[in] format
	*	The source.
	*\return
	*	The depth format matching the given depth and stencil one.
	*/
	VkFormat getDepth( VkFormat format )noexcept;
	/**
	*\return
	*	\p true if given pixel format is a BC pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	bool isBCFormat( VkFormat format )noexcept;
	/**
	*\return
	*	\p true if given pixel format is a ETC2 pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	bool isETC2Format( VkFormat format )noexcept;
	/**
	*\return
	*	\p true if given pixel format is a EAC pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	bool isEACFormat( VkFormat format )noexcept;
	/**
	*\return
	*	\p true if given pixel format is a ASTC pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	bool isASTCFormat( VkFormat format )noexcept;
	/**
	*\return
	*	\p true if given pixel format is a YCBCR pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	bool isYCBCRFormat( VkFormat format )noexcept;
	/**
	*\return
	*	\p true if given pixel format is a PVRTC pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	bool isPVRTCFormat( VkFormat format )noexcept;
	/**
	*\return
	*	\p true if given pixel format is a compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*/
	inline bool isCompressedFormat( VkFormat format )noexcept
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
	inline bool isDepthOrStencilFormat( VkFormat format )noexcept
	{
		return isDepthStencilFormat( format )
			|| isStencilFormat( format )
			|| isDepthFormat( format );
	}
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
	*	Retrieves the minimal VkExtent2D for given pixel format.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The VkExtent2D.
	*/
	VkExtent2D getMinimalExtent2D( VkFormat format )noexcept;
	/**
	*\brief
	*	Retrieves the byte size of given pixel format, for its minimal extent.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*/
	VkDeviceSize getMinimalSize( VkFormat format )noexcept;
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
	VkDeviceSize getSize( VkFormat format
		, VkExtent3D const & extent
		, BlockSize const & texel
		, uint32_t mipLevel
		, uint32_t alignment )noexcept;
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
	inline bool checkExtent( VkFormat format, VkExtent2D const & extent )noexcept
	{
		auto minimal = getMinimalExtent2D( format );
		return extent.width >= minimal.width
			&& extent.height >= minimal.height;
	}
	/**
	*\brief
	*	Retrieves the minimal VkExtent2D for given pixel format.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The VkExtent2D.
	*/
	inline VkExtent3D getMinimalExtent3D( VkFormat format )noexcept
	{
		auto const minimal = getMinimalExtent2D( format );
		return VkExtent3D{ minimal.width, minimal.height, 1 };
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
	inline bool checkExtent( VkFormat format, VkExtent3D const & extent )noexcept
	{
		return checkExtent( format, VkExtent2D{ extent.width, extent.height } );
	}
	/**
	*\brief
	*	Retrieves the aspects mask matching given VkFormat.
	*\param[in] format
	*	The VkFormat.
	*\return
	*	The aspects.
	*/
	inline VkImageAspectFlags getAspectMask( VkFormat format )noexcept
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
	inline uint32_t getMaxMipCount( VkExtent3D extent )
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
	inline T getSubresourceDimension( T const & extent
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
	inline VkExtent3D getSubresourceDimensions( VkExtent2D const & extent
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
	*	Retrieves the real extent for the given mipmap level.
	*\param[in] extent
	*	The level 0 extent.
	*\param[in] mipLevel
	*	The mipmap level.
	*\return
	*	The dimensions.
	*/
	inline VkExtent3D getSubresourceDimensions( VkExtent3D const & extent
		, uint32_t mipLevel )noexcept
	{
		return
		{
			getSubresourceDimension( extent.width, mipLevel ),
			getSubresourceDimension( extent.height, mipLevel ),
			extent.depth
		};
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
	inline VkDeviceSize getSize( VkExtent3D const & extent
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
	inline VkDeviceSize getSize( VkExtent2D const & extent
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
	inline VkDeviceSize getLevelsSize( VkExtent2D const & extent
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
	inline VkDeviceSize getLevelsSize( VkExtent3D const & extent
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

	inline VkDeviceSize getTotalSize( VkExtent2D const & extent
		, VkFormat format
		, uint32_t layerCount
		, uint32_t levelCount
		, uint32_t alignment )noexcept
	{
		return layerCount * getLevelsSize( extent, format, 0u, levelCount, alignment );
	}

	inline VkDeviceSize getTotalSize( VkExtent3D const & extent
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
