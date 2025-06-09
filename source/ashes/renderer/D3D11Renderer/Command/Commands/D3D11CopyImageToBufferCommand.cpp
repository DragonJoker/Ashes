/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CopyImageToBufferCommand.hpp"
#include "Command/Commands/D3D11CopyBufferCommand.hpp"
#include "Command/Commands/D3D11CopyImageCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Core/D3D11Device.hpp"
#include "Image/D3D11Image.hpp"
#include "Image/D3D11ImageView.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::D3D11_NAMESPACE
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

		std::vector< D3D11_BOX > doGetDstBoxes( VkImage image
			, ArrayView< VkBufferImageCopy const > const & copyInfos )
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

		std::vector< VkSubresourceLayout > doGetSrcLayouts( VkDevice device
			, VkImage image
			, ArrayView< VkBufferImageCopy const > const & copyInfos )
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
			VkDeviceSize texelBlockSize;

			if ( !isDepthStencilFormat( format ) )
			{
				texelBlockSize = getSize( texelBlockExtent, format );
			}
			else
			{
				texelBlockSize = texelBlockExtent.width;
			}

			return uint32_t( texelBlockSize );
		}

		void doCopyMapped( VkFormat format
			, VkBufferImageCopy const & copyInfo
			, uint8_t const * srcBuffer
			, VkSubresourceLayout const & srcLayout
			, uint8_t * dstBuffer )
		{
			auto extent = getTexelBlockExtent( format );
			auto extentCombined = extent.width * extent.height * extent.depth;
			auto byteSize = getTexelBlockByteSize( extent, format );

			auto dstRowPitch = byteSize * getBufferRowPitch( copyInfo ) / extentCombined;
			auto dstHeightPitch = getBufferHeightPitch( copyInfo );
			auto dstLayerPitch = dstRowPitch * dstHeightPitch;
			auto dstDepthPitch = dstLayerPitch * getBufferDepthPitch( copyInfo );

			auto srcRowPitch = srcLayout.rowPitch;
			auto srcLayerPitch = srcLayout.arrayPitch;
			auto srcDepthPitch = srcLayout.depthPitch;

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
			, uint32_t size
			, VkDeviceMemory & memory )
		{
			VkBuffer result;
			allocate( result
				, get( device )->getAllocationCallbacks()
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
				, get( device )->getAllocationCallbacks()
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
				, get( device )->getAllocationCallbacks()
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
				, get( device )->getAllocationCallbacks()
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

	CopyImageToBufferCommand::CopyImageToBufferCommand( VkDevice device
		, ArrayView< VkBufferImageCopy const > const & copyInfo
		, VkImage src
		, VkBuffer dst )
		: CommandBase{ device }
		, m_src{ src }
		, m_dst{ dst }
		, m_copyInfo{ copyInfo.begin(), copyInfo.end() }
		, m_format{ getSRVFormat( get( m_src )->getFormat() ) }
		, m_srcLayouts{ doGetSrcLayouts( device, m_src, copyInfo ) }
		, m_dstBoxes{ doGetDstBoxes( m_src, copyInfo ) }
		, m_srcMappable{ get( m_src )->getMemoryRequirements().memoryTypeBits == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT }
		, m_dstMappable{ get( m_dst )->getMemoryRequirements().memoryTypeBits == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT }
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
		, VkBufferImageCopy const & copyInfo
		, VkSubresourceLayout const & srcLayout
		, D3D11_BOX const & dstBox )const
	{
		VkImage stagingSrc{};
		VkBuffer stagingDst{};
		VkDeviceMemory stagingSrcMemory{};
		VkDeviceMemory stagingDstMemory{};
		VkImage const * src = &m_src;
		VkBuffer const * dst = &m_dst;

		if ( !m_srcMappable )
		{
			stagingSrc = getStagingTexture( getDevice()
				, m_src
				, copyInfo.imageExtent
				, stagingSrcMemory );
			doCopyToStaging( context
				, copyInfo
				, m_src
				, stagingSrc );
			src = &stagingSrc;
		}

		if ( !m_dstMappable )
		{
			stagingDst = getStagingBuffer( getDevice()
				, dstBox.right - dstBox.left
				, stagingDstMemory );
			dst = &stagingDst;
		}

		doMapCopy( copyInfo
			, srcLayout
			, dstBox
			, get( *src )->getFormat()
			, get( *src )->getMemory()
			, get( *dst )->getMemory() );

		if ( !m_dstMappable )
		{
			doCopyFromStaging( context
				, copyInfo
				, stagingDst
				, m_dst
				, dstBox );
		}

		if ( stagingDst )
		{
			deallocate( stagingDstMemory, get( getDevice() )->getAllocationCallbacks() );
			deallocate( stagingDst, get( getDevice() )->getAllocationCallbacks() );
		}

		if ( stagingSrc )
		{
			deallocate( stagingSrcMemory, get( getDevice() )->getAllocationCallbacks() );
			deallocate( stagingSrc, get( getDevice() )->getAllocationCallbacks() );
		}
	}

	CommandPtr CopyImageToBufferCommand::clone()const
	{
		return std::make_unique< CopyImageToBufferCommand >( *this );
	}

	void CopyImageToBufferCommand::doMapCopy( VkBufferImageCopy const & copyInfo
		, VkSubresourceLayout const & srcLayout
		, D3D11_BOX const & dstBox
		, VkFormat format
		, VkDeviceMemory src
		, VkDeviceMemory dst )const
	{
		if ( uint8_t * srcBuffer{};
			VK_SUCCESS == get( src )->lock( srcLayout.offset
				, srcLayout.size
				, 0u
				, reinterpret_cast< void ** >( &srcBuffer ) ) )
		{
			if ( uint8_t * dstBuffer{};
				VK_SUCCESS == get( dst )->lock( dstBox.left
					, VkDeviceSize( dstBox.right ) - dstBox.left
					, 0u
					, reinterpret_cast< void ** >( &dstBuffer ) ) )
			{
				doCopyMapped( format
					, copyInfo
					, srcBuffer
					, srcLayout
					, dstBuffer );
				get( dst )->flush( dstBox.left
					, VkDeviceSize( dstBox.right ) - dstBox.left );
				get( dst )->unlock();
			}

			get( src )->flush( srcLayout.offset
				, srcLayout.size );
			get( src )->unlock();
		}
	}

	void CopyImageToBufferCommand::doCopyToStaging( Context const & context
		, VkBufferImageCopy const & copyInfo
		, VkImage src
		, VkImage staging )const
	{
		VkImageSubresourceLayers stagingSurbresouce{ copyInfo.imageSubresource };
		stagingSurbresouce.mipLevel = 0u;
		stagingSurbresouce.baseArrayLayer = 0u;
		CopyImageCommand command{ getDevice()
			, VkImageCopy
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
		, VkBufferImageCopy const & copyInfo
		, VkBuffer staging
		, VkBuffer dst
		, D3D11_BOX const & dstBox )const
	{
		CopyBufferCommand command{ getDevice()
			, VkBufferCopy
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
