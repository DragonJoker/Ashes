/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "CommandBuffer.hpp"

#include "BufferMemoryBarrier.hpp"
#include "CommandPool.hpp"
#include "DescriptorSet.hpp"
#include "ImageMemoryBarrier.hpp"
#include "Pipeline.hpp"
#include "PipelineLayout.hpp"
#include "RenderingResources.hpp"
#include "RenderPass.hpp"
#include "Scissor.hpp"
#include "Texture.hpp"
#include "Viewport.hpp"

namespace renderer
{
	namespace
	{
		vk::CommandBufferCRefArray convert( CommandBufferCRefArray const & commands )
		{
			vk::CommandBufferCRefArray result;

			for ( auto & command : commands )
			{
				result.emplace_back( command.get().getCommandBuffer() );
			}

			return result;
		}
	}

	CommandBuffer::CommandBuffer( Device const & device
		, CommandPool const & pool
		, bool primary )
		: m_commandBuffer{ std::make_unique< vk::CommandBuffer >( device.getDevice()
			, pool.getCommandPool()
			, primary ) }
	{
	}

	CommandBuffer::CommandBuffer( vk::CommandBufferPtr && commandBuffer )
		: m_commandBuffer{ std::move( commandBuffer ) }
	{
	}

	bool CommandBuffer::begin( CommandBufferUsageFlags flags )const
	{
		return m_commandBuffer->begin( convert( flags ) );
	}

	bool CommandBuffer::begin( CommandBufferUsageFlags flags
		, RenderPass const & renderPass
		, uint32_t subpass
		, FrameBuffer const & frameBuffer
		, bool occlusionQueryEnable
		, QueryControlFlags queryFlags
		, QueryPipelineStatisticFlags pipelineStatistics )const
	{
		return m_commandBuffer->begin( convert( flags )
			, renderPass.getRenderPass()
			, subpass
			, frameBuffer.getFrameBuffer()
			, occlusionQueryEnable
			, queryFlags
			, pipelineStatistics );
	}

	bool CommandBuffer::end()const
	{
		return vk::checkError( m_commandBuffer->end() );
	}

	bool CommandBuffer::reset( CommandBufferResetFlags flags )const
	{
		return vk::checkError( m_commandBuffer->reset( convert( flags ) ) );
	}

	void CommandBuffer::beginRenderPass( RenderPass const & renderPass
		, FrameBuffer const & frameBuffer
		, ClearValueArray const & clearValues
		, SubpassContents contents )const
	{
		m_commandBuffer->beginRenderPass( renderPass.getRenderPass()
			, frameBuffer.getFrameBuffer()
			, convert< VkClearValue >( clearValues )
			, convert( contents ) );
	}

	void CommandBuffer::nextSubpass( SubpassContents contents )const
	{
		m_commandBuffer->nextSubpass( convert( contents ) );
	}

	void CommandBuffer::endRenderPass()const
	{
		m_commandBuffer->endRenderPass();
	}

	void CommandBuffer::executeCommands( CommandBufferCRefArray const & commands )const
	{
		m_commandBuffer->executeCommands( convert( commands ) );
	}

	void CommandBuffer::clear( Texture const & image
		, RgbaColour const & colour )const
	{
		m_commandBuffer->clear( image.getImage(), convert( colour ) );
	}

	void CommandBuffer::bindPipeline( Pipeline const & pipeline
		, PipelineBindPoint bindingPoint )const
	{
		m_commandBuffer->bindPipeline( pipeline.getPipeline()
			, convert( bindingPoint ) );
	}

	void CommandBuffer::bindVertexBuffer( VertexBufferBase const & vertexBuffer
		, uint64_t offset )const
	{
		m_commandBuffer->bindVertexBuffer( vertexBuffer.getVbo()
			, offset );
	}

	void CommandBuffer::bindVertexBuffers( std::vector< std::reference_wrapper< VertexBufferBase const > > const & vertexBuffers
		, std::vector< uint64_t > offsets )const
	{
		std::vector< std::reference_wrapper< vk::VertexBuffer const > > buffers;

		for ( auto & buffer : vertexBuffers )
		{
			buffers.emplace_back( buffer.get().getVbo() );
		}

		m_commandBuffer->bindVertexBuffers( buffers
			, offsets );
	}

	void CommandBuffer::memoryBarrier( PipelineStageFlags after
		, PipelineStageFlags before
		, BufferMemoryBarrier const & transitionBarrier )const
	{
		m_commandBuffer->memoryBarrier( convert( after )
			, convert( before )
			, transitionBarrier.getBarrier() );
	}

	void CommandBuffer::memoryBarrier( PipelineStageFlags after
		, PipelineStageFlags before
		, ImageMemoryBarrier const & transitionBarrier )const
	{
		m_commandBuffer->memoryBarrier( convert( after )
			, convert( before )
			, transitionBarrier.getBarrier() );
	}

	void CommandBuffer::bindDescriptorSet( DescriptorSet const & descriptorSet
		, PipelineLayout const & layout
		, PipelineBindPoint bindingPoint )const
	{
		m_commandBuffer->bindDescriptorSet( descriptorSet.getDescriptorSet()
			, layout.getLayout()
			, convert( bindingPoint ) );
	}

	void CommandBuffer::setViewport( Viewport const & viewport )const
	{
		m_commandBuffer->setViewport( viewport.getViewport() );
	}

	void CommandBuffer::setScissor( Scissor const & scissor )const
	{
		m_commandBuffer->setScissor( scissor.getScissor() );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		m_commandBuffer->draw( vtxCount
			, instCount
			, firstVertex
			, firstInstance );
	}

	void CommandBuffer::drawIndexed( uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance )const
	{
		m_commandBuffer->drawIndexed( indexCount
			, instCount
			, firstIndex
			, vertexOffset
			, firstInstance );
	}

	void CommandBuffer::copyImage( StagingBuffer const & src
		, Texture const & dst )const
	{
		m_commandBuffer->copyImage( src.getStagingBuffer().getBuffer()
			, dst.getImage() );
	}
}
