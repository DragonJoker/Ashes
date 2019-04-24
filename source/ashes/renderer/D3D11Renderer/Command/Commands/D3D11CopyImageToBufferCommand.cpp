/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CopyImageToBufferCommand.hpp"
#include "Command/Commands/D3D11CopyBufferCommand.hpp"
#include "Command/Commands/D3D11CopyImageCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Core/D3D11Device.hpp"
#include "Image/D3D11Image.hpp"
#include "Image/D3D11ImageView.hpp"

#include <Ashes/Image/ImageSubresource.hpp>
#include <Ashes/Image/ImageSubresourceRange.hpp>
#include <Ashes/Image/SubresourceLayout.hpp>
#include <Ashes/Miscellaneous/BufferImageCopy.hpp>

namespace ashes::d3d11
{
	namespace
	{
		std::vector< ImageViewPtr > createViews( VkDevice device
			, VkImage texture
			, ashes::VkBufferImageCopyArray const & copies )
		{
			std::vector< ImageViewPtr > result;
			VkImageType type = texture.getType();
			VkImageViewType viewType;

			if ( type == VK_IMAGE_TYPE_3D )
			{
				viewType = VK_IMAGE_VIEW_TYPE_3D;
			}
			else if ( type == VK_IMAGE_TYPE_2D )
			{
				viewType = VK_IMAGE_VIEW_TYPE_2D;
			}
			else if ( type == VK_IMAGE_TYPE_1D )
			{
				viewType = VK_IMAGE_VIEW_TYPE_1D;
			}

			for ( auto & copy : copies )
			{
				ashes::ImageViewCreateInfo createInfo{};
				createInfo.viewType = viewType;
				createInfo.format = texture.getFormat();
				createInfo.subresourceRange.aspectMask = ashes::getAspectMask( createInfo.format );
				createInfo.subresourceRange.baseArrayLayer = copy.imageSubresource.baseArrayLayer;
				createInfo.subresourceRange.layerCount = copy.imageSubresource.layerCount;
				createInfo.subresourceRange.baseMipLevel = copy.imageSubresource.mipLevel;
				createInfo.subresourceRange.levelCount = 1u;
				result.emplace_back( std::make_unique< ImageView >( device
					, static_cast< Image const & >( texture )
					, createInfo ) );
			}

			return result;
		}

		uint32_t getBufferRowPitch( ashes::BufferImageCopy const & copyInfo )
		{
			return ( copyInfo.bufferRowLength
				? copyInfo.bufferRowLength
				: copyInfo.imageExtent.width );
		}

		uint32_t getBufferHeightPitch( ashes::BufferImageCopy const & copyInfo )
		{
			return ( copyInfo.bufferImageHeight
				? copyInfo.bufferImageHeight
				: copyInfo.imageExtent.height );
		}

		uint32_t getBufferDepthPitch( ashes::BufferImageCopy const & copyInfo )
		{
			return copyInfo.imageExtent.depth;
		}

		uint32_t getBufferLayerPitch( ashes::BufferImageCopy const & copyInfo )
		{
			return getBufferRowPitch( copyInfo )
				* getBufferHeightPitch( copyInfo );
		}

		std::vector< D3D11_BOX > doGetDstBoxes( Image const & image
			, ashes::VkBufferImageCopyArray const & copyInfos )
		{
			std::vector< D3D11_BOX > result;

			for ( auto & copyInfo : copyInfos )
			{
				auto size = getSize( VkExtent3D
					{
						std::max( 1u, getBufferRowPitch( copyInfo ) ),
						std::max( 1u, getBufferHeightPitch( copyInfo ) ),
						std::max( 1u, getBufferDepthPitch( copyInfo ) )
					}
				, image.getFormat() );
				result.push_back(
					{
						UINT( copyInfo.bufferOffset ),
						0u,
						0u,
						UINT( copyInfo.bufferOffset + size ),
						1u,
						1u
					}
				);
			}

			return result;
		}

		std::vector< VkSubresourceLayout > doGetSrcLayouts( VkDevice device
			, Image const & image
			, ashes::VkBufferImageCopyArray const & copyInfos )
		{
			std::vector< VkSubresourceLayout > result;
			result.reserve( copyInfos.size() );

			for ( auto & copyInfo : copyInfos )
			{
				result.push_back( {} );
				device.getImageSubresourceLayout( image
					, VkImageSubresource
					{
						copyInfo.imageSubresource.aspectMask,
						copyInfo.imageSubresource.mipLevel,
						copyInfo.imageSubresource.baseArrayLayer,
					}
				, result.back() );
			}

			return result;
		}
		VkExtent3D getTexelBlockExtent( VkFormat format )
		{
			VkExtent3D texelBlockExtent{ 1u, 1u, 1u };

			if ( ashes::isCompressedFormat( format ) )
			{
				auto extent = ashes::getMinimalExtent2D( format );
				texelBlockExtent.width = extent.width;
				texelBlockExtent.height = extent.height;
			}
			else
			{
				texelBlockExtent.width = 1u;
			}

			return texelBlockExtent;
		}

		uint32_t getTexelBlockByteSize( VkExtent3D const & texelBlockExtent
			, VkFormat format )
		{
			uint32_t texelBlockSize;

			if ( !isDepthStencilFormat( format ) )
			{
				texelBlockSize = getSize( texelBlockExtent, format );
			}
			else
			{
				texelBlockSize = texelBlockExtent.width;
			}

			return texelBlockSize;
		}

		void doCopyMapped( VkFormat format
			, ashes::BufferImageCopy const & copyInfo
			, uint8_t const * srcBuffer
			, VkSubresourceLayout const & srcLayout
			, uint8_t * dstBuffer
			, D3D11_BOX const & dstBox )
		{
			auto extent = getTexelBlockExtent( format );
			auto extentCombined = extent.width * extent.height * extent.depth;
			auto byteSize = getTexelBlockByteSize( extent, format );

			auto dstSize = dstBox.right - dstBox.left;
			auto dstRowPitch = byteSize * getBufferRowPitch( copyInfo ) / extentCombined;
			auto dstHeightPitch = getBufferHeightPitch( copyInfo );
			auto dstLayerPitch = dstRowPitch * dstHeightPitch;
			auto dstDepthPitch = dstLayerPitch * getBufferDepthPitch( copyInfo );

			auto srcSize = srcLayout.size;
			auto srcRowPitch = srcLayout.rowPitch;
			auto srcLayerPitch = srcLayout.arrayPitch;
			auto srcDepthPitch = srcLayout.depthPitch;

			auto width = copyInfo.imageExtent.width;
			auto height = copyInfo.imageExtent.height;
			auto depth = copyInfo.imageExtent.depth;

			for ( auto l = 0u; l < copyInfo.imageSubresource.layerCount; ++l )
			{
				auto srcLayer = srcBuffer;
				auto dstLayer = dstBuffer;

				for ( auto z = 0u; z < depth; ++z )
				{
					auto srcPlane = srcLayer;
					auto dstPlane = dstLayer;

					for ( auto y = 0u; y < height; ++y )
					{
						std::memcpy( dstPlane
							, srcPlane
							, std::min( uint64_t( dstRowPitch ), srcRowPitch ) );
						srcPlane += srcRowPitch;
						dstPlane += dstRowPitch;
					}

					srcLayer += srcDepthPitch;
					dstLayer += dstDepthPitch;
				}

				srcBuffer += srcLayerPitch;
				dstBuffer += dstLayerPitch;
			}
		}

		VkBuffer getStagingBuffer( VkDevice device
			, VkBuffer buffer
			, uint32_t size )
		{
			VkBuffer result = std::make_unique< Buffer >( device
				, size
				, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT );
			auto requirements = result->getMemoryRequirements();
			uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
			result->bindMemory( device.allocateMemory( { requirements.size, deduced } ) );
			return result;
		}

		VkImage getStagingTexture( VkDevice device
			, VkImage image
			, VkExtent3D dimensions )
		{
			VkImage result = std::make_unique< Image >( device
				, VkImageCreateInfo
				{
					0u,
					image.getType(),
					image.getFormat(),
					VkExtent3D
				{
					dimensions.width,
					dimensions.height,
					dimensions.depth,
				},
				1u,
				image.getLayerCount(),
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_LINEAR,
				VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				{},
				VK_IMAGE_LAYOUT_UNDEFINED,
				} );
			auto requirements = result->getMemoryRequirements();
			uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
			result->bindMemory( device.allocateMemory( { requirements.size, deduced } ) );
			return result;
		}
	}

	CopyImageToBufferCommand::CopyImageToBufferCommand( VkDevice device
		, ashes::VkBufferImageCopyArray const & copyInfo
		, VkImage src
		, VkBuffer dst )
		: CommandBase{ device }
		, m_src{ static_cast< Image const & >( src ) }
		, m_dst{ static_cast< Buffer const & >( dst ) }
		, m_copyInfo{ copyInfo }
		, m_format{ getSRVFormat( m_src.getFormat() ) }
		, m_srcLayouts{ doGetSrcLayouts( device, m_src, copyInfo ) }
		, m_dstBoxes{ doGetDstBoxes( m_src, copyInfo ) }
		, m_srcMappable{ m_src.getMemoryRequirements().memoryTypeBits == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT }
		, m_dstMappable{ m_dst.getMemoryRequirements().memoryTypeBits == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT }
	{
	}

	void CopyImageToBufferCommand::apply( Context const & context )const
	{
		for ( auto i = 0u; i < m_copyInfo.size(); ++i )
		{
			applyOne( context
				, m_copyInfo[i]
				, m_srcLayouts[i]
				, m_dstBoxes[i] );
		}
	}

	void CopyImageToBufferCommand::applyOne( Context const & context
		, ashes::BufferImageCopy const & copyInfo
		, VkSubresourceLayout const & srcLayout
		, D3D11_BOX const & dstBox )const
	{
		VkImage stagingSrc;
		VkBuffer stagingDst;
		VkImage const * src = &m_src;
		VkBuffer const * dst = &m_dst;

		if ( !m_srcMappable )
		{
			stagingSrc = getStagingTexture( m_device
				, m_src
				, copyInfo.imageExtent );
			doCopyToStaging( context
				, copyInfo
				, m_src
				, *stagingSrc );
			src = stagingSrc.get();
		}

		if ( !m_dstMappable )
		{
			stagingDst = getStagingBuffer( m_device
				, m_dst
				, dstBox.right - dstBox.left );
			dst = stagingDst.get();
		}

		doMapCopy( copyInfo
			, srcLayout
			, dstBox
			, *src
			, *dst );

		if ( !m_dstMappable )
		{
			doCopyFromStaging( context
				, copyInfo
				, *stagingDst
				, m_dst
				, dstBox );
		}
	}

	CommandPtr CopyImageToBufferCommand::clone()const
	{
		return std::make_unique< CopyImageToBufferCommand >( *this );
	}

	void CopyImageToBufferCommand::doMapCopy( ashes::BufferImageCopy const & copyInfo
		, VkSubresourceLayout const & srcLayout
		, D3D11_BOX const & dstBox
		, VkImage src
		, VkBuffer dst )const
	{
		if ( auto srcBuffer = src.lock( srcLayout
			, VkMemoryMapFlagBits::eRead ) )
		{
			if ( auto dstBuffer = dst.lock( dstBox.left
				, dstBox.right - dstBox.left
				, VkMemoryMapFlagBits::eWrite ) )
			{
				doCopyMapped( src.getFormat()
					, copyInfo
					, srcBuffer
					, srcLayout
					, dstBuffer
					, dstBox );
				dst.flush( dstBox.left
					, dstBox.right - dstBox.left );
				dst.unlock();
			}

			src.flush( srcLayout );
			src.unlock();
		}
	}

	void CopyImageToBufferCommand::doCopyToStaging( Context const & context
		, ashes::BufferImageCopy const & copyInfo
		, VkImage src
		, VkImage staging )const
	{
		ashes::ImageSubresourceLayers stagingSurbresouce{ copyInfo.imageSubresource };
		stagingSurbresouce.mipLevel = 0u;
		stagingSurbresouce.baseArrayLayer = 0u;
		CopyImageCommand command{ m_device
			, ashes::ImageCopy
			{
				copyInfo.imageSubresource,
				copyInfo.imageOffset,
				stagingSurbresouce,
				VkOffset3D{},
				copyInfo.imageExtent,
			}
			, src
			, staging };
		command.apply( context );
	}

	void CopyImageToBufferCommand::doCopyFromStaging( Context const & context
		, ashes::BufferImageCopy const & copyInfo
		, VkBuffer staging
		, VkBuffer dst
		, D3D11_BOX const & dstBox )const
	{
		CopyBufferCommand command{ m_device
			, ashes::BufferCopy
			{
				0u,
				copyInfo.bufferOffset,
				dstBox.right - dstBox.left
			}
			, staging
			, dst };
		command.apply( context );
	}
}
