/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Image/StagingTexture.hpp"

#include "AshesPP/Core/Device.hpp"
#include "AshesPP/Image/Image.hpp"
#include "AshesPP/Image/ImageView.hpp"

#include <AshesRenderer/Util/Exception.hpp>
#include <AshesRenderer/Util/Format.hpp>

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
		commandBuffer->begin( VkCommandBufferUsageFlagBits::VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
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
		commandBuffer->begin( VkCommandBufferUsageFlagBits::VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
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
		commandBuffer->begin( VkCommandBufferUsageFlagBits::VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
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
		commandBuffer->begin( VkCommandBufferUsageFlagBits::VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
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
		commandBuffer.memoryBarrier( VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT
			, view.makeTransferDestination( VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
				, 0u ) );
		doCopyStagingToDestination( commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, view );
		commandBuffer.memoryBarrier( VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT
			, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
			, view.makeShaderInputResource( VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
				, VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT ) );
	}

	void StagingTexture::uploadTextureData( CommandBuffer const & commandBuffer
		, VkFormat format
		, uint8_t const * const data
		, ImageView const & view )const
	{
		auto extent = VkExtent3D{ view.getImage().getDimensions() };
		auto mipLevel = view.getSubResourceRange().baseMipLevel;
		extent.width = std::max( 1u, extent.width >> mipLevel );
		extent.height = std::max( 1u, extent.height >> mipLevel );
		uploadTextureData( commandBuffer
			, {
				view.getSubResourceRange().aspectMask,
				mipLevel,
				view.getSubResourceRange().baseArrayLayer,
				view.getSubResourceRange().layerCount
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
		auto extent = view.getImage().getDimensions();
		auto mipLevel = view.getSubResourceRange().baseMipLevel;
		extent.width = std::max( 1u, extent.width >> mipLevel );
		extent.height = std::max( 1u, extent.height >> mipLevel );
		copyTextureData( commandBuffer
			, {
				view.getSubResourceRange().aspectMask,
				mipLevel,
				view.getSubResourceRange().baseArrayLayer,
				view.getSubResourceRange().layerCount
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
		commandBuffer.memoryBarrier( VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT
			, view.makeTransferDestination( VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
				, 0u ) );
		doCopyStagingToDestination( commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, view );
		commandBuffer.memoryBarrier( VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT
			, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
			, view.makeShaderInputResource( VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
				, VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT ) );
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
		commandBuffer->begin( VkCommandBufferUsageFlagBits::VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		commandBuffer->memoryBarrier( VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT
			, view.makeTransferSource( VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED
				, 0u ) );
		doCopyDestinationToStaging( *commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, view );
		commandBuffer->memoryBarrier( VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT
			, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
			, view.makeShaderInputResource( VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
				, VkAccessFlagBits::VK_ACCESS_TRANSFER_READ_BIT ) );
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
		auto extent = view.getImage().getDimensions();
		auto mipLevel = view.getSubResourceRange().baseMipLevel;
		extent.width = std::max( 1u, extent.width >> mipLevel );
		extent.height = std::max( 1u, extent.height >> mipLevel );
		downloadTextureData( queue
			, commandPool
			, {
				view.getSubResourceRange().aspectMask,
				mipLevel,
				view.getSubResourceRange().baseArrayLayer,
				view.getSubResourceRange().layerCount
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
		uint32_t size = getSize( extent, format );
		assert( size <= m_buffer.getSize() );
		auto mappedSize = getAlignedSize( size
			, uint32_t( m_device.getProperties().limits.nonCoherentAtomSize ) );
		assert( size <= m_buffer.getSize() );
		auto buffer = m_buffer.lock( 0u
			, mappedSize
			, 0u );

		if ( !buffer )
		{
			throw Exception{ VkResult::VK_ERROR_MEMORY_MAP_FAILED
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
		uint32_t size = getSize( extent, format );
		assert( size <= m_buffer.getSize() );
		commandBuffer.memoryBarrier( VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT
			, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT
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
			, texture.getImage() );
	}

	void StagingTexture::doCopyDestinationToStaging( CommandBuffer const & commandBuffer
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, ImageView const & texture )const
	{
		uint32_t size = getSize( extent, format );
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
			, texture.getImage()
			, m_buffer );
	}

	void StagingTexture::doCopyFromStagingTexture( uint8_t * data
		, VkFormat format
		, VkExtent2D const & extent )const
	{
		uint32_t size = getSize( extent, format );
		auto mappedSize = getAlignedSize( size
			, uint32_t( m_device.getProperties().limits.nonCoherentAtomSize ) );
		assert( size <= m_buffer.getSize() );
		auto buffer = m_buffer.lock( 0u
			, mappedSize
			, 0u );

		if ( !buffer )
		{
			throw Exception{ VkResult::VK_ERROR_MEMORY_MAP_FAILED, "Staging buffer storage memory mapping" };
		}

		std::memcpy( data
			, buffer
			, size );
		m_buffer.flush( 0u, mappedSize );
		m_buffer.unlock();
		m_device.waitIdle();
	}
}
