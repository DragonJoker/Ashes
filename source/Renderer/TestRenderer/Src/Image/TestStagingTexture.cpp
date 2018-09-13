/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Image/TestStagingTexture.hpp"

#include "Command/TestCommandBuffer.hpp"
#include "Core/TestDevice.hpp"
#include "Image/TestTextureView.hpp"

#include <Core/Exception.hpp>
#include <Sync/BufferMemoryBarrier.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

namespace test_renderer
{
	namespace
	{
		uint32_t getSize( renderer::Extent3D const & extent )
		{
			return std::max( 1u, extent.width )
				* std::max( 1u, extent.height )
				* std::max( 1u, extent.depth );
		}
	}

	StagingTexture::StagingTexture( Device const & device
		, renderer::Format format
		, renderer::Extent3D const & extent )
		: renderer::StagingTexture{ device, extent }
		, m_device{ device }
		, m_buffer{ device, getSize( extent ) * getSize( format ), renderer::BufferTarget::eTransferDst | renderer::BufferTarget::eTransferSrc }
	{
	}

	void StagingTexture::doCopyToStagingTexture( uint8_t const * data
		, renderer::Format format
		, renderer::Extent3D const & extent )const
	{
		uint32_t size = getSize( extent ) * getSize( format );
		assert( size <= m_buffer.getSize() );
		auto buffer = m_buffer.lock( 0u
			, size
			, renderer::MemoryMapFlag::eWrite | renderer::MemoryMapFlag::eInvalidateRange );

		if ( !buffer )
		{
			throw renderer::Exception{ renderer::Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping" };
		}

		std::memcpy( buffer
			, data
			, size );
		m_buffer.flush( 0u, size );
		m_buffer.unlock();
		m_device.waitIdle();
	}

	void StagingTexture::doCopyStagingToDestination( renderer::CommandBuffer const & commandBuffer
		, renderer::ImageSubresourceLayers const & subresourceLayers
		, renderer::Format format
		, renderer::Offset3D const & offset
		, renderer::Extent3D const & extent
		, renderer::TextureView const & texture )const
	{
		uint32_t size = getSize( extent ) * getSize( format );
		assert( size <= m_buffer.getSize() );
		commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit );
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTransfer
			, renderer::PipelineStageFlag::eTransfer
			, m_buffer.makeTransferSource() );
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTopOfPipe
			, renderer::PipelineStageFlag::eTransfer
			, texture.makeTransferDestination( renderer::ImageLayout::eUndefined
				, 0u ) );
		commandBuffer.copyToImage( renderer::BufferImageCopy
			{
				0u,
				0u,
				0u,
				subresourceLayers,
				offset,
				renderer::Extent3D{
					std::max( 1u, extent.width ),
					std::max( 1u, extent.height ),
					std::max( 1u, extent.depth )
				}
			}
			, m_buffer
			, texture.getTexture() );
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTransfer
			, renderer::PipelineStageFlag::eFragmentShader
			, texture.makeShaderInputResource( renderer::ImageLayout::eTransferDstOptimal
				, renderer::AccessFlag::eTransferWrite ) );
		commandBuffer.end();

		auto fence = m_device.createFence( {} );
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( renderer::FenceTimeout );
	}

	void StagingTexture::doCopyDestinationToStaging( renderer::CommandBuffer const & commandBuffer
		, renderer::ImageSubresourceLayers const & subresourceLayers
		, renderer::Format format
		, renderer::Offset3D const & offset
		, renderer::Extent3D const & extent
		, renderer::TextureView const & texture )const
	{
		uint32_t size = getSize( extent ) * getSize( format );
		assert( size <= m_buffer.getSize() );
		commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit );
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTopOfPipe
			, renderer::PipelineStageFlag::eTransfer
			, texture.makeTransferSource( renderer::ImageLayout::eUndefined
				, 0u ) );
		commandBuffer.copyToBuffer( renderer::BufferImageCopy
			{
				0u,
				0u,
				0u,
				subresourceLayers,
				offset,
				renderer::Extent3D{
					std::max( 1u, extent.width ),
					std::max( 1u, extent.height ),
					std::max( 1u, extent.depth )
				}
			}
			, texture.getTexture()
			, m_buffer );
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTransfer
			, renderer::PipelineStageFlag::eFragmentShader
			, texture.makeShaderInputResource( renderer::ImageLayout::eTransferSrcOptimal
				, renderer::AccessFlag::eTransferRead ) );
		commandBuffer.end();

		auto fence = m_device.createFence( {} );
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( renderer::FenceTimeout );
	}

	void StagingTexture::doCopyFromStagingTexture( uint8_t * data
		, renderer::Format format
		, renderer::Extent3D const & extent )const
	{
		uint32_t size = getSize( extent ) * getSize( format );
		assert( size <= m_buffer.getSize() );
		auto buffer = m_buffer.lock( 0u
			, size
			, renderer::MemoryMapFlag::eWrite | renderer::MemoryMapFlag::eInvalidateRange );

		if ( !buffer )
		{
			throw renderer::Exception{ renderer::Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping" };
		}

		std::memcpy( data
			, buffer
			, size );
		m_buffer.flush( 0u, size );
		m_buffer.unlock();
		m_device.waitIdle();
	}
}