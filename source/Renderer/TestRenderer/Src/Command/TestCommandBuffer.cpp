/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Command/TestCommandBuffer.hpp"

#include "Buffer/TestBuffer.hpp"
#include "Buffer/TestUniformBuffer.hpp"
#include "Command/TestCommandPool.hpp"
#include "Core/TestDevice.hpp"
#include "Descriptor/TestDescriptorSet.hpp"
#include "Image/TestTexture.hpp"
#include "Image/TestTextureView.hpp"
#include "Miscellaneous/TestQueryPool.hpp"
#include "Pipeline/TestComputePipeline.hpp"
#include "Pipeline/TestPipeline.hpp"
#include "Pipeline/TestPipelineLayout.hpp"
#include "RenderPass/TestFrameBuffer.hpp"
#include "RenderPass/TestRenderPass.hpp"

#include <Buffer/PushConstantsBuffer.hpp>
#include <Buffer/StagingBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <RenderPass/FrameBufferAttachment.hpp>

namespace test_renderer
{
	namespace
	{
		CommandBufferCRefArray convert( renderer::CommandBufferCRefArray const & commands )
		{
			CommandBufferCRefArray result;

			for ( auto & command : commands )
			{
				result.emplace_back( static_cast< CommandBuffer const & >( command.get() ) );
			}

			return result;
		}

		DescriptorSetCRefArray convert( renderer::DescriptorSetCRefArray const & descriptors )
		{
			DescriptorSetCRefArray result;

			for ( auto & descriptor : descriptors )
			{
				result.emplace_back( static_cast< DescriptorSet const & >( descriptor.get() ) );
			}

			return result;
		}
	}

	CommandBuffer::CommandBuffer( Device const & device
		, CommandPool const & pool
		, bool primary )
		: renderer::CommandBuffer{ device, pool, primary }
		, m_device{ device }
		, m_pool{ pool }
	{
	}

	CommandBuffer::~CommandBuffer()
	{
	}

	void CommandBuffer::begin( renderer::CommandBufferUsageFlags flags )const
	{
		m_currentPipeline = nullptr;
		m_currentComputePipeline = nullptr;
	}

	void CommandBuffer::begin( renderer::CommandBufferUsageFlags flags
		, renderer::CommandBufferInheritanceInfo const & inheritanceInfo )const
	{
		m_currentPipeline = nullptr;
		m_currentComputePipeline = nullptr;
	}

	void CommandBuffer::end()const
	{
		m_currentPipeline = nullptr;
		m_currentComputePipeline = nullptr;
	}

	void CommandBuffer::reset( renderer::CommandBufferResetFlags flags )const
	{
	}

	void CommandBuffer::beginRenderPass( renderer::RenderPass const & renderPass
		, renderer::FrameBuffer const & frameBuffer
		, renderer::ClearValueArray const & clearValues
		, renderer::SubpassContents contents )const
	{
	}

	void CommandBuffer::nextSubpass( renderer::SubpassContents contents )const
	{
	}

	void CommandBuffer::endRenderPass()const
	{
	}

	void CommandBuffer::executeCommands( renderer::CommandBufferCRefArray const & commands )const
	{
	}

	void CommandBuffer::clear( renderer::TextureView const & image
		, renderer::ClearColorValue const & colour )const
	{
	}

	void CommandBuffer::clear( renderer::TextureView const & image
		, renderer::DepthStencilClearValue const & value )const
	{
	}

	void CommandBuffer::clearAttachments( renderer::ClearAttachmentArray const & clearAttachments
		, renderer::ClearRectArray const & clearRects )
	{
	}

	void CommandBuffer::bindPipeline( renderer::Pipeline const & pipeline
		, renderer::PipelineBindPoint bindingPoint )const
	{
		m_currentPipeline = &static_cast< Pipeline const & >( pipeline );
	}

	void CommandBuffer::bindPipeline( renderer::ComputePipeline const & pipeline
		, renderer::PipelineBindPoint bindingPoint )const
	{
		m_currentComputePipeline = &static_cast< ComputePipeline const & >( pipeline );
	}

	void CommandBuffer::bindVertexBuffers( uint32_t firstBinding
		, renderer::BufferCRefArray const & buffers
		, renderer::UInt64Array offsets )const
	{
	}

	void CommandBuffer::bindIndexBuffer( renderer::BufferBase const & buffer
		, uint64_t offset
		, renderer::IndexType indexType )const
	{
	}

	void CommandBuffer::bindDescriptorSets( renderer::DescriptorSetCRefArray const & descriptorSets
		, renderer::PipelineLayout const & layout
		, renderer::UInt32Array const & dynamicOffsets
		, renderer::PipelineBindPoint bindingPoint )const
	{
	}

	void CommandBuffer::setViewport( renderer::Viewport const & viewport )const
	{
	}

	void CommandBuffer::setScissor( renderer::Scissor const & scissor )const
	{
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
	}

	void CommandBuffer::drawIndexed( uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
	}

	void CommandBuffer::drawIndirect( renderer::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
	}

	void CommandBuffer::drawIndexedIndirect( renderer::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
	}

	void CommandBuffer::copyToImage( renderer::BufferImageCopyArray const & copyInfo
		, renderer::BufferBase const & src
		, renderer::Texture const & dst )const
	{
	}

	void CommandBuffer::copyToBuffer( renderer::BufferImageCopyArray const & copyInfo
		, renderer::Texture const & src
		, renderer::BufferBase const & dst )const
	{
	}

	void CommandBuffer::copyBuffer( renderer::BufferCopy const & copyInfo
		, renderer::BufferBase const & src
		, renderer::BufferBase const & dst )const
	{
	}

	void CommandBuffer::copyImage( renderer::ImageCopy const & copyInfo
		, renderer::Texture const & src
		, renderer::ImageLayout srcLayout
		, renderer::Texture const & dst
		, renderer::ImageLayout dstLayout )const
	{
	}

	void CommandBuffer::blitImage( renderer::Texture const & srcImage
		, renderer::ImageLayout srcLayout
		, renderer::Texture const & dstImage
		, renderer::ImageLayout dstLayout
		, std::vector< renderer::ImageBlit > const & regions
		, renderer::Filter filter )const
	{
	}

	void CommandBuffer::resetQueryPool( renderer::QueryPool const & pool
		, uint32_t firstQuery
		, uint32_t queryCount )const
	{
	}

	void CommandBuffer::beginQuery( renderer::QueryPool const & pool
		, uint32_t query
		, renderer::QueryControlFlags flags )const
	{
	}

	void CommandBuffer::endQuery( renderer::QueryPool const & pool
		, uint32_t query )const
	{
	}

	void CommandBuffer::writeTimestamp( renderer::PipelineStageFlag pipelineStage
		, renderer::QueryPool const & pool
		, uint32_t query )const
	{
	}

	void CommandBuffer::pushConstants( renderer::PipelineLayout const & layout
		, renderer::PushConstantsBufferBase const & pcb )const
	{
	}

	void CommandBuffer::dispatch( uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ )const
	{
		assert( m_currentComputePipeline && "No compute pipeline bound." );
	}

	void CommandBuffer::dispatchIndirect( renderer::BufferBase const & buffer
		, uint32_t offset )const
	{
		assert( m_currentComputePipeline && "No pipeline bound." );
	}

	void CommandBuffer::setLineWidth( float width )const
	{
	}

	void CommandBuffer::setDepthBias( float constantFactor
		, float clamp
		, float slopeFactor )const
	{
	}

	void CommandBuffer::doMemoryBarrier( renderer::PipelineStageFlags after
		, renderer::PipelineStageFlags before
		, renderer::BufferMemoryBarrier const & transitionBarrier )const
	{
	}

	void CommandBuffer::doMemoryBarrier( renderer::PipelineStageFlags after
		, renderer::PipelineStageFlags before
		, renderer::ImageMemoryBarrier const & transitionBarrier )const
	{
	}
}
