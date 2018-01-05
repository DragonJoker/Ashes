/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkCommandBuffer.hpp"

#include "VkBuffer.hpp"
#include "VkBufferMemoryBarrier.hpp"
#include "VkCommandPool.hpp"
#include "VkDescriptorSet.hpp"
#include "VkDevice.hpp"
#include "VkFrameBuffer.hpp"
#include "VkGeometryBuffers.hpp"
#include "VkImageMemoryBarrier.hpp"
#include "VkPipeline.hpp"
#include "VkPipelineLayout.hpp"
#include "VkRenderingResources.hpp"
#include "VkRenderPass.hpp"
#include "VkScissor.hpp"
#include "VkStagingBuffer.hpp"
#include "VkTexture.hpp"
#include "VkUniformBuffer.hpp"
#include "VkViewport.hpp"

namespace vk_renderer
{
	namespace
	{
		vk::CommandBufferCRefArray convert( renderer::CommandBufferCRefArray const & commands )
		{
			vk::CommandBufferCRefArray result;

			for ( auto & command : commands )
			{
				result.emplace_back( static_cast< CommandBuffer const & >( command.get() ).getCommandBuffer() );
			}

			return result;
		}
	}

	CommandBuffer::CommandBuffer( renderer::Device const & device
		, renderer::CommandPool const & pool
		, bool primary )
		: renderer::CommandBuffer{ device, pool, primary }
		, m_commandBuffer{ std::make_unique< vk::CommandBuffer >( static_cast< Device const & >( device ).getDevice()
			, static_cast< CommandPool const & >( pool ).getCommandPool()
			, primary ) }
	{
	}

	CommandBuffer::CommandBuffer( vk::CommandBufferPtr && commandBuffer )
		: renderer::CommandBuffer{}
		, m_commandBuffer{ std::move( commandBuffer ) }
	{
	}

	bool CommandBuffer::begin( renderer::CommandBufferUsageFlags flags )const
	{
		return m_commandBuffer->begin( convert( flags ) );
	}

	bool CommandBuffer::begin( renderer::CommandBufferUsageFlags flags
		, renderer::RenderPass const & renderPass
		, uint32_t subpass
		, renderer::FrameBuffer const & frameBuffer
		, bool occlusionQueryEnable
		, renderer::QueryControlFlags queryFlags
		, renderer::QueryPipelineStatisticFlags pipelineStatistics )const
	{
		return m_commandBuffer->begin( convert( flags )
			, static_cast< RenderPass const & >( renderPass ).getRenderPass()
			, subpass
			, static_cast< FrameBuffer const & >( frameBuffer ).getFrameBuffer()
			, occlusionQueryEnable
			, queryFlags
			, pipelineStatistics );
	}

	bool CommandBuffer::end()const
	{
		return vk::checkError( m_commandBuffer->end() );
	}

	bool CommandBuffer::reset( renderer::CommandBufferResetFlags flags )const
	{
		return vk::checkError( m_commandBuffer->reset( convert( flags ) ) );
	}

	void CommandBuffer::beginRenderPass( renderer::RenderPass const & renderPass
		, renderer::FrameBuffer const & frameBuffer
		, renderer::ClearValueArray const & clearValues
		, renderer::SubpassContents contents )const
	{
		m_commandBuffer->beginRenderPass( static_cast< RenderPass const & >( renderPass ).getRenderPass()
			, static_cast< FrameBuffer const & >( frameBuffer ).getFrameBuffer()
			, convert< VkClearValue >( clearValues )
			, convert( contents ) );
	}

	void CommandBuffer::nextSubpass( renderer::SubpassContents contents )const
	{
		m_commandBuffer->nextSubpass( convert( contents ) );
	}

	void CommandBuffer::endRenderPass()const
	{
		m_commandBuffer->endRenderPass();
	}

	void CommandBuffer::executeCommands( renderer::CommandBufferCRefArray const & commands )const
	{
		m_commandBuffer->executeCommands( convert( commands ) );
	}

	void CommandBuffer::clear( renderer::Texture const & image
		, renderer::RgbaColour const & colour )const
	{
		m_commandBuffer->clear( static_cast< Texture const & >( image ).getImage()
			, convert( colour ) );
	}

	void CommandBuffer::bindPipeline( renderer::Pipeline const & pipeline
		, renderer::PipelineBindPoint bindingPoint )const
	{
		m_commandBuffer->bindPipeline( static_cast< Pipeline const & >( pipeline ).getPipeline()
			, convert( bindingPoint ) );
	}

	void CommandBuffer::bindGeometryBuffers( renderer::GeometryBuffers const & geometryBuffers )const
	{
		std::vector< std::reference_wrapper< vk::Buffer const > > buffers;
		std::vector< uint64_t > offsets;

		for ( auto & vbo : geometryBuffers.getVbos() )
		{
			buffers.emplace_back( static_cast< BufferBase const & >( vbo.vbo.getBuffer() ).getBuffer() );
			offsets.emplace_back( vbo.offset );
		}

		m_commandBuffer->bindVertexBuffers( buffers
			, offsets );

		if ( geometryBuffers.hasIbo() )
		{
			auto & ibo = geometryBuffers.getIbo();
			m_commandBuffer->bindIndexBuffer( static_cast< BufferBase const & >( ibo.buffer ).getBuffer()
				, ibo.offset
				, convert( ibo.type ) );
		}
	}

	void CommandBuffer::memoryBarrier( renderer::PipelineStageFlags after
		, renderer::PipelineStageFlags before
		, renderer::BufferMemoryBarrier const & transitionBarrier )const
	{
		m_commandBuffer->memoryBarrier( convert( after )
			, convert( before )
			, convert( transitionBarrier ) );
	}

	void CommandBuffer::memoryBarrier( renderer::PipelineStageFlags after
		, renderer::PipelineStageFlags before
		, renderer::ImageMemoryBarrier const & transitionBarrier )const
	{
		m_commandBuffer->memoryBarrier( convert( after )
			, convert( before )
			, convert( transitionBarrier ) );
	}

	void CommandBuffer::bindDescriptorSet( renderer::DescriptorSet const & descriptorSet
		, renderer::PipelineLayout const & layout
		, renderer::PipelineBindPoint bindingPoint )const
	{
		m_commandBuffer->bindDescriptorSet( static_cast< DescriptorSet const & >( descriptorSet ).getDescriptorSet()
			, static_cast< PipelineLayout const & >( layout ).getLayout()
			, convert( bindingPoint ) );
	}

	void CommandBuffer::setViewport( renderer::Viewport const & viewport )const
	{
		m_commandBuffer->setViewport( convert( viewport ) );
	}

	void CommandBuffer::setScissor( renderer::Scissor const & scissor )const
	{
		m_commandBuffer->setScissor( convert( scissor ) );
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

	void CommandBuffer::copyBuffer( renderer::BufferBase const & src
		, renderer::BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer->copyBuffer( static_cast< BufferBase const & >( src ).getBuffer()
			, static_cast< BufferBase const & >( dst ).getBuffer()
			, size
			, offset );
	}

	void CommandBuffer::copyBuffer( renderer::BufferBase const & src
		, renderer::VertexBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer->copyBuffer( static_cast< BufferBase const & >( src ).getBuffer()
			, static_cast< BufferBase const & >( dst.getBuffer() ).getBuffer()
			, size
			, offset );
	}

	void CommandBuffer::copyBuffer( renderer::VertexBufferBase const & src
		, renderer::BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer->copyBuffer( static_cast< BufferBase const & >( src.getBuffer() ).getBuffer()
			, static_cast< BufferBase const & >( dst ).getBuffer()
			, size
			, offset );
	}

	void CommandBuffer::copyBuffer( renderer::VertexBufferBase const & src
		, renderer::VertexBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer->copyBuffer( static_cast< BufferBase const & >( src.getBuffer() ).getBuffer()
			, static_cast< BufferBase const & >( dst.getBuffer() ).getBuffer()
			, size
			, offset );
	}

	void CommandBuffer::copyBuffer( renderer::BufferBase const & src
		, renderer::UniformBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer->copyBuffer( static_cast< BufferBase const & >( src ).getBuffer()
			, static_cast< BufferBase const & >( dst.getBuffer() ).getBuffer()
			, size
			, offset );
	}

	void CommandBuffer::copyBuffer( renderer::UniformBufferBase const & src
		, renderer::BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer->copyBuffer( static_cast< BufferBase const & >( src.getBuffer() ).getBuffer()
			, static_cast< BufferBase const & >( dst ).getBuffer()
			, size
			, offset );
	}

	void CommandBuffer::copyBuffer( renderer::UniformBufferBase const & src
		, renderer::UniformBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer->copyBuffer( static_cast< BufferBase const & >( src.getBuffer() ).getBuffer()
			, static_cast< BufferBase const & >( dst.getBuffer() ).getBuffer()
			, size
			, offset );
	}

	void CommandBuffer::copyImage( renderer::BufferBase const & src
		, renderer::Texture const & dst )const
	{
		m_commandBuffer->copyImage( static_cast< BufferBase const & >( src ).getBuffer()
			, static_cast< Texture const & >( dst ).getImage() );
	}

	void CommandBuffer::copyImage( renderer::StagingBuffer const & src
		, renderer::Texture const & dst )const
	{
		m_commandBuffer->copyImage( static_cast< BufferBase const & >( src.getBuffer() ).getBuffer()
			, static_cast< Texture const & >( dst ).getImage() );
	}
}
