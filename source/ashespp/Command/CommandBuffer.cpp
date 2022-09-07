/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Command/CommandBuffer.hpp"

#include "ashespp/AccelerationStructure/AccelerationStructure.hpp"
#include "ashespp/Buffer/VertexBuffer.hpp"
#include "ashespp/Buffer/UniformBuffer.hpp"
#include "ashespp/Descriptor/DescriptorSet.hpp"
#include "ashespp/Image/Image.hpp"
#include "ashespp/Image/ImageView.hpp"
#include "ashespp/Miscellaneous/QueryPool.hpp"
#include "ashespp/Pipeline/ComputePipeline.hpp"
#include "ashespp/Pipeline/GraphicsPipeline.hpp"
#include "ashespp/Pipeline/PipelineLayout.hpp"
#include "ashespp/RenderPass/FrameBuffer.hpp"
#include "ashespp/RenderPass/RenderPass.hpp"
#include "ashespp/Sync/Event.hpp"

#include <ashes/common/Format.hpp>

namespace ashes
{
	CommandBuffer::CommandBuffer( Device const & device
		, CommandPool const & pool
		, VkCommandBufferLevel level )
		: CommandBuffer{ device, "CommandBuffer", pool, level }
	{
	}

	CommandBuffer::CommandBuffer( Device const & device
		, std::string const & debugName
		, CommandPool const & pool
		, VkCommandBufferLevel level )
		: VkObject{ debugName }
		, m_device{ device }
		, m_pool{ pool }
	{
		uint32_t const commandBufferCount = 1u;
		VkCommandBufferAllocateInfo cmdAllocInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			nullptr,
			m_pool,
			level,
			commandBufferCount,
		};
		DEBUG_DUMP( cmdAllocInfo );
		auto res = m_device.vkAllocateCommandBuffers( m_device
			, &cmdAllocInfo
			, &m_internal );
		checkError( res, "CommandBuffer creation" );
		registerObject( m_device, debugName, *this );
	}

	CommandBuffer::~CommandBuffer()
	{
		unregisterObject( m_device, *this );
		m_device.vkFreeCommandBuffers( m_device, m_pool, 1, &m_internal );
	}

	void CommandBuffer::begin( VkCommandBufferBeginInfo const & info )const
	{
		DEBUG_DUMP( info );
		auto res = m_device.vkBeginCommandBuffer( m_internal, &info );
		checkError( res, "CommandBuffer record start" );
	}

	void CommandBuffer::end()const
	{
		auto res = m_device.vkEndCommandBuffer( m_internal );
		checkError( res, "CommandBuffer record end" );
	}

	void CommandBuffer::reset( VkCommandBufferResetFlags flags )const
	{
		auto res = m_device.vkResetCommandBuffer( m_internal, flags );
		checkError( res, "CommandBuffer reset" );
	}

	void CommandBuffer::beginRenderPass( VkRenderPassBeginInfo const & beginInfo
		, VkSubpassContents contents )const
	{
		DEBUG_DUMP( beginInfo );
		m_device.vkCmdBeginRenderPass( m_internal
			, &beginInfo
			, contents );
	}

	void CommandBuffer::nextSubpass( VkSubpassContents contents )const
	{
		m_device.vkCmdNextSubpass( m_internal, contents );
	}

	void CommandBuffer::endRenderPass()const
	{
		m_device.vkCmdEndRenderPass( m_internal );
	}

	void CommandBuffer::executeCommands( CommandBufferCRefArray const & commands )const
	{
		auto vkCommands = makeVkArray< VkCommandBuffer >( commands );
		m_device.vkCmdExecuteCommands( m_internal
			, uint32_t( vkCommands.size() )
			, vkCommands.data() );
	}

	void CommandBuffer::clear( ImageView const & view
		, VkClearColorValue const & colour )const
	{
		m_device.vkCmdClearColorImage( m_internal
			, *view.image
			, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			, &colour
			, 1
			, &view->subresourceRange );
	}

	void CommandBuffer::clear( ImageView const & view
		, VkClearDepthStencilValue const & value )const
	{
		m_device.vkCmdClearDepthStencilImage( m_internal
			, *view.image
			, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			, &value
			, 1
			, &view->subresourceRange );
	}

	void CommandBuffer::clearAttachments( VkClearAttachmentArray const & clearAttachments
		, VkClearRectArray const & clearRects )
	{
		m_device.vkCmdClearAttachments( m_internal
			, uint32_t( clearAttachments.size() )
			, clearAttachments.data()
			, uint32_t( clearRects.size() )
			, clearRects.data() );
	}

	void CommandBuffer::bindPipeline( Pipeline const & pipeline
		, VkPipelineBindPoint bindingPoint )const
	{
		m_device.vkCmdBindPipeline( m_internal
			, bindingPoint
			, pipeline );
	}

	void CommandBuffer::bindPipeline( GraphicsPipeline const & pipeline )const
	{
		bindPipeline( pipeline
			, VK_PIPELINE_BIND_POINT_GRAPHICS );
	}

	void CommandBuffer::bindPipeline( ComputePipeline const & pipeline )const
	{
		bindPipeline( pipeline
			, VK_PIPELINE_BIND_POINT_COMPUTE );
	}

	void CommandBuffer::bindVertexBuffers( uint32_t firstBinding
		, BufferCRefArray const & buffers
		, UInt64Array offsets )const
	{
		m_device.vkCmdBindVertexBuffers( m_internal
			, firstBinding
			, uint32_t( buffers.size() )
			, makeVkArray< VkBuffer >( buffers ).data()
			, offsets.data() );
	}

	void CommandBuffer::bindIndexBuffer( BufferBase const & buffer
		, uint64_t offset
		, VkIndexType indexType )const
	{
		m_device.vkCmdBindIndexBuffer( m_internal
			, buffer
			, offset
			, indexType );
	}

	void CommandBuffer::bindDescriptorSets( DescriptorSetCRefArray const & descriptorSets
		, PipelineLayout const & layout
		, UInt32Array const & dynamicOffsets
		, VkPipelineBindPoint bindingPoint )const
	{
		auto vkDescriptors = makeVkArray< VkDescriptorSet >( descriptorSets );
		m_device.vkCmdBindDescriptorSets( m_internal
			, bindingPoint
			, layout
			, descriptorSets.begin()->get().getBindingPoint()
			, uint32_t( descriptorSets.size() )
			, vkDescriptors.data()
			, uint32_t( dynamicOffsets.size() )
			, dynamicOffsets.data() );
	}

	void CommandBuffer::setViewport( uint32_t firstViewport
		, VkViewportArray const & viewports )const
	{
		m_device.vkCmdSetViewport( m_internal
			, firstViewport
			, uint32_t( viewports.size() )
			, viewports.data() );
	}

	void CommandBuffer::setScissor( uint32_t firstScissor
		, VkScissorArray const & scissors )const
	{
		m_device.vkCmdSetScissor( m_internal
			, firstScissor
			, uint32_t( scissors.size() )
			, scissors.data() );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		m_device.vkCmdDraw( m_internal
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
		m_device.vkCmdDrawIndexed( m_internal
			, indexCount
			, instCount
			, firstIndex
			, int32_t( vertexOffset )
			, firstInstance );
	}

	void CommandBuffer::drawIndirect( BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		m_device.vkCmdDrawIndirect( m_internal
			, buffer
			, offset
			, drawCount
			, stride );
	}

	void CommandBuffer::drawIndexedIndirect( BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		m_device.vkCmdDrawIndexedIndirect( m_internal
			, buffer
			, offset
			, drawCount
			, stride );
	}

#if VK_EXT_mesh_shader

	void CommandBuffer::drawMeshTasks( uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ )const
	{
		m_device.vkCmdDrawMeshTasksEXT( m_internal
			, groupCountX
			, groupCountY
			, groupCountZ );
	}

	void CommandBuffer::drawMeshTasksIndirect( BufferBase const & buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		m_device.vkCmdDrawMeshTasksIndirectEXT( m_internal
			, buffer
			, offset
			, drawCount
			, stride );
	}

	void CommandBuffer::drawMeshTasksIndirectCount( BufferBase const & buffer
		, VkDeviceSize offset
		, BufferBase const & countBuffer
		, VkDeviceSize countBufferOffset
		, uint32_t maxDrawCount
		, uint32_t stride )const
	{
		m_device.vkCmdDrawMeshTasksIndirectCountEXT( m_internal
			, buffer
			, offset
			, countBuffer
			, countBufferOffset
			, maxDrawCount
			, stride );
	}

#endif
#if VK_NV_mesh_shader

	void CommandBuffer::drawMeshTasksNV( uint32_t taskCount
		, uint32_t firstTask )const
	{
		m_device.vkCmdDrawMeshTasksNV( m_internal
			, taskCount
			, firstTask );
	}

	void CommandBuffer::drawMeshTasksIndirectNV( BufferBase const & buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		m_device.vkCmdDrawMeshTasksIndirectNV( m_internal
			, buffer
			, offset
			, drawCount
			, stride );
	}

	void CommandBuffer::drawMeshTasksIndirectCountNV( BufferBase const & buffer
		, VkDeviceSize offset
		, BufferBase const & countBuffer
		, VkDeviceSize countBufferOffset
		, uint32_t maxDrawCount
		, uint32_t stride )const
	{
		m_device.vkCmdDrawMeshTasksIndirectCountNV( m_internal
			, buffer
			, offset
			, countBuffer
			, countBufferOffset
			, maxDrawCount
			, stride );
	}

#endif

	void CommandBuffer::copyToImage( VkBufferImageCopyArray const & copyInfo
		, VkBuffer src
		, VkImage dst )const
	{
		DEBUG_DUMP( copyInfo );
		m_device.vkCmdCopyBufferToImage( m_internal
			, src
			, dst
			, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			, uint32_t( copyInfo.size() )
			, copyInfo.data() );
	}

	void CommandBuffer::copyToImage( VkBufferImageCopyArray const & copyInfo
		, BufferBase const & src
		, Image const & dst )const
	{
		DEBUG_DUMP( copyInfo );
		m_device.vkCmdCopyBufferToImage( m_internal
			, src
			, dst
			, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			, uint32_t( copyInfo.size() )
			, copyInfo.data() );
	}

	void CommandBuffer::copyToBuffer( VkBufferImageCopyArray const & copyInfo
		, VkImage src
		, VkBuffer dst )const
	{
		DEBUG_DUMP( copyInfo );
		m_device.vkCmdCopyImageToBuffer( m_internal
			, src
			, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
			, dst
			, uint32_t( copyInfo.size() )
			, copyInfo.data() );
	}

	void CommandBuffer::copyToBuffer( VkBufferImageCopyArray const & copyInfo
		, Image const & src
		, BufferBase const & dst )const
	{
		DEBUG_DUMP( copyInfo );
		m_device.vkCmdCopyImageToBuffer( m_internal
			, src
			, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
			, dst
			, uint32_t( copyInfo.size() )
			, copyInfo.data() );
	}

	void CommandBuffer::copyBuffer( VkBufferCopy const & copyInfo
		, VkBuffer src
		, VkBuffer dst )const
	{
		DEBUG_DUMP( copyInfo );
		m_device.vkCmdCopyBuffer( m_internal
			, src
			, dst
			, 1
			, &copyInfo );
	}

	void CommandBuffer::copyBuffer( VkBufferCopy const & copyInfo
		, BufferBase const & src
		, BufferBase const & dst )const
	{
		DEBUG_DUMP( copyInfo );
		m_device.vkCmdCopyBuffer( m_internal
			, src
			, dst
			, 1
			, &copyInfo );
	}

	void CommandBuffer::copyBuffer( VkBufferCopyArray const & copyInfo
		, BufferBase const & src
		, BufferBase const & dst )const
	{
		DEBUG_DUMP( copyInfo );
		m_device.vkCmdCopyBuffer( m_internal
			, src
			, dst
			, uint32_t( copyInfo.size() )
			, copyInfo.data() );
	}

	void CommandBuffer::updateBuffer( BufferBase const & dst
		, VkDeviceSize dstOffset
		, ArrayView< uint8_t > const & data )const
	{
		m_device.vkCmdUpdateBuffer( m_internal
			, dst
			, dstOffset
			, data.size()
			, data.data() );
	}
	
	void CommandBuffer::copyImage( VkImageCopy const & copyInfo
		, Image const & src
		, VkImageLayout srcLayout
		, Image const & dst
		, VkImageLayout dstLayout )const
	{
		DEBUG_DUMP( copyInfo );
		m_device.vkCmdCopyImage( m_internal
			, src
			, srcLayout
			, dst
			, dstLayout
			, 1
			, &copyInfo );
	}

	void CommandBuffer::copyImage( VkImageCopyArray copyInfos
		, Image const & src
		, VkImageLayout srcLayout
		, Image const & dst
		, VkImageLayout dstLayout )const
	{
		DEBUG_DUMP( copyInfos );
		m_device.vkCmdCopyImage( m_internal
			, src
			, srcLayout
			, dst
			, dstLayout
			, uint32_t( copyInfos.size() )
			, copyInfos.data() );
	}

	void CommandBuffer::blitImage( Image const & srcImage
		, VkImageLayout srcLayout
		, Image const & dstImage
		, VkImageLayout dstLayout
		, std::vector< VkImageBlit > const & regions
		, VkFilter filter )const
	{
		DEBUG_DUMP( vkregions );
		m_device.vkCmdBlitImage( m_internal
			, srcImage
			, srcLayout
			, dstImage
			, dstLayout
			, uint32_t( regions.size() )
			, regions.data()
			, filter );
	}

	void CommandBuffer::resetQueryPool( QueryPool const & pool
		, uint32_t firstQuery
		, uint32_t queryCount )const
	{
		m_device.vkCmdResetQueryPool( m_internal
			, pool
			, firstQuery
			, queryCount );
	}

	void CommandBuffer::beginQuery( QueryPool const & pool
		, uint32_t query
		, VkQueryControlFlags flags )const
	{
		m_device.vkCmdBeginQuery( m_internal
			, pool
			, query
			, flags );
	}

	void CommandBuffer::endQuery( QueryPool const & pool
		, uint32_t query )const
	{
		m_device.vkCmdEndQuery( m_internal
			, pool
			, query );
	}

	void CommandBuffer::writeTimestamp( VkPipelineStageFlagBits pipelineStage
		, QueryPool const & pool
		, uint32_t query )const
	{
		m_device.vkCmdWriteTimestamp( m_internal
			, pipelineStage
			, pool
			, query );
	}

	void CommandBuffer::pushConstants( PipelineLayout const & layout
		, VkShaderStageFlags stageFlags
		, uint32_t offset
		, uint32_t size
		, void const * data )const
	{
		m_device.vkCmdPushConstants( m_internal
			, layout
			, stageFlags
			, offset
			, size
			, data );
	}

	void CommandBuffer::dispatch( uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ )const
	{
		m_device.vkCmdDispatch( m_internal
			, groupCountX
			, groupCountY
			, groupCountZ );
	}

	void CommandBuffer::dispatchIndirect( BufferBase const & buffer
		, uint32_t offset )const
	{
		m_device.vkCmdDispatchIndirect( m_internal
			, buffer
			, offset );
	}

	void CommandBuffer::setLineWidth( float width )const
	{
		m_device.vkCmdSetLineWidth( m_internal, width );
	}

	void CommandBuffer::setDepthBias( float constantFactor
		, float clamp
		, float slopeFactor )const
	{
		m_device.vkCmdSetDepthBias( m_internal
			, constantFactor
			, clamp
			, slopeFactor );
	}

	void CommandBuffer::setEvent( Event const & event
		, VkPipelineStageFlags stageMask )const
	{
		m_device.vkCmdSetEvent( m_internal
			, event
			, stageMask );
	}

	void CommandBuffer::resetEvent( Event const & event
		, VkPipelineStageFlags stageMask )const
	{
		m_device.vkCmdResetEvent( m_internal
			, event
			, stageMask );
	}

	void CommandBuffer::waitEvents( EventCRefArray const & events
		, VkPipelineStageFlags srcStageMask
		, VkPipelineStageFlags dstStageMask
		, VkMemoryBarrierArray const & memoryBarriers
		, VkBufferMemoryBarrierArray const & bufferMemoryBarriers
		, VkImageMemoryBarrierArray const & imageMemoryBarriers )const
	{
		auto vkevents = makeVkArray< VkEvent >( events );
		m_device.vkCmdWaitEvents( m_internal
			, uint32_t( vkevents.size() )
			, vkevents.data()
			, srcStageMask
			, dstStageMask
			, uint32_t( memoryBarriers.size() )
			, memoryBarriers.data()
			, uint32_t( bufferMemoryBarriers.size() )
			, bufferMemoryBarriers.data()
			, uint32_t( imageMemoryBarriers.size() )
			, imageMemoryBarriers.data() );
	}

	void CommandBuffer::pipelineBarrier( VkPipelineStageFlags after
		, VkPipelineStageFlags before
		, VkDependencyFlags dependencyFlags
		, VkMemoryBarrierArray const & memoryBarriers
		, VkBufferMemoryBarrierArray const & bufferMemoryBarriers
		, VkImageMemoryBarrierArray const & imageMemoryBarriers )const
	{
		DEBUG_DUMP( memoryBarriers );
		DEBUG_DUMP( bufferMemoryBarriers );
		DEBUG_DUMP( imageMemoryBarriers );
		m_device.vkCmdPipelineBarrier( m_internal
			, after
			, before
			, dependencyFlags
			, uint32_t( memoryBarriers.size() )
			, memoryBarriers.empty() ? nullptr : memoryBarriers.data()
			, uint32_t( bufferMemoryBarriers.size() )
			, bufferMemoryBarriers.empty() ? nullptr : bufferMemoryBarriers.data()
			, uint32_t( imageMemoryBarriers.size() )
			, imageMemoryBarriers.empty() ? nullptr : imageMemoryBarriers.data() );
	}

	void CommandBuffer::beginRenderPass( VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkExtent2D dimensions
		, VkClearValueArray const & clearValues
		, VkSubpassContents contents )const
	{
		beginRenderPass( { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO
				, nullptr
				, renderPass
				, frameBuffer
				, { { 0, 0 }
					, std::move( dimensions ) }
				, uint32_t( clearValues.size() )
				, clearValues.data() }
			, contents );
	}

	void CommandBuffer::beginRenderPass( RenderPass const & renderPass
		, FrameBuffer const & frameBuffer
		, VkClearValueArray const & clearValues
		, VkSubpassContents contents )const
	{
		beginRenderPass( { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO
				, nullptr
				, renderPass
				, frameBuffer
				, { { 0, 0 }
					, frameBuffer.getDimensions() }
				, uint32_t( clearValues.size() )
				, clearValues.data() }
			, contents );
	}

	void CommandBuffer::bindVertexBuffer( uint32_t binding
		, BufferBase const & buffer
		, uint64_t offset )const
	{
		bindVertexBuffers( binding
			, BufferCRefArray{ buffer }
			, UInt64Array{ offset } );
	}

	void CommandBuffer::copyToImage( VkBufferImageCopy const & copyInfo
		, VkBuffer src
		, VkImage dst )const
	{
		copyToImage( VkBufferImageCopyArray{ 1u, copyInfo }
			, src
			, dst );
	}

	void CommandBuffer::copyToImage( VkBufferImageCopy const & copyInfo
		, BufferBase const & src
		, Image const & dst )const
	{
		copyToImage( VkBufferImageCopyArray{ 1u, copyInfo }
			, src
			, dst );
	}

	void CommandBuffer::copyToBuffer( VkBufferImageCopy const & copyInfo
		, VkImage src
		, VkBuffer dst )const
	{
		copyToBuffer( VkBufferImageCopyArray{ 1u, copyInfo }
			, src
			, dst );
	}

	void CommandBuffer::copyToBuffer( VkBufferImageCopy const & copyInfo
		, Image const & src
		, BufferBase const & dst )const
	{
		copyToBuffer( VkBufferImageCopyArray{ 1u, copyInfo }
			, src
			, dst );
	}

	void CommandBuffer::copyBuffer( VkBuffer src
		, VkBuffer dst
		, VkDeviceSize size
		, VkDeviceSize srcOffset
		, VkDeviceSize dstOffset )const
	{
		VkBufferCopy copyInfo{ srcOffset
			, dstOffset
			, size };
		copyBuffer( copyInfo, src, dst );
	}

	void CommandBuffer::copyBuffer( BufferBase const & src
		, BufferBase const & dst
		, VkDeviceSize size
		, VkDeviceSize srcOffset
		, VkDeviceSize dstOffset )const
	{
		VkBufferCopy copyInfo{ srcOffset
			, dstOffset
			, size };
		copyBuffer( copyInfo, src, dst );
	}

	void CommandBuffer::copyBuffer( BufferBase const & src
		, VertexBufferBase const & dst
		, VkDeviceSize size
		, VkDeviceSize srcOffset
		, VkDeviceSize dstOffset )const
	{
		VkBufferCopy copyInfo{ srcOffset
			, dstOffset
			, size };
		copyBuffer( copyInfo, src, dst.getBuffer() );
	}

	void CommandBuffer::copyBuffer( VertexBufferBase const & src
		, BufferBase const & dst
		, VkDeviceSize size
		, VkDeviceSize srcOffset
		, VkDeviceSize dstOffset )const
	{
		VkBufferCopy copyInfo{ srcOffset
			, dstOffset
			, size };
		copyBuffer( copyInfo, src.getBuffer(), dst );
	}

	void CommandBuffer::copyBuffer( VertexBufferBase const & src
		, VertexBufferBase const & dst
		, VkDeviceSize size
		, VkDeviceSize srcOffset
		, VkDeviceSize dstOffset )const
	{
		VkBufferCopy copyInfo{ srcOffset
			, dstOffset
			, size };
		copyBuffer( copyInfo, src.getBuffer(), dst.getBuffer() );
	}

	void CommandBuffer::copyBuffer( BufferBase const & src
		, UniformBuffer const & dst
		, VkDeviceSize size
		, VkDeviceSize srcOffset
		, VkDeviceSize dstOffset )const
	{
		VkBufferCopy copyInfo{ srcOffset
			, dstOffset
			, size };
		copyBuffer( copyInfo, src, dst.getBuffer() );
	}

	void CommandBuffer::copyBuffer( UniformBuffer const & src
		, BufferBase const & dst
		, VkDeviceSize size
		, VkDeviceSize srcOffset
		, VkDeviceSize dstOffset )const
	{
		VkBufferCopy copyInfo{ srcOffset
			, dstOffset
			, size };
		copyBuffer( copyInfo, src.getBuffer(), dst );
	}

	void CommandBuffer::copyBuffer( UniformBuffer const & src
		, UniformBuffer const & dst
		, VkDeviceSize size
		, VkDeviceSize srcOffset
		, VkDeviceSize dstOffset )const
	{
		VkBufferCopy copyInfo{ srcOffset
			, dstOffset
			, size };
		copyBuffer( copyInfo, src.getBuffer(), dst.getBuffer() );
	}

	void CommandBuffer::copyImage( ImageView const & src
		, ImageView const & dst )const
	{
		auto const & srcRange = src->subresourceRange;
		auto const & dstRange = dst->subresourceRange;
		copyImage( VkImageCopy
			{
				{                                   // srcSubresource
					getAspectMask( src->format ),
					srcRange.baseMipLevel,
					srcRange.baseArrayLayer,
					srcRange.layerCount
				},
				VkOffset3D{                         // srcOffset
					0,                              // x
					0,                              // y
					0                               // z
				},
				{                                   // dstSubresource
					getAspectMask( dst->format ),
					dstRange.baseMipLevel,
					dstRange.baseArrayLayer,
					dstRange.layerCount
				},
				VkOffset3D{                         // dstOffset
					0,                              // x
					0,                              // y
					0                               // z
				},
				dst.image->getDimensions()          // extent
			}
			, *src.image
			, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
			, *dst.image
			, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
	}

	void CommandBuffer::memoryBarrier( VkPipelineStageFlags after
		, VkPipelineStageFlags before
		, VkBufferMemoryBarrier const & transitionBarrier )const
	{
		pipelineBarrier( after
			, before
			, 0u
			, {}
			, { transitionBarrier }
			, {} );
	}

	void CommandBuffer::memoryBarrier( VkPipelineStageFlags after
		, VkPipelineStageFlags before
		, VkImageMemoryBarrier const & transitionBarrier )const
	{
		pipelineBarrier( after
			, before
			, 0u
			, {}
			, {}
			, { transitionBarrier } );
	}

	void CommandBuffer::pushConstants( PipelineLayout const & layout
		, PushConstantsBufferBase const & pcb )const
	{
		pushConstants( layout
			, pcb.getStageFlags()
			, pcb.getOffset()
			, pcb.getSize()
			, reinterpret_cast< void const * >( pcb.getData() ) );
	}

#ifdef VK_KHR_ray_tracing_pipeline

	void CommandBuffer::traceRays( Optional< VkStridedDeviceAddressRegionKHR > const & raygenShaderBindingTable
		, Optional< VkStridedDeviceAddressRegionKHR > const & missShaderBindingTable
		, Optional< VkStridedDeviceAddressRegionKHR > const & hitShaderBindingTable
		, Optional< VkStridedDeviceAddressRegionKHR > const & callableShaderBindingTable
		, uint32_t width
		, uint32_t height
		, uint32_t depth )const
	{
		m_device.vkCmdTraceRaysKHR( m_internal
			, convert( raygenShaderBindingTable )
			, convert( missShaderBindingTable )
			, convert( hitShaderBindingTable )
			, convert( callableShaderBindingTable )
			, width
			, height
			, depth );
	}

	void CommandBuffer::traceRaysIndirect( Optional< VkStridedDeviceAddressRegionKHR > const & raygenShaderBindingTable
		, Optional< VkStridedDeviceAddressRegionKHR > const & missShaderBindingTable
		, Optional< VkStridedDeviceAddressRegionKHR > const & hitShaderBindingTable
		, Optional< VkStridedDeviceAddressRegionKHR > const & callableShaderBindingTable
		, VkDeviceAddress indirectDeviceAddress )const
	{
		m_device.vkCmdTraceRaysIndirectKHR( m_internal
			, convert( raygenShaderBindingTable )
			, convert( missShaderBindingTable )
			, convert( hitShaderBindingTable )
			, convert( callableShaderBindingTable )
			, indirectDeviceAddress );
	}

	void CommandBuffer::setRayTracingPipelineStackSize( uint32_t pipelineStackSize )const
	{
		m_device.vkCmdSetRayTracingPipelineStackSizeKHR( m_internal
			, pipelineStackSize );
	}

#endif
#if VK_KHR_acceleration_structure

	void CommandBuffer::buildAccelerationStructures( AccelerationStructureBuildGeometryInfoArray const & infos
		, AccelerationStructureBuildRangeInfoPtrArray const & pBuildRangeInfos )const
	{
		auto vkinfos = makeVkArray< VkAccelerationStructureBuildGeometryInfoKHR >( infos );
		auto vkbuildRangesInfos = makeVkArray< VkAccelerationStructureBuildRangeInfoKHR const * >( pBuildRangeInfos );
		m_device.vkCmdBuildAccelerationStructuresKHR( m_internal
			, uint32_t( vkinfos.size() )
			, vkinfos.data()
			, vkbuildRangesInfos.data() );
	}

	void CommandBuffer::buildAccelerationStructuresIndirect( AccelerationStructureBuildGeometryInfoArray const & infos
		, VkDeviceAddressArray const & indirectDeviceAddresses
		, UInt32Array const & indirectStrides
		, UInt32PtrArray const & pMaxPrimitiveCounts )const
	{
		auto vkinfos = makeVkArray< VkAccelerationStructureBuildGeometryInfoKHR >( infos );
		m_device.vkCmdBuildAccelerationStructuresIndirectKHR( m_internal
			, uint32_t( vkinfos.size() )
			, vkinfos.data()
			, indirectDeviceAddresses.data()
			, indirectStrides.data()
			, pMaxPrimitiveCounts.data() );
	}

	void CommandBuffer::copyAccelerationStructure( VkCopyAccelerationStructureInfoKHR const & info )const
	{
		m_device.vkCmdCopyAccelerationStructureKHR( m_internal, &info );
	}

	void CommandBuffer::copyAccelerationStructureToMemory( VkCopyAccelerationStructureToMemoryInfoKHR const & info )const
	{
		m_device.vkCmdCopyAccelerationStructureToMemoryKHR( m_internal, &info );
	}

	void CommandBuffer::copyMemoryToAccelerationStructure( VkCopyMemoryToAccelerationStructureInfoKHR const & info )const
	{
		m_device.vkCmdCopyMemoryToAccelerationStructureKHR( m_internal, &info );
	}

	void CommandBuffer::writeAccelerationStructuresProperties( AccelerationStructureCRefArray const & accelerationStructures
		, VkQueryType queryType
		, VkQueryPool queryPool
		, uint32_t firstQuery )const
	{
		auto vkacc = makeVkArray< VkAccelerationStructureKHR >( accelerationStructures );
		m_device.vkCmdWriteAccelerationStructuresPropertiesKHR( m_internal
			, uint32_t( vkacc.size() )
			, vkacc.data()
			, queryType
			, queryPool
			, firstQuery );
	}

#endif
#if VK_EXT_debug_utils

	void CommandBuffer::beginDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
		if ( m_device.vkCmdBeginDebugUtilsLabelEXT )
		{
			m_device.vkCmdBeginDebugUtilsLabelEXT( m_internal, &labelInfo );
		}
	}

	void CommandBuffer::endDebugUtilsLabel()const
	{
		if ( m_device.vkCmdEndDebugUtilsLabelEXT )
		{
			m_device.vkCmdEndDebugUtilsLabelEXT( m_internal );
		}
	}

	void CommandBuffer::insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
		if ( m_device.vkCmdBeginDebugUtilsLabelEXT )
		{
			m_device.vkCmdBeginDebugUtilsLabelEXT( m_internal, &labelInfo );
		}
	}

#endif
#if VK_EXT_debug_marker

	void CommandBuffer::debugMarkerBegin( VkDebugMarkerMarkerInfoEXT const & labelInfo )const
	{
		if ( m_device.vkCmdDebugMarkerBeginEXT )
		{
			m_device.vkCmdDebugMarkerBeginEXT( m_internal, &labelInfo );
		}
	}

	void CommandBuffer::debugMarkerEnd()const
	{
		if ( m_device.vkCmdDebugMarkerEndEXT )
		{
			m_device.vkCmdDebugMarkerEndEXT( m_internal );
		}
	}

	void CommandBuffer::debugMarkerInsert( VkDebugMarkerMarkerInfoEXT const & labelInfo )const
	{
		if ( m_device.vkCmdDebugMarkerInsertEXT )
		{
			m_device.vkCmdDebugMarkerInsertEXT( m_internal, &labelInfo );
		}
	}

#endif
#if VK_EXT_debug_utils || VK_EXT_debug_marker

	void CommandBuffer::beginDebugBlock( DebugBlockInfo const & labelInfo )const
	{
#if VK_EXT_debug_utils
		beginDebugUtilsLabel(
			{
				VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
				nullptr,
				labelInfo.markerName.c_str(),
				{
					labelInfo.color[0],
					labelInfo.color[1],
					labelInfo.color[2],
					labelInfo.color[3],
				}
			} );
#endif
#if VK_EXT_debug_marker
		debugMarkerBegin(
			{
				VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT,
				nullptr,
				labelInfo.markerName.c_str(),
				{
					labelInfo.color[0],
					labelInfo.color[1],
					labelInfo.color[2],
					labelInfo.color[3],
				}
			} );
#endif
	}

	void CommandBuffer::endDebugBlock()const
	{
#if VK_EXT_debug_utils
		endDebugUtilsLabel();
#endif
#if VK_EXT_debug_marker
		debugMarkerEnd();
#endif
	}

	void CommandBuffer::insertDebugBlock( DebugBlockInfo const & labelInfo )const
	{
#if VK_EXT_debug_utils
		insertDebugUtilsLabel(
			{
				VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
				nullptr,
				labelInfo.markerName.c_str(),
				{
					labelInfo.color[0],
					labelInfo.color[1],
					labelInfo.color[2],
					labelInfo.color[3],
				}
			} );
#endif
#if VK_EXT_debug_marker
		debugMarkerInsert(
			{
				VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT,
				nullptr,
				labelInfo.markerName.c_str(),
				{
					labelInfo.color[0],
					labelInfo.color[1],
					labelInfo.color[2],
					labelInfo.color[3],
				}
			} );
#endif
	}

#endif
}
