/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "Commands/D3D11CommandBase.hpp"

#include <Command/CommandBuffer.hpp>

namespace d3d11_renderer
{
	/**
	*\~french
	*\brief
	*	Encapsulation d'un TestCommandBuffer.
	*/
	class CommandBuffer
		: public renderer::CommandBuffer
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
		void execute( Context & context )const;
		/**
		*\copydoc	renderer::CommandBuffer:begin
		*/
		void begin( renderer::CommandBufferUsageFlags flags )const override;
		/**
		*\copydoc	renderer::CommandBuffer:begin
		*/
		void begin( renderer::CommandBufferUsageFlags flags
			, renderer::CommandBufferInheritanceInfo const & inheritanceInfo )const override;
		/**
		*\copydoc	renderer::CommandBuffer:end
		*/
		void end()const override;
		/**
		*\copydoc	renderer::CommandBuffer:reset
		*/
		void reset( renderer::CommandBufferResetFlags flags )const override;
		/**
		*\copydoc	renderer::CommandBuffer:beginRenderPass
		*/
		void beginRenderPass( renderer::RenderPass const & renderPass
			, renderer::FrameBuffer const & frameBuffer
			, renderer::ClearValueArray const & clearValues
			, renderer::SubpassContents contents )const override;
		/**
		*\copydoc	renderer::CommandBuffer:nextSubpass
		*/
		void nextSubpass( renderer::SubpassContents contents )const override;
		/**
		*\copydoc	renderer::CommandBuffer:endRenderPass
		*/
		void endRenderPass()const override;
		/**
		*\copydoc	renderer::CommandBuffer:executeCommands
		*/
		void executeCommands( renderer::CommandBufferCRefArray const & commands )const override;
		/**
		*\copydoc	renderer::CommandBuffer:clear
		*/
		void clear( renderer::TextureView const & image
			, renderer::ClearColorValue const & colour )const override;
		/**
		*\copydoc	renderer::CommandBuffer:clear
		*/
		void clear( renderer::TextureView const & image
			, renderer::DepthStencilClearValue const & colour )const override;
		/**
		*\copydoc	renderer::clearAttachments:clear
		*/
		void clearAttachments( renderer::ClearAttachmentArray const & clearAttachments
			, renderer::ClearRectArray const & clearRects )override;
		/**
		*\copydoc	renderer::CommandBuffer:bindPipeline
		*/
		void bindPipeline( renderer::Pipeline const & pipeline
			, renderer::PipelineBindPoint bindingPoint )const override;
		/**
		*\copydoc	renderer::CommandBuffer:bindPipeline
		*/
		void bindPipeline( renderer::ComputePipeline const & pipeline
			, renderer::PipelineBindPoint bindingPoint )const override;
		/**
		*\copydoc	renderer::CommandBuffer:bindVertexBuffers
		*/
		void bindVertexBuffers( uint32_t firstBinding
			, renderer::BufferCRefArray const & buffers
			, renderer::UInt64Array offsets )const override;
		/**
		*\copydoc	renderer::CommandBuffer:bindIndexBuffer
		*/
		void bindIndexBuffer( renderer::BufferBase const & buffer
			, uint64_t offset
			, renderer::IndexType indexType )const override;
		/**
		*\copydoc	renderer::CommandBuffer:bindDescriptorSet
		*/
		void bindDescriptorSets( renderer::DescriptorSetCRefArray const & descriptorSets
			, renderer::PipelineLayout const & layout
			, renderer::UInt32Array const & dynamicOffsets
			, renderer::PipelineBindPoint bindingPoint )const override;
		/**
		*\copydoc	renderer::CommandBuffer:setViewport
		*/
		void setViewport( renderer::Viewport const & viewport )const override;
		/**
		*\copydoc	renderer::CommandBuffer:setScissor
		*/
		void setScissor( renderer::Scissor const & scissor )const override;
		/**
		*\copydoc	renderer::CommandBuffer:draw
		*/
		void draw( uint32_t vtxCount
			, uint32_t instCount
			, uint32_t firstVertex
			, uint32_t firstInstance )const override;
		/**
		*\copydoc	renderer::CommandBuffer:drawIndexed
		*/
		void drawIndexed( uint32_t indexCount
			, uint32_t instCount
			, uint32_t firstIndex
			, uint32_t vertexOffset
			, uint32_t firstInstance )const override;
		/**
		*\copydoc	renderer::CommandBuffer:drawIndirect
		*/
		void drawIndirect( renderer::BufferBase const & buffer
			, uint32_t offset
			, uint32_t drawCount
			, uint32_t stride )const override;
		/**
		*\copydoc	renderer::CommandBuffer:drawIndexedIndirect
		*/
		void drawIndexedIndirect( renderer::BufferBase const & buffer
			, uint32_t offset
			, uint32_t drawCount
			, uint32_t stride )const override;
		/**
		*\copydoc	renderer::CommandBuffer::copyToImage
		*/
		void copyToImage( renderer::BufferImageCopyArray const & copyInfo
			, renderer::BufferBase const & src
			, renderer::Texture const & dst )const override;
		/**
		*\copydoc	renderer::CommandBuffer::copyToBuffer
		*/
		void copyToBuffer( renderer::BufferImageCopyArray const & copyInfo
			, renderer::Texture const & src
			, renderer::BufferBase const & dst )const override;
		/**
		*\copydoc	renderer::CommandBuffer:copyBuffer
		*/
		void copyBuffer( renderer::BufferCopy const & copyInfo
			, renderer::BufferBase const & src
			, renderer::BufferBase const & dst )const override;
		/**
		*\copydoc	renderer::CommandBuffer:copyImage
		*/
		void copyImage( renderer::ImageCopy const & copyInfo
			, renderer::Texture const & src
			, renderer::ImageLayout srcLayout
			, renderer::Texture const & dst
			, renderer::ImageLayout dstLayout )const override;
		/**
		*\copydoc	renderer::CommandBuffer:blitImage
		*/
		void blitImage( renderer::Texture const & srcImage
			, renderer::ImageLayout srcLayout
			, renderer::Texture const & dstImage
			, renderer::ImageLayout dstLayout
			, std::vector< renderer::ImageBlit > const & regions
			, renderer::Filter filter )const override;
		/**
		*\copydoc	renderer::CommandBuffer:resetQueryPool
		*/
		void resetQueryPool( renderer::QueryPool const & pool
			, uint32_t firstQuery
			, uint32_t queryCount )const override;
		/**
		*\copydoc	renderer::CommandBuffer:beginQuery
		*/
		void beginQuery( renderer::QueryPool const & pool
			, uint32_t query
			, renderer::QueryControlFlags flags )const override;
		/**
		*\copydoc	renderer::CommandBuffer:endQuery
		*/
		void endQuery( renderer::QueryPool const & pool
			, uint32_t query )const override;
		/**
		*\copydoc	renderer::CommandBuffer:writeTimestamp
		*/
		void writeTimestamp( renderer::PipelineStageFlag pipelineStage
			, renderer::QueryPool const & pool
			, uint32_t query )const override;
		/**
		*\copydoc	renderer::CommandBuffer:pushConstants
		*/
		void pushConstants( renderer::PipelineLayout const & layout
			, renderer::PushConstantsBufferBase const & pcb )const override;
		/**
		*\copydoc	renderer::CommandBuffer:dispatch
		*/
		void dispatch( uint32_t groupCountX
			, uint32_t groupCountY
			, uint32_t groupCountZ )const override;
		/**
		*\copydoc	renderer::CommandBuffer:dispatchIndirect
		*/
		void dispatchIndirect( renderer::BufferBase const & buffer
			, uint32_t offset )const override;
		/**
		*\copydoc	renderer::CommandBuffer::setLineWidth
		*/
		void setLineWidth( float width )const override;
		/**
		*\copydoc	renderer::CommandBuffer::setDepthBias
		*/
		void setDepthBias( float constantFactor
			, float clamp
			, float slopeFactor )const override;
		/**
		*\copydoc	renderer::CommandBuffer::setEvent
		*/
		void setEvent( renderer::Event const & event
			, renderer::PipelineStageFlags stageMask )const override;
		/**
		*\copydoc	renderer::CommandBuffer::resetEvent
		*/
		void resetEvent( renderer::Event const & event
			, renderer::PipelineStageFlags stageMask )const override;
		/**
		*\copydoc	renderer::CommandBuffer::waitEvents
		*/
		void waitEvents( renderer::EventCRefArray const & events
			, renderer::PipelineStageFlags srcStageMask
			, renderer::PipelineStageFlags dstStageMask
			, renderer::BufferMemoryBarrierArray const & bufferMemoryBarriers
			, renderer::ImageMemoryBarrierArray const & imageMemoryBarriers )const override;
		/**
		*\copydoc	renderer::CommandBuffer::waitEvents
		*/
		void generateMips( Texture const & texture )const;

		inline CommandArray const & getCommands()const
		{
			return m_commands;
		}

		inline ID3D11CommandList * getCommandList()const
		{
			return m_commandList;
		}

	private:
		/**
		*\copydoc	renderer::CommandBuffer:doMemoryBarrier
		*/
		void doMemoryBarrier( renderer::PipelineStageFlags after
			, renderer::PipelineStageFlags before
			, renderer::BufferMemoryBarrier const & transitionBarrier )const override;
		/**
		*\copydoc	renderer::CommandBuffer:doMemoryBarrier
		*/
		void doMemoryBarrier( renderer::PipelineStageFlags after
			, renderer::PipelineStageFlags before
			, renderer::ImageMemoryBarrier const & transitionBarrier )const override;

		void doFillVboStrides()const;
		void doAddAfterSubmitAction()const;

	private:
		Device const & m_device;
		CommandPool const & m_pool;
		mutable CommandArray m_commands;
		struct State
		{
			renderer::CommandBufferUsageFlags beginFlags{ 0u };
			Pipeline const * currentPipeline{ nullptr };
			std::vector< std::pair < renderer::PipelineLayout const *, renderer::PushConstantsBufferBase const * > > pushConstantBuffers;
			ComputePipeline const * currentComputePipeline{ nullptr };
			renderer::SubpassDescription const * currentSubpass{ nullptr };
			RenderPass const * currentRenderPass{ nullptr };
			renderer::FrameBuffer const * currentFrameBuffer{ nullptr };
			uint32_t currentSubpassIndex{ 0u };
			mutable VbosBindingArray vbos;
			renderer::IndexType indexType;
		};
		mutable State m_state;
		mutable std::vector< std::function< void( Context const & ) > > m_afterSubmitActions;
		mutable ID3D11DeviceContext * m_deferredContext{ nullptr };
		mutable ID3D11CommandList * m_commandList{ nullptr };
	};
}
