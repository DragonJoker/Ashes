/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/VkCommandBuffer.hpp"

#include "Buffer/VkBuffer.hpp"
#include "Command/VkCommandPool.hpp"
#include "Core/VkDevice.hpp"
#include "Descriptor/VkDescriptorSet.hpp"
#include "Image/VkImageSubresourceRange.hpp"
#include "Image/VkImage.hpp"
#include "Image/VkImageView.hpp"
#include "Miscellaneous/VkBufferCopy.hpp"
#include "Miscellaneous/VkBufferImageCopy.hpp"
#include "Miscellaneous/VkImageCopy.hpp"
#include "Miscellaneous/VkQueryPool.hpp"
#include "Pipeline/VkComputePipeline.hpp"
#include "Pipeline/VkPipeline.hpp"
#include "Pipeline/VkPipelineLayout.hpp"
#include "Pipeline/VkScissor.hpp"
#include "Pipeline/VkViewport.hpp"
#include "RenderPass/VkClearValue.hpp"
#include "RenderPass/VkFrameBuffer.hpp"
#include "RenderPass/VkRenderPass.hpp"
#include "Sync/VkBufferMemoryBarrier.hpp"
#include "Sync/VkEvent.hpp"
#include "Sync/VkImageMemoryBarrier.hpp"

#include <Ashes/Buffer/PushConstantsBuffer.hpp>
#include <Ashes/Buffer/StagingBuffer.hpp>
#include <Ashes/Buffer/UniformBuffer.hpp>
#include <Ashes/Buffer/VertexBuffer.hpp>
#include <Ashes/RenderPass/FrameBufferAttachment.hpp>

namespace vk_renderer
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

		std::vector< VkBufferImageCopy > convert( ashes::BufferImageCopyArray const & copies )
		{
			std::vector< VkBufferImageCopy > result;

			for ( auto & copy : copies )
			{
				result.emplace_back( vk_renderer::convert( copy ) );
			}

			return result;
		}

		std::vector< VkEvent > convert( ashes::EventCRefArray const & events )
		{
			std::vector< VkEvent > result;

			for ( auto & event : events )
			{
				result.emplace_back( static_cast< Event const & >( event.get() ) );
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
		VkCommandBufferAllocateInfo cmdAllocInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			nullptr,
			m_pool,                                   // commandPool
			primary                                   // level
				? VK_COMMAND_BUFFER_LEVEL_PRIMARY
				: VK_COMMAND_BUFFER_LEVEL_SECONDARY,
			1                                         // commandBufferCount
		};
		DEBUG_DUMP( cmdAllocInfo );
		auto res = m_device.vkAllocateCommandBuffers( m_device, &cmdAllocInfo, &m_commandBuffer );
		checkError( res, "CommandBuffer creation" );
	}

	CommandBuffer::~CommandBuffer()
	{
		m_device.vkFreeCommandBuffers( m_device, m_pool, 1, &m_commandBuffer );
	}

	void CommandBuffer::begin( ashes::CommandBufferBeginInfo const & info )const
	{
		VkCommandBufferInheritanceInfo * inheritanceInfo = nullptr;

		if ( bool( info.inheritanceInfo ) )
		{
			m_inheritanceInfo = convert( info.inheritanceInfo.value() );
			inheritanceInfo = &m_inheritanceInfo;
		}
		VkCommandBufferBeginInfo cmdBufInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			convert( info.flags ),                       // flags
			inheritanceInfo                              // pInheritanceInfo
		};
		DEBUG_DUMP( cmdBufInfo );
		auto res = m_device.vkBeginCommandBuffer( m_commandBuffer, &cmdBufInfo );
		m_currentPipeline = nullptr;
		m_currentComputePipeline = nullptr;
		checkError( res, "CommandBuffer record start" );
	}

	void CommandBuffer::end()const
	{
		auto res = m_device.vkEndCommandBuffer( m_commandBuffer );
		m_currentPipeline = nullptr;
		m_currentComputePipeline = nullptr;
		checkError( res, "CommandBuffer record end" );
	}

	void CommandBuffer::reset( ashes::CommandBufferResetFlags flags )const
	{
		auto res = m_device.vkResetCommandBuffer( m_commandBuffer, convert( flags ) );
		checkError( res, "CommandBuffer reset" );
	}

	void CommandBuffer::beginRenderPass( ashes::RenderPassBeginInfo const & beginInfo
		, ashes::SubpassContents contents )const
	{
		auto & renderPass = static_cast< RenderPass const & >( *beginInfo.renderPass );
		auto & framebuffer = static_cast< FrameBuffer const & >( *beginInfo.framebuffer );
		auto & vkclearValues = renderPass.getClearValues( beginInfo.clearValues );
		//auto vkclearValues = convert< VkClearValue >( clearValues );
		VkRenderPassBeginInfo vkbeginInfo
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			nullptr,
			renderPass,                                         // renderPass
			framebuffer,                                        // framebuffer
			convert( beginInfo.renderArea ),                    // renderArea
			uint32_t( vkclearValues.size() ),                   // clearValueCount
			vkclearValues.data()                                // pClearValues
		};
		DEBUG_DUMP( vkbeginInfo );
		m_device.vkCmdBeginRenderPass( m_commandBuffer
			, &vkbeginInfo
			, convert( contents ) );
	}

	void CommandBuffer::nextSubpass( ashes::SubpassContents contents )const
	{
		m_device.vkCmdNextSubpass( m_commandBuffer, convert( contents ) );
	}

	void CommandBuffer::endRenderPass()const
	{
		m_device.vkCmdEndRenderPass( m_commandBuffer );
	}

	void CommandBuffer::executeCommands( ashes::CommandBufferCRefArray const & commands )const
	{
		auto vkCommands = makeVkArray< VkCommandBuffer >( convert( commands ) );
		m_device.vkCmdExecuteCommands( m_commandBuffer
			, uint32_t( vkCommands.size() )
			, vkCommands.data() );
	}

	void CommandBuffer::clear( ashes::ImageView const & image
		, ashes::ClearColorValue const & colour )const
	{
		auto vkcolour = convert( colour );
		auto vksubresourceRange = convert( image.getSubResourceRange() );
		m_device.vkCmdClearColorImage( m_commandBuffer
			, static_cast< Image const & >( image.getImage() )
			, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			, &vkcolour
			, 1
			, &vksubresourceRange );
	}

	void CommandBuffer::clear( ashes::ImageView const & image
		, ashes::DepthStencilClearValue const & value )const
	{
		auto vkclear = convert( value );
		auto vksubresourceRange = convert( image.getSubResourceRange() );
		m_device.vkCmdClearDepthStencilImage( m_commandBuffer
			, static_cast< Image const & >( image.getImage() )
			, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			, &vkclear
			, 1
			, &vksubresourceRange );
	}

	void CommandBuffer::clearAttachments( ashes::ClearAttachmentArray const & clearAttachments
		, ashes::ClearRectArray const & clearRects )
	{
		auto vkAttaches = convert< VkClearAttachment >( clearAttachments );
		auto vkRects = convert< VkClearRect >( clearRects );
		m_device.vkCmdClearAttachments( m_commandBuffer
			, uint32_t( vkAttaches.size() )
			, vkAttaches.data()
			, uint32_t( vkRects.size() )
			, vkRects.data() );
	}

	void CommandBuffer::bindPipeline( ashes::Pipeline const & pipeline
		, ashes::PipelineBindPoint bindingPoint )const
	{
		m_device.vkCmdBindPipeline( m_commandBuffer
			, convert( bindingPoint )
			, static_cast< Pipeline const & >( pipeline ) );
		m_currentPipeline = &static_cast< Pipeline const & >( pipeline );
	}

	void CommandBuffer::bindPipeline( ashes::ComputePipeline const & pipeline
		, ashes::PipelineBindPoint bindingPoint )const
	{
		m_device.vkCmdBindPipeline( m_commandBuffer
			, convert( bindingPoint )
			, static_cast< ComputePipeline const & >( pipeline ) );
		m_currentComputePipeline = &static_cast< ComputePipeline const & >( pipeline );
	}

	void CommandBuffer::bindVertexBuffers( uint32_t firstBinding
		, ashes::BufferCRefArray const & buffers
		, ashes::UInt64Array offsets )const
	{
		std::vector< std::reference_wrapper< Buffer const > > vkbuffers;

		for ( auto & buffer : buffers )
		{
			vkbuffers.emplace_back( static_cast< Buffer const & >( buffer.get() ) );
		}

		m_device.vkCmdBindVertexBuffers( m_commandBuffer
			, firstBinding
			, uint32_t( vkbuffers.size() )
			, makeVkArray< VkBuffer >( vkbuffers ).data()
			, offsets.data() );
	}

	void CommandBuffer::bindIndexBuffer( ashes::BufferBase const & buffer
		, uint64_t offset
		, ashes::IndexType indexType )const
	{
		m_device.vkCmdBindIndexBuffer( m_commandBuffer
			, static_cast< Buffer const & >( buffer )
			, offset
			, convert( indexType ) );
	}

	void CommandBuffer::bindDescriptorSets( ashes::DescriptorSetCRefArray const & descriptorSets
		, ashes::PipelineLayout const & layout
		, ashes::UInt32Array const & dynamicOffsets
		, ashes::PipelineBindPoint bindingPoint )const
	{
		auto vkDescriptors = makeVkArray< VkDescriptorSet >( convert( descriptorSets ) );
		m_device.vkCmdBindDescriptorSets( m_commandBuffer
			, convert( bindingPoint )
			, static_cast< PipelineLayout const & >( layout )
			, descriptorSets.begin()->get().getBindingPoint()
			, uint32_t( descriptorSets.size() )
			, vkDescriptors.data()
			, uint32_t( dynamicOffsets.size() )
			, dynamicOffsets.data() );
	}

	void CommandBuffer::setViewport( uint32_t firstViewport
		, ashes::ViewportArray const & viewports )const
	{
		auto vkviewport = convert< VkViewport >( viewports );
		m_device.vkCmdSetViewport( m_commandBuffer
			, firstViewport
			, uint32_t( vkviewport.size() )
			, vkviewport.data() );
	}

	void CommandBuffer::setScissor( uint32_t firstScissor
		, ashes::ScissorArray const & scissors )const
	{
		auto vkscissor = convert< VkRect2D >( scissors );
		m_device.vkCmdSetScissor( m_commandBuffer
			, firstScissor
			, uint32_t( vkscissor.size() )
			, vkscissor.data() );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
		m_device.vkCmdDraw( m_commandBuffer
			, vtxCount
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
		assert( m_currentPipeline && "No pipeline bound." );
		m_device.vkCmdDrawIndexed( m_commandBuffer
			, indexCount
			, instCount
			, firstIndex
			, vertexOffset
			, firstInstance );
	}

	void CommandBuffer::drawIndirect( ashes::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
		m_device.vkCmdDrawIndirect( m_commandBuffer
			, static_cast< Buffer const & >( buffer )
			, offset
			, drawCount
			, stride );
	}

	void CommandBuffer::drawIndexedIndirect( ashes::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
		m_device.vkCmdDrawIndexedIndirect( m_commandBuffer
			, static_cast< Buffer const & >( buffer )
			, offset
			, drawCount
			, stride );
	}

	void CommandBuffer::copyToImage( ashes::BufferImageCopyArray const & copyInfo
		, ashes::BufferBase const & src
		, ashes::Image const & dst )const
	{
		auto vkcopyInfo = convert( copyInfo );
		DEBUG_DUMP( vkcopyInfo );
		m_device.vkCmdCopyBufferToImage( m_commandBuffer
			, static_cast< Buffer const & >( src )
			, static_cast< Image const & >( dst )
			, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			, uint32_t( vkcopyInfo.size() )
			, vkcopyInfo.data() );
	}

	void CommandBuffer::copyToBuffer( ashes::BufferImageCopyArray const & copyInfo
		, ashes::Image const & src
		, ashes::BufferBase const & dst )const
	{
		auto vkcopyInfo = convert( copyInfo );
		DEBUG_DUMP( vkcopyInfo );
		m_device.vkCmdCopyImageToBuffer( m_commandBuffer
			, static_cast< Image const & >( src )
			, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
			, static_cast< Buffer const & >( dst )
			, uint32_t( vkcopyInfo.size() )
			, vkcopyInfo.data() );
	}

	void CommandBuffer::copyBuffer( ashes::BufferCopy const & copyInfo
		, ashes::BufferBase const & src
		, ashes::BufferBase const & dst )const
	{
		auto vkcopyInfo = convert( copyInfo );
		DEBUG_DUMP( vkcopyInfo );
		m_device.vkCmdCopyBuffer( m_commandBuffer
			, static_cast< Buffer const & >( src )
			, static_cast< Buffer const & >( dst )
			, 1
			, &vkcopyInfo );
	}

	void CommandBuffer::copyImage( ashes::ImageCopy const & copyInfo
		, ashes::Image const & src
		, ashes::ImageLayout srcLayout
		, ashes::Image const & dst
		, ashes::ImageLayout dstLayout )const
	{
		auto vkcopyInfo = convert( copyInfo );
		DEBUG_DUMP( vkcopyInfo );
		m_device.vkCmdCopyImage( m_commandBuffer
			, static_cast< Image const & >( src )
			, convert( srcLayout )
			, static_cast< Image const & >( dst )
			, convert( dstLayout )
			, 1
			, &vkcopyInfo );
	}

	void CommandBuffer::blitImage( ashes::Image const & srcImage
		, ashes::ImageLayout srcLayout
		, ashes::Image const & dstImage
		, ashes::ImageLayout dstLayout
		, std::vector< ashes::ImageBlit > const & regions
		, ashes::Filter filter )const
	{
		auto vkregions = convert< VkImageBlit >( regions );
		DEBUG_DUMP( vkregions );
		m_device.vkCmdBlitImage( m_commandBuffer
			, static_cast< Image const & >( srcImage )
			, convert( srcLayout )
			, static_cast< Image const & >( dstImage )
			, convert( dstLayout )
			, uint32_t( vkregions.size() )
			, vkregions.data()
			, convert( filter ) );
	}

	void CommandBuffer::resetQueryPool( ashes::QueryPool const & pool
		, uint32_t firstQuery
		, uint32_t queryCount )const
	{
		m_device.vkCmdResetQueryPool( m_commandBuffer
			, static_cast< QueryPool const & >( pool )
			, firstQuery
			, queryCount );
	}

	void CommandBuffer::beginQuery( ashes::QueryPool const & pool
		, uint32_t query
		, ashes::QueryControlFlags flags )const
	{
		m_device.vkCmdBeginQuery( m_commandBuffer
			, static_cast< QueryPool const & >( pool )
			, query
			, convert( flags ) );
	}

	void CommandBuffer::endQuery( ashes::QueryPool const & pool
		, uint32_t query )const
	{
		m_device.vkCmdEndQuery( m_commandBuffer
			, static_cast< QueryPool const & >( pool )
			, query );
	}

	void CommandBuffer::writeTimestamp( ashes::PipelineStageFlag pipelineStage
		, ashes::QueryPool const & pool
		, uint32_t query )const
	{
		m_device.vkCmdWriteTimestamp( m_commandBuffer
			, convert( pipelineStage )
			, static_cast< QueryPool const & >( pool )
			, query );
	}

	void CommandBuffer::pushConstants( ashes::PipelineLayout const & layout
		, ashes::ShaderStageFlags stageFlags
		, uint32_t offset
		, uint32_t size
		, void const * data )const
	{
		m_device.vkCmdPushConstants( m_commandBuffer
			, static_cast< PipelineLayout const & >( layout )
			, convert( stageFlags )
			, offset
			, size
			, data );
	}

	void CommandBuffer::dispatch( uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ )const
	{
		assert( m_currentComputePipeline && "No compute pipeline bound." );
		m_device.vkCmdDispatch( m_commandBuffer
			, groupCountX
			, groupCountY
			, groupCountZ );
	}

	void CommandBuffer::dispatchIndirect( ashes::BufferBase const & buffer
		, uint32_t offset )const
	{
		assert( m_currentComputePipeline && "No pipeline bound." );
		m_device.vkCmdDispatchIndirect( m_commandBuffer
			, static_cast< Buffer const & >( buffer )
			, offset );
	}

	void CommandBuffer::setLineWidth( float width )const
	{
		m_device.vkCmdSetLineWidth( m_commandBuffer, width );
	}

	void CommandBuffer::setDepthBias( float constantFactor
		, float clamp
		, float slopeFactor )const
	{
		m_device.vkCmdSetDepthBias( m_commandBuffer
			, constantFactor
			, clamp
			, slopeFactor );
	}

	void CommandBuffer::setEvent( ashes::Event const & event
		, ashes::PipelineStageFlags stageMask )const
	{
		m_device.vkCmdSetEvent( m_commandBuffer
			, static_cast< Event const & >( event )
			, convert( stageMask ) );
	}

	void CommandBuffer::resetEvent( ashes::Event const & event
		, ashes::PipelineStageFlags stageMask )const
	{
		m_device.vkCmdResetEvent( m_commandBuffer
			, static_cast< Event const & >( event )
			, convert( stageMask ) );
	}

	void CommandBuffer::waitEvents( ashes::EventCRefArray const & events
		, ashes::PipelineStageFlags srcStageMask
		, ashes::PipelineStageFlags dstStageMask
		, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
		, ashes::ImageMemoryBarrierArray const & imageMemoryBarriers )const
	{
		auto vkevents = convert( events );
		auto vkbufbarriers = convert< VkBufferMemoryBarrier >( bufferMemoryBarriers );
		auto vkimgbarriers = convert< VkImageMemoryBarrier >( imageMemoryBarriers );
		m_device.vkCmdWaitEvents( m_commandBuffer
			, uint32_t( vkevents.size() )
			, vkevents.data()
			, convert( srcStageMask )
			, convert( dstStageMask )
			, 0u
			, nullptr
			, uint32_t( vkbufbarriers.size() )
			, vkbufbarriers.data()
			, uint32_t( vkimgbarriers.size() )
			, vkimgbarriers.data() );
	}

	void CommandBuffer::pipelineBarrier( ashes::PipelineStageFlags after
		, ashes::PipelineStageFlags before
		, ashes::DependencyFlags dependencyFlags
		, ashes::MemoryBarrierArray const & memoryBarriers
		, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
		, ashes::ImageMemoryBarrierArray const & imageMemoryBarriers )const
	{
		auto vkafter = convert( after );
		auto vkbefore = convert( before );
		auto vkmb = convert< VkMemoryBarrier >( memoryBarriers );
		auto vkbb = convert< VkBufferMemoryBarrier >( bufferMemoryBarriers );
		auto vkib = convert< VkImageMemoryBarrier >( imageMemoryBarriers );
		DEBUG_DUMP( vkmb );
		DEBUG_DUMP( vkbb );
		DEBUG_DUMP( vkib );
		m_device.vkCmdPipelineBarrier( m_commandBuffer
			, vkafter
			, vkbefore
			, 0
			, uint32_t( vkmb.size() )
			, vkmb.empty() ? nullptr : vkmb.data()
			, uint32_t( vkbb.size() )
			, vkbb.empty() ? nullptr : vkbb.data()
			, uint32_t( vkib.size() )
			, vkib.empty() ? nullptr : vkib.data() );
	}
}
