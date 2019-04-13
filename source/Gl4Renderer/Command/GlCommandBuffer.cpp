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

#include <AshesRenderer/Helper/VertexInputState.hpp>

#include <algorithm>
#include <numeric>

namespace ashes
{
	inline VkClearValue deepCopy( VkClearValue const & rhs )
	{
		return rhs;
	}
}

namespace ashes::gl4
{
	CommandBuffer::CommandBuffer( VkDevice device
		, VkCommandBufferLevel level )
		: m_device{ device }
		, m_level{ level }
	{
	}

	VkResult CommandBuffer::begin( VkCommandBufferBeginInfo info )const
	{
		m_cmdList.clear();
		m_cmds.clear();
		m_cmdAfterSubmit.clear();
		m_cmdsAfterSubmit.clear();
		m_state = State{};
		m_state.beginFlags = info.flags;
		return VK_SUCCESS;
	}

	VkResult CommandBuffer::end()const
	{
		m_state.pushConstantBuffers.clear();
		size_t totalSize = size_t( std::accumulate( m_cmdList.begin()
			, m_cmdList.end()
			, size_t( 0u )
			, []( size_t value, CmdBuffer const & lookup )
			{
				return value + lookup.size();
			} ) );

		if ( totalSize )
		{
			m_cmds.resize( totalSize );
			auto it = m_cmds.begin();

			for ( auto & cmd : m_cmdList )
			{
				std::copy( cmd.begin()
					, cmd.end()
					, it );
				it += cmd.size();
			}
		}

		return VK_SUCCESS;
	}

	VkResult CommandBuffer::reset( VkCommandBufferResetFlags flags )const
	{
		m_cmdList.clear();
		m_cmds.clear();
		m_cmdAfterSubmit.clear();
		m_cmdsAfterSubmit.clear();
		return VK_SUCCESS;
	}

	void CommandBuffer::beginRenderPass( VkRenderPassBeginInfo beginInfo
		, VkSubpassContents contents )const
	{
		m_state.currentRenderPass = beginInfo.renderPass;
		m_state.currentFrameBuffer = beginInfo.framebuffer;
		m_state.currentSubpassIndex = 0u;
		buildBeginRenderPassCommand( m_state.currentRenderPass
			, m_state.currentFrameBuffer
			, makeVector( beginInfo.pClearValues, beginInfo.clearValueCount )
			, contents
			, m_cmdList );
		m_state.currentSubpass = &get( m_state.currentRenderPass )->getSubpasses()[m_state.currentSubpassIndex++];
		buildBeginSubpassCommand( m_state.currentRenderPass
			, m_state.currentFrameBuffer
			, *m_state.currentSubpass
			, m_cmdList );
	}

	void CommandBuffer::nextSubpass( VkSubpassContents contents )const
	{
		buildEndSubpassCommand( m_state.currentFrameBuffer
			, *m_state.currentSubpass
			, m_cmdList );
		m_state.currentSubpass = &get( m_state.currentRenderPass )->getSubpasses()[m_state.currentSubpassIndex++];
		buildBeginSubpassCommand( m_state.currentRenderPass
			, m_state.currentFrameBuffer
			, *m_state.currentSubpass
			, m_cmdList );
		m_state.boundVbos.clear();
	}

	void CommandBuffer::endRenderPass()const
	{
		buildEndSubpassCommand( m_state.currentFrameBuffer
			, *m_state.currentSubpass
			, m_cmdList );
		buildEndRenderPassCommand( m_cmdList );
		m_state.boundVbos.clear();
	}

	void CommandBuffer::executeCommands( VkCommandBufferArray commands )const
	{
		for ( auto & commandBuffer : commands )
		{
			auto glCommandBuffer = get( commandBuffer );
			glCommandBuffer->initialiseGeometryBuffers();
			m_cmdList.insert( m_cmdList.end()
				, glCommandBuffer->m_cmdList.begin()
				, glCommandBuffer->m_cmdList.end() );
			m_cmdAfterSubmit.insert( m_cmdAfterSubmit.end()
				, glCommandBuffer->m_cmdAfterSubmit.begin()
				, glCommandBuffer->m_cmdAfterSubmit.end() );
		}
	}

	void CommandBuffer::clearColorImage( VkImage image
		, VkImageLayout imageLayout
		, VkClearColorValue value
		, VkImageSubresourceRangeArray ranges )const
	{
		buildClearColourCommand( m_device
			, image
			, imageLayout
			, std::move( value )
			, std::move( ranges )
			, m_cmdList );
	}

	void CommandBuffer::clearDepthStencilImage( VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, VkImageSubresourceRangeArray ranges )const
	{
		buildClearDepthStencilCommand( m_device
			, image
			, imageLayout
			, std::move( value )
			, std::move( ranges )
			, m_cmdList );
	}

	void CommandBuffer::clearAttachments( VkClearAttachmentArray clearAttachments
		, VkClearRectArray clearRects )
	{
		buildClearAttachmentsCommand( std::move( clearAttachments )
			, std::move( clearRects )
			, m_cmdList );
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
			assert( m_state.currentFrameBuffer
				&& "Binding a graphics pipeline without framebuffer." );
			buildBindPipelineCommand( m_device
				, pipeline
				, bindingPoint
				, m_cmdList
				, !get( m_state.currentFrameBuffer )->hasSwapchainImage() );

			for ( auto & pcb : m_state.pushConstantBuffers )
			{
				buildPushConstantsCommand( get( m_state.currentPipeline )->findPushConstantBuffer( pcb.second )
					, m_cmdList );
			}

			m_state.pushConstantBuffers.clear();

			m_cmdAfterSubmit.insert( m_cmdAfterSubmit.begin()
				, makeCmd< OpType::eUseProgram >( 0u ) );
		}
		else if ( bindingPoint == VK_PIPELINE_BIND_POINT_COMPUTE )
		{
			m_state.currentComputePipeline = pipeline;
			buildBindComputePipelineCommand( pipeline
				, bindingPoint
				, m_cmdList );

			for ( auto & pcb : m_state.pushConstantBuffers )
			{
				buildPushConstantsCommand( get( m_state.currentComputePipeline )->findPushConstantBuffer( pcb.second )
					, m_cmdList );
			}

			m_state.pushConstantBuffers.clear();

			m_cmdAfterSubmit.insert( m_cmdAfterSubmit.begin()
				, makeCmd< OpType::eUseProgram >( 0u ) );
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
			buildBindDescriptorSetCommand( descriptorSet
				, layout
				, dynamicOffsets
				, bindingPoint
				, m_cmdList );
		}
	}

	void CommandBuffer::setViewport( uint32_t firstViewport
		, VkViewportArray viewports )const
	{
		m_cmdList.push_back( makeCmd< OpType::eApplyViewport >( viewports[firstViewport] ) );
	}

	void CommandBuffer::setScissor( uint32_t firstScissor
		, VkScissorArray scissors )const
	{
		m_cmdList.push_back( makeCmd< OpType::eApplyScissor >( scissors[firstScissor] ) );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		if ( isEmpty( get( m_state.currentPipeline )->getVertexInputState() ) )
		{
			bindIndexBuffer( get( m_device )->getEmptyIndexedVaoIdx(), 0u, VK_INDEX_TYPE_UINT32 );
			m_state.boundVao = &get( m_device )->getEmptyIndexedVao();
			buildBindGeometryBuffersCommand( *m_state.boundVao
				, m_cmdList );
			buildDrawIndexedCommand( vtxCount
				, instCount
				, 0u
				, firstVertex
				, firstInstance
				, get( m_state.currentPipeline )->getInputAssemblyState().topology
				, m_state.indexType
				, m_cmdList );
		}
		else
		{
			if ( !m_state.boundVao )
			{
				doBindVao();
			}

			buildDrawCommand( vtxCount
				, instCount
				, firstVertex
				, firstInstance
				, get( m_state.currentPipeline )->getInputAssemblyState().topology
				, m_cmdList );
		}

		m_cmdAfterSubmit.insert( m_cmdAfterSubmit.begin()
			, makeCmd< OpType::eBindVextexArray >( nullptr ) );
	}

	void CommandBuffer::drawIndexed( uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance )const
	{
		if ( isEmpty( get( m_state.currentPipeline )->getVertexInputState() ) )
		{
			bindIndexBuffer( get( m_device )->getEmptyIndexedVaoIdx(), 0u, VK_INDEX_TYPE_UINT32 );
			m_state.boundVao = &get( m_device )->getEmptyIndexedVao();
			buildBindGeometryBuffersCommand( *m_state.boundVao
				, m_cmdList );
		}
		else if ( !m_state.boundVao )
		{
			doBindVao();
		}

		buildDrawIndexedCommand( indexCount
			, instCount
			, firstIndex
			, vertexOffset
			, firstInstance
			, get( m_state.currentPipeline )->getInputAssemblyState().topology
			, m_state.indexType
			, m_cmdList );

		m_cmdAfterSubmit.insert( m_cmdAfterSubmit.begin()
			, makeCmd< OpType::eBindVextexArray >( nullptr ) );
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

		buildDrawIndirectCommand( buffer
			, offset
			, drawCount
			, stride
			, get( m_state.currentPipeline )->getInputAssemblyState().topology
			, m_cmdList );

		m_cmdAfterSubmit.insert( m_cmdAfterSubmit.begin()
			, makeCmd< OpType::eBindVextexArray >( nullptr ) );
	}

	void CommandBuffer::drawIndexedIndirect( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		if ( isEmpty( get( m_state.currentPipeline )->getVertexInputState() ) )
		{
			bindIndexBuffer( get( m_device )->getEmptyIndexedVaoIdx(), 0u, VK_INDEX_TYPE_UINT32 );
			m_state.boundVao = &get( m_device )->getEmptyIndexedVao();
			buildBindGeometryBuffersCommand( *m_state.boundVao
				, m_cmdList );
		}
		else if ( !m_state.boundVao )
		{
			doBindVao();
		}

		buildDrawIndexedIndirectCommand( buffer
			, offset
			, drawCount
			, stride
			, get( m_state.currentPipeline )->getInputAssemblyState().topology
			, m_state.indexType
			, m_cmdList );

		m_cmdAfterSubmit.insert( m_cmdAfterSubmit.begin()
			, makeCmd< OpType::eBindVextexArray >( nullptr ) );
	}

	void CommandBuffer::copyToImage( VkBuffer src
		, VkImage dst
		, VkImageLayout dstLayout
		, VkBufferImageCopyArray copyInfos )const
	{
		for ( auto & copyInfo : copyInfos )
		{
			buildCopyBufferToImageCommand( std::move( copyInfo )
				, src
				, dst
				, m_cmdList );
		}
	}

	void CommandBuffer::copyToBuffer( VkImage src
		, VkImageLayout srcLayout
		, VkBuffer dst
		, VkBufferImageCopyArray copyInfos )const
	{
		for ( auto & copyInfo : copyInfos )
		{
			buildCopyImageToBufferCommand( m_device
				, std::move( copyInfo )
				, src
				, dst
				, m_cmdList );
		}
	}

	void CommandBuffer::copyBuffer( VkBuffer src
		, VkBuffer dst
		, VkBufferCopyArray copyInfos )const
	{
		for ( auto & copyInfo : copyInfos )
		{
			buildCopyBufferCommand( std::move( copyInfo )
				, src
				, dst
				, m_cmdList );
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
			buildCopyImageCommand( std::move( copyInfo )
				, src
				, dst
				, m_cmdList );
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
			buildBlitImageCommand( m_device
				, srcImage
				, dstImage
				, std::move( region )
				, filter
				, m_cmdList );
		}
	}

	void CommandBuffer::resetQueryPool( VkQueryPool pool
		, uint32_t firstQuery
		, uint32_t queryCount )const
	{
		buildResetQueryPoolCommand( pool
			, firstQuery
			, queryCount
			, m_cmdList );
	}

	void CommandBuffer::beginQuery( VkQueryPool pool
		, uint32_t query
		, VkQueryControlFlags flags )const
	{
		buildBeginQueryCommand( pool
			, query
			, m_cmdList );
	}

	void CommandBuffer::endQuery( VkQueryPool pool
		, uint32_t query )const
	{
		buildEndQueryCommand( pool
			, query
			, m_cmdList );
	}

	void CommandBuffer::writeTimestamp( VkPipelineStageFlagBits pipelineStage
		, VkQueryPool pool
		, uint32_t query )const
	{
		buildWriteTimestampCommand( pipelineStage
			, pool
			, query
			, m_cmdList );
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
			buildPushConstantsCommand( get( m_state.currentPipeline )->findPushConstantBuffer( desc )
				, m_cmdList );
		}
		else if ( m_state.currentComputePipeline )
		{
			buildPushConstantsCommand( get( m_state.currentComputePipeline )->findPushConstantBuffer( desc )
				, m_cmdList );
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
		buildDispatchCommand( groupCountX
			, groupCountY
			, groupCountZ
			, m_cmdList );
	}

	void CommandBuffer::dispatchIndirect( VkBuffer buffer
		, VkDeviceSize offset )const
	{
		buildDispatchIndirectCommand( buffer
			, offset
			, m_cmdList );
	}

	void CommandBuffer::setLineWidth( float width )const
	{
		buildSetLineWidthCommand( width
			, m_cmdList );
	}

	void CommandBuffer::setDepthBias( float constantFactor
		, float clamp
		, float slopeFactor )const
	{
		buildSetDepthBiasCommand( constantFactor
			, clamp
			, slopeFactor
			, m_cmdList );
	}

	void CommandBuffer::setEvent( VkEvent event
		, VkPipelineStageFlags stageMask )const
	{
		buildSetEventCommand( event
			, stageMask
			, m_cmdList );
	}

	void CommandBuffer::resetEvent( VkEvent event
		, VkPipelineStageFlags stageMask )const
	{
		buildResetEventCommand( event
			, stageMask
			, m_cmdList );
	}

	void CommandBuffer::waitEvents( VkEventArray events
		, VkPipelineStageFlags srcStageMask
		, VkPipelineStageFlags dstStageMask
		, VkMemoryBarrierArray memoryBarriers
		, VkBufferMemoryBarrierArray bufferMemoryBarriers
		, VkImageMemoryBarrierArray imageMemoryBarriers )const
	{
		buildWaitEventsCommand( std::move( events )
			, srcStageMask
			, dstStageMask
			, std::move( memoryBarriers )
			, std::move( bufferMemoryBarriers )
			, std::move( imageMemoryBarriers )
			, m_cmdList );
	}

	void CommandBuffer::generateMipmaps( VkImage texture )
	{
		buildGenerateMipmapsCommand( texture
			, m_cmdList );
	}

	void CommandBuffer::pipelineBarrier( VkPipelineStageFlags after
		, VkPipelineStageFlags before
		, VkDependencyFlags dependencyFlags
		, VkMemoryBarrierArray memoryBarriers
		, VkBufferMemoryBarrierArray bufferMemoryBarriers
		, VkImageMemoryBarrierArray imageMemoryBarriers )const
	{
		buildMemoryBarrierCommand( after
			, before
			, dependencyFlags
			, std::move( memoryBarriers )
			, std::move( bufferMemoryBarriers )
			, std::move( imageMemoryBarriers )
			, m_cmdList );
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

		buildBindGeometryBuffersCommand( *m_state.boundVao
			, m_cmdList );
	}
}
