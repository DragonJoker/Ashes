/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/Command/CommandBuffer.hpp>

namespace test_renderer
{
	/**
	*\~french
	*\brief
	*	Encapsulation d'un TestCommandBuffer.
	*/
	class CommandBuffer
		: public ashes::CommandBuffer
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] pool
		*	Le pool de tampons de commandes.
		*\param[in] primary
		*	Dit si le tampon est un tampon de commandes primaire (\p true) ou secondaire (\p false).
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] pool
		*	The pool from which the command buffer is allocated.
		*\param[in] primary
		*	Specifies if the command buffer is primary (\p true) or secondary (\p false).
		*/
		CommandBuffer( Device const & device
			, CommandPool const & pool
			, bool primary );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~CommandBuffer();
		/**
		*\copydoc	ashes::CommandBuffer:begin
		*/
		void begin( ashes::CommandBufferBeginInfo const & info )const override;
		/**
		*\copydoc	ashes::CommandBuffer:end
		*/
		void end()const override;
		/**
		*\copydoc	ashes::CommandBuffer:reset
		*/
		void reset( ashes::CommandBufferResetFlags flags )const override;
		/**
		*\copydoc	ashes::CommandBuffer:beginRenderPass
		*/
		void beginRenderPass( ashes::RenderPassBeginInfo const & beginInfo
			, ashes::SubpassContents contents )const override;
		/**
		*\copydoc	ashes::CommandBuffer:nextSubpass
		*/
		void nextSubpass( ashes::SubpassContents contents )const override;
		/**
		*\copydoc	ashes::CommandBuffer:endRenderPass
		*/
		void endRenderPass()const override;
		/**
		*\copydoc	ashes::CommandBuffer:executeCommands
		*/
		void executeCommands( ashes::CommandBufferCRefArray const & commands )const override;
		/**
		*\copydoc	ashes::CommandBuffer:clear
		*/
		void clear( ashes::ImageView const & image
			, VkClearColorValue const & colour )const override;
		/**
		*\copydoc	ashes::CommandBuffer:clear
		*/
		void clear( ashes::ImageView const & image
			, ashes::DepthStencilClearValue const & colour )const override;
		/**
		*\copydoc	ashes::clearAttachments:clear
		*/
		void clearAttachments( ashes::ClearAttachmentArray const & clearAttachments
			, ashes::ClearRectArray const & clearRects )override;
		/**
		*\copydoc	ashes::CommandBuffer:bindPipeline
		*/
		void bindPipeline( ashes::Pipeline const & pipeline
			, ashes::PipelineBindPoint bindingPoint )const override;
		/**
		*\copydoc	ashes::CommandBuffer:bindPipeline
		*/
		void bindPipeline( ashes::ComputePipeline const & pipeline
			, ashes::PipelineBindPoint bindingPoint )const override;
		/**
		*\copydoc	ashes::CommandBuffer:bindVertexBuffers
		*/
		void bindVertexBuffers( uint32_t firstBinding
			, ashes::BufferCRefArray const & buffers
			, ashes::UInt64Array offsets )const override;
		/**
		*\copydoc	ashes::CommandBuffer:bindIndexBuffer
		*/
		void bindIndexBuffer( ashes::BufferBase const & buffer
			, uint64_t offset
			, VkIndexType indexType )const override;
		/**
		*\copydoc	ashes::CommandBuffer:bindDescriptorSet
		*/
		void bindDescriptorSets( ashes::DescriptorSetCRefArray const & descriptorSets
			, ashes::PipelineLayout const & layout
			, ashes::UInt32Array const & dynamicOffsets
			, ashes::PipelineBindPoint bindingPoint )const override;
		/**
		*\copydoc	ashes::CommandBuffer:setViewport
		*/
		void setViewport( uint32_t firstViewport
			, ashes::VkViewportArray const & viewports )const override;
		/**
		*\copydoc	ashes::CommandBuffer:setScissor
		*/
		void setScissor( uint32_t firstScissor
			, ashes::VkScissorArray const & scissors )const override;
		/**
		*\copydoc	ashes::CommandBuffer:draw
		*/
		void draw( uint32_t vtxCount
			, uint32_t instCount
			, uint32_t firstVertex
			, uint32_t firstInstance )const override;
		/**
		*\copydoc	ashes::CommandBuffer:drawIndexed
		*/
		void drawIndexed( uint32_t indexCount
			, uint32_t instCount
			, uint32_t firstIndex
			, uint32_t vertexOffset
			, uint32_t firstInstance )const override;
		/**
		*\copydoc	ashes::CommandBuffer:drawIndirect
		*/
		void drawIndirect( ashes::BufferBase const & buffer
			, uint32_t offset
			, uint32_t drawCount
			, uint32_t stride )const override;
		/**
		*\copydoc	ashes::CommandBuffer:drawIndexedIndirect
		*/
		void drawIndexedIndirect( ashes::BufferBase const & buffer
			, uint32_t offset
			, uint32_t drawCount
			, uint32_t stride )const override;
		/**
		*\copydoc	ashes::CommandBuffer::copyToImage
		*/
		void copyToImage( ashes::VkBufferImageCopyArray const & copyInfo
			, ashes::BufferBase const & src
			, ashes::Image const & dst )const override;
		/**
		*\copydoc	ashes::CommandBuffer::copyToBuffer
		*/
		void copyToBuffer( ashes::VkBufferImageCopyArray const & copyInfo
			, ashes::Image const & src
			, ashes::BufferBase const & dst )const override;
		/**
		*\copydoc	ashes::CommandBuffer:copyBuffer
		*/
		void copyBuffer( ashes::BufferCopy const & copyInfo
			, ashes::BufferBase const & src
			, ashes::BufferBase const & dst )const override;
		/**
		*\copydoc	ashes::CommandBuffer:copyImage
		*/
		void copyImage( ashes::ImageCopy const & copyInfo
			, ashes::Image const & src
			, VkImageLayout srcLayout
			, ashes::Image const & dst
			, VkImageLayout dstLayout )const override;
		/**
		*\copydoc	ashes::CommandBuffer:blitImage
		*/
		void blitImage( ashes::Image const & srcImage
			, VkImageLayout srcLayout
			, ashes::Image const & dstImage
			, VkImageLayout dstLayout
			, std::vector< ashes::ImageBlit > const & regions
			, VkFilter filter )const override;
		/**
		*\copydoc	ashes::CommandBuffer:resetQueryPool
		*/
		void resetQueryPool( ashes::QueryPool const & pool
			, uint32_t firstQuery
			, uint32_t queryCount )const override;
		/**
		*\copydoc	ashes::CommandBuffer:beginQuery
		*/
		void beginQuery( ashes::QueryPool const & pool
			, uint32_t query
			, ashes::QueryControlFlags flags )const override;
		/**
		*\copydoc	ashes::CommandBuffer:endQuery
		*/
		void endQuery( ashes::QueryPool const & pool
			, uint32_t query )const override;
		/**
		*\copydoc	ashes::CommandBuffer:writeTimestamp
		*/
		void writeTimestamp( VkPipelineStageFlagBits pipelineStage
			, ashes::QueryPool const & pool
			, uint32_t query )const override;
		/**
		*\copydoc	ashes::CommandBuffer:pushConstants
		*/
		void pushConstants( ashes::PipelineLayout const & layout
			, VkShaderStageFlags stageFlags
			, uint32_t offset
			, uint32_t size
			, void const * data )const override;
		/**
		*\copydoc	ashes::CommandBuffer:dispatch
		*/
		void dispatch( uint32_t groupCountX
			, uint32_t groupCountY
			, uint32_t groupCountZ )const override;
		/**
		*\copydoc	ashes::CommandBuffer:dispatchIndirect
		*/
		void dispatchIndirect( ashes::BufferBase const & buffer
			, uint32_t offset )const override;
		/**
		*\copydoc	ashes::CommandBuffer::setLineWidth
		*/
		void setLineWidth( float width )const override;
		/**
		*\copydoc	ashes::CommandBuffer::setDepthBias
		*/
		void setDepthBias( float constantFactor
			, float clamp
			, float slopeFactor )const override;
		/**
		*\copydoc	ashes::CommandBuffer::setEvent
		*/
		void setEvent( ashes::Event const & event
			, VkPipelineStageFlags stageMask )const override;
		/**
		*\copydoc	ashes::CommandBuffer::resetEvent
		*/
		void resetEvent( ashes::Event const & event
			, VkPipelineStageFlags stageMask )const override;
		/**
		*\copydoc	ashes::CommandBuffer::waitEvents
		*/
		void waitEvents( ashes::EventCRefArray const & events
			, VkPipelineStageFlags srcStageMask
			, VkPipelineStageFlags dstStageMask
			, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
			, ashes::VkImageMemoryBarrierArray const & imageMemoryBarriers )const override;
		/**
		*\copydoc	ashes::CommandBuffer::pipelineBarrier
		*/
		void pipelineBarrier( VkPipelineStageFlags after
			, VkPipelineStageFlags before
			, ashes::DependencyFlags dependencyFlags
			, ashes::MemoryBarrierArray const & memoryBarriers
			, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
			, ashes::VkImageMemoryBarrierArray const & imageMemoryBarriers )const;
		/**
		*\~french
		*\return
		*	Le pipeline actuellement actif.
		*\~english
		*\return
		*	The currently active pipeline.
		*/
		inline Pipeline const & getCurrentPipeline()const
		{
			assert( m_currentPipeline && "No pipeline bound." );
			return *m_currentPipeline;
		}
		/**
		*\~french
		*\return
		*	Dit si un pipeline est actif.
		*\~english
		*\return
		*	Tells if a pipeline is active.
		*/
		inline bool isPipelineBound()const
		{
			return m_currentPipeline != nullptr;
		}

	private:
		Device const & m_device;
		CommandPool const & m_pool;
		mutable Pipeline const * m_currentPipeline{ nullptr };
		mutable ComputePipeline const * m_currentComputePipeline{ nullptr };
	};
}
