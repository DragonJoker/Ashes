/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Image/StagingTexture.hpp"

#include "Ashes/Core/Device.hpp"
#include "Ashes/Core/Exception.hpp"
#include "Ashes/Image/Image.hpp"
#include "Ashes/Image/ImageView.hpp"
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

	void StagingTexture::copyTextureData( Queue const & queue
		, CommandPool const & commandPool
		, Format format
		, ImageView const & view )const
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
		, ImageView const & view )const
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

	void StagingTexture::copyTextureData( CommandBuffer const & commandBuffer
		, Format format
		, ImageView const & view )const
	{
		auto extent = view.getImage().getDimensions();
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
		, ImageView const & view )const
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
}
