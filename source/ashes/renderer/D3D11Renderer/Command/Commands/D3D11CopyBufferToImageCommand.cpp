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
#include "Miscellaneous/D3D11DeviceMemory.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	namespace
	{
		uint32_t getBufferRowPitch( VkBufferImageCopy const & copyInfo )
		{
			return ( copyInfo.bufferRowLength
				? copyInfo.bufferRowLength
				: copyInfo.imageExtent.width );
		}
		
		uint32_t getBufferHeightPitch( VkBufferImageCopy const & copyInfo )
		{
			return ( copyInfo.bufferImageHeight
				? copyInfo.bufferImageHeight
				: copyInfo.imageExtent.height );
		}

		uint32_t getBufferDepthPitch( VkBufferImageCopy const & copyInfo )
		{
			return copyInfo.imageExtent.depth;
		}
		
		uint32_t getBufferLayerPitch( VkBufferImageCopy const & copyInfo )
		{
			return getBufferRowPitch( copyInfo )
				* getBufferHeightPitch( copyInfo );
		}

		std::vector< D3D11_BOX > doGetSrcBoxes( VkImage image
			, VkBufferImageCopyArray const & copyInfos )
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
					, get( image )->getFormat() );
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

		std::vector< VkSubresourceLayout > doGetDstLayouts( VkDevice device
			, VkImage image
			, VkBufferImageCopyArray const & copyInfos )
		{
			std::vector< VkSubresourceLayout > result;
			result.reserve( copyInfos.size() );

			for ( auto & copyInfo : copyInfos )
			{
				result.push_back( {} );
				get( device )->getImageSubresourceLayout( image
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
			, VkBufferImageCopy const & copyInfo
			, uint8_t const * srcBuffer
			, D3D11_BOX const & srcBox
			, uint8_t * dstBuffer
			, VkSubresourceLayout const & dstLayout )
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

		VkBuffer getStagingBuffer( VkDevice device
			, VkBuffer buffer
			, uint32_t size
			, VkDeviceMemory & memory )
		{
			VkBuffer result;
			allocate( result
				, nullptr
				, device
				, VkBufferCreateInfo
				{
					VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
					nullptr,
					0u,
					size,
					VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
					VK_SHARING_MODE_EXCLUSIVE,
					0u,
					nullptr,
				} );
			auto requirements = get( result )->getMemoryRequirements();
			uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
			allocate( memory
				, nullptr
				, device
				, VkMemoryAllocateInfo
				{
					VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
					nullptr,
					requirements.size,
					deduced
				} );
			get( result )->bindMemory( memory, 0u );
			return result;
		}

		VkImage getStagingTexture( VkDevice device
			, VkImage image
			, VkExtent3D dimensions
			, VkDeviceMemory & memory )
		{
			VkImage result;
			allocate( result
				, nullptr
				, device
				, VkImageCreateInfo
				{
					VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
					nullptr,
					0u,
					get( image )->getType(),
					get( image )->getFormat(),
					VkExtent3D
					{
						dimensions.width,
						dimensions.height,
						dimensions.depth,
					},
					1u,
					get( image )->getLayerCount(),
					VK_SAMPLE_COUNT_1_BIT,
					VK_IMAGE_TILING_LINEAR,
					VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
					VK_SHARING_MODE_EXCLUSIVE,
					0u,
					nullptr,
					VK_IMAGE_LAYOUT_UNDEFINED,
				} );
			auto requirements = get( result )->getMemoryRequirements();
			uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
			allocate( memory
				, nullptr
				, device
				, VkMemoryAllocateInfo
				{
					VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
					nullptr,
					requirements.size,
					deduced
				} );
			get( result )->bindMemory( memory, 0u );
			return result;
		}
	}

	CopyBufferToImageCommand::CopyBufferToImageCommand( VkDevice device
		, VkBufferImageCopyArray const & copyInfo
		, VkBuffer src
		, VkImage dst )
		: CommandBase{ device }
		, m_copyInfo{ copyInfo }
		, m_src{ src }
		, m_dst{ dst }
		, m_format{ getSRVFormat( get( m_dst )->getFormat() ) }
		, m_srcBoxes{ doGetSrcBoxes( m_dst, copyInfo ) }
		, m_dstLayouts{ doGetDstLayouts( device, m_dst, copyInfo ) }
		, m_srcMappable{ get( m_src )->getMemoryRequirements().memoryTypeBits == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT }
		, m_dstMappable{ get( m_dst )->getMemoryRequirements().memoryTypeBits == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT }
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
		, VkBufferImageCopy const & copyInfo
		, D3D11_BOX const & srcBox
		, VkSubresourceLayout const & dstLayout )const
	{
		VkBuffer stagingSrc{ VK_NULL_HANDLE };
		VkImage stagingDst{ VK_NULL_HANDLE };
		VkDeviceMemory stagingSrcMemory{ VK_NULL_HANDLE };
		VkDeviceMemory stagingDstMemory{ VK_NULL_HANDLE };
		VkBuffer const * src = &m_src;
		VkImage const * dst = &m_dst;

		if ( !m_srcMappable )
		{
			stagingSrc = getStagingBuffer( m_device
				, m_src
				, srcBox.right - srcBox.left
				, stagingSrcMemory );
			doCopyToStaging( context
				, copyInfo
				, *src
				, stagingSrc
				, srcBox );
			src = &stagingSrc;
		}

		if ( !m_dstMappable )
		{
			stagingDst = getStagingTexture( m_device
				, m_dst
				, copyInfo.imageExtent
				, stagingDstMemory );
			dst = &stagingDst;
		}

		doMapCopy( copyInfo
			, srcBox
			, dstLayout
			, get( *dst )->getFormat()
			, get( *src )->getMemory()
			, get( *dst )->getMemory() );

		if ( !m_dstMappable )
		{
			doCopyFromStaging( context
				, copyInfo
				, stagingDst
				, m_dst );
		}

		if ( stagingDst )
		{
			deallocate( stagingDstMemory, nullptr );
			deallocate( stagingDst, nullptr );
		}

		if ( stagingSrc )
		{
			deallocate( stagingSrcMemory, nullptr );
			deallocate( stagingSrc, nullptr );
		}
	}

	void CopyBufferToImageCommand::doMapCopy( VkBufferImageCopy const & copyInfo
		, D3D11_BOX const & srcBox
		, VkSubresourceLayout const & dstLayout
		, VkFormat format
		, VkDeviceMemory src
		, VkDeviceMemory dst )const
	{
		uint8_t * srcBuffer{ nullptr };

		if ( VK_SUCCESS == get( src )->lock( srcBox.left
			, srcBox.right - srcBox.left
			, 0u 
			, reinterpret_cast< void ** >( &srcBuffer ) ) )
		{
			uint8_t * dstBuffer{ nullptr };

			if ( VK_SUCCESS == get( dst )->lock( dstLayout.offset
				, dstLayout.size
				, 0u
				, reinterpret_cast< void ** >( &dstBuffer ) ) )
			{
				doCopyMapped( format
					, copyInfo
					, srcBuffer
					, srcBox
					, dstBuffer
					, dstLayout );
				get( dst )->flush( dstLayout.offset
					, dstLayout.size );
				get( dst )->unlock();
			}

			get( src )->flush( srcBox.left
				, srcBox.right - srcBox.left );
			get( src )->unlock();
		}
	}

	void CopyBufferToImageCommand::doCopyToStaging( Context const & context
		, VkBufferImageCopy const & copyInfo
		, VkBuffer src
		, VkBuffer staging
		, D3D11_BOX const & srcBox )const
	{
		CopyBufferCommand command{ m_device
			, VkBufferCopy
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
		, VkBufferImageCopy const & copyInfo
		, VkImage staging
		, VkImage dst )const
	{
		VkImageSubresourceLayers stagingSurbresouce{ copyInfo.imageSubresource };
		stagingSurbresouce.mipLevel = 0u;
		stagingSurbresouce.baseArrayLayer = 0u;
		CopyImageCommand command{ m_device
			, VkImageCopy
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
