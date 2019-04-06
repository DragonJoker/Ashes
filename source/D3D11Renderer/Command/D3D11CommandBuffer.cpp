/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/D3D11CommandBuffer.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Command/D3D11CommandPool.hpp"
#include "Core/D3D11Device.hpp"
#include "Descriptor/D3D11DescriptorSet.hpp"
#include "Image/D3D11Image.hpp"
#include "Image/D3D11ImageView.hpp"
#include "Miscellaneous/D3D11QueryPool.hpp"
#include "Pipeline/D3D11ComputePipeline.hpp"
#include "Pipeline/D3D11Pipeline.hpp"
#include "Pipeline/D3D11PipelineLayout.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"
#include "RenderPass/D3D11RenderPass.hpp"

#include "Command/Commands/D3D11BeginQueryCommand.hpp"
#include "Command/Commands/D3D11BeginRenderPassCommand.hpp"
#include "Command/Commands/D3D11BeginSubpassCommand.hpp"
#include "Command/Commands/D3D11BindComputePipelineCommand.hpp"
#include "Command/Commands/D3D11BindDescriptorSetCommand.hpp"
#include "Command/Commands/D3D11BindIndexBufferCommand.hpp"
#include "Command/Commands/D3D11BindVertexBuffersCommand.hpp"
#include "Command/Commands/D3D11BindPipelineCommand.hpp"
#include "Command/Commands/D3D11BlitImageCommand.hpp"
#include "Command/Commands/D3D11ClearAttachmentsCommand.hpp"
#include "Command/Commands/D3D11ClearColourCommand.hpp"
#include "Command/Commands/D3D11ClearDepthStencilCommand.hpp"
#include "Command/Commands/D3D11CopyBufferCommand.hpp"
#include "Command/Commands/D3D11CopyBufferToImageCommand.hpp"
#include "Command/Commands/D3D11CopyImageCommand.hpp"
#include "Command/Commands/D3D11CopyImageToBufferCommand.hpp"
#include "Command/Commands/D3D11DispatchCommand.hpp"
#include "Command/Commands/D3D11DispatchIndirectCommand.hpp"
#include "Command/Commands/D3D11DrawCommand.hpp"
#include "Command/Commands/D3D11DrawIndexedCommand.hpp"
#include "Command/Commands/D3D11DrawIndexedIndirectCommand.hpp"
#include "Command/Commands/D3D11DrawIndirectCommand.hpp"
#include "Command/Commands/D3D11EndQueryCommand.hpp"
#include "Command/Commands/D3D11EndRenderPassCommand.hpp"
#include "Command/Commands/D3D11EndSubpassCommand.hpp"
#include "Command/Commands/D3D11ExecuteCommandsCommand.hpp"
#include "Command/Commands/D3D11GenerateMipsCommand.hpp"
#include "Command/Commands/D3D11MemoryBarrierCommand.hpp"
#include "Command/Commands/D3D11PushConstantsCommand.hpp"
#include "Command/Commands/D3D11ResetEventCommand.hpp"
#include "Command/Commands/D3D11ResetQueryPoolCommand.hpp"
#include "Command/Commands/D3D11ScissorCommand.hpp"
#include "Command/Commands/D3D11SetDepthBiasCommand.hpp"
#include "Command/Commands/D3D11SetEventCommand.hpp"
#include "Command/Commands/D3D11SetLineWidthCommand.hpp"
#include "Command/Commands/D3D11ViewportCommand.hpp"
#include "Command/Commands/D3D11WaitEventsCommand.hpp"
#include "Command/Commands/D3D11WriteTimestampCommand.hpp"

#include <Ashes/Buffer/PushConstantsBuffer.hpp>
#include <Ashes/Buffer/StagingBuffer.hpp>
#include <Ashes/Buffer/UniformBuffer.hpp>
#include <Ashes/Buffer/VertexBuffer.hpp>
#include <Ashes/RenderPass/FrameBufferAttachment.hpp>

#define AshesD3D_UseCommandsList 0

namespace d3d11_renderer
{
	namespace
	{
		CommandBufferCRefArray convert( ashes::CommandBufferCRefArray const & commands )
		{
			CommandBufferCRefArray result;

			for ( auto & command : commands )
			{
				result.emplace_back( static_cast< CommandBuffer const & >( command.get() ) );
			}

			return result;
		}

		DescriptorSetCRefArray convert( ashes::DescriptorSetCRefArray const & descriptors )
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
		: ashes::CommandBuffer{ device, pool, primary }
		, m_device{ device }
		, m_pool{ pool }
	{
#if AshesD3D_UseCommandsList

		if ( !primary )
		{
			m_device.getDevice()->CreateDeferredContext( 0u, &m_deferredContext );
		}

#endif
	}

	CommandBuffer::~CommandBuffer()
	{
#if AshesD3D_UseCommandsList

		safeRelease( m_commandList );
		safeRelease( m_deferredContext );

#endif
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

	void CommandBuffer::begin( ashes::CommandBufferBeginInfo const & info )const
	{
		m_commands.clear();
		m_afterSubmitActions.clear();
		m_state = State{};
		m_state.beginInfo = info;
	}

	void CommandBuffer::end()const
	{
		m_state.pushConstantBuffers.clear();

#if AshesD3D_UseCommandsList

		if ( m_deferredContext )
		{
			safeRelease( m_commandList );
			Context context{ m_deferredContext };
			m_deferredContext->AddRef();
			execute( context );
			m_deferredContext->FinishCommandList( TRUE, &m_commandList );
		}

#endif
	}

	void CommandBuffer::reset( ashes::CommandBufferResetFlags flags )const
	{
		m_commands.clear();
	}

	void CommandBuffer::beginRenderPass( ashes::RenderPassBeginInfo const & beginInfo
		, ashes::SubpassContents contents )const
	{
		m_state.currentRenderPass = static_cast< RenderPass const * >( beginInfo.renderPass );
		m_state.currentFrameBuffer = beginInfo.framebuffer;
		m_state.currentSubpassIndex = 0u;
		m_state.currentSubpass = &m_state.currentRenderPass->getSubpasses()[m_state.currentSubpassIndex++];
		m_state.vbos.clear();
		m_commands.emplace_back( std::make_unique< BeginRenderPassCommand >( m_device
			, *m_state.currentRenderPass
			, *m_state.currentFrameBuffer
			, beginInfo.clearValues ) );
		m_commands.emplace_back( std::make_unique< BeginSubpassCommand >( m_device
			, *m_state.currentRenderPass
			, *m_state.currentFrameBuffer
			, *m_state.currentSubpass ) );
	}

	void CommandBuffer::nextSubpass( ashes::SubpassContents contents )const
	{
		m_commands.emplace_back( std::make_unique< EndSubpassCommand >( m_device
			, *m_state.currentFrameBuffer
			, *m_state.currentSubpass ) );
		m_state.currentSubpass = &m_state.currentRenderPass->getSubpasses()[m_state.currentSubpassIndex++];
		m_commands.emplace_back( std::make_unique< BeginSubpassCommand >( m_device
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

	void CommandBuffer::executeCommands( ashes::CommandBufferCRefArray const & commands )const
	{
#if AshesD3D_UseCommandsList

		for ( auto & commandBuffer : commands )
		{
			m_commands.emplace_back( std::make_unique< ExecuteCommandsCommand >( m_device
				, commandBuffer ) );
		}

#else

		for ( auto & commandBuffer : commands )
		{
			auto & dxCommandBuffer = static_cast< CommandBuffer const & >( commandBuffer.get() );

			for ( auto & command : dxCommandBuffer.getCommands() )
			{
				m_commands.emplace_back( command->clone() );
			}

			m_afterSubmitActions.insert( m_afterSubmitActions.end()
				, dxCommandBuffer.m_afterSubmitActions.begin()
				, dxCommandBuffer.m_afterSubmitActions.end() );
		}

#endif
	}

	void CommandBuffer::clear( ashes::ImageView const & image
		, VkClearColorValue const & colour )const
	{
		m_commands.emplace_back( std::make_unique< ClearColourCommand >( m_device
			, image
			, colour ) );
	}

	void CommandBuffer::clear( ashes::ImageView const & image
		, ashes::DepthStencilClearValue const & value )const
	{
		m_commands.emplace_back( std::make_unique< ClearDepthStencilCommand >( m_device
			, image
			, value ) );
	}

	void CommandBuffer::clearAttachments( ashes::ClearAttachmentArray const & clearAttachments
		, ashes::ClearRectArray const & clearRects )
	{
		m_commands.emplace_back( std::make_unique< ClearAttachmentsCommand >( m_device
			, *m_state.currentRenderPass
			, *m_state.currentSubpass
			, static_cast< FrameBuffer const & >( *m_state.currentFrameBuffer )
			, clearAttachments
			, clearRects ) );
	}

	void CommandBuffer::bindPipeline( ashes::Pipeline const & pipeline
		, ashes::PipelineBindPoint bindingPoint )const
	{
		auto & dxpipeline = static_cast< Pipeline const & >( pipeline );

		if ( m_state.currentPipeline )
		{
			auto src = m_state.currentPipeline->getVertexInputStateHash();
			auto dst = static_cast< Pipeline const & >( pipeline ).getVertexInputStateHash();

			if ( src != dst )
			{
				m_state.vbos.clear();
			}
		}

		m_state.currentPipeline = &dxpipeline;
		m_commands.emplace_back( std::make_unique< BindPipelineCommand >( m_device
			, pipeline
			, bindingPoint ) );
		doAddAfterSubmitAction();

		for ( auto & pcb : m_state.pushConstantBuffers )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, m_state.currentPipeline->findPushConstantBuffer( pcb.second ) ) );
			doAddAfterSubmitAction();
		}

		m_state.pushConstantBuffers.clear();
	}

	void CommandBuffer::bindPipeline( ashes::ComputePipeline const & pipeline
		, ashes::PipelineBindPoint bindingPoint )const
	{
		auto & dxpipeline = static_cast< ComputePipeline const & >( pipeline );
		m_state.currentComputePipeline = &dxpipeline;
		m_commands.emplace_back( std::make_unique< BindComputePipelineCommand >( m_device
			, pipeline
			, bindingPoint ) );
		doAddAfterSubmitAction();

		for ( auto & pcb : m_state.pushConstantBuffers )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, m_state.currentPipeline->findPushConstantBuffer( pcb.second ) ) );
			doAddAfterSubmitAction();
		}

		m_state.pushConstantBuffers.clear();
	}

	void CommandBuffer::bindVertexBuffers( uint32_t firstBinding
		, ashes::BufferCRefArray const & buffers
		, ashes::UInt64Array offsets )const
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

	void CommandBuffer::bindIndexBuffer( ashes::BufferBase const & buffer
		, uint64_t offset
		, ashes::IndexType indexType )const
	{
		m_commands.emplace_back( std::make_unique< BindIndexBufferCommand >( m_device
			, static_cast< Buffer const & >( buffer )
			, offset
			, indexType ) );
		doAddAfterSubmitAction();
		m_state.indexType = indexType;
	}

	void CommandBuffer::bindDescriptorSets( ashes::DescriptorSetCRefArray const & descriptorSets
		, ashes::PipelineLayout const & layout
		, ashes::UInt32Array const & dynamicOffsets
		, ashes::PipelineBindPoint bindingPoint )const
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

	void CommandBuffer::setViewport( uint32_t firstViewport
		, ashes::VkViewportArray const & viewports )const
	{
		m_commands.emplace_back( std::make_unique< ViewportCommand >( m_device, firstViewport, viewports ) );
	}

	void CommandBuffer::setScissor( uint32_t firstScissor
		, ashes::VkScissorArray const & scissors )const
	{
		m_commands.emplace_back( std::make_unique< ScissorCommand >( m_device, firstScissor, scissors ) );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		doFillVboStrides();

		if ( !m_state.currentPipeline->hasVertexLayout() )
		{
			bindIndexBuffer( m_device.getEmptyIndexedVaoIdx(), 0u, ashes::IndexType::eUInt32 );
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
			bindIndexBuffer( m_device.getEmptyIndexedVaoIdx(), 0u, ashes::IndexType::eUInt32 );
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

	void CommandBuffer::drawIndirect( ashes::BufferBase const & buffer
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

	void CommandBuffer::drawIndexedIndirect( ashes::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		if ( !m_state.currentPipeline->hasVertexLayout() )
		{
			bindIndexBuffer( m_device.getEmptyIndexedVaoIdx(), 0u, ashes::IndexType::eUInt32 );
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

	void CommandBuffer::copyToImage( ashes::VkBufferImageCopyArray const & copyInfo
		, ashes::BufferBase const & src
		, ashes::Image const & dst )const
	{
		if ( !m_device.onCopyToImageCommand( *this, copyInfo, src, dst ) )
		{
			m_commands.emplace_back( std::make_unique< CopyBufferToImageCommand >( m_device
				, copyInfo
				, src
				, dst ) );
		}
	}

	void CommandBuffer::copyToBuffer( ashes::VkBufferImageCopyArray const & copyInfo
		, ashes::Image const & src
		, ashes::BufferBase const & dst )const
	{
		m_commands.emplace_back( std::make_unique< CopyImageToBufferCommand >( m_device
			, copyInfo
			, src
			, dst ) );
	}

	void CommandBuffer::copyBuffer( ashes::BufferCopy const & copyInfo
		, ashes::BufferBase const & src
		, ashes::BufferBase const & dst )const
	{
		m_commands.emplace_back( std::make_unique< CopyBufferCommand >( m_device
			, copyInfo
			, src
			, dst ) );
	}

	void CommandBuffer::copyImage( ashes::ImageCopy const & copyInfo
		, ashes::Image const & src
		, ashes::ImageLayout srcLayout
		, ashes::Image const & dst
		, ashes::ImageLayout dstLayout )const
	{
		m_commands.emplace_back( std::make_unique< CopyImageCommand >( m_device
			, copyInfo
			, src
			, dst ) );
	}

	void CommandBuffer::blitImage( ashes::Image const & srcImage
		, ashes::ImageLayout srcLayout
		, ashes::Image const & dstImage
		, ashes::ImageLayout dstLayout
		, std::vector< ashes::ImageBlit > const & regions
		, ashes::Filter filter )const
	{
		m_commands.emplace_back( std::make_unique< BlitImageCommand >( m_device
			, srcImage
			, dstImage
			, regions
			, filter ) );
	}

	void CommandBuffer::resetQueryPool( ashes::QueryPool const & pool
		, uint32_t firstQuery
		, uint32_t queryCount )const
	{
		m_commands.emplace_back( std::make_unique< ResetQueryPoolCommand >( m_device
			, pool
			, firstQuery
			, queryCount ) );
	}

	void CommandBuffer::beginQuery( ashes::QueryPool const & pool
		, uint32_t query
		, ashes::QueryControlFlags flags )const
	{
		m_commands.emplace_back( std::make_unique< BeginQueryCommand >( m_device
			, pool
			, query
			, flags ) );
	}

	void CommandBuffer::endQuery( ashes::QueryPool const & pool
		, uint32_t query )const
	{
		m_commands.emplace_back( std::make_unique< EndQueryCommand >( m_device
			, pool
			, query ) );
	}

	void CommandBuffer::writeTimestamp( ashes::PipelineStageFlag pipelineStage
		, ashes::QueryPool const & pool
		, uint32_t query )const
	{
		m_commands.emplace_back( std::make_unique< WriteTimestampCommand >( m_device
			, pipelineStage
			, pool
			, query ) );
	}

	void CommandBuffer::pushConstants( ashes::PipelineLayout const & layout
		, ashes::ShaderStageFlags stageFlags
		, uint32_t offset
		, uint32_t size
		, void const * data )const
	{
		PushConstantsDesc desc
		{
			stageFlags,
			offset,
			size,
			{ reinterpret_cast< uint8_t const * >( data ), reinterpret_cast< uint8_t const * >( data ) + size }
		};

		if ( m_state.currentPipeline )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, m_state.currentPipeline->findPushConstantBuffer( desc ) ) );
			doAddAfterSubmitAction();
		}
		else if ( m_state.currentComputePipeline )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, m_state.currentComputePipeline->findPushConstantBuffer( desc ) ) );
			doAddAfterSubmitAction();
		}
		else
		{
			m_state.pushConstantBuffers.emplace_back( &layout, desc );
		}
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

	void CommandBuffer::dispatchIndirect( ashes::BufferBase const & buffer
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

	void CommandBuffer::setEvent( ashes::Event const & event
		, ashes::PipelineStageFlags stageMask )const
	{
		m_commands.emplace_back( std::make_unique< SetEventCommand >( m_device
			, event
			, stageMask ) );
	}

	void CommandBuffer::resetEvent( ashes::Event const & event
		, ashes::PipelineStageFlags stageMask )const
	{
		m_commands.emplace_back( std::make_unique< ResetEventCommand >( m_device
			, event
			, stageMask ) );
	}

	void CommandBuffer::waitEvents( ashes::EventCRefArray const & events
		, ashes::PipelineStageFlags srcStageMask
		, ashes::PipelineStageFlags dstStageMask
		, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
		, ashes::VkImageMemoryBarrierArray const & imageMemoryBarriers )const
	{
		m_commands.emplace_back( std::make_unique< WaitEventsCommand >( m_device
			, events
			, srcStageMask
			, dstStageMask
			, bufferMemoryBarriers
			, imageMemoryBarriers ) );
	}

	void CommandBuffer::generateMips( Image const & texture )const
	{
		m_commands.emplace_back( std::make_unique< GenerateMipsCommand >( m_device
			, texture ) );
	}

	void CommandBuffer::pipelineBarrier( ashes::PipelineStageFlags after
		, ashes::PipelineStageFlags before
		, ashes::DependencyFlags dependencyFlags
		, ashes::MemoryBarrierArray const & memoryBarriers
		, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
		, ashes::VkImageMemoryBarrierArray const & imageMemoryBarriers )const
	{
		m_commands.emplace_back( std::make_unique< MemoryBarrierCommand >( m_device
			, after
			, before
			, memoryBarriers
			, bufferMemoryBarriers
			, imageMemoryBarriers ) );
	}

	void CommandBuffer::doFillVboStrides()const
	{
		auto & state = m_state.currentPipeline->getVertexInputState();

		if ( state.vertexBindingDescriptions.empty() )
		{
			m_state.vbos.clear();
		}

		for ( auto & binding : m_state.vbos )
		{
			auto startIndex = binding.startIndex;
			binding.strides.clear();

			for ( auto i = 0u; i < binding.buffers.size(); ++i )
			{
				auto it = std::find_if( state.vertexBindingDescriptions.begin()
					, state.vertexBindingDescriptions.end()
					, [startIndex]( ::ashes::VertexInputBindingDescription const & desc )
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
