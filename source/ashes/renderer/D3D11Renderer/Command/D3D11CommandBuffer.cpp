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

#include "ashesd3d11_api.hpp"

#define AshesD3D_UseCommandsList 0

namespace ashes::d3d11
{
	CommandBuffer::CommandBuffer( VkDevice device
		, VkCommandPool commandPool
		, bool primary )
		: m_device{ device }
	{
		get( commandPool )->registerCommands( get( this ) );
#if AshesD3D_UseCommandsList

		if ( !primary )
		{
			get( m_device )->getDevice()->CreateDeferredContext( 0u, &m_deferredContext );
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

	VkResult CommandBuffer::begin( VkCommandBufferBeginInfo info )const
	{
		m_commands.clear();
		m_afterSubmitActions.clear();
		m_state = State{};
		m_state.beginInfo = std::move( info );
		return VK_SUCCESS;
	}

	VkResult CommandBuffer::end()const
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

		return VK_SUCCESS;
	}

	VkResult CommandBuffer::reset( VkCommandBufferResetFlags flags )const
	{
		m_commands.clear();
		return VK_SUCCESS;
	}

	void CommandBuffer::beginRenderPass( VkRenderPassBeginInfo beginInfo
		, VkSubpassContents contents )const
	{
		m_state.currentRenderPass = beginInfo.renderPass;
		m_state.currentFrameBuffer = beginInfo.framebuffer;
		m_state.currentSubpassIndex = 0u;
		m_state.currentSubpass = &get( m_state.currentRenderPass )->getSubpasses()[m_state.currentSubpassIndex++];
		m_state.vbos.clear();
		m_commands.emplace_back( std::make_unique< BeginRenderPassCommand >( m_device
			, m_state.currentRenderPass
			, m_state.currentFrameBuffer
			, VkClearValueArray{ beginInfo.pClearValues, beginInfo.pClearValues + beginInfo.clearValueCount } ) );
		m_commands.emplace_back( std::make_unique< BeginSubpassCommand >( m_device
			, m_state.currentRenderPass
			, m_state.currentFrameBuffer
			, *m_state.currentSubpass ) );
	}

	void CommandBuffer::nextSubpass( VkSubpassContents contents )const
	{
		m_commands.emplace_back( std::make_unique< EndSubpassCommand >( m_device
			, m_state.currentFrameBuffer
			, *m_state.currentSubpass ) );
		m_state.currentSubpass = &get( m_state.currentRenderPass )->getSubpasses()[m_state.currentSubpassIndex++];
		m_commands.emplace_back( std::make_unique< BeginSubpassCommand >( m_device
			, m_state.currentRenderPass
			, m_state.currentFrameBuffer
			, *m_state.currentSubpass ) );
		m_state.vbos.clear();
	}

	void CommandBuffer::endRenderPass()const
	{
		m_commands.emplace_back( std::make_unique< EndSubpassCommand >( m_device
			, m_state.currentFrameBuffer
			, *m_state.currentSubpass ) );
		m_commands.emplace_back( std::make_unique< EndRenderPassCommand >( m_device
			, m_state.currentRenderPass
			, m_state.currentFrameBuffer ) );
		m_state.vbos.clear();
	}

	void CommandBuffer::executeCommands( VkCommandBufferArray commands )const
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
			auto & dxCommandBuffer = *get( commandBuffer );

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

	void CommandBuffer::clearColorImage( VkImage image
		, VkImageLayout imageLayout
		, VkClearColorValue colour
		, VkImageSubresourceRangeArray ranges )const
	{
		m_commands.emplace_back( std::make_unique< ClearColourCommand >( m_device
			, image
			, ranges
			, colour ) );
	}

	void CommandBuffer::clearDepthStencilImage( VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, VkImageSubresourceRangeArray ranges )const
	{
		m_commands.emplace_back( std::make_unique< ClearDepthStencilCommand >( m_device
			, image
			, ranges
			, value ) );
	}

	void CommandBuffer::clearAttachments( VkClearAttachmentArray clearAttachments
		, VkClearRectArray clearRects )
	{
		m_commands.emplace_back( std::make_unique< ClearAttachmentsCommand >( m_device
			, m_state.currentRenderPass
			, *m_state.currentSubpass
			, m_state.currentFrameBuffer
			, clearAttachments
			, clearRects ) );
	}

	void CommandBuffer::bindPipeline( VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint )const
	{
		if ( bindingPoint == VK_PIPELINE_BIND_POINT_GRAPHICS )
		{
			if ( m_state.currentPipeline )
			{
				auto src = get( m_state.currentPipeline )->getVertexInputStateHash();
				auto dst = get( pipeline )->getVertexInputStateHash();

				if ( src != dst )
				{
					m_state.vbos.clear();
				}
			}

			m_state.currentPipeline = pipeline;
			m_commands.emplace_back( std::make_unique< BindPipelineCommand >( m_device
				, pipeline
				, bindingPoint ) );
			doAddAfterSubmitAction();

			for ( auto & pcb : m_state.pushConstantBuffers )
			{
				m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
					, get( m_state.currentPipeline )->findPushConstantBuffer( pcb.second ) ) );
				doAddAfterSubmitAction();
			}

			m_state.pushConstantBuffers.clear();
		}
		else
		{
			m_state.currentComputePipeline = pipeline;
			m_commands.emplace_back( std::make_unique< BindComputePipelineCommand >( m_device
				, pipeline
				, bindingPoint ) );
			doAddAfterSubmitAction();

			for ( auto & pcb : m_state.pushConstantBuffers )
			{
				m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
					, get( m_state.currentPipeline )->findPushConstantBuffer( pcb.second ) ) );
				doAddAfterSubmitAction();
			}

			m_state.pushConstantBuffers.clear();
		}
	}

	void CommandBuffer::bindVertexBuffers( uint32_t firstBinding
		, VkBufferArray buffers
		, UInt64Array offsets )const
	{
		assert( buffers.size() == offsets.size() );
		VbosBinding binding;
		binding.startIndex = firstBinding;

		for ( auto i = 0u; i < buffers.size(); ++i )
		{
			binding.buffers.push_back( get( buffers[i] )->getBuffer() );
			binding.offsets.push_back( UINT( offsets[i] ) );
		}

		m_state.vbos.push_back( binding );
		doAddAfterSubmitAction();
	}

	void CommandBuffer::bindIndexBuffer( VkBuffer buffer
		, uint64_t offset
		, VkIndexType indexType )const
	{
		m_commands.emplace_back( std::make_unique< BindIndexBufferCommand >( m_device
			, static_cast< VkBuffer >( buffer )
			, offset
			, indexType ) );
		doAddAfterSubmitAction();
		m_state.indexType = indexType;
	}

	void CommandBuffer::bindDescriptorSets( VkPipelineBindPoint bindingPoint
		, VkPipelineLayout layout
		, uint32_t firstSet
		, VkDescriptorSetArray descriptorSets
		, UInt32Array dynamicOffsets )const
	{
		for ( auto & descriptorSet : descriptorSets )
		{
			m_commands.emplace_back( std::make_unique< BindDescriptorSetCommand >( m_device
				, descriptorSet
				, layout
				, dynamicOffsets
				, bindingPoint ) );
			doAddAfterSubmitAction();
		}
	}

	void CommandBuffer::setViewport( uint32_t firstViewport
		, VkViewportArray viewports )const
	{
		m_commands.emplace_back( std::make_unique< ViewportCommand >( m_device, firstViewport, viewports ) );
	}

	void CommandBuffer::setScissor( uint32_t firstScissor
		, VkScissorArray scissors )const
	{
		m_commands.emplace_back( std::make_unique< ScissorCommand >( m_device, firstScissor, scissors ) );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		doFillVboStrides();

		if ( !get( m_state.currentPipeline )->hasVertexLayout() )
		{
			bindIndexBuffer( get( m_device )->getEmptyIndexedVaoIdx(), 0u, VK_INDEX_TYPE_UINT32 );
			m_commands.emplace_back( std::make_unique< DrawIndexedCommand >( m_device
				, vtxCount
				, instCount
				, 0u
				, firstVertex
				, firstInstance
				, get( m_state.currentPipeline )->getInputAssemblyState().topology
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
				, get( m_state.currentPipeline )->getInputAssemblyState().topology
				, m_state.vbos ) );
		}
	}

	void CommandBuffer::drawIndexed( uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance )const
	{
		if ( !get( m_state.currentPipeline )->hasVertexLayout() )
		{
			bindIndexBuffer( get( m_device )->getEmptyIndexedVaoIdx(), 0u, VK_INDEX_TYPE_UINT32 );
		}

		doFillVboStrides();
		m_commands.emplace_back( std::make_unique< DrawIndexedCommand >( m_device
			, indexCount
			, instCount
			, firstIndex
			, vertexOffset
			, firstInstance
			, get( m_state.currentPipeline )->getInputAssemblyState().topology
			, m_state.indexType
			, m_state.vbos ) );
	}

	void CommandBuffer::drawIndirect( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		doFillVboStrides();
		m_commands.emplace_back( std::make_unique< DrawIndirectCommand >( m_device
			, buffer
			, offset
			, drawCount
			, stride
			, get( m_state.currentPipeline )->getInputAssemblyState().topology
			, m_state.vbos ) );
	}

	void CommandBuffer::drawIndexedIndirect( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		if ( !get( m_state.currentPipeline )->hasVertexLayout() )
		{
			bindIndexBuffer( get( m_device )->getEmptyIndexedVaoIdx(), 0u, VK_INDEX_TYPE_UINT32 );
		}

		doFillVboStrides();
		m_commands.emplace_back( std::make_unique< DrawIndexedIndirectCommand >( m_device
			, buffer
			, offset
			, drawCount
			, stride
			, get( m_state.currentPipeline )->getInputAssemblyState().topology
			, m_state.indexType
			, m_state.vbos ) );
	}

	void CommandBuffer::copyToImage( VkBuffer src
		, VkImage dst
		, VkImageLayout dstLayout
		, VkBufferImageCopyArray copyInfos )const
	{
		if ( !get( m_device )->onCopyToImageCommand( get( this ), copyInfos, src, dst ) )
		{
			m_commands.emplace_back( std::make_unique< CopyBufferToImageCommand >( m_device
				, std::move( copyInfos )
				, src
				, dst ) );
		}
	}

	void CommandBuffer::copyToBuffer( VkImage src
		, VkImageLayout srcLayout
		, VkBuffer dst
		, VkBufferImageCopyArray copyInfos )const
	{
		m_commands.emplace_back( std::make_unique< CopyImageToBufferCommand >( m_device
			, std::move( copyInfos )
			, src
			, dst ) );
	}

	void CommandBuffer::copyBuffer( VkBuffer src
		, VkBuffer dst
		, VkBufferCopyArray copyInfos )const
	{
		for ( auto & copyInfo : copyInfos )
		{
			m_commands.emplace_back( std::make_unique< CopyBufferCommand >( m_device
				, std::move( copyInfo )
				, src
				, dst ) );
		}
	}

	void CommandBuffer::copyImage( VkImage src
		, VkImageLayout srcLayout
		, VkImage dst
		, VkImageLayout dstLayout
		, VkImageCopyArray copyInfos )const
	{
		for ( auto & copyInfo : copyInfos )
		{
			m_commands.emplace_back( std::make_unique< CopyImageCommand >( m_device
				, copyInfo
				, src
				, dst ) );
		}
	}

	void CommandBuffer::blitImage( VkImage srcImage
		, VkImageLayout srcLayout
		, VkImage dstImage
		, VkImageLayout dstLayout
		, VkImageBlitArray regions
		, VkFilter filter )const
	{
		m_commands.emplace_back( std::make_unique< BlitImageCommand >( m_device
			, srcImage
			, dstImage
			, regions
			, filter ) );
	}

	void CommandBuffer::resetQueryPool( VkQueryPool pool
		, uint32_t firstQuery
		, uint32_t queryCount )const
	{
		m_commands.emplace_back( std::make_unique< ResetQueryPoolCommand >( m_device
			, pool
			, firstQuery
			, queryCount ) );
	}

	void CommandBuffer::beginQuery( VkQueryPool pool
		, uint32_t query
		, VkQueryControlFlags flags )const
	{
		m_commands.emplace_back( std::make_unique< BeginQueryCommand >( m_device
			, pool
			, query
			, flags ) );
	}

	void CommandBuffer::endQuery( VkQueryPool pool
		, uint32_t query )const
	{
		m_commands.emplace_back( std::make_unique< EndQueryCommand >( m_device
			, pool
			, query ) );
	}

	void CommandBuffer::writeTimestamp( VkPipelineStageFlagBits pipelineStage
		, VkQueryPool pool
		, uint32_t query )const
	{
		m_commands.emplace_back( std::make_unique< WriteTimestampCommand >( m_device
			, pipelineStage
			, pool
			, query ) );
	}

	void CommandBuffer::pushConstants( VkPipelineLayout layout
		, VkShaderStageFlags stageFlags
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
				, get( m_state.currentPipeline )->findPushConstantBuffer( desc ) ) );
			doAddAfterSubmitAction();
		}
		else if ( m_state.currentComputePipeline )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, get( m_state.currentComputePipeline )->findPushConstantBuffer( desc ) ) );
			doAddAfterSubmitAction();
		}
		else
		{
			m_state.pushConstantBuffers.emplace_back( layout, desc );
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

	void CommandBuffer::dispatchIndirect( VkBuffer buffer
		, VkDeviceSize offset )const
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

	void CommandBuffer::setEvent( VkEvent event
		, VkPipelineStageFlags stageMask )const
	{
		m_commands.emplace_back( std::make_unique< SetEventCommand >( m_device
			, event
			, stageMask ) );
	}

	void CommandBuffer::resetEvent( VkEvent event
		, VkPipelineStageFlags stageMask )const
	{
		m_commands.emplace_back( std::make_unique< ResetEventCommand >( m_device
			, event
			, stageMask ) );
	}

	void CommandBuffer::waitEvents( VkEventArray events
		, VkPipelineStageFlags srcStageMask
		, VkPipelineStageFlags dstStageMask
		, VkMemoryBarrierArray memoryBarriers
		, VkBufferMemoryBarrierArray bufferMemoryBarriers
		, VkImageMemoryBarrierArray imageMemoryBarriers )const
	{
		m_commands.emplace_back( std::make_unique< WaitEventsCommand >( m_device
			, events
			, srcStageMask
			, dstStageMask
			, bufferMemoryBarriers
			, imageMemoryBarriers ) );
	}

	void CommandBuffer::pipelineBarrier( VkPipelineStageFlags after
		, VkPipelineStageFlags before
		, VkDependencyFlags dependencyFlags
		, VkMemoryBarrierArray memoryBarriers
		, VkBufferMemoryBarrierArray bufferMemoryBarriers
		, VkImageMemoryBarrierArray imageMemoryBarriers )const
	{
		m_commands.emplace_back( std::make_unique< MemoryBarrierCommand >( m_device
			, after
			, before
			, memoryBarriers
			, bufferMemoryBarriers
			, imageMemoryBarriers ) );
	}

	void CommandBuffer::generateMipmaps( VkImage texture )const
	{
		m_commands.emplace_back( std::make_unique< GenerateMipsCommand >( m_device
			, texture ) );
	}

	void CommandBuffer::doFillVboStrides()const
	{
		auto & state = get( m_state.currentPipeline )->getVertexInputState();

		if ( !state.vertexBindingDescriptionCount )
		{
			m_state.vbos.clear();
		}

		auto bindingsEnd = state.pVertexBindingDescriptions + state.vertexBindingDescriptionCount;

		for ( auto & binding : m_state.vbos )
		{
			auto startIndex = binding.startIndex;
			binding.strides.clear();

			for ( auto i = 0u; i < binding.buffers.size(); ++i )
			{
				auto it = std::find_if( state.pVertexBindingDescriptions
					, bindingsEnd
					, [startIndex]( ::VkVertexInputBindingDescription const & desc )
					{
						return desc.binding == startIndex;
					} );

				if ( it != bindingsEnd )
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
