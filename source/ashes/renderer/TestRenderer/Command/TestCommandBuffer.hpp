/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestBlitImageCommand.hpp"

#include <renderer/RendererCommon/IcdObject.hpp>

#define AshesTest_DummyCommandBuffer

namespace ashes::test
{
	class CommandBuffer
		: public ashes::IcdObject
	{
	public:
		CommandBuffer( VkDevice device
			, VkCommandPool commandPool
			, bool primary );

		void execute()const;

		VkResult begin( VkCommandBufferBeginInfo const & info )const;
		VkResult end()const;
		VkResult reset( VkCommandBufferResetFlags flags )const;
		void beginRenderPass( VkRenderPassBeginInfo const & beginInfo
			, VkSubpassContents contents )const;
		void nextSubpass( VkSubpassContents contents )const;
		void endRenderPass()const;
		void executeCommands( VkCommandBufferArray const & commands )const;
		void clearColorImage( VkImage image
			, VkImageLayout imageLayout
			, VkClearColorValue colour
			, VkImageSubresourceRangeArray const & ranges )const;
		void clearDepthStencilImage( VkImage image
			, VkImageLayout imageLayout
			, VkClearDepthStencilValue value
			, VkImageSubresourceRangeArray const & ranges )const;
		void clearAttachments( VkClearAttachmentArray const & clearAttachments
			, VkClearRectArray const & clearRects );
		void bindPipeline( VkPipeline pipeline
			, VkPipelineBindPoint bindingPoint )const;
		void bindVertexBuffers( uint32_t firstBinding
			, VkBufferArray const & buffers
			, VkDeviceSizeArray const & offsets )const;
		void bindIndexBuffer( VkBuffer buffer
			, VkDeviceSize offset
			, VkIndexType indexType )const;
		void bindDescriptorSets( VkPipelineBindPoint bindingPoint
			, VkPipelineLayout layout
			, uint32_t firstSet
			, VkDescriptorSetArray const & descriptorSets
			, UInt32Array const & dynamicOffsets )const;
		void setViewport( uint32_t firstViewport
			, VkViewportArray const & viewports )const;
		void setScissor( uint32_t firstScissor
			, VkScissorArray const & scissors )const;
		void draw( uint32_t vtxCount
			, uint32_t instCount
			, uint32_t firstVertex
			, uint32_t firstInstance )const;
		void drawIndexed( uint32_t indexCount
			, uint32_t instCount
			, uint32_t firstIndex
			, uint32_t vertexOffset
			, uint32_t firstInstance )const;
		void drawIndirect( VkBuffer buffer
			, VkDeviceSize offset
			, uint32_t drawCount
			, uint32_t stride )const;
		void drawIndexedIndirect( VkBuffer buffer
			, VkDeviceSize offset
			, uint32_t drawCount
			, uint32_t stride )const;
		void copyToImage( VkBuffer src
			, VkImage dst
			, VkImageLayout dstLayout
			, VkBufferImageCopyArray const & copyInfos )const;
		void copyToBuffer( VkImage src
			, VkImageLayout srcLayout
			, VkBuffer dst
			, VkBufferImageCopyArray const & copyInfos )const;
		void updateBuffer( VkBuffer dstBuffer
			, VkDeviceSize dstOffset
			, ArrayView< uint8_t const > data );
		void fillBuffer( VkBuffer dstBuffer
			, VkDeviceSize dstOffset
			, VkDeviceSize size
			, uint32_t data );
		void copyBuffer( VkBuffer src
			, VkBuffer dst
			, VkBufferCopyArray const & copyInfos )const;
		void copyImage( VkImage src
			, VkImageLayout srcLayout
			, VkImage dst
			, VkImageLayout dstLayout
			, VkImageCopyArray const & copyInfos )const;
		void blitImage( VkImage srcImage
			, VkImageLayout srcLayout
			, VkImage dstImage
			, VkImageLayout dstLayout
			, VkImageBlitArray const & regions
			, VkFilter filter )const;
		void resolveImage( VkImage srcImage
			, VkImageLayout srcLayout
			, VkImage dstImage
			, VkImageLayout dstLayout
			, VkImageResolveArray const & regions )const;
		void resetQueryPool( VkQueryPool pool
			, uint32_t firstQuery
			, uint32_t queryCount )const;
		void beginQuery( VkQueryPool pool
			, uint32_t query
			, VkQueryControlFlags flags )const;
		void endQuery( VkQueryPool pool
			, uint32_t query )const;
		void writeTimestamp( VkPipelineStageFlagBits pipelineStage
			, VkQueryPool pool
			, uint32_t query )const;
		void copyQueryPoolResults( VkQueryPool queryPool
			, uint32_t firstQuery
			, uint32_t queryCount
			, VkBuffer dstBuffer
			, VkDeviceSize dstOffset
			, VkDeviceSize stride
			, VkQueryResultFlags flags )const;
		void pushConstants( VkPipelineLayout layout
			, VkShaderStageFlags stageFlags
			, uint32_t offset
			, uint32_t size
			, void const * data )const;
		void dispatch( uint32_t groupCountX
			, uint32_t groupCountY
			, uint32_t groupCountZ )const;
		void dispatchIndirect( VkBuffer buffer
			, VkDeviceSize offset )const;
		void setLineWidth( float width )const;
		void setDepthBias( float constantFactor
			, float clamp
			, float slopeFactor )const;
		void setBlendConstants( ArrayView< float const > blendConstants )const;
		void setDepthBounds( float minDepthBounds
			, float maxDepthBounds )const;
		void setStencilCompareMask( VkStencilFaceFlags faceMask
			, uint32_t compareMask )const;
		void setStencilWriteMask( VkStencilFaceFlags faceMask
			, uint32_t writeMask )const;
		void setStencilReference( VkStencilFaceFlags faceMask
			, uint32_t reference );
		void setEvent( VkEvent event
			, VkPipelineStageFlags stageMask )const;
		void resetEvent( VkEvent event
			, VkPipelineStageFlags stageMask )const;
		void waitEvents( VkEventArray const & events
			, VkPipelineStageFlags srcStageMask
			, VkPipelineStageFlags dstStageMask
			, VkMemoryBarrierArray const & memoryBarriers
			, VkBufferMemoryBarrierArray const & bufferMemoryBarriers
			, VkImageMemoryBarrierArray const & imageMemoryBarriers )const;
		void pipelineBarrier( VkPipelineStageFlags after
			, VkPipelineStageFlags before
			, VkDependencyFlags dependencyFlags
			, VkMemoryBarrierArray const & memoryBarriers
			, VkBufferMemoryBarrierArray const & bufferMemoryBarriers
			, VkImageMemoryBarrierArray const & imageMemoryBarriers )const;
		void generateMipmaps( VkImage texture )const;
#if VK_EXT_debug_utils
		void beginDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const;
		void endDebugUtilsLabel()const;
		void insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const;
#endif
#if VK_EXT_debug_marker
		void debugMarkerBegin( VkDebugMarkerMarkerInfoEXT const & labelInfo )const;
		void debugMarkerEnd()const;
		void debugMarkerInsert( VkDebugMarkerMarkerInfoEXT const & labelInfo )const;
#endif

		CommandArray const & getCommands()const
		{
			return m_commands;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		struct ResourceIndex
		{
			ResourceIndex( ObjectMemory const * memory
				, size_t index
				, DeviceMemoryDestroyConnection connection )
				: memory{ memory }
				, index{ index }
				, connection{ std::move( connection ) }
			{
			}

			ObjectMemory const * memory;
			size_t index;
			DeviceMemoryDestroyConnection connection;
		};

	private:
		void doFillVboStrides()const;
		void doAddAfterSubmitAction()const;
		void doProcessMappedBoundDescriptorResourcesIn( VkDescriptorSet descriptor )const;
		void doProcessMappedBoundDescriptorsResourcesOut()const;
		void doProcessMappedBoundVaoBuffersIn()const;
		void doProcessMappedBoundResourceIn( VkBuffer buffer
			, VkDeviceSize offset
			, VkDeviceSize range )const;
		void doProcessMappedBoundResourceIn( VkImageView image )const;
		void doProcessMappedBoundResourceOut( VkBuffer buffer
			, VkDeviceSize offset
			, VkDeviceSize range )const;
		void doProcessMappedBoundResourceOut( VkImageView image )const;
		ResourceIndex & doAddMappedResource( ObjectMemory const * memory
			, VkDeviceSize offset
			, VkDeviceSize range
			, bool isInput )const;
		ResourceIndex & doAddMappedResource( ObjectMemory const * memory )const;
		void doRemoveMappedResource( ObjectMemory const * memory )const;

	private:
		VkDevice m_device;
		VkCommandPool m_commandPool;
		mutable CommandArray m_commands;
		mutable std::vector< ResourceIndex > m_mappedResources;
		struct State
		{
			VkCommandBufferBeginInfo beginInfo{};
			VkPipeline currentPipeline{};
			std::vector< std::pair < VkPipelineLayout, PushConstantsDesc > > pushConstantBuffers;
			VkPipeline currentComputePipeline{};
			VkSubpassDescription const * currentSubpass{};
			VkRenderPass currentRenderPass{};
			VkFramebuffer currentFrameBuffer{};
			uint32_t currentSubpassIndex{ 0u };
			mutable VbosBindingArray vbos;
			VkIndexType indexType{};
			VkDescriptorSetArray boundDescriptors;
			VkBuffer newlyBoundIbo{};
		};
		mutable State m_state;
		mutable ActionArray m_afterSubmitActions;
		mutable Optional< DebugLabel > m_label;
	};
}
