/*
This file belongs to Ashes.
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

#include <Ashes/Buffer/PushConstantsBuffer.hpp>
#include <Ashes/Buffer/StagingBuffer.hpp>
#include <Ashes/Buffer/VertexBuffer.hpp>
#include <Ashes/RenderPass/FrameBufferAttachment.hpp>

namespace test_renderer
{
	namespace
	{
		CommandBufferCRefArray convert( ashes::CommandBufferCRefArray const & commands )
		{
			CommandBufferCRefArray result;

			for ( auto & command : commands )
			{
				result.emplace_back( static_cast< CommandBuffer const & >( command.get() ) );
			}

			return result;
		}

		DescriptorSetCRefArray convert( ashes::DescriptorSetCRefArray const & descriptors )
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
		: ashes::CommandBuffer{ device, pool, primary }
		, m_device{ device }
		, m_pool{ pool }
	{
	}

	CommandBuffer::~CommandBuffer()
	{
	}

	void CommandBuffer::begin( ashes::CommandBufferBeginInfo const & info )const
	{
		m_currentPipeline = nullptr;
		m_currentComputePipeline = nullptr;
	}

	void CommandBuffer::end()const
	{
		m_currentPipeline = nullptr;
		m_currentComputePipeline = nullptr;
	}

	void CommandBuffer::reset( ashes::CommandBufferResetFlags flags )const
	{
	}

	void CommandBuffer::beginRenderPass( ashes::RenderPassBeginInfo const & beginInfo
		, ashes::SubpassContents contents )const
	{
	}

	void CommandBuffer::nextSubpass( ashes::SubpassContents contents )const
	{
	}

	void CommandBuffer::endRenderPass()const
	{
	}

	void CommandBuffer::executeCommands( ashes::CommandBufferCRefArray const & commands )const
	{
	}

	void CommandBuffer::clear( ashes::TextureView const & image
		, ashes::ClearColorValue const & colour )const
	{
	}

	void CommandBuffer::clear( ashes::TextureView const & image
		, ashes::DepthStencilClearValue const & value )const
	{
	}

	void CommandBuffer::clearAttachments( ashes::ClearAttachmentArray const & clearAttachments
		, ashes::ClearRectArray const & clearRects )
	{
	}

	void CommandBuffer::bindPipeline( ashes::Pipeline const & pipeline
		, ashes::PipelineBindPoint bindingPoint )const
	{
		m_currentPipeline = &static_cast< Pipeline const & >( pipeline );
	}

	void CommandBuffer::bindPipeline( ashes::ComputePipeline const & pipeline
		, ashes::PipelineBindPoint bindingPoint )const
	{
		m_currentComputePipeline = &static_cast< ComputePipeline const & >( pipeline );
	}

	void CommandBuffer::bindVertexBuffers( uint32_t firstBinding
		, ashes::BufferCRefArray const & buffers
		, ashes::UInt64Array offsets )const
	{
	}

	void CommandBuffer::bindIndexBuffer( ashes::BufferBase const & buffer
		, uint64_t offset
		, ashes::IndexType indexType )const
	{
	}

	void CommandBuffer::bindDescriptorSets( ashes::DescriptorSetCRefArray const & descriptorSets
		, ashes::PipelineLayout const & layout
		, ashes::UInt32Array const & dynamicOffsets
		, ashes::PipelineBindPoint bindingPoint )const
	{
	}

	void CommandBuffer::setViewport( uint32_t firstViewport
		, ashes::ViewportArray const & viewports )const
	{
	}

	void CommandBuffer::setScissor( uint32_t firstScissor
		, ashes::ScissorArray const & scissors )const
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

	void CommandBuffer::drawIndirect( ashes::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
	}

	void CommandBuffer::drawIndexedIndirect( ashes::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
	}

	void CommandBuffer::copyToImage( ashes::BufferImageCopyArray const & copyInfo
		, ashes::BufferBase const & src
		, ashes::Texture const & dst )const
	{
	}

	void CommandBuffer::copyToBuffer( ashes::BufferImageCopyArray const & copyInfo
		, ashes::Texture const & src
		, ashes::BufferBase const & dst )const
	{
	}

	void CommandBuffer::copyBuffer( ashes::BufferCopy const & copyInfo
		, ashes::BufferBase const & src
		, ashes::BufferBase const & dst )const
	{
	}

	void CommandBuffer::copyImage( ashes::ImageCopy const & copyInfo
		, ashes::Texture const & src
		, ashes::ImageLayout srcLayout
		, ashes::Texture const & dst
		, ashes::ImageLayout dstLayout )const
	{
	}

	void CommandBuffer::blitImage( ashes::Texture const & srcImage
		, ashes::ImageLayout srcLayout
		, ashes::Texture const & dstImage
		, ashes::ImageLayout dstLayout
		, std::vector< ashes::ImageBlit > const & regions
		, ashes::Filter filter )const
	{
	}

	void CommandBuffer::resetQueryPool( ashes::QueryPool const & pool
		, uint32_t firstQuery
		, uint32_t queryCount )const
	{
	}

	void CommandBuffer::beginQuery( ashes::QueryPool const & pool
		, uint32_t query
		, ashes::QueryControlFlags flags )const
	{
	}

	void CommandBuffer::endQuery( ashes::QueryPool const & pool
		, uint32_t query )const
	{
	}

	void CommandBuffer::writeTimestamp( ashes::PipelineStageFlag pipelineStage
		, ashes::QueryPool const & pool
		, uint32_t query )const
	{
	}

	void CommandBuffer::pushConstants( ashes::PipelineLayout const & layout
		, ashes::ShaderStageFlags stageFlags
		, uint32_t offset
		, uint32_t size
		, void const * data )const
	{
	}

	void CommandBuffer::dispatch( uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ )const
	{
		assert( m_currentComputePipeline && "No compute pipeline bound." );
	}

	void CommandBuffer::dispatchIndirect( ashes::BufferBase const & buffer
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

	void CommandBuffer::setEvent( ashes::Event const & event
		, ashes::PipelineStageFlags stageMask )const
	{
	}

	void CommandBuffer::resetEvent( ashes::Event const & event
		, ashes::PipelineStageFlags stageMask )const
	{
	}

	void CommandBuffer::waitEvents( ashes::EventCRefArray const & events
		, ashes::PipelineStageFlags srcStageMask
		, ashes::PipelineStageFlags dstStageMask
		, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
		, ashes::ImageMemoryBarrierArray const & imageMemoryBarriers )const
	{
	}

	void CommandBuffer::pipelineBarrier( ashes::PipelineStageFlags after
		, ashes::PipelineStageFlags before
		, ashes::DependencyFlags dependencyFlags
		, ashes::MemoryBarrierArray const & memoryBarriers
		, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
		, ashes::ImageMemoryBarrierArray const & imageMemoryBarriers )const
	{
	}
}
