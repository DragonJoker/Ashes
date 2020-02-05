/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11BlitImageCommand.hpp"

namespace ashes::d3d11
{
	class CommandBuffer
	{
	public:
		CommandBuffer( VkDevice device
			, VkCommandPool commandPool
			, bool primary );
		~CommandBuffer();

		void execute( Context & context )const;

		VkResult begin( VkCommandBufferBeginInfo info )const;
		VkResult end()const;
		VkResult reset( VkCommandBufferResetFlags flags )const;
		void beginRenderPass( VkRenderPassBeginInfo beginInfo
			, VkSubpassContents contents )const;
		void nextSubpass( VkSubpassContents contents )const;
		void endRenderPass()const;
		void executeCommands( ArrayView< VkCommandBuffer const > commands )const;
		void clearColorImage( VkImage image
			, VkImageLayout imageLayout
			, VkClearColorValue colour
			, ArrayView < VkImageSubresourceRange const > ranges )const;
		void clearDepthStencilImage( VkImage image
			, VkImageLayout imageLayout
			, VkClearDepthStencilValue value
			, ArrayView< VkImageSubresourceRange const > ranges )const;
		void clearAttachments( ArrayView< VkClearAttachment const > clearAttachments
			, ArrayView< VkClearRect const > clearRects );
		void bindPipeline( VkPipeline pipeline
			, VkPipelineBindPoint bindingPoint )const;
		void bindVertexBuffers( uint32_t firstBinding
			, ArrayView< VkBuffer const > buffers
			, ArrayView< VkDeviceSize const > offsets )const;
		void bindIndexBuffer( VkBuffer buffer
			, VkDeviceSize offset
			, VkIndexType indexType )const;
		void bindDescriptorSets( VkPipelineBindPoint bindingPoint
			, VkPipelineLayout layout
			, uint32_t firstSet
			, ArrayView< VkDescriptorSet const > descriptorSets
			, ArrayView< uint32_t const > dynamicOffsets )const;
		void setViewport( uint32_t firstViewport
			, ArrayView< VkViewport const > viewports )const;
		void setScissor( uint32_t firstScissor
			, ArrayView< VkRect2D const > scissors )const;
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
			, ArrayView< VkBufferImageCopy const > copyInfos )const;
		void copyToBuffer( VkImage src
			, VkImageLayout srcLayout
			, VkBuffer dst
			, ArrayView< VkBufferImageCopy const > copyInfos )const;
		void updateBuffer( VkBuffer dstBuffer
			, VkDeviceSize dstOffset
			, ArrayView< uint8_t const > data );
		void fillBuffer( VkBuffer dstBuffer
			, VkDeviceSize dstOffset
			, VkDeviceSize size
			, uint32_t data );
		void copyBuffer( VkBuffer src
			, VkBuffer dst
			, ArrayView< VkBufferCopy const > copyInfos )const;
		void copyImage( VkImage src
			, VkImageLayout srcLayout
			, VkImage dst
			, VkImageLayout dstLayout
			, ArrayView< VkImageCopy const > copyInfos )const;
		void blitImage( VkImage srcImage
			, VkImageLayout srcLayout
			, VkImage dstImage
			, VkImageLayout dstLayout
			, ArrayView< VkImageBlit const > regions
			, VkFilter filter )const;
		void resolveImage( VkImage srcImage
			, VkImageLayout srcLayout
			, VkImage dstImage
			, VkImageLayout dstLayout
			, ArrayView< VkImageResolve const > regions )const;
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
		void setBlendConstants( const float blendConstants[4] );
		void setDepthBounds( float minDepthBounds
			, float maxDepthBounds );
		void setStencilCompareMask( VkStencilFaceFlags faceMask
			, uint32_t compareMask );
		void setStencilWriteMask( VkStencilFaceFlags faceMask
			, uint32_t writeMask );
		void setStencilReference( VkStencilFaceFlags faceMask
			, uint32_t reference );
		void setEvent( VkEvent event
			, VkPipelineStageFlags stageMask )const;
		void resetEvent( VkEvent event
			, VkPipelineStageFlags stageMask )const;
		void waitEvents( ArrayView< VkEvent const > events
			, VkPipelineStageFlags srcStageMask
			, VkPipelineStageFlags dstStageMask
			, ArrayView< VkMemoryBarrier const > memoryBarriers
			, ArrayView< VkBufferMemoryBarrier const > bufferMemoryBarriers
			, ArrayView< VkImageMemoryBarrier const > imageMemoryBarriers )const;
		void pipelineBarrier( VkPipelineStageFlags after
			, VkPipelineStageFlags before
			, VkDependencyFlags dependencyFlags
			, ArrayView< VkMemoryBarrier const > memoryBarriers
			, ArrayView< VkBufferMemoryBarrier const > bufferMemoryBarriers
			, ArrayView< VkImageMemoryBarrier const > imageMemoryBarriers )const;
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

		inline CommandArray const & getCommands()const
		{
			return m_commands;
		}

		inline ID3D11CommandList * getCommandList()const
		{
			return m_commandList;
		}

		inline VkDevice getDevice()const
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
			, UINT subresource
			, bool isInput )const;
		ResourceIndex & doAddMappedResource( ObjectMemory const * memory )const;
		void doRemoveMappedResource( ObjectMemory const * memory )const;
		BlitPipeline const & doGetBlitPipeline( VkFormat src, VkFormat dst )const;
		void doApplyDynamicStates()const;
		void doBindDynamicStates()const;

	private:
		VkDevice m_device;
		VkCommandPool m_commandPool;
		mutable CommandArray m_commands;
		mutable std::vector< ResourceIndex > m_mappedResources;
		struct State
		{
			VkCommandBufferBeginInfo beginInfo;
			VkPipeline currentPipeline{ VK_NULL_HANDLE };
			std::vector< std::pair < VkPipelineLayout, PushConstantsDesc > > pushConstantBuffers;
			VkPipeline currentComputePipeline{ VK_NULL_HANDLE };
			VkSubpassDescription const * currentSubpass{ VK_NULL_HANDLE };
			VkRenderPass currentRenderPass{ VK_NULL_HANDLE };
			VkFramebuffer currentFrameBuffer{ VK_NULL_HANDLE };
			uint32_t currentSubpassIndex{ 0u };
			mutable VbosBindingArray vbos;
			VkIndexType indexType;
			VkDescriptorSetArray boundDescriptors;
			VkBuffer newlyBoundIbo{ VK_NULL_HANDLE };
			Optional< float > lineWidth;
			Optional< DepthBias > depthBias;
			Optional< std::array< float, 4u > > blendConstants;
			Optional< DepthBounds > depthBounds;
			std::array< Optional< uint32_t >, 2u > stencilCompareMask;
			std::array< Optional< uint32_t >, 2u > stencilWriteMask;
			std::array< Optional< uint32_t >, 2u > stencilReference;
		};
		mutable State m_state;
		mutable std::vector< std::function< void( Context const & ) > > m_afterSubmitActions;
		mutable ID3D11DeviceContext * m_deferredContext{ nullptr };
		mutable ID3D11CommandList * m_commandList{ nullptr };
		mutable std::map< uint64_t, std::unique_ptr< BlitPipeline > > m_blitPipelines;
		mutable Optional< DebugLabel > m_label;
	};
}
