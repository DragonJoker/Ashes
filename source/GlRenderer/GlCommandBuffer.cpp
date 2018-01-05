/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "GlCommandBuffer.hpp"

#include "GlBuffer.hpp"
#include "GlBufferMemoryBarrier.hpp"
#include "GlCommandPool.hpp"
#include "GlDescriptorSet.hpp"
#include "GlDevice.hpp"
#include "GlFrameBuffer.hpp"
#include "GlGeometryBuffers.hpp"
#include "GlImageMemoryBarrier.hpp"
#include "GlPipeline.hpp"
#include "GlPipelineLayout.hpp"
#include "GlRenderingResources.hpp"
#include "GlRenderPass.hpp"
#include "GlStagingBuffer.hpp"
#include "GlTexture.hpp"
#include "GlUniformBuffer.hpp"

#include "Commands/GlBeginRenderPassCommand.hpp"
#include "Commands/GlBindDescriptorSetCommand.hpp"
#include "Commands/GlBindGeometryBuffersCommand.hpp"
#include "Commands/GlBindPipelineCommand.hpp"
#include "Commands/GlCopyBufferCommand.hpp"
#include "Commands/GlCopyImageCommand.hpp"
#include "Commands/GlDrawCommand.hpp"
#include "Commands/GlDrawIndexedCommand.hpp"
#include "Commands/GlClearCommand.hpp"
#include "Commands/GlEndRenderPassCommand.hpp"
#include "Commands/GlScissorCommand.hpp"
#include "Commands/GlViewportCommand.hpp"

namespace gl_renderer
{
	CommandBuffer::CommandBuffer( renderer::Device const & device
		, renderer::CommandPool const & pool
		, bool primary )
		: renderer::CommandBuffer{ device, pool, primary }
	{
	}

	bool CommandBuffer::begin( renderer::CommandBufferUsageFlags flags )const
	{
		m_beginFlags = flags;
		return true;
	}

	bool CommandBuffer::begin( renderer::CommandBufferUsageFlags flags
		, renderer::RenderPass const & renderPass
		, uint32_t subpass
		, renderer::FrameBuffer const & frameBuffer
		, bool occlusionQueryEnable
		, renderer::QueryControlFlags queryFlags
		, renderer::QueryPipelineStatisticFlags pipelineStatistics )const
	{
		m_commands.clear();
		m_beginFlags = flags;
		return true;
	}

	bool CommandBuffer::end()const
	{
		return glGetError() == GL_NO_ERROR;
	}

	bool CommandBuffer::reset( renderer::CommandBufferResetFlags flags )const
	{
		m_commands.clear();
		return true;
	}

	void CommandBuffer::beginRenderPass( renderer::RenderPass const & renderPass
		, renderer::FrameBuffer const & frameBuffer
		, renderer::ClearValueArray const & clearValues
		, renderer::SubpassContents contents )const
	{
		m_commands.emplace_back( std::make_unique< BeginRenderPassCommand >( renderPass
			, frameBuffer
			, clearValues
			, contents ) );
	}

	void CommandBuffer::nextSubpass( renderer::SubpassContents contents )const
	{
	}

	void CommandBuffer::endRenderPass()const
	{
		m_commands.emplace_back( std::make_unique< EndRenderPassCommand >() );
	}

	void CommandBuffer::executeCommands( renderer::CommandBufferCRefArray const & commands )const
	{
		for ( auto & commandBuffer : commands )
		{
			for ( auto & command : static_cast< CommandBuffer const & >( commandBuffer.get() ).getCommands() )
			{
				m_commands.emplace_back( command->clone() );
			}
		}
	}

	void CommandBuffer::clear( renderer::Texture const & image
		, renderer::RgbaColour const & colour )const
	{
		m_commands.emplace_back( std::make_unique< ClearCommand >( image, colour ) );
	}

	void CommandBuffer::bindPipeline( renderer::Pipeline const & pipeline
		, renderer::PipelineBindPoint bindingPoint )const
	{
		m_currentPipeline = &pipeline;
		m_commands.emplace_back( std::make_unique< BindPipelineCommand >( pipeline, bindingPoint ) );
	}

	void CommandBuffer::bindGeometryBuffers( renderer::GeometryBuffers const & geometryBuffers )const
	{
		m_commands.emplace_back( std::make_unique< BindGeometryBuffersCommand >( geometryBuffers ) );

		if ( geometryBuffers.hasIbo() )
		{
			m_indexType = geometryBuffers.getIbo().type;
		}
	}

	void CommandBuffer::memoryBarrier( renderer::PipelineStageFlags after
		, renderer::PipelineStageFlags before
		, renderer::BufferMemoryBarrier const & transitionBarrier )const
	{
		//m_commandBuffer->memoryBarrier( convert( after )
		//	, convert( before )
		//	, convert( transitionBarrier ) );
	}

	void CommandBuffer::memoryBarrier( renderer::PipelineStageFlags after
		, renderer::PipelineStageFlags before
		, renderer::ImageMemoryBarrier const & transitionBarrier )const
	{
		//m_commandBuffer->memoryBarrier( convert( after )
		//	, convert( before )
		//	, convert( transitionBarrier ) );
	}

	void CommandBuffer::bindDescriptorSet( renderer::DescriptorSet const & descriptorSet
		, renderer::PipelineLayout const & layout
		, renderer::PipelineBindPoint bindingPoint )const
	{
		m_commands.emplace_back( std::make_unique< BindDescriptorSetCommand >( descriptorSet, layout, bindingPoint ) );
		//m_commandBuffer->bindDescriptorSet( static_cast< DescriptorSet const & >( descriptorSet ).getDescriptorSet()
		//	, static_cast< PipelineLayout const & >( layout ).getLayout()
		//	, convert( bindingPoint ) );
	}

	void CommandBuffer::setViewport( renderer::Viewport const & viewport )const
	{
		m_commands.emplace_back( std::make_unique< ViewportCommand >( viewport ) );
	}

	void CommandBuffer::setScissor( renderer::Scissor const & scissor )const
	{
		m_commands.emplace_back( std::make_unique< ScissorCommand >( scissor ) );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		m_commands.emplace_back( std::make_unique< DrawCommand >( vtxCount
			, instCount
			, firstVertex
			, firstInstance
			, m_currentPipeline->getPrimitiveType() ) );
	}

	void CommandBuffer::drawIndexed( uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance )const
	{
		m_commands.emplace_back( std::make_unique< DrawIndexedCommand >( indexCount
			, instCount
			, firstIndex
			, vertexOffset
			, firstInstance
			, m_currentPipeline->getPrimitiveType()
			, m_indexType ) );
	}

	void CommandBuffer::copyBuffer( renderer::BufferBase const & src
		, renderer::BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commands.emplace_back( std::make_unique< CopyBufferCommand >( src
			, dst
			, size
			, offset ) );
	}

	void CommandBuffer::copyBuffer( renderer::BufferBase const & src
		, renderer::VertexBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commands.emplace_back( std::make_unique< CopyBufferCommand >( src
			, dst.getBuffer()
			, size
			, offset ) );
	}

	void CommandBuffer::copyBuffer( renderer::VertexBufferBase const & src
		, renderer::BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commands.emplace_back( std::make_unique< CopyBufferCommand >( src.getBuffer()
			, dst
			, size
			, offset ) );
	}

	void CommandBuffer::copyBuffer( renderer::VertexBufferBase const & src
		, renderer::VertexBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commands.emplace_back( std::make_unique< CopyBufferCommand >( src.getBuffer()
			, dst.getBuffer()
			, size
			, offset ) );
	}

	void CommandBuffer::copyBuffer( renderer::BufferBase const & src
		, renderer::UniformBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commands.emplace_back( std::make_unique< CopyBufferCommand >( src
			, dst.getBuffer()
			, size
			, offset ) );
	}

	void CommandBuffer::copyBuffer( renderer::UniformBufferBase const & src
		, renderer::BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commands.emplace_back( std::make_unique< CopyBufferCommand >( src.getBuffer()
			, dst
			, size
			, offset ) );
	}

	void CommandBuffer::copyBuffer( renderer::UniformBufferBase const & src
		, renderer::UniformBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commands.emplace_back( std::make_unique< CopyBufferCommand >( src.getBuffer()
			, dst.getBuffer()
			, size
			, offset ) );
	}

	void CommandBuffer::copyImage( renderer::BufferBase const & src
		, renderer::Texture const & dst )const
	{
		m_commands.emplace_back( std::make_unique< CopyImageCommand >( src
			, dst ) );
	}

	void CommandBuffer::copyImage( renderer::StagingBuffer const & src
		, renderer::Texture const & dst )const
	{
		m_commands.emplace_back( std::make_unique< CopyImageCommand >( src.getBuffer()
			, dst ) );
	}
}
