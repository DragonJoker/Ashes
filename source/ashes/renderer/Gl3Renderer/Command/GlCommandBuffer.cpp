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
#include "Command/Commands/GlClearColourFboCommand.hpp"
#include "Command/Commands/GlClearDepthStencilCommand.hpp"
#include "Command/Commands/GlClearDepthStencilFboCommand.hpp"
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

#include "ashesgl3_api.hpp"

#include <renderer/RendererCommon/Helper/VertexInputState.hpp>

#include <algorithm>
#include <numeric>

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl3
{
	namespace
	{
		void mergeList( CmdList const & list
			, CmdBuffer & cmds )
		{
			size_t totalSize = size_t( std::accumulate( list.begin()
				, list.end()
				, size_t( 0u )
				, []( size_t value, CmdBuffer const & lookup )
				{
					return value + lookup.size();
				} ) );

			if ( totalSize )
			{
				cmds.resize( totalSize );
				auto it = cmds.begin();

				for ( auto & cmd : list )
				{
					std::copy( cmd.begin()
						, cmd.end()
						, it );
					it += cmd.size();
				}
			}
		}
	}

	CommandBuffer::CommandBuffer( VkDevice device
		, VkCommandBufferLevel level )
		: m_device{ device }
		, m_level{ level }
	{
	}

	CommandBuffer::~CommandBuffer()
	{
		doReset();
	}

	VkResult CommandBuffer::begin( VkCommandBufferBeginInfo info )const
	{
		doReset();
		m_state = State{};
		m_state.stack = std::make_unique< ContextStateStack >( m_device );
		m_state.beginFlags = info.flags;

		if ( info.pInheritanceInfo )
		{
			// Fake a bound framebuffer here : the one in the inheritance info.
			m_state.stack->setCurrentFramebuffer( info.pInheritanceInfo->framebuffer );
			m_state.currentFrameBuffer = info.pInheritanceInfo->framebuffer;
			m_state.currentRenderPass = info.pInheritanceInfo->renderPass;
			m_state.currentSubpassIndex = info.pInheritanceInfo->subpass;
		}

		return VK_SUCCESS;
	}

	VkResult CommandBuffer::end()const
	{
		m_state.pushConstantBuffers.clear();
		mergeList( m_cmdList, m_cmds );
		mergeList( m_cmdAfterSubmit, m_cmdsAfterSubmit );
		return VK_SUCCESS;
	}

	VkResult CommandBuffer::reset( VkCommandBufferResetFlags flags )const
	{
		doReset();
		return VK_SUCCESS;
	}

	void CommandBuffer::beginRenderPass( VkRenderPassBeginInfo beginInfo
		, VkSubpassContents contents )const
	{
		m_state.currentRenderPass = beginInfo.renderPass;
		m_state.currentFrameBuffer = beginInfo.framebuffer;
		m_state.currentSubpassIndex = 0u;
		buildBeginRenderPassCommand( *m_state.stack
			, m_state.currentRenderPass
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
		buildEndSubpassCommand( m_device
			, m_state.currentFrameBuffer
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
		buildEndSubpassCommand( m_device
			, m_state.currentFrameBuffer
			, *m_state.currentSubpass
			, m_cmdList );
		buildEndRenderPassCommand( *m_state.stack
			, m_cmdList );
		m_state.boundVbos.clear();
	}

	void CommandBuffer::executeCommands( VkCommandBufferArray commands )const
	{
		for ( auto & commandBuffer : commands )
		{
			auto glCommandBuffer = get( commandBuffer );
			m_state.vaos.insert( m_state.vaos.end()
				, glCommandBuffer->m_state.vaos.begin()
				, glCommandBuffer->m_state.vaos.end() );
			glCommandBuffer->m_state.vaos.clear();
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
		if ( !get( get( m_device )->getInstance() )->getFeatures().hasClearTexImage )
		{
			buildClearColourFboCommand( m_device
				, *m_state.stack
				, image
				, imageLayout
				, std::move( value )
				, std::move( ranges )
				, m_cmdList );
		}
		else
		{
			buildClearColourCommand( m_device
				, image
				, imageLayout
				, std::move( value )
				, std::move( ranges )
				, m_cmdList );
		}
	}

	void CommandBuffer::clearDepthStencilImage( VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, VkImageSubresourceRangeArray ranges )const
	{
		if ( !get( get( m_device )->getInstance() )->getFeatures().hasClearTexImage )
		{
			buildClearDepthStencilFboCommand( m_device
				, *m_state.stack
				, image
				, imageLayout
				, std::move( value )
				, std::move( ranges )
				, m_cmdList );
		}
		else
		{
			buildClearDepthStencilCommand( m_device
				, image
				, imageLayout
				, std::move( value )
				, std::move( ranges )
				, m_cmdList );
		}
	}

	void CommandBuffer::clearAttachments( VkClearAttachmentArray clearAttachments
		, VkClearRectArray clearRects )
	{
		buildClearAttachmentsCommand( *m_state.stack
			, std::move( clearAttachments )
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
			buildBindPipelineCommand( *m_state.stack
				, m_device
				, pipeline
				, bindingPoint
				, m_cmdList );

			for ( auto & pcb : m_state.pushConstantBuffers )
			{
				buildPushConstantsCommand( get( m_state.currentPipeline )->findPushConstantBuffer( pcb.second )
					, m_cmdList );
			}

			m_state.pushConstantBuffers.clear();

			buildUnbindPipelineCommand( *m_state.stack
				, m_device
				, pipeline
				, VK_NULL_HANDLE
				, m_cmdAfterSubmit );
		}
		else if ( bindingPoint == VK_PIPELINE_BIND_POINT_COMPUTE )
		{
			if ( get( get( m_device )->getInstance() )->getFeatures().hasComputeShaders )
			{
				m_state.currentComputePipeline = pipeline;
				buildBindComputePipelineCommand( *m_state.stack
					, pipeline
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
				m_state.stack->setCurrentProgram( 0u );
			}
			else
			{
				get( m_device )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
					, VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT
					, 0ull
					, 0u
					, VK_ERROR_VALIDATION_FAILED_EXT
					, "OpenGL"
					, "Compute shaders are not supported" );
			}
		}

		for ( auto & layout : get( pipeline )->getDescriptorsLayouts() )
		{
			auto it = std::remove_if( m_state.boundDescriptors.begin()
				, m_state.boundDescriptors.end()
				, [&layout]( VkDescriptorSet lookup )
				{
					return get( lookup )->getLayout() == layout;
				} );

			if ( it != m_state.boundDescriptors.begin() )
			{
				m_state.boundDescriptors.erase( m_state.boundDescriptors.begin(), it );
			}
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

		m_state.selectedVao = nullptr;
	}

	void CommandBuffer::bindIndexBuffer( VkBuffer buffer
		, VkDeviceSize offset
		, VkIndexType indexType )const
	{
		m_state.boundIbo = BufferObjectBinding{ get( buffer )->getInternal(), offset, buffer };
		m_state.newlyBoundIbo = m_state.boundIbo;
		m_state.indexType = indexType;
		m_state.selectedVao = nullptr;
	}

	void CommandBuffer::bindDescriptorSets( VkPipelineBindPoint bindingPoint
		, VkPipelineLayout layout
		, uint32_t firstSet
		, VkDescriptorSetArray descriptorSets
		, UInt32Array dynamicOffsets )const
	{
		for ( auto & descriptorSet : descriptorSets )
		{
			m_state.boundDescriptors.push_back( descriptorSet );
			doProcessMappedBoundDescriptorBuffersIn( descriptorSet );
			buildBindDescriptorSetCommand( m_device
				, descriptorSet
				, layout
				, dynamicOffsets
				, bindingPoint
				, m_cmdList );
		}
	}

	void CommandBuffer::setViewport( uint32_t firstViewport
		, VkViewportArray viewports )const
	{
		buildViewportCommand( *m_state.stack, firstViewport, viewports, m_cmdList );
	}

	void CommandBuffer::setScissor( uint32_t firstScissor
		, VkScissorArray scissors )const
	{
		buildScissorCommand( *m_state.stack, firstScissor, scissors, m_cmdList );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		if ( firstInstance > 0
			&& !get( get( m_device )->getInstance() )->getFeatures().hasBaseInstance )
		{
			get( m_device )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT
				, 0ull
				, 0u
				, VK_ERROR_VALIDATION_FAILED_EXT
				, "OpenGL"
				, "Base instance rendering is not supported" );
		}
		else
		{
			if ( isEmpty( get( m_state.currentPipeline )->getVertexInputState() ) )
			{
				bindIndexBuffer( get( m_device )->getEmptyIndexedVaoIdx(), 0u, VK_INDEX_TYPE_UINT32 );
				m_state.selectedVao = &get( m_device )->getEmptyIndexedVao();

				if ( m_state.stack->isPrimitiveRestartEnabled() )
				{
					m_cmdList.emplace_back( makeCmd< OpType::ePrimitiveRestartIndex >( 0xFFFFFFFFu ) );
				}

				doProcessMappedBoundVaoBuffersIn();
				buildBindGeometryBuffersCommand( *m_state.selectedVao
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
				if ( !m_state.selectedVao )
				{
					doSelectVao();
				}

				doProcessMappedBoundVaoBuffersIn();
				buildBindGeometryBuffersCommand( *m_state.selectedVao
					, m_cmdList );
				buildDrawCommand( vtxCount
					, instCount
					, firstVertex
					, firstInstance
					, get( m_state.currentPipeline )->getInputAssemblyState().topology
					, m_cmdList );
			}

			m_cmdList.push_back( makeCmd< OpType::eBindVextexArray >( nullptr ) );
			doProcessMappedBoundDescriptorsBuffersOut();
		}
	}

	void CommandBuffer::drawIndexed( uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance )const
	{
		if ( firstInstance > 0
			&& !get( get( m_device )->getInstance() )->getFeatures().hasBaseInstance )
		{
			get( m_device )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT
				, 0ull
				, 0u
				, VK_ERROR_VALIDATION_FAILED_EXT
				, "OpenGL"
				, "Base instance rendering is not supported" );
		}
		else
		{
			if ( isEmpty( get( m_state.currentPipeline )->getVertexInputState() )
				&& !m_state.newlyBoundIbo )
			{
				bindIndexBuffer( get( m_device )->getEmptyIndexedVaoIdx(), 0u, VK_INDEX_TYPE_UINT32 );
				m_state.selectedVao = &get( m_device )->getEmptyIndexedVao();
			}
			else if ( !m_state.selectedVao )
			{
				doSelectVao();
			}

			if ( m_state.stack->isPrimitiveRestartEnabled() )
			{
				m_cmdList.emplace_back( makeCmd< OpType::ePrimitiveRestartIndex >( m_state.indexType == VK_INDEX_TYPE_UINT32
					? 0xFFFFFFFFu
					: 0x0000FFFFu ) );
			}

			doProcessMappedBoundVaoBuffersIn();
			buildBindGeometryBuffersCommand( *m_state.selectedVao
				, m_cmdList );
			buildDrawIndexedCommand( indexCount
				, instCount
				, firstIndex
				, vertexOffset
				, firstInstance
				, get( m_state.currentPipeline )->getInputAssemblyState().topology
				, m_state.indexType
				, m_cmdList );
			m_cmdList.push_back( makeCmd< OpType::eBindVextexArray >( nullptr ) );
			doProcessMappedBoundDescriptorsBuffersOut();
			m_state.newlyBoundIbo = IboBinding{};
		}
	}

	void CommandBuffer::drawIndirect( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		if ( !get( get( m_device )->getPhysicalDevice() )->getFeatures().multiDrawIndirect )
		{
			get( m_device )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT
				, 0ull
				, 0u
				, VK_ERROR_VALIDATION_FAILED_EXT
				, "OpenGL"
				, "Multi draw indirect is not supported" );
		}
		else
		{
			if ( !m_state.selectedVao )
			{
				doSelectVao();
			}

			doProcessMappedBoundVaoBuffersIn();
			buildBindGeometryBuffersCommand( *m_state.selectedVao
				, m_cmdList );
			buildDrawIndirectCommand( buffer
				, offset
				, drawCount
				, stride
				, get( m_state.currentPipeline )->getInputAssemblyState().topology
				, m_cmdList );
			m_cmdList.push_back( makeCmd< OpType::eBindVextexArray >( nullptr ) );
			doProcessMappedBoundDescriptorsBuffersOut();
		}
	}

	void CommandBuffer::drawIndexedIndirect( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		if ( !get( get( m_device )->getPhysicalDevice() )->getFeatures().multiDrawIndirect )
		{
			get( m_device )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT
				, 0ull
				, 0u
				, VK_ERROR_VALIDATION_FAILED_EXT
				, "OpenGL"
				, "Multi draw indirect is not supported" );
		}
		else
		{
			if ( isEmpty( get( m_state.currentPipeline )->getVertexInputState() )
				&& !m_state.newlyBoundIbo )
			{
				bindIndexBuffer( get( m_device )->getEmptyIndexedVaoIdx(), 0u, VK_INDEX_TYPE_UINT32 );
				m_state.selectedVao = &get( m_device )->getEmptyIndexedVao();
			}
			else if ( !m_state.selectedVao )
			{
				doSelectVao();
			}

			if ( m_state.stack->isPrimitiveRestartEnabled() )
			{
				m_cmdList.emplace_back( makeCmd< OpType::ePrimitiveRestartIndex >( m_state.indexType == VK_INDEX_TYPE_UINT32
					? 0xFFFFFFFFu
					: 0x0000FFFFu ) );
			}

			doProcessMappedBoundVaoBuffersIn();
			buildBindGeometryBuffersCommand( *m_state.selectedVao
				, m_cmdList );
			buildDrawIndexedIndirectCommand( buffer
				, offset
				, drawCount
				, stride
				, get( m_state.currentPipeline )->getInputAssemblyState().topology
				, m_state.indexType
				, m_cmdList );
			m_cmdList.push_back( makeCmd< OpType::eBindVextexArray >( nullptr ) );
			doProcessMappedBoundDescriptorsBuffersOut();
			m_state.newlyBoundIbo = IboBinding{};
		}
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
			buildCopyImageToBufferCommand( *m_state.stack
				, m_device
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
			buildBlitImageCommand( *m_state.stack
				, m_device
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
		if ( get( get( m_device )->getInstance() )->getFeatures().hasComputeShaders )
		{
			buildDispatchCommand( groupCountX
				, groupCountY
				, groupCountZ
				, m_cmdList );
			doProcessMappedBoundDescriptorsBuffersOut();
		}
		else
		{
			get( m_device )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT
				, 0ull
				, 0u
				, VK_ERROR_VALIDATION_FAILED_EXT
				, "OpenGL"
				, "Compute shaders are not supported" );
		}
	}

	void CommandBuffer::dispatchIndirect( VkBuffer buffer
		, VkDeviceSize offset )const
	{
		if ( get( get( m_device )->getInstance() )->getFeatures().hasComputeShaders )
		{
			buildDispatchIndirectCommand( buffer
				, offset
				, m_cmdList );
			doProcessMappedBoundDescriptorsBuffersOut();
		}
		else
		{
			get( m_device )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT
				, 0ull
				, 0u
				, VK_ERROR_VALIDATION_FAILED_EXT
				, "OpenGL"
				, "Compute shaders are not supported" );
		}
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
		if ( get( m_device )->hasMemoryBarrier() )
		{
			buildMemoryBarrierCommand( after
				, before
				, dependencyFlags
				, std::move( memoryBarriers )
				, std::move( bufferMemoryBarriers )
				, std::move( imageMemoryBarriers )
				, m_cmdList );
		}
	}

#if VK_EXT_debug_utils

	void CommandBuffer::beginDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pLabelName,
		};
	}

	void CommandBuffer::endDebugUtilsLabel()const
	{
		m_label = ashes::nullopt;
	}

	void CommandBuffer::insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pLabelName,
		};
	}

#endif
#if VK_EXT_debug_marker

	void CommandBuffer::debugMarkerBegin( VkDebugMarkerMarkerInfoEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pMarkerName,
		};
	}

	void CommandBuffer::debugMarkerEnd()const
	{
		m_label = ashes::nullopt;
	}

	void CommandBuffer::debugMarkerInsert( VkDebugMarkerMarkerInfoEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pMarkerName,
		};
	}

#endif

	void CommandBuffer::initialiseGeometryBuffers( ContextLock & context )const
	{
		for ( auto & vao : m_state.vaos )
		{
			vao.get().initialise( context );
		}

		m_state.vaos.clear();
	}

	void CommandBuffer::doReset()const
	{
		m_mappedBuffers.clear();
		m_cmdList.clear();
		m_cmds.clear();
		m_cmdAfterSubmit.clear();
		m_cmdsAfterSubmit.clear();

		for ( auto & view : m_blitViews )
		{
			deallocate( view, nullptr );
		}

		m_blitViews.clear();
	}

	void CommandBuffer::doSelectVao()const
	{
		m_state.selectedVao = get( m_state.currentPipeline )->findGeometryBuffers( m_state.boundVbos, m_state.boundIbo );

		if ( !m_state.selectedVao )
		{
			m_state.selectedVao = &get( m_state.currentPipeline )->createGeometryBuffers( m_state.boundVbos, m_state.boundIbo, m_state.indexType ).get();
			m_state.vaos.emplace_back( *m_state.selectedVao );
		}
		else if ( m_state.selectedVao->getVao() == GL_INVALID_INDEX )
		{
			auto it = std::find_if( m_state.vaos.begin()
				, m_state.vaos.end()
				, [this]( GeometryBuffersRef const & lookup )
				{
					return &lookup.get() == m_state.selectedVao;
				} );

			if ( it == m_state.vaos.end() )
			{
				m_state.vaos.emplace_back( *m_state.selectedVao );
			}
		}
	}

	void CommandBuffer::doProcessMappedBoundDescriptorBuffersIn( VkDescriptorSet descriptor )const
	{
		for ( auto & writes : get( descriptor )->getDynamicBuffers() )
		{
			for ( auto & write : writes->writes )
			{
				doProcessMappedBoundBufferIn( write.pBufferInfo->buffer );
			}
		}

		for ( auto & writes : get( descriptor )->getStorageBuffers() )
		{
			for ( auto & write : writes->writes )
			{
				doProcessMappedBoundBufferIn( write.pBufferInfo->buffer );
			}
		}

		for ( auto & writes : get( descriptor )->getUniformBuffers() )
		{
			for ( auto & write : writes->writes )
			{
				doProcessMappedBoundBufferIn( write.pBufferInfo->buffer );
			}
		}
	}

	void CommandBuffer::doProcessMappedBoundDescriptorsBuffersOut()const
	{
		for ( auto descriptor : m_state.boundDescriptors )
		{
			for ( auto & writes : get( descriptor )->getDynamicBuffers() )
			{
				if ( writes->descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC )
				{
					for ( auto & write : writes->writes )
					{
						if ( write.descriptorType )
							doProcessMappedBoundBufferOut( write.pBufferInfo->buffer );
					}
				}
			}

			for ( auto & writes : get( descriptor )->getStorageBuffers() )
			{
				for ( auto & write : writes->writes )
				{
					doProcessMappedBoundBufferOut( write.pBufferInfo->buffer );
				}
			}
		}
	}

	void CommandBuffer::doProcessMappedBoundVaoBuffersIn()const
	{
		assert( m_state.selectedVao );

		for ( auto & vbo : m_state.selectedVao->getVbos() )
		{
			doProcessMappedBoundBufferIn( vbo.vbo );
		}

		if ( m_state.selectedVao->hasIbo() )
		{
			doProcessMappedBoundBufferIn( m_state.selectedVao->getIbo().ibo );
		}
	}

	void CommandBuffer::doProcessMappedBoundBufferIn( VkBuffer buffer )const
	{
		if ( get( buffer )->isMapped() )
		{
			doAddMappedBuffer( buffer, true );
		}
	}

	void CommandBuffer::doProcessMappedBoundBufferOut( VkBuffer buffer )const
	{
		if ( get( buffer )->isMapped() )
		{
			doAddMappedBuffer( buffer, false );
		}
	}

	CommandBuffer::BufferIndex & CommandBuffer::doAddMappedBuffer( VkBuffer buffer, bool isInput )const
	{
		auto buf = get( buffer );
		auto internal = buf->getInternal();

		if ( isInput )
		{
			m_cmdList.emplace_back( makeCmd< OpType::eUploadMemory >( buf->getMemory() ) );
		}
		else
		{
			m_cmdList.emplace_back( makeCmd< OpType::eDownloadMemory >( buf->getMemory() ) );
		}

		auto it = std::find_if( m_mappedBuffers.begin()
			, m_mappedBuffers.end()
			, [internal]( BufferIndex const & lookup )
		{
			return lookup.name == internal;
		} );

		CommandBuffer::BufferIndex * result;

		if ( it == m_mappedBuffers.end() )
		{
			m_mappedBuffers.emplace_back( internal
				, m_cmdList.size() - 1u
				, get( buf->getMemory() )->onDestroy.connect( [this]( GLuint name )
				{
					doRemoveMappedBuffer( name );
				} ) );
			result = &m_mappedBuffers.back();
		}
		else
		{
			result = &( *it );
		}

		return *result;
	}

	void CommandBuffer::doRemoveMappedBuffer( GLuint internal )const
	{
		auto it = std::find_if( m_mappedBuffers.begin()
			, m_mappedBuffers.end()
			, [internal]( BufferIndex const & lookup )
			{
				return lookup.name == internal;
			} );

		if ( it != m_mappedBuffers.end() )
		{
			auto index = it->index;
			m_cmdList.erase( m_cmdList.begin() + index );
			it = m_mappedBuffers.erase( it );

			while ( it != m_mappedBuffers.end() )
			{
				it->index--;
				++it;
			}
		}
	}
}
