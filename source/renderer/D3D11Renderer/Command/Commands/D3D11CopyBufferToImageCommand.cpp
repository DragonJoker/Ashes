/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CopyBufferToImageCommand.hpp"

#include "Command/Commands/D3D11CopyBufferCommand.hpp"
#include "Command/Commands/D3D11CopyImageCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Image/D3D11Image.hpp"
#include "Image/D3D11ImageView.hpp"

#include <Ashes/Image/ImageSubresource.hpp>
#include <Ashes/Image/ImageSubresourceRange.hpp>
#include <Ashes/Image/SubresourceLayout.hpp>

namespace ashes::d3d11
{
	namespace
	{
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

		std::vector< D3D11_BOX > doGetSrcBoxes( Image const & image
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

		std::vector< ashes::SubresourceLayout > doGetDstLayouts( Device const & device
			, Image const & image
			, ashes::VkBufferImageCopyArray const & copyInfos )
		{
			std::vector< ashes::SubresourceLayout > result;
			result.reserve( copyInfos.size() );

			for ( auto & copyInfo : copyInfos )
			{
				result.push_back( {} );
				device.getImageSubresourceLayout( image
					, ashes::ImageSubresource
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
			, D3D11_BOX const & srcBox
			, uint8_t * dstBuffer
			, ashes::SubresourceLayout const & dstLayout )
		{
			auto extent = getTexelBlockExtent( format );
			auto extentCombined = extent.width * extent.height * extent.depth;
			auto byteSize = getTexelBlockByteSize( extent, format );
			auto bufferSize = srcBox.right - srcBox.left;
			auto bufferRowPitch = byteSize * getBufferRowPitch( copyInfo ) / extentCombined;
			//auto bufferHeightPitch = byteSize * getBufferHeightPitch( copyInfo ) / extentCombined;
			auto bufferHeightPitch = getBufferHeightPitch( copyInfo );
			auto bufferLayerPitch = bufferRowPitch * bufferHeightPitch;
			auto bufferDepthPitch = bufferLayerPitch * getBufferDepthPitch( copyInfo );
			auto imageSize = dstLayout.size;
			auto imageRowPitch = dstLayout.rowPitch;
			auto imageLayerPitch = dstLayout.arrayPitch;
			auto imageDepthPitch = dstLayout.depthPitch;
			auto imageWidth = copyInfo.imageExtent.width;
			auto imageHeight = copyInfo.imageExtent.height;
			auto imageDepth = copyInfo.imageExtent.depth;

			for ( auto l = 0u; l < copyInfo.imageSubresource.layerCount; ++l )
			{
				auto srcLayer = srcBuffer;
				auto dstLayer = dstBuffer;

				for ( auto z = 0u; z < imageDepth; ++z )
				{
					auto srcPlane = srcLayer;
					auto dstPlane = dstLayer;

					for ( auto y = 0u; y < imageHeight; ++y )
					{
						std::memcpy( dstPlane
							, srcPlane
							, std::min( uint64_t( bufferRowPitch ), imageRowPitch ) );
						srcPlane += bufferRowPitch;
						dstPlane += imageRowPitch;
					}

					srcLayer += bufferDepthPitch;
					dstLayer += imageDepthPitch;
				}

				srcBuffer += bufferLayerPitch;
				dstBuffer += imageLayerPitch;
			}
		}

		ashes::BufferBasePtr getStagingBuffer( Device const & device
			, ashes::BufferBase const & buffer
			, uint32_t size )
		{
			ashes::BufferBasePtr result = std::make_unique< Buffer >( device
				, size
				, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT );
			auto requirements = result->getMemoryRequirements();
			uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
			result->bindMemory( device.allocateMemory( { requirements.size, deduced } ) );
			return result;
		}

		ashes::ImagePtr getStagingTexture( Device const & device
			, ashes::Image const & image
			, VkExtent3D dimensions )
		{
			ashes::ImagePtr result = std::make_unique< Image >( device
				, ashes::ImageCreateInfo
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

	CopyBufferToImageCommand::CopyBufferToImageCommand( Device const & device
		, ashes::VkBufferImageCopyArray const & copyInfo
		, ashes::BufferBase const & src
		, ashes::Image const & dst )
		: CommandBase{ device }
		, m_copyInfo{ copyInfo }
		, m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Image const & >( dst ) }
		, m_format{ getSRVFormat( m_dst.getFormat() ) }
		, m_srcBoxes{ doGetSrcBoxes( m_dst, copyInfo ) }
		, m_dstLayouts{ doGetDstLayouts( device, m_dst, copyInfo ) }
		, m_srcMappable{ m_src.getMemoryRequirements().memoryTypeBits == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT }
		, m_dstMappable{ m_dst.getMemoryRequirements().memoryTypeBits == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT }
	{
	}

	void CopyBufferToImageCommand::apply( Context const & context )const
	{
		for ( auto i = 0u; i < m_copyInfo.size(); ++i )
		{
			applyOne( context
				, m_copyInfo[i]
				, m_srcBoxes[i]
				, m_dstLayouts[i] );
		}
	}

	CommandPtr CopyBufferToImageCommand::clone()const
	{
		return std::make_unique< CopyBufferToImageCommand >( *this );
	}

	void CopyBufferToImageCommand::applyOne( Context const & context
		, ashes::BufferImageCopy const & copyInfo
		, D3D11_BOX const & srcBox
		, ashes::SubresourceLayout const & dstLayout )const
	{
		ashes::BufferBasePtr stagingSrc;
		ashes::ImagePtr stagingDst;
		ashes::BufferBase const * src = &m_src;
		ashes::Image const * dst = &m_dst;

		if ( !m_srcMappable )
		{
			stagingSrc = getStagingBuffer( m_device
				, m_src
				, srcBox.right - srcBox.left );
			doCopyToStaging( context
				, copyInfo
				, *src
				, *stagingSrc
				, srcBox );
			src = stagingSrc.get();
		}

		if ( !m_dstMappable )
		{
			stagingDst = getStagingTexture( m_device
				, m_dst
				, copyInfo.imageExtent );
			dst = stagingDst.get();
		}

		doMapCopy( copyInfo
			, srcBox
			, dstLayout
			, *src
			, *dst );

		if ( !m_dstMappable )
		{
			doCopyFromStaging( context
				, copyInfo
				, *stagingDst
				, m_dst );
		}
	}

	void CopyBufferToImageCommand::doMapCopy( ashes::BufferImageCopy const & copyInfo
		, D3D11_BOX const & srcBox
		, ashes::SubresourceLayout const & dstLayout
		, ashes::BufferBase const & src
		, ashes::Image const & dst )const
	{
		if ( auto srcBuffer = src.lock( srcBox.left
			, srcBox.right - srcBox.left
			, VkMemoryMapFlagBits::eRead ) )
		{
			if ( auto dstBuffer = dst.lock( dstLayout
				, VkMemoryMapFlagBits::eWrite ) )
			{
				doCopyMapped( dst.getFormat()
					, copyInfo
					, srcBuffer
					, srcBox
					, dstBuffer
					, dstLayout );
				dst.flush( dstLayout );
				dst.unlock();
			}

			src.flush( srcBox.left
				, srcBox.right - srcBox.left );
			src.unlock();
		}
	}

	void CopyBufferToImageCommand::doCopyToStaging( Context const & context
		, ashes::BufferImageCopy const & copyInfo
		, ashes::BufferBase const & src
		, ashes::BufferBase const & staging
		, D3D11_BOX const & srcBox )const
	{
		CopyBufferCommand command{ m_device
			, ashes::BufferCopy
			{
				copyInfo.bufferOffset,
				0u,
				srcBox.right - srcBox.left
			}
			, src
			, staging };
		command.apply( context );
	}

	void CopyBufferToImageCommand::doCopyFromStaging( Context const & context
		, ashes::BufferImageCopy const & copyInfo
		, ashes::Image const & staging
		, ashes::Image const & dst )const
	{
		ashes::ImageSubresourceLayers stagingSurbresouce{ copyInfo.imageSubresource };
		stagingSurbresouce.mipLevel = 0u;
		stagingSurbresouce.baseArrayLayer = 0u;
		CopyImageCommand command{ m_device
			, ashes::ImageCopy
			{
				stagingSurbresouce,
				VkOffset3D{},
				copyInfo.imageSubresource,
				copyInfo.imageOffset,
				copyInfo.imageExtent,
			}
			, staging
			, dst };
		command.apply( context );
	}
}
