/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Buffer/StagingBuffer.hpp"

#include "ashespp/Buffer/Buffer.hpp"
#include "ashespp/Command/CommandBuffer.hpp"
#include "ashespp/Core/Device.hpp"
#include "ashespp/Image/Image.hpp"
#include "ashespp/Image/ImageView.hpp"

#include <ashes/common/Exception.hpp>

namespace ashes
{
	namespace
	{
		template< typename T, typename U >
		T getAligned( T value, U align )
		{
			return T( ( value + align - 1 ) & ~( align - 1 ) );
		}

		template< typename T >
		T getSubresourceValue( T value, uint32_t mipLevel )
		{
			return T( value >> mipLevel );
		}

		VkMemoryAllocateInfo getAllocateInfo( Device const & device
			, BufferBase const & buffer )
		{
			auto requirements = buffer.getMemoryRequirements();
			auto deduced = device.deduceMemoryType( requirements.memoryTypeBits
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
			return
			{
				VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				nullptr,
				requirements.size,
				deduced
			};
		}

		VkBufferImageCopy makeValidCopyInfo( Image const & image
			, VkExtent2D const & size
			, VkOffset3D offset
			, VkImageSubresourceLayers const & subresourceLayers )
		{
			auto format = image.getFormat();
			auto type = image.getType();
			auto texelBlockExtent = getMinimalExtent3D( format );

			VkImageSubresourceLayers layers{ subresourceLayers };
			layers.baseArrayLayer = type == VK_IMAGE_TYPE_3D
				? 0
				: layers.baseArrayLayer;
			layers.layerCount = type == VK_IMAGE_TYPE_3D
				? 1u
				: layers.layerCount;

			VkExtent3D imageExtent
			{
				getSubresourceValue( size.width, subresourceLayers.mipLevel ),
				getSubresourceValue( size.height, subresourceLayers.mipLevel ),
				1u,
			};
			imageExtent.width = getAligned( imageExtent.width, texelBlockExtent.width );
			imageExtent.height = getAligned( imageExtent.height, texelBlockExtent.height );
			imageExtent.depth = getAligned( imageExtent.depth, texelBlockExtent.depth );
			imageExtent.height = type == VK_IMAGE_TYPE_1D
				? 1u
				: imageExtent.height;
			imageExtent.depth = ( type == VK_IMAGE_TYPE_1D || type == VK_IMAGE_TYPE_2D )
				? 1u
				: imageExtent.depth;

			VkExtent3D subresourceExtent
			{
				getSubresourceValue( size.width, subresourceLayers.mipLevel ),
				getSubresourceValue( size.height, subresourceLayers.mipLevel ),
				image.getDimensions().depth,
			};
			subresourceExtent.width = getAligned( subresourceExtent.width, texelBlockExtent.width );
			subresourceExtent.height = getAligned( subresourceExtent.height, texelBlockExtent.height );
			subresourceExtent.depth = getAligned( subresourceExtent.depth, texelBlockExtent.depth );
			subresourceExtent.height = type == VK_IMAGE_TYPE_1D
				? 1u
				: subresourceExtent.height;
			subresourceExtent.depth = ( type == VK_IMAGE_TYPE_1D || type == VK_IMAGE_TYPE_2D )
				? 1u
				: subresourceExtent.depth;

			VkOffset3D imageOffset
			{
				std::min( int32_t( subresourceExtent.width - imageExtent.width ), std::max( 0, offset.x ) ),
				std::min( int32_t( subresourceExtent.height - imageExtent.height ), std::max( 0, offset.y ) ),
				std::min( int32_t( subresourceExtent.depth - imageExtent.depth ), std::max( 0, offset.z ) ),
			};

			offset.y = type == VK_IMAGE_TYPE_1D
				? 0
				: offset.y;
			offset.z = ( type == VK_IMAGE_TYPE_1D || type == VK_IMAGE_TYPE_2D )
				? 0
				: offset.z;

			auto bufferOffset = 0u;
			auto bufferRowLength = imageExtent.width;
			auto bufferImageHeight = imageExtent.height;

			return VkBufferImageCopy
			{
				bufferOffset,
				bufferRowLength,
				bufferImageHeight,
				layers,
				imageOffset,
				imageExtent,
			};
		}

		ashes::VkBufferImageCopyArray makeValidCopyInfos( ImageView const & view
			, VkImageSubresourceLayers subresourceLayers
			, VkExtent2D const & size
			, VkOffset3D const & offset )
		{
			ashes::VkBufferImageCopyArray copyInfos;
			copyInfos.push_back( makeValidCopyInfo( *view.image
				, size
				, offset
				, subresourceLayers ) );
			return copyInfos;
		}
	}

	StagingBuffer::StagingBuffer( Device const & device
		, VkBufferUsageFlags usage
		, VkDeviceSize size
		, QueueShare const & sharingMode )
		: StagingBuffer{ device, "StagingBuffer", usage, size, sharingMode }
	{
	}

	StagingBuffer::StagingBuffer( Device const & device
		, std::string const & debugName
		, VkBufferUsageFlags usage
		, VkDeviceSize size
		, QueueShare const & sharingMode )
		: m_device{ device }
		, m_buffer{ device.createBuffer( debugName
			, size
			, usage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, sharingMode ) }
		, m_storage{ device.allocateMemory( debugName, getAllocateInfo( device, *m_buffer ) ) }
	{
		m_buffer->bindMemory( m_storage );
	}

	void StagingBuffer::uploadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, ByteArray const & data
		, BufferBase const & buffer )const
	{
		uploadBufferData( queue
			, commandPool
			, data.data()
			, data.size()
			, 0u
			, buffer );
	}

	void StagingBuffer::uploadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, uint8_t const * const data
		, VkDeviceSize size
		, BufferBase const & buffer )const
	{
		uploadBufferData( queue
			, commandPool
			, data
			, size
			, 0u
			, buffer );
	}

	void StagingBuffer::uploadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, ByteArray const & data
		, VkDeviceSize offset
		, BufferBase const & buffer )const
	{
		uploadBufferData( queue
			, commandPool
			, data.data()
			, data.size()
			, offset
			, buffer );
	}

	void StagingBuffer::uploadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, uint8_t const * const data
		, VkDeviceSize size
		, VkDeviceSize offset
		, BufferBase const & buffer )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( "StaginBufferUpload"
			, VK_COMMAND_BUFFER_LEVEL_PRIMARY );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		uploadBufferData( *commandBuffer
			, data
			, size
			, offset
			, buffer );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( MaxTimeout );
	}

	void StagingBuffer::uploadBufferData( CommandBuffer const & commandBuffer
		, ByteArray const & data
		, BufferBase const & buffer )const
	{
		uploadBufferData( commandBuffer
			, data.data()
			, data.size()
			, 0u
			, buffer );
	}

	void StagingBuffer::uploadBufferData( CommandBuffer const & commandBuffer
		, uint8_t const * const data
		, VkDeviceSize size
		, BufferBase const & buffer )const
	{
		uploadBufferData( commandBuffer
			, data
			, size
			, 0u
			, buffer );
	}

	void StagingBuffer::uploadBufferData( CommandBuffer const & commandBuffer
		, ByteArray const & data
		, VkDeviceSize offset
		, BufferBase const & buffer )const
	{
		uploadBufferData( commandBuffer
			, data.data()
			, data.size()
			, offset
			, buffer );
	}

	void StagingBuffer::uploadBufferData( CommandBuffer const & commandBuffer
		, uint8_t const * const data
		, VkDeviceSize size
		, VkDeviceSize offset
		, BufferBase const & buffer )const
	{
		doCopyToStagingBuffer( data
			, size );
		doCopyFromStagingBuffer( commandBuffer
			, size
			, offset
			, buffer );
	}

	void StagingBuffer::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, uint8_t const * const data
		, ImageView const & view
		, VkImageLayout dstLayout )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( "StagingBufferUploadTex"
			, VK_COMMAND_BUFFER_LEVEL_PRIMARY );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		uploadTextureData( *commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, data
			, view
			, dstLayout );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( ashes::MaxTimeout );
	}

	void StagingBuffer::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkFormat format
		, uint8_t const * const data
		, ImageView const & view
		, VkImageLayout dstLayout )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( "StagingBufferUploadTex"
			, VK_COMMAND_BUFFER_LEVEL_PRIMARY );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		uploadTextureData( *commandBuffer
			, format
			, data
			, view
			, dstLayout );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( ashes::MaxTimeout );
	}

	void StagingBuffer::uploadTextureData( CommandBuffer const & commandBuffer
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, uint8_t const * const data
		, ImageView const & view
		, VkImageLayout dstLayout )const
	{
		auto range = view->subresourceRange;
		auto layerSize = getLevelsSize( extent
			, format
			, range.baseMipLevel
			, range.levelCount
			, uint32_t( view.image->getMemoryRequirements().alignment ) );
		doCopyToStagingBuffer( data
			, layerSize );
		doCopyFromStagingBuffer( commandBuffer
			, extent
			, offset
			, view
			, dstLayout
			, subresourceLayers );
	}

	void StagingBuffer::uploadTextureData( CommandBuffer const & commandBuffer
		, VkFormat format
		, uint8_t const * const data
		, ImageView const & view
		, VkImageLayout dstLayout )const
	{
		VkExtent3D extent{ view.image->getDimensions() };
		auto mipLevel = view->subresourceRange.baseMipLevel;
		uploadTextureData( commandBuffer
			, {
				getAspectMask( view->format ),
				mipLevel,
				view->subresourceRange.baseArrayLayer,
				view->subresourceRange.layerCount
			}
			, format
			, VkOffset3D{}
			, { extent.width, extent.height }
			, data
			, view
			, dstLayout );
	}

	void StagingBuffer::downloadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, [[maybe_unused]] VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, uint8_t * data
		, ImageView const & view
		, VkImageLayout dstLayout )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( "StagingBufferDownloadTex"
			, VK_COMMAND_BUFFER_LEVEL_PRIMARY );
		auto mipLevel = view->subresourceRange.baseMipLevel;
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		commandBuffer->memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, view.makeTransferSource( VK_IMAGE_LAYOUT_UNDEFINED ) );
		doCopyToStagingBuffer( *commandBuffer
			, extent
			, offset
			, view
			, dstLayout
			, { getAspectMask( view->format )
				, mipLevel
				, view->subresourceRange.baseArrayLayer
				, view->subresourceRange.layerCount } );
		commandBuffer->memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
			, view.makeShaderInputResource( VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL ) );
		commandBuffer->end();

		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( ashes::MaxTimeout );

		auto range = view->subresourceRange;
		doCopyFromStagingBuffer( data
			, getLevelsSize( extent
				, format
				, range.baseMipLevel
				, range.levelCount
				, uint32_t( view.image->getMemoryRequirements().alignment ) ) );
	}

	void StagingBuffer::downloadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkFormat format
		, uint8_t * data
		, ImageView const & view
		, VkImageLayout dstLayout )const
	{
		VkExtent3D extent{ view.image->getDimensions() };
		auto mipLevel = view->subresourceRange.baseMipLevel;
		downloadTextureData( queue
			, commandPool
			, {
				getAspectMask( view->format ),
				mipLevel,
				view->subresourceRange.baseArrayLayer,
				view->subresourceRange.layerCount
			}
			, format
			, VkOffset3D{}
			, { extent.width, extent.height }
			, data
			, view
			, dstLayout );
	}

	void StagingBuffer::doCopyToStagingBuffer( uint8_t const * data
		, VkDeviceSize size )const
	{
		assert( size <= getBuffer().getSize() );
		auto mappedSize = getAlignedSize( size
			, m_device.getProperties().limits.nonCoherentAtomSize );
		mappedSize = mappedSize > getBuffer().getSize()
			? WholeSize
			: mappedSize;
		auto buffer = getBuffer().lock( 0u
			, mappedSize
			, 0u );

		if ( !buffer )
		{
			throw Exception{ VK_ERROR_MEMORY_MAP_FAILED, "Staging buffer storage memory mapping" };
		}

		std::memcpy( buffer
			, data
			, size_t( size ) );
		getBuffer().flush( 0u, mappedSize );
		getBuffer().unlock();
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, VkDeviceSize size
		, VkDeviceSize offset
		, BufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, getBuffer().makeTransferSource() );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, buffer.makeTransferDestination() );
		commandBuffer.copyBuffer( getBuffer()
			, buffer
			, size
			, 0u
			, offset );
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, VkDeviceSize size
		, VkDeviceSize offset
		, VertexBufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, buffer.getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( getBuffer()
			, buffer.getBuffer()
			, size
			, 0u
			, offset );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, buffer.getBuffer().makeVertexShaderInputResource() );
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, VkDeviceSize size
		, VkDeviceSize offset
		, UniformBuffer const & buffer
		, VkPipelineStageFlags dstStageFlags )const
	{
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, buffer.getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( getBuffer()
			, buffer.getBuffer()
			, size
			, 0u
			, offset );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, dstStageFlags
			, buffer.getBuffer().makeUniformBufferInput() );
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, VkExtent2D const & size
		, VkOffset3D const & offset
		, ImageView const & view
		, VkImageLayout dstLayout
		, VkImageSubresourceLayers const & subresourceLayers )const
	{
		assert( getLevelsSize( size
			, view->format
			, view->subresourceRange.baseMipLevel
			, view->subresourceRange.levelCount
			, uint32_t( view.image->getMemoryRequirements().alignment ) ) <= getBuffer().getSize() );

		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, getBuffer().makeTransferSource() );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, view.makeTransferDestination( VK_IMAGE_LAYOUT_UNDEFINED ) );
		commandBuffer.copyToImage( makeValidCopyInfos( view, subresourceLayers, size, offset )
			, getBuffer()
			, *view.image );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, getStageMask( dstLayout )
			, view.makeLayoutTransition( VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
				, dstLayout
				, VK_QUEUE_FAMILY_IGNORED
				, VK_QUEUE_FAMILY_IGNORED ) );
	}

	void StagingBuffer::doCopyFromStagingBuffer( uint8_t * data
		, VkDeviceSize size )const
	{
		assert( size <= getBuffer().getSize() );
		auto mappedSize = getAlignedSize( size
			, m_device.getProperties().limits.nonCoherentAtomSize );
		mappedSize = mappedSize > getBuffer().getSize()
			? WholeSize
			: mappedSize;
		auto buffer = getBuffer().lock( 0u
			, mappedSize
			, 0u );

		if ( !buffer )
		{
			throw Exception{ VK_ERROR_MEMORY_MAP_FAILED, "Staging buffer storage memory mapping failed." };
		}

		std::memcpy( data
			, buffer
			, size_t( size ) );
		getBuffer().flush( 0u, mappedSize );
		getBuffer().unlock();
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, VkDeviceSize size
		, VkDeviceSize offset
		, BufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, buffer.makeTransferSource() );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( buffer
			, getBuffer()
			, size
			, 0u
			, offset );
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, VkDeviceSize size
		, VkDeviceSize offset
		, VertexBufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, buffer.getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( buffer.getBuffer()
			, getBuffer()
			, size
			, 0u
			, offset );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, buffer.getBuffer().makeVertexShaderInputResource() );
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, VkDeviceSize size
		, VkDeviceSize offset
		, UniformBuffer const & buffer
		, VkPipelineStageFlags dstStageFlags )const
	{
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, buffer.getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( buffer.getBuffer()
			, getBuffer()
			, size
			, 0u
			, offset );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, dstStageFlags
			, buffer.getBuffer().makeUniformBufferInput() );
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, VkExtent2D const & size
		, VkOffset3D const & offset
		, ImageView const & view
		, VkImageLayout dstLayout
		, VkImageSubresourceLayers const & subresourceLayers )const
	{
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, view.makeTransferSource( VK_IMAGE_LAYOUT_UNDEFINED ) );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, getBuffer().makeTransferDestination() );
		commandBuffer.copyToBuffer( makeValidCopyInfos( view, subresourceLayers, size, offset )
			, *view.image
			, getBuffer() );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, getStageMask( dstLayout )
			, view.makeShaderInputResource( VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL ) );
	}
}
