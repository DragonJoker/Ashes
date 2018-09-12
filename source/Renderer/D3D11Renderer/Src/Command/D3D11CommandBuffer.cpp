/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Command/D3D11CommandBuffer.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Buffer/D3D11UniformBuffer.hpp"
#include "Command/D3D11CommandPool.hpp"
#include "Core/D3D11Device.hpp"
#include "Descriptor/D3D11DescriptorSet.hpp"
#include "Image/D3D11Texture.hpp"
#include "Image/D3D11TextureView.hpp"
#include "Miscellaneous/D3D11QueryPool.hpp"
#include "Pipeline/D3D11ComputePipeline.hpp"
#include "Pipeline/D3D11Pipeline.hpp"
#include "Pipeline/D3D11PipelineLayout.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"
#include "RenderPass/D3D11RenderPass.hpp"

#include "Commands/D3D11BeginQueryCommand.hpp"
#include "Commands/D3D11BeginRenderPassCommand.hpp"
#include "Commands/D3D11BindComputePipelineCommand.hpp"
#include "Commands/D3D11BindDescriptorSetCommand.hpp"
#include "Commands/D3D11BindIndexBufferCommand.hpp"
#include "Commands/D3D11BindVertexBuffersCommand.hpp"
#include "Commands/D3D11BindPipelineCommand.hpp"
#include "Commands/D3D11BlitImageCommand.hpp"
#include "Commands/D3D11BufferMemoryBarrierCommand.hpp"
#include "Commands/D3D11ClearAttachmentsCommand.hpp"
#include "Commands/D3D11ClearColourCommand.hpp"
#include "Commands/D3D11ClearDepthStencilCommand.hpp"
#include "Commands/D3D11CopyBufferCommand.hpp"
#include "Commands/D3D11CopyBufferToImageCommand.hpp"
#include "Commands/D3D11CopyImageCommand.hpp"
#include "Commands/D3D11CopyImageToBufferCommand.hpp"
#include "Commands/D3D11DispatchCommand.hpp"
#include "Commands/D3D11DispatchIndirectCommand.hpp"
#include "Commands/D3D11DrawCommand.hpp"
#include "Commands/D3D11DrawIndexedCommand.hpp"
#include "Commands/D3D11DrawIndexedIndirectCommand.hpp"
#include "Commands/D3D11DrawIndirectCommand.hpp"
#include "Commands/D3D11EndQueryCommand.hpp"
#include "Commands/D3D11EndRenderPassCommand.hpp"
#include "Commands/D3D11EndSubpassCommand.hpp"
#include "Commands/D3D11ExecuteCommandsCommand.hpp"
#include "Commands/D3D11ImageMemoryBarrierCommand.hpp"
#include "Commands/D3D11NextSubpassCommand.hpp"
#include "Commands/D3D11PushConstantsCommand.hpp"
#include "Commands/D3D11ResetEventCommand.hpp"
#include "Commands/D3D11ResetQueryPoolCommand.hpp"
#include "Commands/D3D11ScissorCommand.hpp"
#include "Commands/D3D11SetDepthBiasCommand.hpp"
#include "Commands/D3D11SetEventCommand.hpp"
#include "Commands/D3D11SetLineWidthCommand.hpp"
#include "Commands/D3D11ViewportCommand.hpp"
#include "Commands/D3D11WaitEventsCommand.hpp"
#include "Commands/D3D11WriteTimestampCommand.hpp"

#include <Buffer/PushConstantsBuffer.hpp>
#include <Buffer/StagingBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <RenderPass/FrameBufferAttachment.hpp>

namespace d3d11_renderer
{
	namespace
	{
		CommandBufferCRefArray convert( renderer::CommandBufferCRefArray const & commands )
		{
			CommandBufferCRefArray result;

			for ( auto & command : commands )
			{
				result.emplace_back( static_cast< CommandBuffer const & >( command.get() ) );
			}

			return result;
		}

		DescriptorSetCRefArray convert( renderer::DescriptorSetCRefArray const & descriptors )
		{
			DescriptorSetCRefArray result;

			for ( auto & descriptor : descriptors )
			{
				result.emplace_back( static_cast< DescriptorSet const & >( descriptor.get() ) );
			}

			return result;
		}
	}

	CommandBuffer::CommandBuffer( Device const & device
		, CommandPool const & pool
		, bool primary )
		: renderer::CommandBuffer{ device, pool, primary }
		, m_device{ device }
		, m_pool{ pool }
	{
	}

	CommandBuffer::~CommandBuffer()
	{
	}

	void CommandBuffer::execute( Context & context )const
	{
		for ( auto & command : m_commands )
		{
			command->fillContext( context );
		}

		for ( auto & command : m_commands )
		{
			command->apply( context );
		}

		for ( auto & action : m_afterSubmitActions )
		{
			action( context );
		}
	}

	void CommandBuffer::begin( renderer::CommandBufferUsageFlags flags )const
	{
		m_commands.clear();
		m_state = State{};
		m_state.beginFlags = flags;
	}

	void CommandBuffer::begin( renderer::CommandBufferUsageFlags flags
		, renderer::CommandBufferInheritanceInfo const & inheritanceInfo )const
	{
		m_commands.clear();
		m_state = State{};
		m_state.beginFlags = flags;
	}

	void CommandBuffer::end()const
	{
		m_state.pushConstantBuffers.clear();
	}

	void CommandBuffer::reset( renderer::CommandBufferResetFlags flags )const
	{
		m_commands.clear();
	}

	void CommandBuffer::beginRenderPass( renderer::RenderPass const & renderPass
		, renderer::FrameBuffer const & frameBuffer
		, renderer::ClearValueArray const & clearValues
		, renderer::SubpassContents contents )const
	{
		m_state.currentRenderPass = &static_cast< RenderPass const & >( renderPass );
		m_state.currentFrameBuffer = &frameBuffer;
		m_state.currentSubpassIndex = 0u;
		m_state.currentSubpass = &m_state.currentRenderPass->getSubpasses()[m_state.currentSubpassIndex++];
		m_state.vbos.clear();
		m_commands.emplace_back( std::make_unique< BeginRenderPassCommand >( m_device
			, *m_state.currentRenderPass
			, *m_state.currentFrameBuffer
			, clearValues
			, contents
			, *m_state.currentSubpass ) );
	}

	void CommandBuffer::nextSubpass( renderer::SubpassContents contents )const
	{
		m_commands.emplace_back( std::make_unique< EndSubpassCommand >( m_device
			, *m_state.currentFrameBuffer
			, *m_state.currentSubpass ) );
		m_state.currentSubpass = &m_state.currentRenderPass->getSubpasses()[m_state.currentSubpassIndex++];
		m_commands.emplace_back( std::make_unique< NextSubpassCommand >( m_device
			, *m_state.currentRenderPass
			, *m_state.currentFrameBuffer
			, *m_state.currentSubpass ) );
		m_state.vbos.clear();
	}

	void CommandBuffer::endRenderPass()const
	{
		m_commands.emplace_back( std::make_unique< EndSubpassCommand >( m_device
			, *m_state.currentFrameBuffer
			, *m_state.currentSubpass ) );
		m_commands.emplace_back( std::make_unique< EndRenderPassCommand >( m_device
			, *m_state.currentRenderPass
			, *m_state.currentFrameBuffer ) );
		m_state.vbos.clear();
	}

	void CommandBuffer::executeCommands( renderer::CommandBufferCRefArray const & commands )const
	{
		for ( auto & commandBuffer : commands )
		{
			m_commands.emplace_back( std::make_unique< ExecuteCommandsCommand >( m_device
				, commandBuffer ) );
		}
	}

	void CommandBuffer::clear( renderer::TextureView const & image
		, renderer::ClearColorValue const & colour )const
	{
		m_commands.emplace_back( std::make_unique< ClearColourCommand >( m_device
			, image
			, colour ) );
	}

	void CommandBuffer::clear( renderer::TextureView const & image
		, renderer::DepthStencilClearValue const & value )const
	{
		m_commands.emplace_back( std::make_unique< ClearDepthStencilCommand >( m_device
			, image
			, value ) );
	}

	void CommandBuffer::clearAttachments( renderer::ClearAttachmentArray const & clearAttachments
		, renderer::ClearRectArray const & clearRects )
	{
		m_commands.emplace_back( std::make_unique< ClearAttachmentsCommand >( m_device
			, clearAttachments
			, clearRects ) );
	}

	void CommandBuffer::bindPipeline( renderer::Pipeline const & pipeline
		, renderer::PipelineBindPoint bindingPoint )const
	{
		auto & dxpipeline = static_cast< Pipeline const & >( pipeline );
		m_state.currentPipeline = &dxpipeline;
		m_commands.emplace_back( std::make_unique< BindPipelineCommand >( m_device
			, pipeline
			, bindingPoint ) );
		doAddAfterSubmitAction();

		for ( auto & pcb : m_state.pushConstantBuffers )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, *pcb.first
				, *pcb.second ) );
			doAddAfterSubmitAction();
		}

		for ( auto & pcb : dxpipeline.getConstantsPcbs() )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, dxpipeline.getLayout()
				, *pcb ) );
			doAddAfterSubmitAction();
		}

		m_state.pushConstantBuffers.clear();
	}

	void CommandBuffer::bindPipeline( renderer::ComputePipeline const & pipeline
		, renderer::PipelineBindPoint bindingPoint )const
	{
		m_state.currentComputePipeline = &static_cast< ComputePipeline const & >( pipeline );
		m_commands.emplace_back( std::make_unique< BindComputePipelineCommand >( m_device
			, pipeline
			, bindingPoint ) );
		doAddAfterSubmitAction();

		for ( auto & pcb : m_state.pushConstantBuffers )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, *pcb.first
				, *pcb.second ) );
			doAddAfterSubmitAction();
		}

		for ( auto & pcb : m_state.currentComputePipeline->getConstantsPcbs() )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, m_state.currentComputePipeline->getLayout()
				, *pcb ) );
			doAddAfterSubmitAction();
		}

		m_state.pushConstantBuffers.clear();
	}

	void CommandBuffer::bindVertexBuffers( uint32_t firstBinding
		, renderer::BufferCRefArray const & buffers
		, renderer::UInt64Array offsets )const
	{
		assert( buffers.size() == offsets.size() );
		VbosBinding binding;
		binding.startIndex = firstBinding;

		for ( auto i = 0u; i < buffers.size(); ++i )
		{
			binding.buffers.push_back( static_cast< Buffer const & >( buffers[i].get() ).getBuffer() );
			binding.offsets.push_back( UINT( offsets[i] ) );
		}

		m_state.vbos.push_back( binding );
		doAddAfterSubmitAction();
	}

	void CommandBuffer::bindIndexBuffer( renderer::BufferBase const & buffer
		, uint64_t offset
		, renderer::IndexType indexType )const
	{
		m_commands.emplace_back( std::make_unique< BindIndexBufferCommand >( m_device
			, static_cast< Buffer const & >( buffer )
			, offset
			, indexType ) );
		doAddAfterSubmitAction();
		m_state.indexType = indexType;
	}

	void CommandBuffer::bindDescriptorSets( renderer::DescriptorSetCRefArray const & descriptorSets
		, renderer::PipelineLayout const & layout
		, renderer::UInt32Array const & dynamicOffsets
		, renderer::PipelineBindPoint bindingPoint )const
	{
		for ( auto & descriptorSet : descriptorSets )
		{
			m_commands.emplace_back( std::make_unique< BindDescriptorSetCommand >( m_device
				, descriptorSet.get()
				, layout
				, dynamicOffsets
				, bindingPoint ) );
			doAddAfterSubmitAction();
		}
	}

	void CommandBuffer::setViewport( renderer::Viewport const & viewport )const
	{
		m_commands.emplace_back( std::make_unique< ViewportCommand >( m_device, viewport ) );
	}

	void CommandBuffer::setScissor( renderer::Scissor const & scissor )const
	{
		m_commands.emplace_back( std::make_unique< ScissorCommand >( m_device, scissor ) );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		doFillVboStrides();

		if ( !m_state.currentPipeline->hasVertexLayout() )
		{
			bindIndexBuffer( m_device.getEmptyIndexedVaoIdx(), 0u, renderer::IndexType::eUInt32 );
			m_commands.emplace_back( std::make_unique< DrawIndexedCommand >( m_device
				, vtxCount
				, instCount
				, 0u
				, firstVertex
				, firstInstance
				, m_state.currentPipeline->getInputAssemblyState().topology
				, m_state.indexType
				, m_state.vbos ) );
		}
		else
		{
			m_commands.emplace_back( std::make_unique< DrawCommand >( m_device
				, vtxCount
				, instCount
				, firstVertex
				, firstInstance
				, m_state.currentPipeline->getInputAssemblyState().topology
				, m_state.vbos ) );
		}
	}

	void CommandBuffer::drawIndexed( uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance )const
	{
		if ( !m_state.currentPipeline->hasVertexLayout() )
		{
			bindIndexBuffer( m_device.getEmptyIndexedVaoIdx(), 0u, renderer::IndexType::eUInt32 );
		}

		doFillVboStrides();
		m_commands.emplace_back( std::make_unique< DrawIndexedCommand >( m_device
			, indexCount
			, instCount
			, firstIndex
			, vertexOffset
			, firstInstance
			, m_state.currentPipeline->getInputAssemblyState().topology
			, m_state.indexType
			, m_state.vbos ) );
	}

	void CommandBuffer::drawIndirect( renderer::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		doFillVboStrides();
		m_commands.emplace_back( std::make_unique< DrawIndirectCommand >( m_device
			, buffer
			, offset
			, drawCount
			, stride
			, m_state.currentPipeline->getInputAssemblyState().topology
			, m_state.vbos ) );
	}

	void CommandBuffer::drawIndexedIndirect( renderer::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		if ( !m_state.currentPipeline->hasVertexLayout() )
		{
			bindIndexBuffer( m_device.getEmptyIndexedVaoIdx(), 0u, renderer::IndexType::eUInt32 );
		}

		doFillVboStrides();
		m_commands.emplace_back( std::make_unique< DrawIndexedIndirectCommand >( m_device
			, buffer
			, offset
			, drawCount
			, stride
			, m_state.currentPipeline->getInputAssemblyState().topology
			, m_state.indexType
			, m_state.vbos ) );
	}

	void CommandBuffer::copyToImage( renderer::BufferImageCopyArray const & copyInfo
		, renderer::BufferBase const & src
		, renderer::Texture const & dst )const
	{
		m_commands.emplace_back( std::make_unique< CopyBufferToImageCommand >( m_device
			, copyInfo
			, src
			, dst ) );
	}

	void CommandBuffer::copyToBuffer( renderer::BufferImageCopyArray const & copyInfo
		, renderer::Texture const & src
		, renderer::BufferBase const & dst )const
	{
		m_commands.emplace_back( std::make_unique< CopyImageToBufferCommand >( m_device
			, copyInfo
			, src
			, dst ) );
	}

	void CommandBuffer::copyBuffer( renderer::BufferCopy const & copyInfo
		, renderer::BufferBase const & src
		, renderer::BufferBase const & dst )const
	{
		m_commands.emplace_back( std::make_unique< CopyBufferCommand >( m_device
			, copyInfo
			, src
			, dst ) );
	}

	void CommandBuffer::copyImage( renderer::ImageCopy const & copyInfo
		, renderer::Texture const & src
		, renderer::ImageLayout srcLayout
		, renderer::Texture const & dst
		, renderer::ImageLayout dstLayout )const
	{
		m_commands.emplace_back( std::make_unique< CopyImageCommand >( m_device
			, copyInfo
			, src
			, dst ) );
	}

	void CommandBuffer::blitImage( renderer::Texture const & srcImage
		, renderer::ImageLayout srcLayout
		, renderer::Texture const & dstImage
		, renderer::ImageLayout dstLayout
		, std::vector< renderer::ImageBlit > const & regions
		, renderer::Filter filter )const
	{
		m_commands.emplace_back( std::make_unique< BlitImageCommand >( m_device
			, srcImage
			, dstImage
			, regions
			, filter ) );
	}

	void CommandBuffer::resetQueryPool( renderer::QueryPool const & pool
		, uint32_t firstQuery
		, uint32_t queryCount )const
	{
		m_commands.emplace_back( std::make_unique< ResetQueryPoolCommand >( m_device
			, pool
			, firstQuery
			, queryCount ) );
	}

	void CommandBuffer::beginQuery( renderer::QueryPool const & pool
		, uint32_t query
		, renderer::QueryControlFlags flags )const
	{
		m_commands.emplace_back( std::make_unique< BeginQueryCommand >( m_device
			, pool
			, query
			, flags ) );
	}

	void CommandBuffer::endQuery( renderer::QueryPool const & pool
		, uint32_t query )const
	{
		m_commands.emplace_back( std::make_unique< EndQueryCommand >( m_device
			, pool
			, query ) );
	}

	void CommandBuffer::writeTimestamp( renderer::PipelineStageFlag pipelineStage
		, renderer::QueryPool const & pool
		, uint32_t query )const
	{
		m_commands.emplace_back( std::make_unique< WriteTimestampCommand >( m_device
			, pipelineStage
			, pool
			, query ) );
	}

	void CommandBuffer::pushConstants( renderer::PipelineLayout const & layout
		, renderer::PushConstantsBufferBase const & pcb )const
	{
		m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
			, layout
			, pcb ) );
		doAddAfterSubmitAction();
	}

	void CommandBuffer::dispatch( uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ )const
	{
		m_commands.emplace_back( std::make_unique< DispatchCommand >( m_device
			, groupCountX
			, groupCountY
			, groupCountZ ) );
	}

	void CommandBuffer::dispatchIndirect( renderer::BufferBase const & buffer
		, uint32_t offset )const
	{
		m_commands.emplace_back( std::make_unique< DispatchIndirectCommand >( m_device
			, buffer
			, offset ) );
	}

	void CommandBuffer::setLineWidth( float width )const
	{
		m_commands.emplace_back( std::make_unique< SetLineWidthCommand >( m_device
			, width ) );
	}

	void CommandBuffer::setDepthBias( float constantFactor
		, float clamp
		, float slopeFactor )const
	{
		m_commands.emplace_back( std::make_unique< SetDepthBiasCommand >( m_device
			, constantFactor
			, clamp
			, slopeFactor ) );
	}

	void CommandBuffer::setEvent( renderer::Event const & event
		, renderer::PipelineStageFlags stageMask )const
	{
		m_commands.emplace_back( std::make_unique< SetEventCommand >( m_device
			, event
			, stageMask ) );
	}

	void CommandBuffer::resetEvent( renderer::Event const & event
		, renderer::PipelineStageFlags stageMask )const
	{
		m_commands.emplace_back( std::make_unique< ResetEventCommand >( m_device
			, event
			, stageMask ) );
	}

	void CommandBuffer::waitEvents( renderer::EventCRefArray const & events
		, renderer::PipelineStageFlags srcStageMask
		, renderer::PipelineStageFlags dstStageMask
		, renderer::BufferMemoryBarrierArray const & bufferMemoryBarriers
		, renderer::ImageMemoryBarrierArray const & imageMemoryBarriers )const
	{
		m_commands.emplace_back( std::make_unique< WaitEventsCommand >( m_device
			, events
			, srcStageMask
			, dstStageMask
			, bufferMemoryBarriers
			, imageMemoryBarriers ) );
	}

	void CommandBuffer::doMemoryBarrier( renderer::PipelineStageFlags after
		, renderer::PipelineStageFlags before
		, renderer::BufferMemoryBarrier const & transitionBarrier )const
	{
		m_commands.emplace_back( std::make_unique< BufferMemoryBarrierCommand >( m_device
			, after
			, before
			, transitionBarrier ) );
	}

	void CommandBuffer::doMemoryBarrier( renderer::PipelineStageFlags after
		, renderer::PipelineStageFlags before
		, renderer::ImageMemoryBarrier const & transitionBarrier )const
	{
		m_commands.emplace_back( std::make_unique< ImageMemoryBarrierCommand >( m_device
			, after
			, before
			, transitionBarrier ) );
	}

	void CommandBuffer::doFillVboStrides()const
	{
		auto & state = m_state.currentPipeline->getVertexInputState();

		for ( auto & binding : m_state.vbos )
		{
			auto startIndex = binding.startIndex;
			binding.strides.clear();

			for ( auto i = 0u; i < binding.buffers.size(); ++i )
			{
				auto it = std::find_if( state.vertexBindingDescriptions.begin()
					, state.vertexBindingDescriptions.end()
					, [startIndex]( ::renderer::VertexInputBindingDescription const & desc )
					{
						return desc.binding == startIndex;
					} );

				if ( it != state.vertexBindingDescriptions.end() )
				{
					binding.strides.push_back( it->stride );
				}

				++startIndex;
			}
		}
	}

	void CommandBuffer::doAddAfterSubmitAction()const
	{
		auto & command = *m_commands.back();
		m_afterSubmitActions.insert( m_afterSubmitActions.begin()
			, [&command]( Context const & context )
			{
				command.remove( context );
			} );
	}
}
