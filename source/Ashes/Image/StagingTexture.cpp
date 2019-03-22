/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Image/StagingTexture.hpp"

#include "Ashes/Core/Device.hpp"
#include "Ashes/Core/Exception.hpp"
#include "Ashes/Image/Texture.hpp"
#include "Ashes/Image/TextureView.hpp"
#include "Ashes/Miscellaneous/Offset2D.hpp"
#include "Ashes/Sync/BufferMemoryBarrier.hpp"
#include "Ashes/Sync/ImageMemoryBarrier.hpp"

namespace ashes
{
	StagingTexture::StagingTexture( Device const & device
		, Extent2D const & extent )
		: m_device{ device }
	{
	}

	void StagingTexture::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, uint8_t const * const data
		, TextureView const & view )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
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
		fence->wait( FenceTimeout );
	}

	void StagingTexture::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, Format format
		, uint8_t const * const data
		, TextureView const & view )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		uploadTextureData( *commandBuffer
			, format
			, data
			, view );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	void StagingTexture::copyTextureData( Queue const & queue
		, CommandPool const & commandPool
		, Format format
		, TextureView const & view )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		copyTextureData( *commandBuffer
			, format
			, view );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	void StagingTexture::copyTextureData( Queue const & queue
		, CommandPool const & commandPool
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, TextureView const & view )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
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
		fence->wait( FenceTimeout );
	}

	void StagingTexture::uploadTextureData( CommandBuffer const & commandBuffer
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, uint8_t const * const data
		, TextureView const & view )const
	{
		doCopyToStagingTexture( data
			, format
			, extent );
		commandBuffer.memoryBarrier( ashes::PipelineStageFlag::eTopOfPipe
			, ashes::PipelineStageFlag::eTransfer
			, view.makeTransferDestination( ashes::ImageLayout::eUndefined
				, 0u ) );
		doCopyStagingToDestination( commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, view );
		commandBuffer.memoryBarrier( ashes::PipelineStageFlag::eTransfer
			, ashes::PipelineStageFlag::eFragmentShader
			, view.makeShaderInputResource( ashes::ImageLayout::eTransferDstOptimal
				, ashes::AccessFlag::eTransferWrite ) );
	}

	void StagingTexture::uploadTextureData( CommandBuffer const & commandBuffer
		, Format format
		, uint8_t const * const data
		, TextureView const & view )const
	{
		auto extent = Extent3D{ view.getTexture().getDimensions() };
		auto mipLevel = view.getSubResourceRange().baseMipLevel;
		extent.width = std::max( 1u, extent.width >> mipLevel );
		extent.height = std::max( 1u, extent.height >> mipLevel );
		uploadTextureData( commandBuffer
			, {
				getAspectMask( view.getFormat() ),
				mipLevel,
				view.getSubResourceRange().baseArrayLayer,
				view.getSubResourceRange().layerCount
			}
			, format
			, Offset3D{}
			, { extent.width, extent.height }
			, data
			, view );
	}

	void StagingTexture::copyTextureData( CommandBuffer const & commandBuffer
		, Format format
		, TextureView const & view )const
	{
		auto extent = view.getTexture().getDimensions();
		auto mipLevel = view.getSubResourceRange().baseMipLevel;
		extent.width = std::max( 1u, extent.width >> mipLevel );
		extent.height = std::max( 1u, extent.height >> mipLevel );
		copyTextureData( commandBuffer
			, {
				getAspectMask( view.getFormat() ),
				mipLevel,
				view.getSubResourceRange().baseArrayLayer,
				view.getSubResourceRange().layerCount
			}
			, format
			, Offset3D{}
			, { extent.width, extent.height }
			, view );
	}

	void StagingTexture::copyTextureData( CommandBuffer const & commandBuffer
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, TextureView const & view )const
	{
		commandBuffer.memoryBarrier( ashes::PipelineStageFlag::eTopOfPipe
			, ashes::PipelineStageFlag::eTransfer
			, view.makeTransferDestination( ashes::ImageLayout::eUndefined
				, 0u ) );
		doCopyStagingToDestination( commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, view );
		commandBuffer.memoryBarrier( ashes::PipelineStageFlag::eTransfer
			, ashes::PipelineStageFlag::eFragmentShader
			, view.makeShaderInputResource( ashes::ImageLayout::eTransferDstOptimal
				, ashes::AccessFlag::eTransferWrite ) );
	}

	void StagingTexture::downloadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, uint8_t * data
		, TextureView const & view )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		commandBuffer->memoryBarrier( ashes::PipelineStageFlag::eTopOfPipe
			, ashes::PipelineStageFlag::eTransfer
			, view.makeTransferSource( ashes::ImageLayout::eUndefined
				, 0u ) );
		doCopyDestinationToStaging( *commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, view );
		commandBuffer->memoryBarrier( ashes::PipelineStageFlag::eTransfer
			, ashes::PipelineStageFlag::eFragmentShader
			, view.makeShaderInputResource( ashes::ImageLayout::eTransferSrcOptimal
				, ashes::AccessFlag::eTransferRead ) );
		commandBuffer->end();

		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( ashes::FenceTimeout );

		doCopyFromStagingTexture( data
			, format
			, extent );
	}

	void StagingTexture::downloadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, Format format
		, uint8_t * data
		, TextureView const & view )const
	{
		auto extent = view.getTexture().getDimensions();
		auto mipLevel = view.getSubResourceRange().baseMipLevel;
		extent.width = std::max( 1u, extent.width >> mipLevel );
		extent.height = std::max( 1u, extent.height >> mipLevel );
		downloadTextureData( queue
			, commandPool
			, {
				getAspectMask( view.getFormat() ),
				mipLevel,
				view.getSubResourceRange().baseArrayLayer,
				view.getSubResourceRange().layerCount
			}
			, format
			, Offset3D{}
			, { extent.width, extent.height }
			, data
			, view );
	}
}
