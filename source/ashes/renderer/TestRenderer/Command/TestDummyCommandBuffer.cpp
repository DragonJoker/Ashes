/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/TestCommandBuffer.hpp"

#if defined( AshesTest_DummyCommandBuffer )

#include "ashestest_api.hpp"

namespace ashes::test
{
	CommandBuffer::CommandBuffer( VkDevice device
		, VkCommandPool commandPool
		, [[maybe_unused]] bool primary )
		: m_device{ device }
		, m_commandPool{ commandPool }
	{
		get( m_commandPool )->registerCommands( get( this ) );
	}

	void CommandBuffer::execute()const
	{
	}

	VkResult CommandBuffer::begin( [[maybe_unused]] VkCommandBufferBeginInfo const & info )const
	{
		return VK_SUCCESS;
	}

	VkResult CommandBuffer::end()const
	{
		return VK_SUCCESS;
	}

	VkResult CommandBuffer::reset( [[maybe_unused]] VkCommandBufferResetFlags flags )const
	{
		return VK_SUCCESS;
	}

	void CommandBuffer::beginRenderPass( [[maybe_unused]] VkRenderPassBeginInfo const & beginInfo
		, [[maybe_unused]] VkSubpassContents contents )const
	{
	}

	void CommandBuffer::nextSubpass( [[maybe_unused]] VkSubpassContents contents )const
	{
	}

	void CommandBuffer::endRenderPass()const
	{
	}

	void CommandBuffer::executeCommands( [[maybe_unused]] VkCommandBufferArray const & commands )const
	{
	}

	void CommandBuffer::clearColorImage( [[maybe_unused]] VkImage image
		, [[maybe_unused]] VkImageLayout imageLayout
		, [[maybe_unused]] VkClearColorValue colour
		, [[maybe_unused]] VkImageSubresourceRangeArray const & ranges )const
	{
	}

	void CommandBuffer::clearDepthStencilImage( [[maybe_unused]] VkImage image
		, [[maybe_unused]] VkImageLayout imageLayout
		, [[maybe_unused]] VkClearDepthStencilValue value
		, [[maybe_unused]] VkImageSubresourceRangeArray const & ranges )const
	{
	}

	void CommandBuffer::clearAttachments( [[maybe_unused]] VkClearAttachmentArray const & clearAttachments
		, [[maybe_unused]] VkClearRectArray const & clearRects )
	{
	}

	void CommandBuffer::bindPipeline( [[maybe_unused]] VkPipeline pipeline
		, [[maybe_unused]] VkPipelineBindPoint bindingPoint )const
	{
	}

	void CommandBuffer::bindVertexBuffers( [[maybe_unused]] uint32_t firstBinding
		, [[maybe_unused]] VkBufferArray const & buffers
		, [[maybe_unused]] UInt64Array const & offsets )const
	{
	}

	void CommandBuffer::bindIndexBuffer( [[maybe_unused]] VkBuffer buffer
		, [[maybe_unused]] uint64_t offset
		, [[maybe_unused]] VkIndexType indexType )const
	{
	}

	void CommandBuffer::bindDescriptorSets( [[maybe_unused]] VkPipelineBindPoint bindingPoint
		, [[maybe_unused]] VkPipelineLayout layout
		, [[maybe_unused]] uint32_t firstSet
		, [[maybe_unused]] VkDescriptorSetArray const & descriptorSets
		, [[maybe_unused]] UInt32Array const & dynamicOffsets )const
	{
	}

	void CommandBuffer::setViewport( [[maybe_unused]] uint32_t firstViewport
		, [[maybe_unused]] VkViewportArray const & viewports )const
	{
	}

	void CommandBuffer::setScissor( [[maybe_unused]] uint32_t firstScissor
		, [[maybe_unused]] VkScissorArray const & scissors )const
	{
	}

	void CommandBuffer::draw( [[maybe_unused]] uint32_t vtxCount
		, [[maybe_unused]] uint32_t instCount
		, [[maybe_unused]] uint32_t firstVertex
		, [[maybe_unused]] uint32_t firstInstance )const
	{
	}

	void CommandBuffer::drawIndexed( [[maybe_unused]] uint32_t indexCount
		, [[maybe_unused]] uint32_t instCount
		, [[maybe_unused]] uint32_t firstIndex
		, [[maybe_unused]] uint32_t vertexOffset
		, [[maybe_unused]] uint32_t firstInstance )const
	{
	}

	void CommandBuffer::drawIndirect( [[maybe_unused]] VkBuffer buffer
		, [[maybe_unused]] VkDeviceSize offset
		, [[maybe_unused]] uint32_t drawCount
		, [[maybe_unused]] uint32_t stride )const
	{
	}

	void CommandBuffer::drawIndexedIndirect( [[maybe_unused]] VkBuffer buffer
		, [[maybe_unused]] VkDeviceSize offset
		, [[maybe_unused]] uint32_t drawCount
		, [[maybe_unused]] uint32_t stride )const
	{
	}

	void CommandBuffer::copyToImage( [[maybe_unused]] VkBuffer src
		, [[maybe_unused]] VkImage dst
		, [[maybe_unused]] VkImageLayout dstLayout
		, [[maybe_unused]] VkBufferImageCopyArray const & copyInfos )const
	{
	}

	void CommandBuffer::copyToBuffer( [[maybe_unused]] VkImage src
		, [[maybe_unused]] VkImageLayout srcLayout
		, [[maybe_unused]] VkBuffer dst
		, [[maybe_unused]] VkBufferImageCopyArray const & copyInfos )const
	{
	}

	void CommandBuffer::updateBuffer( [[maybe_unused]] VkBuffer dstBuffer
		, [[maybe_unused]] VkDeviceSize dstOffset
		, [[maybe_unused]] ArrayView< uint8_t const > data )
	{
	}

	void CommandBuffer::fillBuffer( [[maybe_unused]] VkBuffer dstBuffer
		, [[maybe_unused]] VkDeviceSize dstOffset
		, [[maybe_unused]] VkDeviceSize size
		, [[maybe_unused]] uint32_t data )
	{
	}

	void CommandBuffer::copyBuffer( [[maybe_unused]] VkBuffer src
		, [[maybe_unused]] VkBuffer dst
		, [[maybe_unused]] VkBufferCopyArray const & copyInfos )const
	{
	}

	void CommandBuffer::copyImage( [[maybe_unused]] VkImage src
		, [[maybe_unused]] VkImageLayout srcLayout
		, [[maybe_unused]] VkImage dst
		, [[maybe_unused]] VkImageLayout dstLayout
		, [[maybe_unused]] VkImageCopyArray const & copyInfos )const
	{
	}

	void CommandBuffer::blitImage( [[maybe_unused]] VkImage srcImage
		, [[maybe_unused]] VkImageLayout srcLayout
		, [[maybe_unused]] VkImage dstImage
		, [[maybe_unused]] VkImageLayout dstLayout
		, [[maybe_unused]] VkImageBlitArray const & regions
		, [[maybe_unused]] VkFilter filter )const
	{
	}

	void CommandBuffer::resolveImage( [[maybe_unused]] VkImage srcImage
		, [[maybe_unused]] VkImageLayout srcLayout
		, [[maybe_unused]] VkImage dstImage
		, [[maybe_unused]] VkImageLayout dstLayout
		, [[maybe_unused]] VkImageResolveArray const & regions )const
	{
	}

	void CommandBuffer::resetQueryPool( [[maybe_unused]] VkQueryPool pool
		, [[maybe_unused]] uint32_t firstQuery
		, [[maybe_unused]] uint32_t queryCount )const
	{
	}

	void CommandBuffer::beginQuery( [[maybe_unused]] VkQueryPool pool
		, [[maybe_unused]] uint32_t query
		, [[maybe_unused]] VkQueryControlFlags flags )const
	{
	}

	void CommandBuffer::endQuery( [[maybe_unused]] VkQueryPool pool
		, [[maybe_unused]] uint32_t query )const
	{
	}

	void CommandBuffer::writeTimestamp( [[maybe_unused]] VkPipelineStageFlagBits pipelineStage
		, [[maybe_unused]] VkQueryPool pool
		, [[maybe_unused]] uint32_t query )const
	{
	}

	void CommandBuffer::copyQueryPoolResults( [[maybe_unused]] VkQueryPool queryPool
		, [[maybe_unused]] uint32_t firstQuery
		, [[maybe_unused]] uint32_t queryCount
		, [[maybe_unused]] VkBuffer dstBuffer
		, [[maybe_unused]] VkDeviceSize dstOffset
		, [[maybe_unused]] VkDeviceSize stride
		, [[maybe_unused]] VkQueryResultFlags flags )const
	{
	}

	void CommandBuffer::pushConstants( [[maybe_unused]] VkPipelineLayout layout
		, [[maybe_unused]] VkShaderStageFlags stageFlags
		, [[maybe_unused]] uint32_t offset
		, [[maybe_unused]] uint32_t size
		, [[maybe_unused]] void const * data )const
	{
	}

	void CommandBuffer::dispatch( [[maybe_unused]] uint32_t groupCountX
		, [[maybe_unused]] uint32_t groupCountY
		, [[maybe_unused]] uint32_t groupCountZ )const
	{
	}

	void CommandBuffer::dispatchIndirect( [[maybe_unused]] VkBuffer buffer
		, [[maybe_unused]] VkDeviceSize offset )const
	{
	}

	void CommandBuffer::setLineWidth( [[maybe_unused]] float width )const
	{
	}

	void CommandBuffer::setDepthBias( [[maybe_unused]] float constantFactor
		, [[maybe_unused]] float clamp
		, [[maybe_unused]] float slopeFactor )const
	{
	}

	void CommandBuffer::setBlendConstants( [[maybe_unused]] ArrayView< float const > blendConstants )const
	{
	}

	void CommandBuffer::setDepthBounds( [[maybe_unused]] float minDepthBounds
		, [[maybe_unused]] float maxDepthBounds )const
	{
	}

	void CommandBuffer::setStencilCompareMask( [[maybe_unused]] VkStencilFaceFlags faceMask
		, [[maybe_unused]] uint32_t compareMask )const
	{
	}

	void CommandBuffer::setStencilWriteMask( [[maybe_unused]] VkStencilFaceFlags faceMask
		, [[maybe_unused]] uint32_t writeMask )const
	{
	}

	void CommandBuffer::setStencilReference( [[maybe_unused]] VkStencilFaceFlags faceMask
		, [[maybe_unused]] uint32_t reference )
	{
	}

	void CommandBuffer::setEvent( [[maybe_unused]] VkEvent event
		, [[maybe_unused]] VkPipelineStageFlags stageMask )const
	{
	}

	void CommandBuffer::resetEvent( [[maybe_unused]] VkEvent event
		, [[maybe_unused]] VkPipelineStageFlags stageMask )const
	{
	}

	void CommandBuffer::waitEvents( [[maybe_unused]] VkEventArray const & events
		, [[maybe_unused]] VkPipelineStageFlags srcStageMask
		, [[maybe_unused]] VkPipelineStageFlags dstStageMask
		, [[maybe_unused]] VkMemoryBarrierArray const & memoryBarriers
		, [[maybe_unused]] VkBufferMemoryBarrierArray const & bufferMemoryBarriers
		, [[maybe_unused]] VkImageMemoryBarrierArray const & imageMemoryBarriers )const
	{
	}

	void CommandBuffer::pipelineBarrier( [[maybe_unused]] VkPipelineStageFlags after
		, [[maybe_unused]] VkPipelineStageFlags before
		, [[maybe_unused]] VkDependencyFlags dependencyFlags
		, [[maybe_unused]] VkMemoryBarrierArray const & memoryBarriers
		, [[maybe_unused]] VkBufferMemoryBarrierArray const & bufferMemoryBarriers
		, [[maybe_unused]] VkImageMemoryBarrierArray const & imageMemoryBarriers )const
	{
	}

	void CommandBuffer::generateMipmaps( [[maybe_unused]] VkImage texture )const
	{
	}

#if VK_EXT_debug_utils

	void CommandBuffer::beginDebugUtilsLabel( [[maybe_unused]] VkDebugUtilsLabelEXT const & labelInfo )const
	{
	}

	void CommandBuffer::endDebugUtilsLabel()const
	{
	}

	void CommandBuffer::insertDebugUtilsLabel( [[maybe_unused]] VkDebugUtilsLabelEXT const & labelInfo )const
	{
	}

#endif
#if VK_EXT_debug_marker

	void CommandBuffer::debugMarkerBegin( [[maybe_unused]] VkDebugMarkerMarkerInfoEXT const & labelInfo )const
	{
	}

	void CommandBuffer::debugMarkerEnd()const
	{
	}

	void CommandBuffer::debugMarkerInsert( [[maybe_unused]] VkDebugMarkerMarkerInfoEXT const & labelInfo )const
	{
	}

#endif
}

#endif
