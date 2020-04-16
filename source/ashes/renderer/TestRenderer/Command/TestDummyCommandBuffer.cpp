/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/TestCommandBuffer.hpp"

#if AshesTest_DummyCommandBuffer

#include "ashestest_api.hpp"

namespace ashes::test
{
	CommandBuffer::CommandBuffer( VkDevice device
		, VkCommandPool commandPool
		, bool primary )
		: m_device{ device }
	{
		get( commandPool )->registerCommands( get( this ) );
	}

	CommandBuffer::~CommandBuffer()
	{
	}

	void CommandBuffer::execute()const
	{
	}

	VkResult CommandBuffer::begin( VkCommandBufferBeginInfo info )const
	{
		return VK_SUCCESS;
	}

	VkResult CommandBuffer::end()const
	{
		return VK_SUCCESS;
	}

	VkResult CommandBuffer::reset( VkCommandBufferResetFlags flags )const
	{
		return VK_SUCCESS;
	}

	void CommandBuffer::beginRenderPass( VkRenderPassBeginInfo beginInfo
		, VkSubpassContents contents )const
	{
	}

	void CommandBuffer::nextSubpass( VkSubpassContents contents )const
	{
	}

	void CommandBuffer::endRenderPass()const
	{
	}

	void CommandBuffer::executeCommands( VkCommandBufferArray commands )const
	{
	}

	void CommandBuffer::clearColorImage( VkImage image
		, VkImageLayout imageLayout
		, VkClearColorValue colour
		, VkImageSubresourceRangeArray ranges )const
	{
	}

	void CommandBuffer::clearDepthStencilImage( VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, VkImageSubresourceRangeArray ranges )const
	{
	}

	void CommandBuffer::clearAttachments( VkClearAttachmentArray clearAttachments
		, VkClearRectArray clearRects )
	{
	}

	void CommandBuffer::bindPipeline( VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint )const
	{
	}

	void CommandBuffer::bindVertexBuffers( uint32_t firstBinding
		, VkBufferArray buffers
		, UInt64Array offsets )const
	{
	}

	void CommandBuffer::bindIndexBuffer( VkBuffer buffer
		, uint64_t offset
		, VkIndexType indexType )const
	{
	}

	void CommandBuffer::bindDescriptorSets( VkPipelineBindPoint bindingPoint
		, VkPipelineLayout layout
		, uint32_t firstSet
		, VkDescriptorSetArray descriptorSets
		, UInt32Array dynamicOffsets )const
	{
	}

	void CommandBuffer::setViewport( uint32_t firstViewport
		, VkViewportArray viewports )const
	{
	}

	void CommandBuffer::setScissor( uint32_t firstScissor
		, VkScissorArray scissors )const
	{
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
	}

	void CommandBuffer::drawIndexed( uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance )const
	{
	}

	void CommandBuffer::drawIndirect( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
	}

	void CommandBuffer::drawIndexedIndirect( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
	}

	void CommandBuffer::copyToImage( VkBuffer src
		, VkImage dst
		, VkImageLayout dstLayout
		, VkBufferImageCopyArray copyInfos )const
	{
	}

	void CommandBuffer::copyToBuffer( VkImage src
		, VkImageLayout srcLayout
		, VkBuffer dst
		, VkBufferImageCopyArray copyInfos )const
	{
	}

	void CommandBuffer::updateBuffer( VkBuffer dstBuffer
		, VkDeviceSize dstOffset
		, ArrayView< uint8_t const > data )
	{
	}

	void CommandBuffer::fillBuffer( VkBuffer dstBuffer
		, VkDeviceSize dstOffset
		, VkDeviceSize size
		, uint32_t data )
	{
	}

	void CommandBuffer::copyBuffer( VkBuffer src
		, VkBuffer dst
		, VkBufferCopyArray copyInfos )const
	{
	}

	void CommandBuffer::copyImage( VkImage src
		, VkImageLayout srcLayout
		, VkImage dst
		, VkImageLayout dstLayout
		, VkImageCopyArray copyInfos )const
	{
	}

	void CommandBuffer::blitImage( VkImage srcImage
		, VkImageLayout srcLayout
		, VkImage dstImage
		, VkImageLayout dstLayout
		, VkImageBlitArray regions
		, VkFilter filter )const
	{
	}

	void CommandBuffer::resolveImage( VkImage srcImage
		, VkImageLayout srcLayout
		, VkImage dstImage
		, VkImageLayout dstLayout
		, VkImageResolveArray regions )const
	{
	}

	void CommandBuffer::resetQueryPool( VkQueryPool pool
		, uint32_t firstQuery
		, uint32_t queryCount )const
	{
	}

	void CommandBuffer::beginQuery( VkQueryPool pool
		, uint32_t query
		, VkQueryControlFlags flags )const
	{
	}

	void CommandBuffer::endQuery( VkQueryPool pool
		, uint32_t query )const
	{
	}

	void CommandBuffer::writeTimestamp( VkPipelineStageFlagBits pipelineStage
		, VkQueryPool pool
		, uint32_t query )const
	{
	}

	void CommandBuffer::copyQueryPoolResults( VkQueryPool queryPool
		, uint32_t firstQuery
		, uint32_t queryCount
		, VkBuffer dstBuffer
		, VkDeviceSize dstOffset
		, VkDeviceSize stride
		, VkQueryResultFlags flags )const
	{
	}

	void CommandBuffer::pushConstants( VkPipelineLayout layout
		, VkShaderStageFlags stageFlags
		, uint32_t offset
		, uint32_t size
		, void const * data )const
	{
	}

	void CommandBuffer::dispatch( uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ )const
	{
	}

	void CommandBuffer::dispatchIndirect( VkBuffer buffer
		, VkDeviceSize offset )const
	{
	}

	void CommandBuffer::setLineWidth( float width )const
	{
	}

	void CommandBuffer::setDepthBias( float constantFactor
		, float clamp
		, float slopeFactor )const
	{
	}

	void CommandBuffer::setBlendConstants( float const blendConstants[4] )const
	{
	}

	void CommandBuffer::setDepthBounds( float minDepthBounds
		, float maxDepthBounds )const
	{
	}

	void CommandBuffer::setStencilCompareMask( VkStencilFaceFlags faceMask
		, uint32_t compareMask )const
	{
	}

	void CommandBuffer::setStencilWriteMask( VkStencilFaceFlags faceMask
		, uint32_t writeMask )const
	{
	}

	void CommandBuffer::setStencilReference( VkStencilFaceFlags faceMask
		, uint32_t reference )
	{
	}

	void CommandBuffer::setEvent( VkEvent event
		, VkPipelineStageFlags stageMask )const
	{
	}

	void CommandBuffer::resetEvent( VkEvent event
		, VkPipelineStageFlags stageMask )const
	{
	}

	void CommandBuffer::waitEvents( VkEventArray events
		, VkPipelineStageFlags srcStageMask
		, VkPipelineStageFlags dstStageMask
		, VkMemoryBarrierArray memoryBarriers
		, VkBufferMemoryBarrierArray bufferMemoryBarriers
		, VkImageMemoryBarrierArray imageMemoryBarriers )const
	{
	}

	void CommandBuffer::pipelineBarrier( VkPipelineStageFlags after
		, VkPipelineStageFlags before
		, VkDependencyFlags dependencyFlags
		, VkMemoryBarrierArray memoryBarriers
		, VkBufferMemoryBarrierArray bufferMemoryBarriers
		, VkImageMemoryBarrierArray imageMemoryBarriers )const
	{
	}

	void CommandBuffer::generateMipmaps( VkImage texture )const
	{
	}

#if VK_EXT_debug_utils

	void CommandBuffer::beginDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
	}

	void CommandBuffer::endDebugUtilsLabel()const
	{
	}

	void CommandBuffer::insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
	}

#endif
#if VK_EXT_debug_marker

	void CommandBuffer::debugMarkerBegin( VkDebugMarkerMarkerInfoEXT const & labelInfo )const
	{
	}

	void CommandBuffer::debugMarkerEnd()const
	{
	}

	void CommandBuffer::debugMarkerInsert( VkDebugMarkerMarkerInfoEXT const & labelInfo )const
	{
	}

#endif
}

#endif
