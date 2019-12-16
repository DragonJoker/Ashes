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
		, VkExtent2D const & extent )
		: m_device{ device }
		, m_buffer
		{
			device,
			uint32_t( getAlignedSize( getSize( extent, format )
				, uint32_t( m_device.getProperties().limits.nonCoherentAtomSize ) ) ),
			VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT | VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		}
	{
		auto requirements = m_buffer.getMemoryRequirements();
		auto deduced = m_device.deduceMemoryType( requirements.memoryTypeBits
			, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
		m_buffer.bindMemory( device.allocateMemory(
			{
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
		, uint8_t const * const data
		, ImageView const & view )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		uploadTextureData( *commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, data
			, view );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( MaxTimeout );
	}

	void StagingTexture::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkFormat format
		, uint8_t const * const data
		, ImageView const & view )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		uploadTextureData( *commandBuffer
			, format
			, data
			, view );
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
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		copyTextureData( *commandBuffer
			, format
			, view );
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
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		copyTextureData( *commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, view );
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
		, uint8_t const * const data
		, ImageView const & view )const
	{
		doCopyToStagingTexture( data
			, format
			, extent );
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
		, uint8_t const * const data
		, ImageView const & view )const
	{
		auto extent = VkExtent3D{ view.image->getDimensions() };
		auto mipLevel = view->subresourceRange.baseMipLevel;
		extent.width = std::max( 1u, extent.width >> mipLevel );
		extent.height = std::max( 1u, extent.height >> mipLevel );
		uploadTextureData( commandBuffer
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

	void StagingTexture::copyTextureData( CommandBuffer const & commandBuffer
		, VkFormat format
		, ImageView const & view )const
	{
		auto extent = view.image->getDimensions();
		auto mipLevel = view->subresourceRange.baseMipLevel;
		extent.width = std::max( 1u, extent.width >> mipLevel );
		extent.height = std::max( 1u, extent.height >> mipLevel );
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
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
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
		auto extent = view.image->getDimensions();
		auto mipLevel = view->subresourceRange.baseMipLevel;
		extent.width = std::max( 1u, extent.width >> mipLevel );
		extent.height = std::max( 1u, extent.height >> mipLevel );
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
		, VkExtent2D const & extent )const
	{
		VkDeviceSize size = getSize( extent, format );
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
		m_device.waitIdle();
	}

	void StagingTexture::doCopyStagingToDestination( CommandBuffer const & commandBuffer
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, ImageView const & texture )const
	{
		VkDeviceSize size = getSize( extent, format );
		assert( size <= m_buffer.getSize() );
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
		VkDeviceSize size = getSize( extent, format );
		assert( size <= m_buffer.getSize() );
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
		m_device.waitIdle();
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
