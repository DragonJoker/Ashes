/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
namespace ashes
{
	/**
	*\name
	*	Upload.
	**/
	/**@{*/
	inline void StagingTexture::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, ByteArray const & data
		, TextureView const & texture )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		uploadTextureData( *commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, data.data()
			, texture );
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	inline void StagingTexture::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, Format format
		, ByteArray const & data
		, TextureView const & texture )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		uploadTextureData( *commandBuffer
			, format
			, data.data()
			, texture );
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	inline void StagingTexture::uploadTextureData( CommandBuffer const & commandBuffer
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, ByteArray const & data
		, TextureView const & texture )const
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
		, Format format
		, ByteArray const & data
		, TextureView const & texture )const
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
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, ByteArray & data
		, TextureView const & texture )const
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
		, Format format
		, ByteArray & data
		, TextureView const & texture )const
	{
		downloadTextureData( queue
			, commandPool
			, format
			, data.data()
			, texture );
	}
	/**@}*/
}
