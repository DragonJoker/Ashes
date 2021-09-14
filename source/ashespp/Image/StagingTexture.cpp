/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Image/StagingTexture.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Image/Image.hpp"
#include "ashespp/Image/ImageView.hpp"

#include <ashes/common/Exception.hpp>
#include <ashes/common/Format.hpp>

namespace ashes
{
	StagingTexture::StagingTexture( Device const & device
		, VkFormat format
		, VkExtent2D const & extent
		, uint32_t mipLevels )
		: StagingTexture{ device, "StagingTexture", format, extent, mipLevels }
	{
	}

	StagingTexture::StagingTexture( Device const & device
		, std::string const & debugName
		, VkFormat format
		, VkExtent2D const & extent
		, uint32_t mipLevels )
		: m_device{ device }
		, m_buffer
		{
			device,
			debugName,
			uint32_t( getAlignedSize( getLevelsSize( extent, format, 0u, mipLevels, 1u )
				, uint32_t( m_device.getProperties().limits.nonCoherentAtomSize ) ) ),
			VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT | VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		}
	{
		auto requirements = m_buffer.getMemoryRequirements();
		auto deduced = m_device.deduceMemoryType( requirements.memoryTypeBits
			, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
		m_buffer.bindMemory( device.allocateMemory( debugName
			, {
				VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				nullptr,
				requirements.size,
				deduced
			} ) );
	}

	void StagingTexture::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, uint8_t const * data
		, ImageView const & view )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( "StagingTextureUpload"
			, VK_COMMAND_BUFFER_LEVEL_PRIMARY );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		commandBuffer->beginDebugBlock( { "Staging Texture Upload"
			, { 0.5f, 0.5f, 0.5f, 1.0f } } );
		uploadTextureData( *commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, data
			, view );
		commandBuffer->endDebugBlock();
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( MaxTimeout );
	}

	void StagingTexture::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkFormat format
		, uint8_t const * data
		, ImageView const & view )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( "StagingTextureUpload"
			, VK_COMMAND_BUFFER_LEVEL_PRIMARY );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		commandBuffer->beginDebugBlock( { "Staging Texture Upload"
			, { 0.5f, 0.5f, 0.5f, 1.0f } } );
		uploadTextureData( *commandBuffer
			, format
			, data
			, view );
		commandBuffer->endDebugBlock();
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( MaxTimeout );
	}

	void StagingTexture::copyTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkFormat format
		, ImageView const & view )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( "StagingTextureCopy"
			, VK_COMMAND_BUFFER_LEVEL_PRIMARY );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		commandBuffer->beginDebugBlock( { "Staging Texture Copy"
			, { 0.5f, 0.5f, 0.5f, 1.0f } } );
		copyTextureData( *commandBuffer
			, format
			, view );
		commandBuffer->endDebugBlock();
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( MaxTimeout );
	}

	void StagingTexture::copyTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, ImageView const & view )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( "StagingTextureCopy"
			, VK_COMMAND_BUFFER_LEVEL_PRIMARY );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		commandBuffer->beginDebugBlock( { "Staging Texture Copy"
			, { 0.5f, 0.5f, 0.5f, 1.0f } } );
		copyTextureData( *commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, view );
		commandBuffer->endDebugBlock();
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( MaxTimeout );
	}

	void StagingTexture::uploadTextureData( CommandBuffer const & commandBuffer
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, uint8_t const * data
		, ImageView const & view )const
	{
		doCopyToStagingTexture( data
			, format
			, extent
			, 1u );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, view.makeTransferDestination( VK_IMAGE_LAYOUT_UNDEFINED ) );
		doCopyStagingToDestination( commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, view );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
			, view.makeShaderInputResource( VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL ) );
	}

	void StagingTexture::uploadTextureData( CommandBuffer const & commandBuffer
		, VkFormat format
		, uint8_t const * data
		, ImageView const & view )const
	{
		auto mipLevel = view->subresourceRange.baseMipLevel;
		auto extent = getSubresourceDimensions( view.image->getDimensions(), mipLevel, format );
		doCopyToStagingTexture( data
			, format
			, { extent.width, extent.height }
			, view->subresourceRange.levelCount );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, view.makeTransferDestination( VK_IMAGE_LAYOUT_UNDEFINED ) );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, m_buffer.makeTransferSource() );
		VkDeviceSize offset = 0u;
		VkImageSubresourceLayers subresourceLayers
		{
			view->subresourceRange.aspectMask,
			0u,
			view->subresourceRange.baseArrayLayer,
			view->subresourceRange.layerCount
		};

		for ( uint32_t level = 0u; level < view->subresourceRange.levelCount; ++level )
		{
			subresourceLayers.mipLevel = mipLevel;
			VkDeviceSize size = getSize( extent, format );
			assert( offset + size <= m_buffer.getSize() );
			commandBuffer.copyToImage( VkBufferImageCopy
				{
					offset,
					0u,
					0u,
					subresourceLayers,
					VkOffset3D{},
					VkExtent3D
					{
						std::max( 1u, view.image->getDimensions().width >> mipLevel ),
						std::max( 1u, view.image->getDimensions().height >> mipLevel ),
						1u
					}
				}
				, m_buffer
				, *view.image );
			offset += size;
			++mipLevel;
		}

		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
			, view.makeShaderInputResource( VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL ) );
	}

	void StagingTexture::copyTextureData( CommandBuffer const & commandBuffer
		, VkFormat format
		, ImageView const & view )const
	{
		auto mipLevel = view->subresourceRange.baseMipLevel;
		auto extent = getSubresourceDimensions( view.image->getDimensions(), mipLevel, format );
		copyTextureData( commandBuffer
			, {
				view->subresourceRange.aspectMask,
				mipLevel,
				view->subresourceRange.baseArrayLayer,
				view->subresourceRange.layerCount
			}
			, format
			, VkOffset3D{}
			, { extent.width, extent.height }
			, view );
	}

	void StagingTexture::copyTextureData( CommandBuffer const & commandBuffer
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, ImageView const & view )const
	{
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, view.makeTransferDestination( VK_IMAGE_LAYOUT_UNDEFINED ) );
		doCopyStagingToDestination( commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, view );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
			, view.makeShaderInputResource( VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL ) );
	}

	void StagingTexture::downloadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, uint8_t * data
		, ImageView const & view )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( "StagingTextureDownload"
			, VK_COMMAND_BUFFER_LEVEL_PRIMARY );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		commandBuffer->beginDebugBlock( { "Staging Texture Download"
			, { 0.5f, 0.5f, 0.5f, 1.0f } } );
		commandBuffer->memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, view.makeTransferSource( VK_IMAGE_LAYOUT_UNDEFINED ) );
		doCopyDestinationToStaging( *commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, view );
		commandBuffer->memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
			, view.makeShaderInputResource( VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL ) );
		commandBuffer->endDebugBlock();
		commandBuffer->end();

		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( MaxTimeout );

		doCopyFromStagingTexture( data
			, format
			, extent );
	}

	void StagingTexture::downloadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkFormat format
		, uint8_t * data
		, ImageView const & view )const
	{
		auto mipLevel = view->subresourceRange.baseMipLevel;
		auto extent = getSubresourceDimensions( view.image->getDimensions(), mipLevel, format );
		downloadTextureData( queue
			, commandPool
			, {
				view->subresourceRange.aspectMask,
				mipLevel,
				view->subresourceRange.baseArrayLayer,
				view->subresourceRange.layerCount
			}
			, format
			, VkOffset3D{}
			, { extent.width, extent.height }
			, data
			, view );
	}

	void StagingTexture::doCopyToStagingTexture( uint8_t const * data
		, VkFormat format
		, VkExtent2D const & extent
		, uint32_t mipLevels )const
	{
		VkDeviceSize size = getLevelsSize( extent, format, 0u, mipLevels, 1u );
		assert( size <= m_buffer.getSize() );
		auto mappedSize = getAlignedSize( size
			, m_device.getProperties().limits.nonCoherentAtomSize );
		assert( size <= m_buffer.getSize() );
		auto buffer = m_buffer.lock( 0u
			, mappedSize
			, 0u );

		if ( !buffer )
		{
			throw Exception{ VK_ERROR_MEMORY_MAP_FAILED
				, "Staging buffer storage memory mapping" };
		}

		std::memcpy( buffer
			, data
			, size );
		m_buffer.flush( 0u, mappedSize );
		m_buffer.unlock();
	}

	void StagingTexture::doCopyStagingToDestination( CommandBuffer const & commandBuffer
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, ImageView const & texture )const
	{
		assert( getSize( extent, format ) <= m_buffer.getSize() );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, m_buffer.makeTransferSource() );
		commandBuffer.copyToImage( VkBufferImageCopy
			{
				0u,
				0u,
				0u,
				subresourceLayers,
				offset,
				VkExtent3D
				{
					std::max( 1u, extent.width ),
					std::max( 1u, extent.height ),
					1u
				}
			}
			, m_buffer
			, *texture.image );
	}

	void StagingTexture::doCopyDestinationToStaging( CommandBuffer const & commandBuffer
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, ImageView const & texture )const
	{
		assert( getSize( extent, format ) <= m_buffer.getSize() );
		commandBuffer.copyToBuffer( VkBufferImageCopy
			{
				0u,
				0u,
				0u,
				subresourceLayers,
				offset,
				VkExtent3D
				{
					std::max( 1u, extent.width ),
					std::max( 1u, extent.height ),
					1u
				}
			}
			, *texture.image
			, m_buffer );
	}

	void StagingTexture::doCopyFromStagingTexture( uint8_t * data
		, VkFormat format
		, VkExtent2D const & extent )const
	{
		VkDeviceSize size = getSize( extent, format );
		auto mappedSize = getAlignedSize( size
			, uint32_t( m_device.getProperties().limits.nonCoherentAtomSize ) );
		assert( size <= m_buffer.getSize() );
		auto buffer = m_buffer.lock( 0u
			, mappedSize
			, 0u );

		if ( !buffer )
		{
			throw Exception{ VK_ERROR_MEMORY_MAP_FAILED, "Staging buffer storage memory mapping" };
		}

		std::memcpy( data
			, buffer
			, size );
		m_buffer.flush( 0u, mappedSize );
		m_buffer.unlock();
	}

	uint8_t * StagingTexture::lock( VkDeviceSize offset
		, VkDeviceSize size
		, VkMemoryMapFlags flags )const
	{
		return m_buffer.lock( offset, size, flags );
	}

	void StagingTexture::invalidate( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		return m_buffer.invalidate( offset, size );
	}

	void StagingTexture::flush( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		return m_buffer.flush( offset, size );
	}

	void StagingTexture::unlock()const
	{
		return m_buffer.unlock();
	}
}
