/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/GlCommandBuffer.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Buffer/GlGeometryBuffers.hpp"
#include "Command/GlCommandPool.hpp"
#include "Core/GlDevice.hpp"
#include "Descriptor/GlDescriptorSet.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "Pipeline/GlComputePipeline.hpp"
#include "Pipeline/GlPipeline.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include "Command/Commands/GlBeginQueryCommand.hpp"
#include "Command/Commands/GlBeginRenderPassCommand.hpp"
#include "Command/Commands/GlBeginSubpassCommand.hpp"
#include "Command/Commands/GlBindComputePipelineCommand.hpp"
#include "Command/Commands/GlBindDescriptorSetCommand.hpp"
#include "Command/Commands/GlBindGeometryBuffersCommand.hpp"
#include "Command/Commands/GlBindPipelineCommand.hpp"
#include "Command/Commands/GlBlitImageCommand.hpp"
#include "Command/Commands/GlClearAttachmentsCommand.hpp"
#include "Command/Commands/GlClearColourCommand.hpp"
#include "Command/Commands/GlClearDepthStencilCommand.hpp"
#include "Command/Commands/GlCopyBufferCommand.hpp"
#include "Command/Commands/GlCopyBufferToImageCommand.hpp"
#include "Command/Commands/GlCopyImageCommand.hpp"
#include "Command/Commands/GlCopyImageToBufferCommand.hpp"
#include "Command/Commands/GlDispatchCommand.hpp"
#include "Command/Commands/GlDispatchIndirectCommand.hpp"
#include "Command/Commands/GlDrawCommand.hpp"
#include "Command/Commands/GlDrawIndexedCommand.hpp"
#include "Command/Commands/GlDrawIndexedIndirectCommand.hpp"
#include "Command/Commands/GlDrawIndirectCommand.hpp"
#include "Command/Commands/GlEndQueryCommand.hpp"
#include "Command/Commands/GlEndRenderPassCommand.hpp"
#include "Command/Commands/GlEndSubpassCommand.hpp"
#include "Command/Commands/GlGenerateMipmapsCommand.hpp"
#include "Command/Commands/GlMemoryBarrierCommand.hpp"
#include "Command/Commands/GlPushConstantsCommand.hpp"
#include "Command/Commands/GlResetEventCommand.hpp"
#include "Command/Commands/GlResetQueryPoolCommand.hpp"
#include "Command/Commands/GlScissorCommand.hpp"
#include "Command/Commands/GlSetDepthBiasCommand.hpp"
#include "Command/Commands/GlSetEventCommand.hpp"
#include "Command/Commands/GlSetLineWidthCommand.hpp"
#include "Command/Commands/GlViewportCommand.hpp"
#include "Command/Commands/GlWaitEventsCommand.hpp"
#include "Command/Commands/GlWriteTimestampCommand.hpp"

#include "ashesgl4_api.hpp"

#include <algorithm>

namespace ashes::gl4
{
	CommandBuffer::CommandBuffer( VkDevice device
		, VkCommandBufferLevel level )
		: m_device{ device }
		, m_level{ level }
	{
	}

	void CommandBuffer::applyPostSubmitActions( ContextLock const & context )const
	{
		for ( auto & action : m_afterSubmitActions )
		{
			action( context );
		}
	}

	VkResult CommandBuffer::begin( VkCommandBufferBeginInfo info )const
	{
		m_afterSubmitActions.clear();
		m_commands.clear();
		m_state = State{};
		m_state.beginFlags = info.flags;
		return VK_SUCCESS;
	}

	VkResult CommandBuffer::end()const
	{
		m_state.pushConstantBuffers.clear();
		return VK_SUCCESS;
	}

	VkResult CommandBuffer::reset( VkCommandBufferResetFlags flags )const
	{
		m_afterSubmitActions.clear();
		m_commands.clear();
		return VK_SUCCESS;
	}

	void CommandBuffer::beginRenderPass( VkRenderPassBeginInfo beginInfo
		, VkSubpassContents contents )const
	{
		m_state.currentRenderPass = beginInfo.renderPass;
		m_state.currentFrameBuffer = beginInfo.framebuffer;
		m_state.currentSubpassIndex = 0u;
		m_commands.emplace_back( std::make_unique< BeginRenderPassCommand >( m_device
			, std::move( beginInfo.renderPass )
			, std::move( beginInfo.framebuffer )
			, VkClearValueArray{ beginInfo.pClearValues, beginInfo.pClearValues + beginInfo.clearValueCount }
			, contents ) );
		m_state.currentSubpass = &get( m_state.currentRenderPass )->getSubpasses()[m_state.currentSubpassIndex++];
		m_commands.emplace_back( std::make_unique< BeginSubpassCommand >( m_device
			, std::move( m_state.currentRenderPass )
			, std::move( m_state.currentFrameBuffer )
			, *m_state.currentSubpass ) );
	}

	void CommandBuffer::nextSubpass( VkSubpassContents contents )const
	{
		m_commands.emplace_back( std::make_unique< EndSubpassCommand >( m_device
			, std::move( m_state.currentFrameBuffer )
			, *m_state.currentSubpass ) );
		m_state.currentSubpass = &get( m_state.currentRenderPass )->getSubpasses()[m_state.currentSubpassIndex++];
		m_commands.emplace_back( std::make_unique< BeginSubpassCommand >( m_device
			, std::move( m_state.currentRenderPass )
			, std::move( m_state.currentFrameBuffer )
			, *m_state.currentSubpass ) );
		m_state.boundVbos.clear();
	}

	void CommandBuffer::endRenderPass()const
	{
		m_commands.emplace_back( std::make_unique< EndSubpassCommand >( m_device
			, std::move( m_state.currentFrameBuffer )
			, *m_state.currentSubpass ) );
		m_commands.emplace_back( std::make_unique< EndRenderPassCommand >( m_device ) );
		m_state.boundVbos.clear();
	}

	void CommandBuffer::executeCommands( VkCommandBufferArray commands )const
	{
		for ( auto & commandBuffer : commands )
		{
			auto glCommandBuffer = get( commandBuffer );
			glCommandBuffer->initialiseGeometryBuffers();

			for ( auto & command : glCommandBuffer->getCommands() )
			{
				m_commands.emplace_back( command->clone() );
			}

			m_afterSubmitActions.insert( m_afterSubmitActions.end()
				, glCommandBuffer->m_afterSubmitActions.begin()
				, glCommandBuffer->m_afterSubmitActions.end() );
		}
	}

	void CommandBuffer::clearColorImage( VkImage image
		, VkImageLayout imageLayout
		, VkClearColorValue value
		, VkImageSubresourceRangeArray ranges )const
	{
		m_commands.emplace_back( std::make_unique< ClearColourCommand >( m_device
			, std::move( image )
			, std::move( imageLayout )
			, std::move( value )
			, std::move( ranges ) ) );
	}

	void CommandBuffer::clearDepthStencilImage( VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, VkImageSubresourceRangeArray ranges )const
	{
		m_commands.emplace_back( std::make_unique< ClearDepthStencilCommand >( m_device
			, std::move( image )
			, std::move( imageLayout )
			, std::move( value )
			, std::move( ranges ) ) );
	}

	void CommandBuffer::clearAttachments( VkClearAttachmentArray clearAttachments
		, VkClearRectArray clearRects )
	{
		m_commands.emplace_back( std::make_unique< ClearAttachmentsCommand >( m_device
			, std::move( clearAttachments )
			, std::move( clearRects ) ) );
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
					IboBinding empty{};
					m_state.boundIbo.swap( empty );
					m_state.boundVbos.clear();
				}
			}

			m_state.currentPipeline = pipeline;
			m_commands.emplace_back( std::make_unique< BindPipelineCommand >( m_device
				, std::move( pipeline )
				, bindingPoint ) );

			for ( auto & pcb : m_state.pushConstantBuffers )
			{
				m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
					, get( m_state.currentPipeline )->findPushConstantBuffer( pcb.second ) ) );
			}

			m_state.pushConstantBuffers.clear();

			m_afterSubmitActions.insert( m_afterSubmitActions.begin()
				, []( ContextLock const & context )
				{
					glLogCall( context
						, glUseProgram
						, 0u );
				} );
		}
		else if ( bindingPoint == VK_PIPELINE_BIND_POINT_COMPUTE )
		{
			m_state.currentComputePipeline = pipeline;
			m_commands.emplace_back( std::make_unique< BindComputePipelineCommand >( m_device
				, std::move( pipeline )
				, bindingPoint ) );

			for ( auto & pcb : m_state.pushConstantBuffers )
			{
				m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
					, get( m_state.currentComputePipeline )->findPushConstantBuffer( pcb.second ) ) );
			}

			m_state.pushConstantBuffers.clear();

			m_afterSubmitActions.insert( m_afterSubmitActions.begin()
				, []( ContextLock const & context )
				{
					glLogCall( context
						, glUseProgram
						, 0u );
				} );
		}
	}

	void CommandBuffer::bindVertexBuffers( uint32_t firstBinding
		, VkBufferArray buffers
		, VkDeviceSizeArray offsets )const
	{
		assert( buffers.size() == offsets.size() );
		uint32_t binding = firstBinding;

		for ( auto i = 0u; i < buffers.size(); ++i )
		{
			m_state.boundVbos[binding] = { get( buffers[i] )->getInternal(), offsets[i], buffers[i] };
			++binding;
		}

		m_state.boundVao = nullptr;
	}

	void CommandBuffer::bindIndexBuffer( VkBuffer buffer
		, VkDeviceSize offset
		, VkIndexType indexType )const
	{
		m_state.boundIbo = BufferObjectBinding{ get( buffer )->getInternal(), offset, buffer };
		m_state.indexType = indexType;
		m_state.boundVao = nullptr;
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
				, std::move( descriptorSet )
				, layout
				, dynamicOffsets
				, bindingPoint ) );
		}
	}

	void CommandBuffer::setViewport( uint32_t firstViewport
		, VkViewportArray viewports )const
	{
		m_commands.emplace_back( std::make_unique< ViewportCommand >( m_device
			, firstViewport
			, std::move( viewports ) ) );
	}

	void CommandBuffer::setScissor( uint32_t firstScissor
		, VkScissorArray scissors )const
	{
		m_commands.emplace_back( std::make_unique< ScissorCommand >( m_device
			, firstScissor
			, std::move( scissors ) ) );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		if ( !get( m_state.currentPipeline )->hasVertexLayout() )
		{
			bindIndexBuffer( get( m_device )->getEmptyIndexedVaoIdx(), 0u, VK_INDEX_TYPE_UINT32 );
			m_state.boundVao = &get( m_device )->getEmptyIndexedVao();
			m_commands.emplace_back( std::make_unique< BindGeometryBuffersCommand >( m_device
				, *m_state.boundVao ) );
			m_commands.emplace_back( std::make_unique< DrawIndexedCommand >( m_device
				, vtxCount
				, instCount
				, 0u
				, firstVertex
				, firstInstance
				, get( m_state.currentPipeline )->getInputAssemblyState().topology
				, m_state.indexType ) );
		}
		else
		{
			if ( !m_state.boundVao )
			{
				doBindVao();
			}

			m_commands.emplace_back( std::make_unique< DrawCommand >( m_device
				, vtxCount
				, instCount
				, firstVertex
				, firstInstance
				, get( m_state.currentPipeline )->getInputAssemblyState().topology ) );
		}

		m_afterSubmitActions.insert( m_afterSubmitActions.begin()
			, []( ContextLock const & context )
			{
				glLogCall( context
					, glBindVertexArray
					, 0u );
			} );
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
			m_state.boundVao = &get( m_device )->getEmptyIndexedVao();
			m_commands.emplace_back( std::make_unique< BindGeometryBuffersCommand >( m_device
				, *m_state.boundVao ) );
		}
		else if ( !m_state.boundVao )
		{
			doBindVao();
		}

		m_commands.emplace_back( std::make_unique< DrawIndexedCommand >( m_device
			, indexCount
			, instCount
			, firstIndex
			, vertexOffset
			, firstInstance
			, get( m_state.currentPipeline )->getInputAssemblyState().topology
			, m_state.indexType ) );

		m_afterSubmitActions.insert( m_afterSubmitActions.begin()
			, []( ContextLock const & context )
			{
				glLogCall( context
					, glBindVertexArray
					, 0u );
			} );
	}

	void CommandBuffer::drawIndirect( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		if ( !m_state.boundVao )
		{
			doBindVao();
		}

		m_commands.emplace_back( std::make_unique< DrawIndirectCommand >( m_device
			, buffer
			, offset
			, drawCount
			, stride
			, get( m_state.currentPipeline )->getInputAssemblyState().topology ) );

		m_afterSubmitActions.insert( m_afterSubmitActions.begin()
			, []( ContextLock const & context )
			{
				glLogCall( context
					, glBindVertexArray
					, 0u );
			} );
	}

	void CommandBuffer::drawIndexedIndirect( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		if ( !get( m_state.currentPipeline )->hasVertexLayout() )
		{
			bindIndexBuffer( get( m_device )->getEmptyIndexedVaoIdx(), 0u, VK_INDEX_TYPE_UINT32 );
			m_state.boundVao = &get( m_device )->getEmptyIndexedVao();
			m_commands.emplace_back( std::make_unique< BindGeometryBuffersCommand >( m_device
				, *m_state.boundVao ) );
		}
		else if ( !m_state.boundVao )
		{
			doBindVao();
		}

		m_commands.emplace_back( std::make_unique< DrawIndexedIndirectCommand >( m_device
			, buffer
			, offset
			, drawCount
			, stride
			, get( m_state.currentPipeline )->getInputAssemblyState().topology
			, m_state.indexType ) );

		m_afterSubmitActions.insert( m_afterSubmitActions.begin()
			, []( ContextLock const & context )
			{
				glLogCall( context
					, glBindVertexArray
					, 0u );
			} );
	}

	void CommandBuffer::copyToImage( VkBuffer src
		, VkImage dst
		, VkImageLayout dstLayout
		, VkBufferImageCopyArray copyInfos )const
	{
		for ( auto & copyInfo : copyInfos )
		{
			m_commands.emplace_back( std::make_unique< CopyBufferToImageCommand >( m_device
				, std::move( copyInfo )
				, src
				, dst ) );
		}
	}

	void CommandBuffer::copyToBuffer( VkImage src
		, VkImageLayout srcLayout
		, VkBuffer dst
		, VkBufferImageCopyArray copyInfos )const
	{
		for ( auto & copyInfo : copyInfos )
		{
			m_commands.emplace_back( std::make_unique< CopyImageToBufferCommand >( m_device
				, std::move( copyInfo )
				, src
				, dst ) );
		}
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
				, std::move( copyInfo )
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
		for ( auto & region : regions )
		{
			m_commands.emplace_back( std::make_unique< BlitImageCommand >( m_device
				, srcImage
				, dstImage
				, std::move( region )
				, filter ) );
		}
	}

	void CommandBuffer::resetQueryPool( VkQueryPool pool
		, uint32_t firstQuery
		, uint32_t queryCount )const
	{
		m_commands.emplace_back( std::make_unique< ResetQueryPoolCommand >( m_device
			, std::move( pool )
			, std::move( firstQuery )
			, std::move( queryCount ) ) );
	}

	void CommandBuffer::beginQuery( VkQueryPool pool
		, uint32_t query
		, VkQueryControlFlags flags )const
	{
		m_commands.emplace_back( std::make_unique< BeginQueryCommand >( m_device
			, std::move( pool )
			, std::move( query )
			, std::move( flags ) ) );
	}

	void CommandBuffer::endQuery( VkQueryPool pool
		, uint32_t query )const
	{
		m_commands.emplace_back( std::make_unique< EndQueryCommand >( m_device
			, std::move( pool )
			, std::move( query ) ) );
	}

	void CommandBuffer::writeTimestamp( VkPipelineStageFlagBits pipelineStage
		, VkQueryPool pool
		, uint32_t query )const
	{
		m_commands.emplace_back( std::make_unique< WriteTimestampCommand >( m_device
			, std::move( pipelineStage )
			, std::move( pool )
			, std::move( query ) ) );
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
			{},
			{ reinterpret_cast< uint8_t const * >( data ), reinterpret_cast< uint8_t const * >( data ) + size }
		};

		if ( m_state.currentPipeline )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, get( m_state.currentPipeline )->findPushConstantBuffer( desc ) ) );
		}
		else if ( m_state.currentComputePipeline )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, get( m_state.currentComputePipeline )->findPushConstantBuffer( desc ) ) );
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
			, std::move( buffer )
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
			, std::move( events )
			, srcStageMask
			, dstStageMask
			, std::move( memoryBarriers )
			, std::move( bufferMemoryBarriers )
			, std::move( imageMemoryBarriers ) ) );
	}

	void CommandBuffer::generateMipmaps( VkImage texture )
	{
		m_commands.emplace_back( std::make_unique< GenerateMipmapsCommand >( m_device
			, texture ) );
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
			, dependencyFlags
			, std::move( memoryBarriers )
			, std::move( bufferMemoryBarriers )
			, std::move( imageMemoryBarriers ) ) );
	}

	void CommandBuffer::initialiseGeometryBuffers()const
	{
		for ( auto & vao : m_state.vaos )
		{
			vao.get().initialise();
		}

		m_state.vaos.clear();
	}

	void CommandBuffer::doBindVao()const
	{
		m_state.boundVao = get( m_state.currentPipeline )->findGeometryBuffers( m_state.boundVbos, m_state.boundIbo );

		if ( !m_state.boundVao )
		{
			m_state.boundVao = &get( m_state.currentPipeline )->createGeometryBuffers( m_state.boundVbos, m_state.boundIbo, m_state.indexType ).get();
			m_state.vaos.emplace_back( *m_state.boundVao );
		}
		else if ( m_state.boundVao->getVao() == GL_INVALID_INDEX )
		{
			auto it = std::find_if( m_state.vaos.begin()
				, m_state.vaos.end()
				, [this]( GeometryBuffersRef const & lookup )
				{
					return &lookup.get() == m_state.boundVao;
				} );

			if ( it == m_state.vaos.end() )
			{
				m_state.vaos.emplace_back( *m_state.boundVao );
			}
		}

		m_commands.emplace_back( std::make_unique< BindGeometryBuffersCommand >( m_device
			, *m_state.boundVao ) );
	}
}
