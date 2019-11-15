/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/Command/Commands/GlCommandBase.hpp"
#include "renderer/Gl3Renderer/Command/GlCommandPool.hpp"
#include "renderer/Gl3Renderer/Core/GlContextStateStack.hpp"
#include "renderer/Gl3Renderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl3
{
	class CommandBuffer
	{
	public:
		CommandBuffer( VkDevice device
			, VkCommandBufferLevel level );
		~CommandBuffer();

		VkResult begin( VkCommandBufferBeginInfo info )const;
		VkResult end()const;
		VkResult reset( VkCommandBufferResetFlags flags )const;
		void beginRenderPass( VkRenderPassBeginInfo beginInfo
			, VkSubpassContents contents )const;
		void nextSubpass( VkSubpassContents contents )const;
		void endRenderPass()const;
		void executeCommands( VkCommandBufferArray commands )const;
		void clearColorImage( VkImage image
			, VkImageLayout imageLayout
			, VkClearColorValue colour
			, VkImageSubresourceRangeArray ranges )const;
		void clearDepthStencilImage( VkImage image
			, VkImageLayout imageLayout
			, VkClearDepthStencilValue value
			, VkImageSubresourceRangeArray ranges )const;
		void clearAttachments( VkClearAttachmentArray clearAttachments
			, VkClearRectArray clearRects );
		void bindPipeline( VkPipeline pipeline
			, VkPipelineBindPoint bindingPoint )const;
		void bindVertexBuffers( uint32_t firstBinding
			, VkBufferArray buffers
			, VkDeviceSizeArray offsets )const;
		void bindIndexBuffer( VkBuffer buffer
			, VkDeviceSize offset
			, VkIndexType indexType )const;
		void bindDescriptorSets( VkPipelineBindPoint bindingPoint
			, VkPipelineLayout layout
			, uint32_t firstSet
			, VkDescriptorSetArray descriptorSets
			, UInt32Array dynamicOffsets )const;
		void setViewport( uint32_t firstViewport
			, VkViewportArray viewports )const;
		void setScissor( uint32_t firstScissor
			, VkScissorArray scissors )const;
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
			, VkBufferImageCopyArray copyInfos )const;
		void copyToBuffer( VkImage src
			, VkImageLayout srcLayout
			, VkBuffer dst
			, VkBufferImageCopyArray copyInfos )const;
		void copyBuffer( VkBuffer src
			, VkBuffer dst
			, VkBufferCopyArray copyInfos )const;
		void copyImage( VkImage src
			, VkImageLayout srcLayout
			, VkImage dst
			, VkImageLayout dstLayout
			, VkImageCopyArray copyInfos )const;
		void blitImage( VkImage srcImage
			, VkImageLayout srcLayout
			, VkImage dstImage
			, VkImageLayout dstLayout
			, VkImageBlitArray regions
			, VkFilter filter )const;
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
		void setBlendConstants( float const blendConstants[4] )const;
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
		void waitEvents( VkEventArray events
			, VkPipelineStageFlags srcStageMask
			, VkPipelineStageFlags dstStageMask
			, VkMemoryBarrierArray memoryBarriers
			, VkBufferMemoryBarrierArray bufferMemoryBarriers
			, VkImageMemoryBarrierArray imageMemoryBarriers )const;
		void pipelineBarrier( VkPipelineStageFlags after
			, VkPipelineStageFlags before
			, VkDependencyFlags dependencyFlags
			, VkMemoryBarrierArray memoryBarriers
			, VkBufferMemoryBarrierArray bufferMemoryBarriers
			, VkImageMemoryBarrierArray imageMemoryBarriers )const;

		void generateMipmaps( VkImage texture );
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

		inline CmdBuffer const & getCmds()const
		{
			return m_cmds;
		}

		inline CmdBuffer const & getCmdsAfterSubmit()const
		{
			return m_cmdsAfterSubmit;
		}

		void initialiseGeometryBuffers( ContextLock & context )const;

	private:
		struct BufferIndex
		{
			BufferIndex( GLuint name
				, size_t index
				, DeviceMemoryDestroyConnection connection )
				: name{ name }
				, index{ index }
				, connection{ std::move( connection ) }
			{
			}

			GLuint name;
			size_t index;
			DeviceMemoryDestroyConnection connection;
		};

	private:
		void doReset()const;
		void doSelectVao()const;
		void doProcessMappedBoundDescriptorBuffersIn( VkDescriptorSet descriptor )const;
		void doProcessMappedBoundDescriptorsBuffersOut()const;
		void doProcessMappedBoundVaoBuffersIn()const;
		void doProcessMappedBoundBufferIn( VkBuffer buffer )const;
		void doProcessMappedBoundBufferOut( VkBuffer buffer )const;
		BufferIndex & doAddMappedBuffer( VkBuffer buffer, bool isInput )const;
		void doRemoveMappedBuffer( GLuint internal )const;

	private:
		VkDevice m_device;
		VkCommandBufferLevel m_level;
		mutable CmdList m_cmdList;
		mutable CmdBuffer m_cmds;
		mutable CmdList m_cmdAfterSubmit;
		mutable CmdBuffer m_cmdsAfterSubmit;
		mutable std::vector< BufferIndex > m_mappedBuffers;
		struct State
		{
			std::unique_ptr< ContextStateStack > stack;
			VkCommandBufferUsageFlags beginFlags{ 0u };
			VkPipeline currentPipeline{ nullptr };
			std::vector< std::pair < VkPipelineLayout, PushConstantsDesc > > pushConstantBuffers;
			VkPipeline currentComputePipeline{ nullptr };
			uint32_t currentSubpassIndex{ 0u };
			VkSubpassDescription const * currentSubpass{ nullptr };
			VkRenderPass currentRenderPass{ nullptr };
			VkFramebuffer currentFrameBuffer{ nullptr };
			VboBindings boundVbos;
			IboBinding boundIbo;
			IboBinding newlyBoundIbo;
			VkIndexType indexType;
			GeometryBuffers * selectedVao{ nullptr };
			GeometryBuffersRefArray vaos;
			VkDescriptorSetArray boundDescriptors;
		};
		mutable State m_state;
		mutable VkImageViewArray m_blitViews;
		mutable Optional< DebugLabel > m_label;
	};
}
