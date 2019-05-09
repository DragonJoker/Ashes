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

		VkDeviceSize doGetBufferSize( VkFormat format
			, VkBufferImageCopy const & copyInfo )
		{
			VkExtent3D bufferPitch
			{
				std::max( 1u, getBufferRowPitch( copyInfo ) ),
				std::max( 1u, getBufferHeightPitch( copyInfo ) ),
				std::max( 1u, getBufferDepthPitch( copyInfo ) )
			};
			return getSize( bufferPitch, format );
		}

		D3D11_BOX doGetSrcBox( VkFormat format
			, VkBufferImageCopy const & copyInfo )
		{

			return
			{
				UINT( copyInfo.bufferOffset ),
				0u,
				0u,
				UINT( copyInfo.bufferOffset + doGetBufferSize( format, copyInfo ) ),
				1u,
				1u
			};
		}

		VkSubresourceLayout doGetDstLayout( VkDevice device
			, VkImage image
			, VkBufferImageCopy const & copyInfo )
		{
			VkSubresourceLayout layout{};
			get( device )->getImageSubresourceLayout( image
				, VkImageSubresource
				{
					copyInfo.imageSubresource.aspectMask,
					copyInfo.imageSubresource.mipLevel,
					copyInfo.imageSubresource.baseArrayLayer,
				}
				, layout );
			return layout;
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
			, D3D11_MAPPED_SUBRESOURCE srcBuffer
			, D3D11_BOX const & srcBox
			, D3D11_MAPPED_SUBRESOURCE dstBuffer
			, VkSubresourceLayout const & dstLayout )
		{
			auto extent = getTexelBlockExtent( format );
			auto extentCombined = extent.width * extent.height * extent.depth;
			auto byteSize = getTexelBlockByteSize( extent, format );
			auto bufferSize = srcBox.right - srcBox.left;
			auto bufferRowPitch = srcBuffer.RowPitch;
			auto bufferHeightPitch = getBufferHeightPitch( copyInfo );
			auto bufferDepthPitch = srcBuffer.DepthPitch;
			auto bufferLayerPitch = bufferDepthPitch;
			auto imageSize = dstLayout.size;
			auto imageRowPitch = dstBuffer.RowPitch;
			auto imageLayerPitch = dstLayout.arrayPitch;
			auto imageDepthPitch = dstBuffer.DepthPitch;
			auto imageWidth = copyInfo.imageExtent.width;
			auto imageHeight = copyInfo.imageExtent.height;
			auto imageDepth = copyInfo.imageExtent.depth;
			auto imageLayers = copyInfo.imageSubresource.layerCount;
			auto srcData = reinterpret_cast< uint8_t * >( srcBuffer.pData );
			auto dstData = reinterpret_cast< uint8_t * >( dstBuffer.pData );

			if ( imageRowPitch >= imageSize )
			{
				bufferRowPitch = getTexelBlockByteSize( copyInfo.imageExtent, format ) / ( imageHeight * imageDepth * imageLayers );
				bufferDepthPitch = bufferRowPitch * imageHeight;
				bufferLayerPitch = bufferDepthPitch;

				for ( auto l = 0u; l < imageLayers; ++l )
				{
					auto srcLayer = srcData;
					auto dstLayer = dstData;

					for ( auto z = 0u; z < imageDepth; ++z )
					{
						auto srcPlane = srcLayer;
						auto dstPlane = dstLayer;

						for ( auto y = 0u; y < imageHeight; ++y )
						{
							std::memcpy( dstPlane
								, srcPlane
								, std::min( bufferRowPitch, imageRowPitch ) );
							srcPlane += bufferRowPitch;
							dstPlane += imageRowPitch;
						}

						srcLayer += bufferDepthPitch;
						dstLayer += imageDepthPitch;
					}

					srcData += bufferLayerPitch;
					dstData += imageLayerPitch;
				}

				return;
			}

			for ( auto l = 0u; l < imageLayers; ++l )
			{
				auto srcLayer = srcData;
				auto dstLayer = dstData;

				for ( auto z = 0u; z < imageDepth; ++z )
				{
					auto srcPlane = srcLayer;
					auto dstPlane = dstLayer;

					if ( bufferRowPitch == imageRowPitch
						|| bufferRowPitch == bufferSize
						|| ( bufferRowPitch > imageRowPitch && ( bufferRowPitch % imageRowPitch ) )
						|| ( imageRowPitch > bufferRowPitch && ( imageRowPitch % bufferRowPitch ) ) )
					{
						if ( bufferRowPitch == bufferSize )
						{
							bufferRowPitch /= imageHeight;
							assert( bufferSize == ( bufferRowPitch * imageHeight ) );
						}

						for ( auto y = 0u; y < imageHeight; ++y )
						{
							std::memcpy( dstPlane
								, srcPlane
								, std::min( bufferRowPitch, imageRowPitch ) );
							srcPlane += bufferRowPitch;
							dstPlane += imageRowPitch;
						}
					}
					else if ( bufferRowPitch > imageRowPitch )
					{
						// bufferRowPitch = n * imageRowPitch
						for ( auto y = 0u; y < imageHeight; ++y )
						{
							std::memcpy( dstPlane
								, srcPlane
								, bufferRowPitch );
							srcPlane += bufferRowPitch;
							dstPlane += bufferRowPitch;
						}
					}
					else
					{
						// imageRowPitch = n * bufferRowPitch
						for ( auto y = 0u; y < imageHeight; ++y )
						{
							std::memcpy( dstPlane
								, srcPlane
								, imageRowPitch );
							srcPlane += imageRowPitch;
							dstPlane += imageRowPitch;
						}
					}

					srcLayer += bufferDepthPitch;
					dstLayer += imageDepthPitch;
				}

				srcData += bufferLayerPitch;
				dstData += imageLayerPitch;
			}
		}

		VkBuffer getStagingBuffer( VkDevice device
			, VkBuffer buffer
			, VkDeviceSize size
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
					get( image )->getDimensions(),
					get( image )->getMipmapLevels(),
					1u, // layerCount, we process images layer per layer.
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

		VkBufferCopy doGetCopyToStaging( VkFormat format
			, VkBufferImageCopyArray const & copyInfos )
		{
			static auto constexpr MaxValue = std::numeric_limits< VkDeviceSize >::max();
			static auto constexpr MinValue = std::numeric_limits< VkDeviceSize >::lowest();
			VkDeviceSize minOffset = MaxValue;
			VkDeviceSize maxSize = MinValue;

			for ( auto & copyInfo : copyInfos )
			{
				auto size = doGetBufferSize( format, copyInfo );
				minOffset = std::min( minOffset, copyInfo.bufferOffset );
				maxSize = std::max( maxSize, copyInfo.bufferOffset + size );
			}

			return VkBufferCopy
			{
				minOffset,
				0u,
				maxSize - minOffset,
			};
		}

		Optional< CopyToStagingProcess > getCopyToStagingProcess( VkDevice device
			, VkImage dst
			, VkBufferImageCopyArray const & copyInfos
			, bool srcMappable
			, VkBuffer & src
			, VkBufferImageCopy & mapCopyInfo )
		{
			Optional< CopyToStagingProcess > result = std::nullopt;

			if ( !srcMappable )
			{
				auto copyToStaging = doGetCopyToStaging( get( dst )->getFormat(), copyInfos );
				CopyToStagingProcess process;
				process.copyToStaging = copyToStaging;
				process.stagingSrc = getStagingBuffer( device
					, src
					, process.copyToStaging.size
					, process.stagingSrcMemory );

				mapCopyInfo.bufferOffset = copyToStaging.srcOffset;
				src = process.stagingSrc;
				result = process;
			}
			else
			{
				mapCopyInfo.bufferOffset = 0u;
			}

			return result;
		}

		void getCopyFromStagingProcess( VkDevice device
			, VkImage image
			, bool dstMappable
			, VkBufferImageCopy bufferImageCopy
			, MapCopyImage & mapCopyImage )
		{
			if ( !dstMappable )
			{
				auto stagingImageSubresource = bufferImageCopy.imageSubresource;
				stagingImageSubresource.layerCount = 1u;
				stagingImageSubresource.baseArrayLayer = 0u;

				CopyFromStagingProcess process;
				process.stagingDst = getStagingTexture( device
					, image
					, process.stagingDstMemory );

				mapCopyImage.dst = process.stagingDst;
				mapCopyImage.dstMemory = process.stagingDstMemory;
				mapCopyImage.dstSubresource = D3D11CalcSubresource( stagingImageSubresource.mipLevel
					, stagingImageSubresource.baseArrayLayer
					, 1u );
				process.stagingDstSubresource = mapCopyImage.dstSubresource;

				process.copyFromStaging.srcOffset = bufferImageCopy.imageOffset;
				process.copyFromStaging.srcSubresource = stagingImageSubresource;

				process.copyFromStaging.dstOffset = bufferImageCopy.imageOffset;
				process.copyFromStaging.dstSubresource = bufferImageCopy.imageSubresource;
				assert( process.copyFromStaging.dstSubresource.layerCount == 1u );

				process.copyFromStaging.extent = bufferImageCopy.imageExtent;

				// We copy to layer 0 of staging image (it has only 1 layer).
				bufferImageCopy.imageSubresource.baseArrayLayer = 0u;
				mapCopyImage.dstLayout = doGetDstLayout( device, mapCopyImage.dst, bufferImageCopy );
				mapCopyImage.copyFromStaging = process;
			}
		}

		MapCopyProcess getMapCopyProcess( VkDevice device
			, VkBuffer src
			, VkImage dst
			, Optional< CopyToStagingProcess > const & copyToStaging
			, VkBufferImageCopyArray const & copyInfos
			, VkBufferImageCopy const & baseBufferImageCopy
			, bool dstMappable )
		{
			MapCopyProcess result
			{
				src,
				get( src )->getMemory(),
			};

			for ( auto & copy : copyInfos )
			{
				auto copyInfo = copy;
				copyInfo.imageSubresource.layerCount = 1u;
				copyInfo.bufferOffset -= baseBufferImageCopy.bufferOffset;
				auto layerPitch = getBufferRowPitch( copyInfo ) * getBufferHeightPitch( copyInfo );

				for ( uint32_t layer = 0u; layer < copy.imageSubresource.layerCount; ++layer )
				{
					auto srcBox = doGetSrcBox( get( dst )->getFormat(), copyInfo );
					auto dstLayout = doGetDstLayout( device, dst, copyInfo );
					MapCopyImage mapCopyImage
					{
						dst,
						get( dst )->getMemory(),
						copyInfo,
						srcBox,
						D3D11CalcSubresource( copyInfo.imageSubresource.mipLevel
							, copyInfo.imageSubresource.baseArrayLayer
							, 1u ),
						dstLayout,
					};
					getCopyFromStagingProcess( device
						, dst
						, dstMappable
						, copyInfo
						, mapCopyImage );
					result.mapCopyImages.push_back( mapCopyImage );
					copyInfo.imageSubresource.baseArrayLayer++;
					copyInfo.bufferOffset += layerPitch;
				}
			}

			return result;
		}

		BufferToImageCopyProcess buildProcess( VkDevice device
			, VkBufferImageCopyArray const & copyInfos
			, VkBuffer src
			, VkImage dst
			, bool srcMappable
			, bool dstMappable )
		{
			VkBufferImageCopy baseBufferImageCopy;
			auto copyToStaging = getCopyToStagingProcess( device
				, dst
				, copyInfos
				, srcMappable
				, src
				, baseBufferImageCopy );

			auto mapCopy = getMapCopyProcess( device
				, src
				, dst
				, copyToStaging
				, copyInfos
				, baseBufferImageCopy
				, dstMappable );
			return BufferToImageCopyProcess
			{
				copyToStaging,
				mapCopy,
			};
		}
	}

	CopyBufferToImageCommand::CopyBufferToImageCommand( VkDevice device
		, VkBufferImageCopyArray const & copyInfos
		, VkBuffer src
		, VkImage dst )
		: CommandBase{ device }
		, m_src{ src }
		, m_dst{ dst }
		, m_format{ getSRVFormat( get( m_dst )->getFormat() ) }
		, m_srcMappable{ get( m_src )->getMemoryRequirements().memoryTypeBits == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT }
		, m_dstMappable{ get( m_dst )->getMemoryRequirements().memoryTypeBits == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT }
		, m_process{ buildProcess ( device, copyInfos, src, dst, m_srcMappable, m_dstMappable ) }
	{
	}

	CopyBufferToImageCommand::~CopyBufferToImageCommand()
	{
		for ( auto & mapCopies : m_process.mapCopy.mapCopyImages )
		{
			if ( mapCopies.copyFromStaging )
			{
				deallocate( mapCopies.copyFromStaging->stagingDstMemory, nullptr );
				deallocate( mapCopies.copyFromStaging->stagingDst, nullptr );
			}
		}

		if ( m_process.copyToStaging )
		{
			deallocate( m_process.copyToStaging->stagingSrcMemory, nullptr );
			deallocate( m_process.copyToStaging->stagingSrc, nullptr );
		}
	}

	void CopyBufferToImageCommand::apply( Context const & context )const
	{
		if ( m_process.copyToStaging )
		{
			apply( context, *m_process.copyToStaging );
		}

		apply( context, m_process.mapCopy );
	}

	void CopyBufferToImageCommand::apply( Context const & context
		, CopyToStagingProcess const & process )const
	{
		CopyBufferCommand command{ m_device
			, process.copyToStaging
			, m_src
			, process.stagingSrc };
		command.apply( context );
	}

	void CopyBufferToImageCommand::apply( Context const & context
		, CopyFromStagingProcess const & process )const
	{
		CopyImageCommand command{ m_device
			, process.copyFromStaging
			, process.stagingDst
			, m_dst };
		command.apply( context );
	}

	void CopyBufferToImageCommand::apply( Context const & context
		, MapCopyProcess const & process )const
	{
		for ( auto & mapCopyImage : process.mapCopyImages )
		{
			doMapCopy( mapCopyImage
				, get( mapCopyImage.dst )->getFormat()
				, process.srcMemory
				, mapCopyImage.dstMemory );

			if ( mapCopyImage.copyFromStaging )
			{
				apply( context, *mapCopyImage.copyFromStaging );
			}
		}
	}

	CommandPtr CopyBufferToImageCommand::clone()const
	{
		return std::make_unique< CopyBufferToImageCommand >( *this );
	}

	void CopyBufferToImageCommand::doMapCopy( MapCopyImage const & mapCopy
		, VkFormat format
		, VkDeviceMemory src
		, VkDeviceMemory dst )const
	{
		ID3D11DeviceContext * context;
		get( m_device )->getDevice()->GetImmediateContext( &context );
		D3D11_MAPPED_SUBRESOURCE srcMapped{};

		if ( VK_SUCCESS == get( src )->getImpl().lock( context
			, 0u
			, srcMapped ) )
		{
			D3D11_MAPPED_SUBRESOURCE dstMapped{};

			if ( VK_SUCCESS == get( dst )->getImpl().lock( context
				, mapCopy.dstSubresource
				, dstMapped ) )
			{
				doCopyMapped( format
					, mapCopy.mapCopy
					, srcMapped
					, mapCopy.srcBox
					, dstMapped
					, mapCopy.dstLayout );
				get( dst )->getImpl().unlock( context, mapCopy.dstSubresource );
			}

			get( src )->getImpl().unlock( context, 0u );
		}

		safeRelease( context );
	}

	void CopyBufferToImageCommand::doCopyToStaging( Context const & context
		, VkDeviceSize srcOffset
		, VkDeviceSize size
		, VkBuffer src
		, VkBuffer staging )const
	{
		CopyBufferCommand command{ m_device
			, VkBufferCopy
			{
				srcOffset,
				0u,
				size
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
