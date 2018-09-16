/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Command/CommandBuffer.hpp>

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
		void begin( ashes::CommandBufferUsageFlags flags )const override;
		/**
		*\copydoc	ashes::CommandBuffer:begin
		*/
		void begin( ashes::CommandBufferUsageFlags flags
			, ashes::CommandBufferInheritanceInfo const & inheritanceInfo )const override;
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
		void beginRenderPass( ashes::RenderPass const & renderPass
			, ashes::FrameBuffer const & frameBuffer
			, ashes::ClearValueArray const & clearValues
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
		void clear( ashes::TextureView const & image
			, ashes::ClearColorValue const & colour )const override;
		/**
		*\copydoc	ashes::CommandBuffer:clear
		*/
		void clear( ashes::TextureView const & image
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
			, ashes::IndexType indexType )const override;
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
		void setViewport( ashes::Viewport const & viewport )const override;
		/**
		*\copydoc	ashes::CommandBuffer:setScissor
		*/
		void setScissor( ashes::Scissor const & scissor )const override;
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
		void copyToImage( ashes::BufferImageCopyArray const & copyInfo
			, ashes::BufferBase const & src
			, ashes::Texture const & dst )const override;
		/**
		*\copydoc	ashes::CommandBuffer::copyToBuffer
		*/
		void copyToBuffer( ashes::BufferImageCopyArray const & copyInfo
			, ashes::Texture const & src
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
			, ashes::Texture const & src
			, ashes::ImageLayout srcLayout
			, ashes::Texture const & dst
			, ashes::ImageLayout dstLayout )const override;
		/**
		*\copydoc	ashes::CommandBuffer:blitImage
		*/
		void blitImage( ashes::Texture const & srcImage
			, ashes::ImageLayout srcLayout
			, ashes::Texture const & dstImage
			, ashes::ImageLayout dstLayout
			, std::vector< ashes::ImageBlit > const & regions
			, ashes::Filter filter )const override;
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
		void writeTimestamp( ashes::PipelineStageFlag pipelineStage
			, ashes::QueryPool const & pool
			, uint32_t query )const override;
		/**
		*\copydoc	ashes::CommandBuffer:pushConstants
		*/
		void pushConstants( ashes::PipelineLayout const & layout
			, ashes::ShaderStageFlags stageFlags
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
			, ashes::PipelineStageFlags stageMask )const override;
		/**
		*\copydoc	ashes::CommandBuffer::resetEvent
		*/
		void resetEvent( ashes::Event const & event
			, ashes::PipelineStageFlags stageMask )const override;
		/**
		*\copydoc	ashes::CommandBuffer::waitEvents
		*/
		void waitEvents( ashes::EventCRefArray const & events
			, ashes::PipelineStageFlags srcStageMask
			, ashes::PipelineStageFlags dstStageMask
			, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
			, ashes::ImageMemoryBarrierArray const & imageMemoryBarriers )const override;
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
		/**
		*\copydoc	ashes::CommandBuffer:doMemoryBarrier
		*/
		void doMemoryBarrier( ashes::PipelineStageFlags after
			, ashes::PipelineStageFlags before
			, ashes::BufferMemoryBarrier const & transitionBarrier )const override;
		/**
		*\copydoc	ashes::CommandBuffer:doMemoryBarrier
		*/
		void doMemoryBarrier( ashes::PipelineStageFlags after
			, ashes::PipelineStageFlags before
			, ashes::ImageMemoryBarrier const & transitionBarrier )const override;

	private:
		Device const & m_device;
		CommandPool const & m_pool;
		mutable Pipeline const * m_currentPipeline{ nullptr };
		mutable ComputePipeline const * m_currentComputePipeline{ nullptr };
	};
}
