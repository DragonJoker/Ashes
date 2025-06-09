/*
This file belongs to Ashes.
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
			, VkBufferImageCopy const & copyInfo
			, uint32_t slice )
		{
			auto begin = copyInfo.bufferOffset;
			auto end = begin + doGetBufferSize( format, copyInfo );
			return
			{
				UINT( begin ),    // left
				0u,               // top
				slice,            // front
				UINT( end ),      // right
				1u,               // bottom
				slice + 1u        // back
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

			if ( copyInfo.imageOffset.z )
			{
				layout.offset += layout.depthPitch * copyInfo.imageOffset.z;
			}

			return layout;
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

		void doCopy( uint8_t const * srcData
			, uint8_t * dstData
			, uint32_t layers
			, VkDeviceSize srcLayerPitch
			, VkDeviceSize dstLayerPitch
			, uint32_t depth
			, VkDeviceSize srcDepthPitch
			, VkDeviceSize dstDepthPitch
			, uint32_t height
			, VkDeviceSize srcRowPitch
			, VkDeviceSize dstRowPitch )
		{
			auto size = std::min( srcRowPitch, dstRowPitch );

			for ( auto l = 0u; l < layers; ++l )
			{
				auto srcLayer = srcData;
				auto dstLayer = dstData;

				for ( auto z = 0u; z < depth; ++z )
				{
					auto srcSlice = srcLayer;
					auto dstSlice = dstLayer;

					for ( auto y = 0u; y < height; ++y )
					{
						std::memcpy( dstSlice, srcSlice, size );
						srcSlice += srcRowPitch;
						dstSlice += dstRowPitch;
					}

					srcLayer += srcDepthPitch;
					dstLayer += dstDepthPitch;
				}

				srcData += srcLayerPitch;
				dstData += dstLayerPitch;
			}
		}

		void doAdjustPitches( uint32_t bufferSize
			, uint32_t height
			, UINT & srcRowPitch
			, UINT & dstRowPitch
			, uint32_t depth
			, UINT & srcDepthPitch
			, UINT & dstDepthPitch )
		{
			auto checkedSize = bufferSize;

			if ( srcRowPitch == checkedSize )
			{
				if ( srcRowPitch == dstRowPitch )
				{
					dstRowPitch /= height;
				}

				srcRowPitch /= height;
			}

			if ( srcDepthPitch == checkedSize )
			{
				if ( srcDepthPitch == dstDepthPitch )
				{
					dstDepthPitch /= depth;
				}

				srcDepthPitch /= depth;
				checkedSize = srcDepthPitch;
			}

			if ( srcRowPitch == checkedSize )
			{
				if ( srcRowPitch == dstRowPitch )
				{
					dstRowPitch /= height;
				}

				srcRowPitch /= height;
			}
		}

		void doCopyMapped( VkFormat format
			, VkBufferImageCopy const & copyInfo
			, D3D11_MAPPED_SUBRESOURCE srcBuffer
			, D3D11_BOX const & srcBox
			, D3D11_MAPPED_SUBRESOURCE dstBuffer
			, VkSubresourceLayout const & dstLayout )
		{
			auto bufferSize = ( srcBox.right - srcBox.left ) * ( srcBox.back - srcBox.front );
			auto bufferRowPitch = srcBuffer.RowPitch;
			auto bufferDepthPitch = srcBuffer.DepthPitch;
			auto bufferLayerPitch = bufferDepthPitch;
			auto imageSize = dstLayout.size;
			auto imageRowPitch = dstBuffer.RowPitch;
			auto imageLayerPitch = dstLayout.arrayPitch;
			auto imageDepthPitch = dstBuffer.DepthPitch;
			auto imageHeight = copyInfo.imageExtent.height;
			auto imageDepth = srcBox.back - srcBox.front;
			auto imageLayers = copyInfo.imageSubresource.layerCount;
			auto srcData = reinterpret_cast< uint8_t * >( srcBuffer.pData ) + srcBox.left;
			auto dstData = reinterpret_cast< uint8_t * >( dstBuffer.pData ) + ( copyInfo.imageOffset.z * dstBuffer.DepthPitch );

			if ( imageRowPitch >= imageSize )
			{
				bufferRowPitch = getTexelBlockByteSize( copyInfo.imageExtent, format ) / ( imageHeight * imageDepth * imageLayers );
				bufferDepthPitch = bufferRowPitch * imageHeight;
				bufferLayerPitch = bufferDepthPitch;

				doCopy( srcData
					, dstData
					, imageLayers
					, bufferLayerPitch
					, imageLayerPitch
					, imageDepth
					, bufferDepthPitch
					, imageDepthPitch
					, imageHeight
					, bufferRowPitch
					, imageRowPitch );
				return;
			}

			doAdjustPitches( bufferSize
				, imageHeight
				, bufferRowPitch
				, imageRowPitch
				, imageDepth
				, bufferDepthPitch
				, imageDepthPitch );

			if ( bufferRowPitch && imageRowPitch )
			{
				auto bufferSteps = bufferDepthPitch / bufferRowPitch;
				auto imageSteps = imageDepthPitch / imageRowPitch;
				assert( bufferSteps == imageSteps
					|| ( bufferSteps > imageSteps && ( bufferSteps % imageSteps ) == 0 )
					|| ( imageSteps > bufferSteps && ( imageSteps % bufferSteps ) == 0 ) );

				if ( bufferSteps == imageSteps )
				{
					doCopy( srcData
						, dstData
						, imageLayers
						, bufferLayerPitch
						, imageLayerPitch
						, imageDepth
						, bufferDepthPitch
						, imageDepthPitch
						, bufferSteps
						, bufferRowPitch
						, imageRowPitch );
				}
				else if ( bufferSteps > imageSteps )
				{
					// bufferSteps = n * imageSteps
					// => imageRowPitch = n * bufferRowPitch
					doCopy( srcData
						, dstData
						, imageLayers
						, bufferLayerPitch
						, imageLayerPitch
						, imageDepth
						, bufferDepthPitch
						, imageDepthPitch
						, imageSteps
						, imageRowPitch
						, imageRowPitch );
				}
				else
				{
					// imageSteps = n * bufferSteps
					// => bufferRowPitch = n * imageRowPitch
					doCopy( srcData
						, dstData
						, imageLayers
						, bufferLayerPitch
						, imageLayerPitch
						, imageDepth
						, bufferDepthPitch
						, imageDepthPitch
						, bufferSteps
						, bufferRowPitch
						, bufferRowPitch );
				}
			}
		}

		VkBuffer getStagingBuffer( VkDevice device
			, VkDeviceSize size
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

		VkBufferCopy doGetCopyToStaging( VkFormat format
			, ArrayView< VkBufferImageCopy const > const & copyInfos )
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
			, ArrayView< VkBufferImageCopy const > const & copyInfos
			, bool srcMappable
			, VkBuffer & src
			, VkBufferImageCopy & mapCopyInfo )
		{
			Optional< CopyToStagingProcess > result = ashes::nullopt;

			if ( !srcMappable )
			{
				auto copyToStaging = doGetCopyToStaging( get( dst )->getFormat(), copyInfos );
				CopyToStagingProcess process;
				process.copyToStaging = copyToStaging;
				process.stagingSrc = getStagingBuffer( device
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
				VkImageSubresourceLayers stagingImageSubresource = bufferImageCopy.imageSubresource;
				stagingImageSubresource.layerCount = 1u;
				stagingImageSubresource.baseArrayLayer = 0u;

				CopyFromStagingProcess process;
				process.stagingDst = get( device )->getStagingImage( image
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
			, ArrayView< VkBufferImageCopy const > const & copyInfos
			, VkBufferImageCopy const & baseBufferImageCopy
			, bool dstMappable )
		{
			MapCopyProcess result
			{
				src,
				get( src )->getMemory(),
			};
			auto minExtent = ashes::getMinimalExtent2D( get( dst )->getFormat() );

			for ( auto & copy : copyInfos )
			{
				VkBufferImageCopy copyInfo = copy;
				// We process images array level per array level
				copyInfo.imageSubresource.layerCount = 1u;
				// We process images depth slice per depth slice
				copyInfo.imageExtent.width = std::max( copyInfo.imageExtent.width, minExtent.width );
				copyInfo.imageExtent.height = std::max( copyInfo.imageExtent.height, minExtent.height );
				copyInfo.imageExtent.depth = 1u;
				copyInfo.bufferOffset -= baseBufferImageCopy.bufferOffset;
				auto slicePitch = getSize( VkExtent3D{ getBufferRowPitch( copyInfo ), getBufferHeightPitch( copyInfo ), 1u }, get( dst )->getFormat() );

				for ( uint32_t layer = 0u; layer < copy.imageSubresource.layerCount; ++layer )
				{
					for ( int32_t slice = copy.imageOffset.z; slice < copy.imageOffset.z + int32_t( copy.imageExtent.depth ); ++slice )
					{
						auto srcBox = doGetSrcBox( get( dst )->getFormat(), copyInfo, uint32_t( slice ) );
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
						copyInfo.imageOffset.z++;
						copyInfo.bufferOffset += slicePitch;
					}

					copyInfo.imageSubresource.baseArrayLayer++;
				}
			}

			return result;
		}

		BufferToImageCopyProcess buildProcess( VkDevice device
			, ArrayView< VkBufferImageCopy const > const & copyInfos
			, VkBuffer src
			, VkImage dst
			, bool srcMappable
			, bool dstMappable )
		{
			VkBufferImageCopy baseBufferImageCopy{};
			auto copyToStaging = getCopyToStagingProcess( device
				, dst
				, copyInfos
				, srcMappable
				, src
				, baseBufferImageCopy );

			auto mapCopy = getMapCopyProcess( device
				, src
				, dst
				, copyInfos
				, baseBufferImageCopy
				, dstMappable );
			return BufferToImageCopyProcess{ copyToStaging, mapCopy };
		}
	}

	CopyBufferToImageCommand::CopyBufferToImageCommand( VkDevice device
		, ArrayView< VkBufferImageCopy const > const & copyInfos
		, VkBuffer src
		, VkImage dst )
		: CommandBase{ device }
		, m_src{ src }
		, m_dst{ dst }
		, m_copyInfos{ copyInfos.begin(), copyInfos.end() }
		, m_format{ getSRVFormat( get( m_dst )->getFormat() ) }
		, m_srcMappable{ checkFlag( get( get( m_src )->getMemory() )->getMemoryPropertyFlags(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) }
		, m_dstMappable{ checkFlag( get( get( m_dst )->getMemory() )->getMemoryPropertyFlags(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) }
		, m_process{ buildProcess ( device, copyInfos, src, dst, m_srcMappable, m_dstMappable ) }
	{
	}

	CopyBufferToImageCommand::~CopyBufferToImageCommand()noexcept
	{
		if ( m_process.copyToStaging )
		{
			deallocate( m_process.copyToStaging->stagingSrcMemory, get( getDevice() )->getAllocationCallbacks() );
			deallocate( m_process.copyToStaging->stagingSrc, get( getDevice() )->getAllocationCallbacks() );
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
		CopyBufferCommand command{ getDevice()
			, process.copyToStaging
			, m_src
			, process.stagingSrc };
		command.apply( context );
	}

	void CopyBufferToImageCommand::apply( Context const & context
		, CopyFromStagingProcess const & process )const
	{
		CopyImageCommand command{ getDevice()
			, process.copyFromStaging
			, process.stagingDst
			, m_dst };
		command.apply( context );
	}

	void CopyBufferToImageCommand::apply( Context const & context
		, MapCopyProcess const & process )const
	{
		D3D11_MAPPED_SUBRESOURCE srcMapped{};

		if ( auto & src = get( process.src )->getObjectMemory();
			VK_SUCCESS == src.lock( *context.context
				, 0u
				, srcMapped )
			&& srcMapped.pData )
		{
			for ( auto & mapCopyImage : process.mapCopyImages )
			{
				doMapCopy( context
					, mapCopyImage
					, get( mapCopyImage.dst )->getFormat()
					, srcMapped
					, get( mapCopyImage.dst )->getObjectMemory() );

				if ( mapCopyImage.copyFromStaging )
				{
					apply( context, *mapCopyImage.copyFromStaging );
				}
			}

			src.unlock( *context.context, 0u );
		}
	}

	CommandPtr CopyBufferToImageCommand::clone()const
	{
		return std::make_unique< CopyBufferToImageCommand >( getDevice()
			, makeArrayView( m_copyInfos.data(), m_copyInfos.size() )
			, m_src
			, m_dst );
	}

	void CopyBufferToImageCommand::doMapCopy( Context const & context
		, MapCopyImage const & mapCopy
		, VkFormat format
		, D3D11_MAPPED_SUBRESOURCE srcMapped
		, ObjectMemory const & dst )const
	{
		auto bufferSize = ( mapCopy.srcBox.right - mapCopy.srcBox.left ) * ( mapCopy.srcBox.back - mapCopy.srcBox.front );
		auto imageSize = getSize( VkExtent3D{ mapCopy.mapCopy.imageExtent.width, mapCopy.mapCopy.imageExtent.height, 1u }, format );

		if ( srcMapped.RowPitch > bufferSize )
		{
			// Use destination row pitch.
			assert( bufferSize >= imageSize );
			srcMapped.RowPitch = UINT( imageSize / mapCopy.mapCopy.imageExtent.height );
		}

		if ( srcMapped.DepthPitch > bufferSize )
		{
			// Use destination depth pitch.
			srcMapped.DepthPitch = UINT( imageSize );
		}

		if ( D3D11_MAPPED_SUBRESOURCE dstMapped{};
			VK_SUCCESS == dst.lock( *context.context
				, mapCopy.dstSubresource
				, dstMapped )
			&& dstMapped.pData )
		{
			doCopyMapped( format
				, mapCopy.mapCopy
				, srcMapped
				, mapCopy.srcBox
				, dstMapped
				, mapCopy.dstLayout );
			dst.unlock( *context.context, mapCopy.dstSubresource );
		}
	}

	void CopyBufferToImageCommand::doCopyToStaging( Context const & context
		, VkDeviceSize srcOffset
		, VkDeviceSize size
		, VkBuffer src
		, VkBuffer staging )const
	{
		CopyBufferCommand command{ getDevice()
			, VkBufferCopy{ srcOffset, 0u, size }
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
		CopyImageCommand command{ getDevice()
			, VkImageCopy{ stagingSurbresouce
				, VkOffset3D{}
				, copyInfo.imageSubresource
				, copyInfo.imageOffset
				, copyInfo.imageExtent }
			, staging
			, dst };
		command.apply( context );
	}
}
