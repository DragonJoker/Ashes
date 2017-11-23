/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "CommandBuffer.hpp"

#include "BufferMemoryBarrier.hpp"
#include "DescriptorSet.hpp"
#include "ImageMemoryBarrier.hpp"
#include "Pipeline.hpp"
#include "PipelineLayout.hpp"
#include "RenderingResources.hpp"
#include "Scissor.hpp"
#include "Texture.hpp"
#include "Viewport.hpp"

namespace renderer
{
	CommandBuffer::CommandBuffer( Device const & device
		, vk::CommandPool const & pool )
		: m_device{ device }
		, m_commandBuffer{ m_device.getDevice(), pool }
	{
	}

	bool CommandBuffer::begin( CommandBufferUsageFlags flags )const
	{
		return m_commandBuffer.begin( convert( flags ) );
	}

	bool CommandBuffer::end()const
	{
		return vk::checkError( m_commandBuffer.end() );
	}

	bool CommandBuffer::reset( CommandBufferResetFlags flags )const
	{
		return vk::checkError( m_commandBuffer.reset( convert( flags ) ) );
	}

	void CommandBuffer::clear( Texture const & image
		, RgbaColour const & colour )const
	{
		VkClearColorValue col;
		col.float32[0] = colour.r;
		col.float32[1] = colour.g;
		col.float32[2] = colour.b;
		col.float32[3] = colour.a;
		m_commandBuffer.clear( image.getImage(), col );
	}

	void CommandBuffer::bindPipeline( Pipeline const & pipeline
		, PipelineBindPoint bindingPoint )const
	{
		m_commandBuffer.bindPipeline( pipeline.getPipeline()
			, convert( bindingPoint ) );
	}

	void CommandBuffer::bindVertexBuffer( VertexBufferBase const & vertexBuffer
		, uint64_t offset )const
	{
		m_commandBuffer.bindVertexBuffer( vertexBuffer.getVbo()
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

		m_commandBuffer.bindVertexBuffers( buffers
			, offsets );
	}

	void CommandBuffer::memoryBarrier( PipelineStageFlags after
		, PipelineStageFlags before
		, BufferMemoryBarrier const & transitionBarrier )const
	{
		m_commandBuffer.memoryBarrier( convert( after )
			, convert( before )
			, transitionBarrier.getBarrier() );
	}

	void CommandBuffer::memoryBarrier( PipelineStageFlags after
		, PipelineStageFlags before
		, ImageMemoryBarrier const & transitionBarrier )const
	{
		m_commandBuffer.memoryBarrier( convert( after )
			, convert( before )
			, transitionBarrier.getBarrier() );
	}

	void CommandBuffer::bindDescriptorSet( DescriptorSet const & descriptorSet
		, PipelineLayout const & layout
		, PipelineBindPoint bindingPoint )const
	{
		m_commandBuffer.bindDescriptorSet( descriptorSet.getDescriptorSet()
			, layout.getLayout()
			, convert( bindingPoint ) );
	}

	void CommandBuffer::setViewport( Viewport const & viewport )const
	{
		m_commandBuffer.setViewport( viewport.getViewport() );
	}

	void CommandBuffer::setScissor( Scissor const & scissor )const
	{
		m_commandBuffer.setScissor( scissor.getScissor() );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		m_commandBuffer.draw( vtxCount
			, instCount
			, firstVertex
			, firstInstance );
	}
}
