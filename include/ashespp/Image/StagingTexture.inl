/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Command/CommandBuffer.hpp"
#include "ashespp/Sync/Fence.hpp"

namespace ashes
{
	/**
	*\name
	*	Upload.
	**/
	/**@{*/
	inline void StagingTexture::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, ByteArray const & data
		, ImageView const & texture )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( "StagingTextureUpload"
			, VK_COMMAND_BUFFER_LEVEL_PRIMARY );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		uploadTextureData( *commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, data.data()
			, texture );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( MaxTimeout );
	}

	inline void StagingTexture::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkFormat format
		, ByteArray const & data
		, ImageView const & texture )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( "StagingTextureUpload"
			, VK_COMMAND_BUFFER_LEVEL_PRIMARY );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		uploadTextureData( *commandBuffer
			, format
			, data.data()
			, texture );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( MaxTimeout );
	}

	inline void StagingTexture::uploadTextureData( CommandBuffer const & commandBuffer
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, ByteArray const & data
		, ImageView const & texture )const
	{
		uploadTextureData( commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, data.data()
			, texture );
	}

	inline void StagingTexture::uploadTextureData( CommandBuffer const & commandBuffer
		, VkFormat format
		, ByteArray const & data
		, ImageView const & texture )const
	{
		uploadTextureData( commandBuffer
			, format
			, data.data()
			, texture );
	}
	/**@}*/
	/**
	*\name
	*	Upload.
	**/
	/**@{*/
	inline void StagingTexture::downloadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkImageSubresourceLayers const & subresourceLayers
		, VkFormat format
		, VkOffset3D const & offset
		, VkExtent2D const & extent
		, ByteArray & data
		, ImageView const & texture )const
	{
		downloadTextureData( queue
			, commandPool
			, subresourceLayers
			, format
			, offset
			, extent
			, data.data()
			, texture );
	}

	inline void StagingTexture::downloadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, VkFormat format
		, ByteArray & data
		, ImageView const & texture )const
	{
		downloadTextureData( queue
			, commandPool
			, format
			, data.data()
			, texture );
	}
	/**@}*/
}
