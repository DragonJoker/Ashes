/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/Command/Commands/GlCommandBase.hpp"
#include "renderer/Gl4Renderer/Command/GlCommandPool.hpp"
#include "renderer/Gl4Renderer/Core/GlContextStateStack.hpp"
#include "renderer/Gl4Renderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl4
{
	/**
	*\brief
	*	Emulation d'un command buffer, à la manière de Vulkan.
	*/
	class CommandBuffer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] pool
		*	Le pool de tampons de commandes.
		*\param[in] primary
		*	Dit si le tampon est un tampon de commandes primaire (\p true) ou secondaire (\p false).
		*/
		CommandBuffer( VkDevice device
			, VkCommandBufferLevel level );
		~CommandBuffer();
		/**
		*\copydoc	CommandBuffer::begin
		*/
		VkResult begin( VkCommandBufferBeginInfo info )const;
		/**
		*\copydoc	CommandBuffer::end
		*/
		VkResult end()const;
		/**
		*\copydoc	CommandBuffer::reset
		*/
		VkResult reset( VkCommandBufferResetFlags flags )const;
		/**
		*\copydoc	CommandBuffer::beginRenderPass
		*/
		void beginRenderPass( VkRenderPassBeginInfo beginInfo
			, VkSubpassContents contents )const;
		/**
		*\copydoc	CommandBuffer::nextSubpass
		*/
		void nextSubpass( VkSubpassContents contents )const;
		/**
		*\copydoc	CommandBuffer::endRenderPass
		*/
		void endRenderPass()const;
		/**
		*\copydoc	CommandBuffer::executeCommands
		*/
		void executeCommands( VkCommandBufferArray commands )const;
		/**
		*\copydoc	CommandBuffer::clear
		*/
		void clearColorImage( VkImage image
			, VkImageLayout imageLayout
			, VkClearColorValue colour
			, VkImageSubresourceRangeArray ranges )const;
		/**
		*\copydoc	CommandBuffer::clear
		*/
		void clearDepthStencilImage( VkImage image
			, VkImageLayout imageLayout
			, VkClearDepthStencilValue value
			, VkImageSubresourceRangeArray ranges )const;
		/**
		*\copydoc	clearAttachments:clear
		*/
		void clearAttachments( VkClearAttachmentArray clearAttachments
			, VkClearRectArray clearRects );
		/**
		*\copydoc	CommandBuffer::bindPipeline
		*/
		void bindPipeline( VkPipeline pipeline
			, VkPipelineBindPoint bindingPoint )const;
		/**
		*\copydoc	CommandBuffer:bindVertexBuffers
		*/
		void bindVertexBuffers( uint32_t firstBinding
			, VkBufferArray buffers
			, VkDeviceSizeArray offsets )const;
		/**
		*\copydoc	CommandBuffer:bindIndexBuffer
		*/
		void bindIndexBuffer( VkBuffer buffer
			, VkDeviceSize offset
			, VkIndexType indexType )const;
		/**
		*\copydoc	CommandBuffer::bindDescriptorSet
		*/
		void bindDescriptorSets( VkPipelineBindPoint bindingPoint
			, VkPipelineLayout layout
			, uint32_t firstSet
			, VkDescriptorSetArray descriptorSets
			, UInt32Array dynamicOffsets )const;
		/**
		*\copydoc	CommandBuffer::setViewport
		*/
		void setViewport( uint32_t firstViewport
			, VkViewportArray viewports )const;
		/**
		*\copydoc	CommandBuffer::setScissor
		*/
		void setScissor( uint32_t firstScissor
			, VkScissorArray scissors )const;
		/**
		*\copydoc	CommandBuffer::draw
		*/
		void draw( uint32_t vtxCount
			, uint32_t instCount
			, uint32_t firstVertex
			, uint32_t firstInstance )const;
		/**
		*\copydoc	CommandBuffer::drawIndexed
		*/
		void drawIndexed( uint32_t indexCount
			, uint32_t instCount
			, uint32_t firstIndex
			, uint32_t vertexOffset
			, uint32_t firstInstance )const;
		/**
		*\copydoc	CommandBuffer:drawIndirect
		*/
		void drawIndirect( VkBuffer buffer
			, VkDeviceSize offset
			, uint32_t drawCount
			, uint32_t stride )const;
		/**
		*\copydoc	CommandBuffer:drawIndexedIndirect
		*/
		void drawIndexedIndirect( VkBuffer buffer
			, VkDeviceSize offset
			, uint32_t drawCount
			, uint32_t stride )const;
		/**
		*\copydoc	CommandBuffer::copyToImage
		*/
		void copyToImage( VkBuffer src
			, VkImage dst
			, VkImageLayout dstLayout
			, VkBufferImageCopyArray copyInfos )const;
		/**
		*\copydoc	CommandBuffer::copyToBuffer
		*/
		void copyToBuffer( VkImage src
			, VkImageLayout srcLayout
			, VkBuffer dst
			, VkBufferImageCopyArray copyInfos )const;
		/**
		*\copydoc	CommandBuffer::copyBuffer
		*/
		void copyBuffer( VkBuffer src
			, VkBuffer dst
			, VkBufferCopyArray copyInfos )const;
		/**
		*\copydoc	CommandBuffer::copyImage
		*/
		void copyImage( VkImage src
			, VkImageLayout srcLayout
			, VkImage dst
			, VkImageLayout dstLayout
			, VkImageCopyArray copyInfos )const;
		/**
		*\copydoc	CommandBuffer::blitImage
		*/
		void blitImage( VkImage srcImage
			, VkImageLayout srcLayout
			, VkImage dstImage
			, VkImageLayout dstLayout
			, VkImageBlitArray regions
			, VkFilter filter )const;
		/**
		*\copydoc	CommandBuffer::resetQueryPool
		*/
		void resetQueryPool( VkQueryPool pool
			, uint32_t firstQuery
			, uint32_t queryCount )const;
		/**
		*\copydoc	CommandBuffer::beginQuery
		*/
		void beginQuery( VkQueryPool pool
			, uint32_t query
			, VkQueryControlFlags flags )const;
		/**
		*\copydoc	CommandBuffer::endQuery
		*/
		void endQuery( VkQueryPool pool
			, uint32_t query )const;
		/**
		*\copydoc	CommandBuffer::writeTimestamp
		*/
		void writeTimestamp( VkPipelineStageFlagBits pipelineStage
			, VkQueryPool pool
			, uint32_t query )const;
		/**
		*\copydoc	CommandBuffer:pushConstants
		*/
		void pushConstants( VkPipelineLayout layout
			, VkShaderStageFlags stageFlags
			, uint32_t offset
			, uint32_t size
			, void const * data )const;
		/**
		*\copydoc	CommandBuffer::dispatch
		*/
		void dispatch( uint32_t groupCountX
			, uint32_t groupCountY
			, uint32_t groupCountZ )const;
		/**
		*\copydoc	CommandBuffer:dispatchIndirect
		*/
		void dispatchIndirect( VkBuffer buffer
			, VkDeviceSize offset )const;
		/**
		*\copydoc	CommandBuffer::setLineWidth
		*/
		void setLineWidth( float width )const;
		/**
		*\copydoc	CommandBuffer::setDepthBias
		*/
		void setDepthBias( float constantFactor
			, float clamp
			, float slopeFactor )const;
		/**
		*\copydoc	CommandBuffer::setEvent
		*/
		void setEvent( VkEvent event
			, VkPipelineStageFlags stageMask )const;
		/**
		*\copydoc	CommandBuffer::resetEvent
		*/
		void resetEvent( VkEvent event
			, VkPipelineStageFlags stageMask )const;
		/**
		*\copydoc	CommandBuffer::waitEvents
		*/
		void waitEvents( VkEventArray events
			, VkPipelineStageFlags srcStageMask
			, VkPipelineStageFlags dstStageMask
			, VkMemoryBarrierArray memoryBarriers
			, VkBufferMemoryBarrierArray bufferMemoryBarriers
			, VkImageMemoryBarrierArray imageMemoryBarriers )const;
		/**
		*\copydoc	CommandBuffer::pipelineBarrier
		*/
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
		/**
		*\return
		*	Le tableau de commandes.
		*/
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
