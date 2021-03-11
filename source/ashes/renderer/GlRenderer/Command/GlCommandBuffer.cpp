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

#include "ashesgl_api.hpp"

#include <renderer/RendererCommon/Helper/VertexInputState.hpp>

#include <algorithm>
#include <numeric>

using ashes::operator==;
using ashes::operator!=;

bool operator==( VkPushConstantRange const & lhs
	, VkPushConstantRange const & rhs )
{
	return lhs.offset == rhs.offset
		&& lhs.size == rhs.size
		&& lhs.stageFlags == rhs.stageFlags;
}

namespace ashes::gl
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

		bool areCompatible( VkPushConstantRangeArray const & lhs
			, VkPushConstantRangeArray const & rhs )
		{
			return lhs == rhs;
		}

		bool areCompatible( VkDescriptorSetLayoutBinding const & lhs
			, VkDescriptorSetLayoutBinding const & rhs )
		{
			return lhs.binding == rhs.binding
				&& lhs.descriptorCount == rhs.descriptorCount
				&& lhs.descriptorType == rhs.descriptorType
				&& lhs.stageFlags == rhs.stageFlags;
		}

		bool areCompatible( VkDescriptorSetLayout lhs
			, VkDescriptorSetLayout rhs )
		{
			auto lhsIt = get( lhs )->begin();
			auto lhsEnd = get( lhs )->end();
			auto rhsIt = get( rhs )->begin();
			auto rhsEnd = get( rhs )->end();
			bool result = true;

			while ( lhsIt != lhsEnd && rhsIt != rhsEnd && result )
			{
				result = areCompatible( *lhsIt, *rhsIt );
				++lhsIt;
				++rhsIt;
			}

			return result;
		}

		uint32_t areCompatible( VkDescriptorSetLayoutArray const & lhs
			, VkDescriptorSetLayoutArray const & rhs )
		{
			auto size = std::min( lhs.size(), rhs.size() );
			uint32_t result = 0u;

			for ( size_t i = 0u; i < size; ++i )
			{
				auto lhsLayout = lhs[i];
				auto rhsLayout = rhs[i];

				if ( !areCompatible( lhs[i], rhs[i] ) )
				{
					i = size;
				}
				else
				{
					++result;
				}
			}

			return result;
		}
	}

	CommandBuffer::CommandBuffer( VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkCommandBufferLevel level )
		: m_device{ device }
		, m_level{ level }
	{
	}

	CommandBuffer::~CommandBuffer()
	{
		doReset();
	}

	template< typename StructTypeAccessT >
	bool checkPointerAccess( VkStructureType type
		, StructTypeAccessT access )
	{
#if defined( WIN32 )

		__try
		{
			return type == access();
		}
		__except ( GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION )
		{
			return false;
		}

#else

		return true;

#endif
	}

	VkResult CommandBuffer::begin( VkCommandBufferBeginInfo info )const
	{
		doReset();
		m_state = State{};
		m_state.stack = std::make_unique< ContextStateStack >( m_device );
		m_state.beginFlags = info.flags;

		if ( checkFlag( m_state.beginFlags, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT ) )
		{
			if ( info.pInheritanceInfo
				&& checkPointerAccess( VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO
					, [&info]()
					{
						return info.pInheritanceInfo->sType;
					} ) )
			{
				// Fake a bound framebuffer here : the one in the inheritance info.
				m_state.stack->setCurrentFramebuffer( info.pInheritanceInfo->framebuffer );

				if ( info.pInheritanceInfo->framebuffer )
				{
					m_state.stack->setRenderArea( get( info.pInheritanceInfo->framebuffer )->getDimensions() );
				}

				m_state.currentFrameBuffer = info.pInheritanceInfo->framebuffer;
				m_state.currentRenderPass = info.pInheritanceInfo->renderPass;
				m_state.currentSubpassIndex = info.pInheritanceInfo->subpass;
			}
		}

		return VK_SUCCESS;
	}

	VkResult CommandBuffer::end()const
	{
		m_state.pushConstantBuffers.clear();

		if ( m_level == VK_COMMAND_BUFFER_LEVEL_PRIMARY )
		{
			mergeList( m_cmdList, m_cmds );
			mergeList( m_cmdAfterSubmit, m_cmdsAfterSubmit );
		}

		return VK_SUCCESS;
	}

	VkResult CommandBuffer::reset( VkCommandBufferResetFlags flags )const
	{
		m_preExecuteActions.clear();
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
			, m_cmdList
			, m_preExecuteActions );
		m_state.currentSubpass = &get( m_state.currentRenderPass )->getSubpasses()[m_state.currentSubpassIndex++];
		buildBeginSubpassCommand( *m_state.stack
			, m_state.currentRenderPass
			, m_state.currentFrameBuffer
			, *m_state.currentSubpass
			, m_cmdList );
		doApplyPreExecuteCommands( *m_state.stack );
		m_preExecuteActions.clear();
	}

	void CommandBuffer::nextSubpass( VkSubpassContents contents )const
	{
		buildEndSubpassCommand( m_device
			, *m_state.stack
			, m_state.currentFrameBuffer
			, *m_state.currentSubpass
			, m_cmdList
			, m_preExecuteActions );
		m_state.currentSubpass = &get( m_state.currentRenderPass )->getSubpasses()[m_state.currentSubpassIndex++];
		buildBeginSubpassCommand( *m_state.stack
			, m_state.currentRenderPass
			, m_state.currentFrameBuffer
			, *m_state.currentSubpass
			, m_cmdList );
		m_state.boundVbos.clear();
	}

	void CommandBuffer::endRenderPass()const
	{
		buildEndSubpassCommand( m_device
			, *m_state.stack
			, m_state.currentFrameBuffer
			, *m_state.currentSubpass
			, m_cmdList
			, m_preExecuteActions );
		buildEndRenderPassCommand( *m_state.stack
			, m_cmdList );
		m_state.boundVbos.clear();
		m_state.boundDescriptors.clear();
		m_state.pushConstantBuffers.clear();
		m_state.boundIbo = ashes::nullopt;
		m_state.currentFrameBuffer = nullptr;
		m_state.currentRenderPass = nullptr;
		m_state.currentSubpass = nullptr;
		m_state.currentSubpassIndex = 0u;
		m_state.waitingDescriptors.clear();
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
			glCommandBuffer->doApplyPreExecuteCommands( *m_state.stack );
			mergeList( glCommandBuffer->m_cmdList, glCommandBuffer->m_cmds );
			mergeList( glCommandBuffer->m_cmdAfterSubmit, glCommandBuffer->m_cmdsAfterSubmit );
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
		, ArrayView< VkImageSubresourceRange const > ranges )const
	{
		if ( !get( getInstance( m_device ) )->getFeatures().hasClearTexImage )
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
		, ArrayView< VkImageSubresourceRange const > ranges )const
	{
		if ( !get( getInstance( m_device ) )->getFeatures().hasClearTexImage )
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

	void CommandBuffer::clearAttachments( ArrayView< VkClearAttachment const > clearAttachments
		, ArrayView< VkClearRect const > clearRects )
	{
		buildClearAttachmentsCommand( *m_state.stack
			, std::move( clearAttachments )
			, std::move( clearRects )
			, m_cmdList
			, m_preExecuteActions );
	}

	void CommandBuffer::bindPipeline( VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint )const
	{
		if ( bindingPoint == VK_PIPELINE_BIND_POINT_GRAPHICS )
		{
			doCheckPipelineLayoutCompatibility( get( pipeline )->getLayout()
				, m_state.currentGraphicsPipelineLayout );
			m_state.currentGraphicsPipeline = nullptr;

			if ( m_state.currentGraphicsPipeline )
			{
				auto src = get( m_state.currentGraphicsPipeline )->getVertexInputStateHash();
				auto dst = get( pipeline )->getVertexInputStateHash();

				if ( src != dst )
				{
					IboBinding empty{};
					m_state.boundIbo.swap( empty );
					m_state.boundVbos.clear();
				}
			}

			m_state.currentGraphicsPipeline = pipeline;
			buildBindPipelineCommand( *m_state.stack
				, m_device
				, pipeline
				, bindingPoint
				, m_cmdList
				, m_preExecuteActions );
			buildUnbindPipelineCommand( *m_state.stack
				, m_device
				, m_cmdAfterSubmit );
		}
		else if ( bindingPoint == VK_PIPELINE_BIND_POINT_COMPUTE )
		{
			doCheckPipelineLayoutCompatibility( get( pipeline )->getLayout()
				, m_state.currentComputePipelineLayout );
			m_state.currentComputePipeline = nullptr;

			if ( get( getInstance( m_device ) )->getFeatures().hasComputeShaders )
			{
				m_state.currentComputePipeline = pipeline;
				buildBindComputePipelineCommand( *m_state.stack
					, pipeline
					, bindingPoint
					, m_cmdList );
				buildUnbindComputePipelineCommand( *m_state.stack
					, m_cmdAfterSubmit );
			}
			else
			{
				reportError( m_device
					, VK_ERROR_FEATURE_NOT_PRESENT
					, "Unsupported Feature"
					, "Compute Shaders" );
			}
		}

		if ( m_state.currentGraphicsPipeline || m_state.currentComputePipeline )
		{
			uint32_t dynamicOffsetIndex = 0u;
			VkDescriptorSet ds = VK_NULL_HANDLE;

			for ( auto & waiting : m_state.waitingDescriptors )
			{
				ds = waiting.second( ds, dynamicOffsetIndex );
			}

			m_state.waitingDescriptors.clear();

			for ( auto & pcb : m_state.pushConstantBuffers )
			{
				buildPushConstantsCommand( get( pipeline )->getDevice()
					, pcb.second.stageFlags
					, get( pipeline )->findPushConstantBuffer( pcb.second, doIsRtotFbo() )
					, m_cmdList );
			}

			m_state.pushConstantBuffers.clear();
		}

	}

	void CommandBuffer::bindVertexBuffers( uint32_t firstBinding
		, ArrayView< VkBuffer const > buffers
		, ArrayView< VkDeviceSize const > offsets )const
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
		, ArrayView< VkDescriptorSet const > descriptorSets
		, ArrayView < uint32_t const > dynamicOffsets )const
	{
		auto currentSet = firstSet;
		doCheckPipelineLayoutCompatibility( layout
			, ( bindingPoint == VK_PIPELINE_BIND_POINT_GRAPHICS
				? m_state.currentGraphicsPipelineLayout
				: m_state.currentComputePipelineLayout ) );
		uint32_t dynamicOffsetIndex = 0u;

		for ( auto & descriptorSet : descriptorSets )
		{
			m_state.boundDescriptors.emplace( currentSet, descriptorSet );
			doProcessMappedBoundDescriptorBuffersIn( descriptorSet );
			buildBindDescriptorSetCommand( m_device
				, descriptorSet
				, currentSet
				, layout
				, dynamicOffsets
				, dynamicOffsetIndex
				, bindingPoint
				, m_cmdList );
			++currentSet;
		}
	}

	void CommandBuffer::setViewport( uint32_t firstViewport
		, ArrayView< VkViewport const > viewports )const
	{
		buildViewportCommand( *m_state.stack
			, firstViewport
			, viewports
			, m_cmdList
			, m_preExecuteActions );
	}

	void CommandBuffer::setScissor( uint32_t firstScissor
		, ArrayView< VkRect2D const > scissors )const
	{
		buildScissorCommand( *m_state.stack
			, firstScissor
			, scissors
			, m_cmdList
			, m_preExecuteActions );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		if ( firstInstance > 0
			&& !get( getInstance( m_device ) )->getFeatures().hasBaseInstance )
		{
			reportError( get( this )
				, VK_ERROR_FEATURE_NOT_PRESENT
				, "Unsupported feature"
				, "Base instance rendering" );
		}
		else
		{
			if ( isEmpty( get( m_state.currentGraphicsPipeline )->getVertexInputState() ) )
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
					, get( m_state.currentGraphicsPipeline )->getInputAssemblyState().topology
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
					, get( m_state.currentGraphicsPipeline )->getInputAssemblyState().topology
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
			&& !get( getInstance( m_device ) )->getFeatures().hasBaseInstance )
		{
			reportError( get( this )
				, VK_ERROR_FEATURE_NOT_PRESENT
				, "Unsupported feature"
				, "Base instance rendering" );
		}
		else
		{
			if ( isEmpty( get( m_state.currentGraphicsPipeline )->getVertexInputState() )
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
				, get( m_state.currentGraphicsPipeline )->getInputAssemblyState().topology
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
			reportError( get( this )
				, VK_ERROR_FEATURE_NOT_PRESENT
				, "Unsupported feature"
				, "Multi draw indirect" );
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
				, get( m_state.currentGraphicsPipeline )->getInputAssemblyState().topology
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
			reportError( get( this )
				, VK_ERROR_FEATURE_NOT_PRESENT
				, "Unsupported feature"
				, "Multi draw indirect" );
		}
		else
		{
			if ( isEmpty( get( m_state.currentGraphicsPipeline )->getVertexInputState() )
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
				, get( m_state.currentGraphicsPipeline )->getInputAssemblyState().topology
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
		, ArrayView< VkBufferImageCopy const > copyInfos )const
	{
		for ( auto & copyInfo : copyInfos )
		{
			buildCopyBufferToImageCommand( *m_state.stack
				, get( src )->getDevice()
				, std::move( copyInfo )
				, src
				, dst
				, m_cmdList );
		}
	}

	void CommandBuffer::copyToBuffer( VkImage src
		, VkImageLayout srcLayout
		, VkBuffer dst
		, ArrayView< VkBufferImageCopy const > copyInfos )const
	{
		for ( auto & copyInfo : copyInfos )
		{
			buildCopyImageToBufferCommand( *m_state.stack
				, m_device
				, std::move( copyInfo )
				, src
				, dst
				, m_cmdList
				, m_blitViews );
		}
	}

	void CommandBuffer::updateBuffer( VkBuffer dstBuffer
		, VkDeviceSize dstOffset
		, ArrayView< uint8_t const > data )
	{
		m_updatesData.push_back( std::make_unique< ByteArray >( data.begin(), data.end() ) );
		m_cmdList.push_back( makeCmd< OpType::eUpdateBuffer >( get( dstBuffer )->getMemoryBinding().getParent()
			, dstOffset + get( dstBuffer )->getOffset()
			, VkDeviceSize( data.size() )
			, m_updatesData.back()->data() ) );
	}

	void CommandBuffer::fillBuffer( VkBuffer dstBuffer
		, VkDeviceSize dstOffset
		, VkDeviceSize size
		, uint32_t data )
	{
		m_cmdList.push_back( makeCmd< OpType::eFillBuffer >( get( dstBuffer )->getMemoryBinding().getParent()
			, dstOffset + get( dstBuffer )->getOffset()
			, size == VK_WHOLE_SIZE ? get( dstBuffer )->getMemoryRequirements().size : size
			, data ) );
	}

	void CommandBuffer::copyBuffer( VkBuffer src
		, VkBuffer dst
		, ArrayView< VkBufferCopy const > copyInfos )const
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
		, ArrayView< VkImageCopy const > copyInfos )const
	{
		for ( auto & copyInfo : copyInfos )
		{
			buildCopyImageCommand( *m_state.stack
				, m_device
				, std::move( copyInfo )
				, src
				, dst
				, m_cmdList
				, m_blitViews );
		}
	}

	void CommandBuffer::blitImage( VkImage srcImage
		, VkImageLayout srcLayout
		, VkImage dstImage
		, VkImageLayout dstLayout
		, ArrayView< VkImageBlit const > regions
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
				, m_cmdList
				, m_blitViews );
		}
	}

	void CommandBuffer::resolveImage( VkImage srcImage
		, VkImageLayout srcLayout
		, VkImage dstImage
		, VkImageLayout dstLayout
		, ArrayView< VkImageResolve const > regions )const
	{
		for ( auto & region : regions )
		{
			buildBlitImageCommand( *m_state.stack
				, m_device
				, srcImage
				, dstImage
				, {
					region.srcSubresource,
					{ region.srcOffset, { int32_t( region.extent.width ), int32_t( region.extent.height ), int32_t( region.extent.depth ) } },
					region.dstSubresource,
					{ region.dstOffset, { int32_t( region.extent.width ), int32_t( region.extent.height ), int32_t( region.extent.depth ) } },
				}
				, VK_FILTER_NEAREST
				, m_cmdList
				, m_blitViews );
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

	void CommandBuffer::copyQueryPoolResults( VkQueryPool queryPool
		, uint32_t firstQuery
		, uint32_t queryCount
		, VkBuffer dstBuffer
		, VkDeviceSize dstOffset
		, VkDeviceSize stride
		, VkQueryResultFlags flags )const
	{
		if ( !get( get( m_device )->getPhysicalDevice() )->find( ARB_query_buffer_object ) )
		{
			reportError( queryPool
				, VK_ERROR_FEATURE_NOT_PRESENT
				, "Unsupported Feature"
				, "Query Buffer Objects" );
		}
		else
		{
			m_cmdList.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_QUERY
				, get( dstBuffer )->getInternal() ) );
			m_cmdList.push_back( makeCmd< OpType::eGetQueryResults >( queryPool
				, firstQuery
				, queryCount
				, stride
				, flags
				, dstOffset + get( dstBuffer )->getOffset() ) );
			m_cmdList.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_QUERY
				, 0u ) );
		}
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

		if ( m_state.currentGraphicsPipeline
			&& 0 != ( stageFlags & VK_SHADER_STAGE_ALL_GRAPHICS ) )
		{
			doCheckPipelineLayoutCompatibility( layout, m_state.currentGraphicsPipelineLayout );
			buildPushConstantsCommand( get( layout )->getDevice()
				, stageFlags
				, get( m_state.currentGraphicsPipeline )->findPushConstantBuffer( desc, doIsRtotFbo() )
				, m_cmdList );
		}

		if ( m_state.currentComputePipeline
			&& checkFlag( stageFlags, VK_SHADER_STAGE_COMPUTE_BIT ) )
		{
			doCheckPipelineLayoutCompatibility( layout, m_state.currentComputePipelineLayout );
			buildPushConstantsCommand( get( layout )->getDevice()
				, stageFlags
				, get( m_state.currentComputePipeline )->findPushConstantBuffer( desc, false )
				, m_cmdList );
		}

		if ( ( !m_state.currentGraphicsPipeline
				|| !checkFlag( stageFlags, VK_SHADER_STAGE_ALL_GRAPHICS ) )
			&& ( !m_state.currentComputePipeline
				|| !checkFlag( stageFlags, VK_SHADER_STAGE_COMPUTE_BIT ) ) )
		{
			m_state.pushConstantBuffers.emplace_back( layout, desc );
		}
	}

	void CommandBuffer::dispatch( uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ )const
	{
		if ( get( getInstance( m_device ) )->getFeatures().hasComputeShaders )
		{
			buildDispatchCommand( groupCountX
				, groupCountY
				, groupCountZ
				, m_cmdList );
			doProcessMappedBoundDescriptorsBuffersOut();
		}
		else
		{
			reportError( get( this )
				, VK_ERROR_FEATURE_NOT_PRESENT
				, "Unsupported feature"
				, "Compute Shaders" );
		}
	}

	void CommandBuffer::dispatchIndirect( VkBuffer buffer
		, VkDeviceSize offset )const
	{
		if ( get( getInstance( m_device ) )->getFeatures().hasComputeShaders )
		{
			buildDispatchIndirectCommand( buffer
				, offset
				, m_cmdList );
			doProcessMappedBoundDescriptorsBuffersOut();
		}
		else
		{
			reportError( get( this )
				, VK_ERROR_FEATURE_NOT_PRESENT
				, "Unsupported feature"
				, "Compute Shaders" );
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
		m_state.stack->applyDepthBias( m_cmdList
			, constantFactor
			, clamp
			, slopeFactor );
	}

	void CommandBuffer::setBlendConstants( float const blendConstants[4] )const
	{
		m_cmdList.push_back( makeCmd< OpType::eBlendConstants >( blendConstants[0]
			, blendConstants[1] 
			, blendConstants[2] 
			, blendConstants[3] ) );
	}

	void CommandBuffer::setDepthBounds( float minDepthBounds
		, float maxDepthBounds )const
	{
		m_cmdList.push_back( makeCmd< OpType::eDepthRange >( minDepthBounds, maxDepthBounds ) );
	}

	void CommandBuffer::setStencilCompareMask( VkStencilFaceFlags faceMask
		, uint32_t compareMask )const
	{
		m_state.stack->applyStencilCompareMask( m_cmdList
			, compareMask
			, faceMask );
	}

	void CommandBuffer::setStencilWriteMask( VkStencilFaceFlags faceMask
		, uint32_t writeMask )const
	{
		m_state.stack->applyStencilWriteMask( m_cmdList
			, writeMask
			, faceMask );
	}

	void CommandBuffer::setStencilReference( VkStencilFaceFlags faceMask
		, uint32_t reference )
	{
		m_state.stack->applyStencilReference( m_cmdList
			, reference
			, faceMask );
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
		, ArrayView< VkMemoryBarrier const > memoryBarriers
		, ArrayView< VkBufferMemoryBarrier const > bufferMemoryBarriers
		, ArrayView< VkImageMemoryBarrier const > imageMemoryBarriers )const
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
		, ArrayView< VkMemoryBarrier const > memoryBarriers
		, ArrayView< VkBufferMemoryBarrier const > bufferMemoryBarriers
		, ArrayView< VkImageMemoryBarrier const > imageMemoryBarriers )const
	{
		if ( get( m_device )->hasMemoryBarrier() )
		{
			buildMemoryBarrierCommand( after
				, before
				, dependencyFlags
				, std::move( memoryBarriers )
				, std::move( bufferMemoryBarriers )
				, std::move( imageMemoryBarriers )
				, m_downloads
				, m_uploads
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
		m_cmdList.push_back( makeCmd< OpType::ePushDebugGroup >( GL_DEBUG_SOURCE_APPLICATION
			, 1u
			, GLsizei( m_label.value().labelName.size() )
			, m_label.value().labelName.c_str() ) );
	}

	void CommandBuffer::endDebugUtilsLabel()const
	{
		m_cmdList.push_back( makeCmd< OpType::ePopDebugGroup >() );
		m_label = ashes::nullopt;
	}

	void CommandBuffer::insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
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
		m_cmdList.push_back( makeCmd< OpType::ePushDebugGroup >( GL_DEBUG_SOURCE_APPLICATION
			, 1u
			, GLsizei( m_label.value().labelName.size() )
			, m_label.value().labelName.c_str() ) );
	}

	void CommandBuffer::debugMarkerEnd()const
	{
		m_cmdList.push_back( makeCmd< OpType::ePopDebugGroup >() );
		m_label = ashes::nullopt;
	}

	void CommandBuffer::debugMarkerInsert( VkDebugMarkerMarkerInfoEXT const & labelInfo )const
	{
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

	void CommandBuffer::doApplyPreExecuteCommands( ContextStateStack const & stack )const
	{
		for ( auto & action : m_preExecuteActions )
		{
			action( m_cmdList, stack );
		}
	}

	void CommandBuffer::doReset()const
	{
		m_mappedBuffers.clear();
		m_cmdList.clear();
		m_cmds.clear();
		m_cmdAfterSubmit.clear();
		m_cmdsAfterSubmit.clear();
		m_downloads.clear();
		m_uploads.clear();

		for ( auto & view : m_blitViews )
		{
			deallocate( view
				, get( m_device )->getAllocationCallbacks() );
		}

		m_blitViews.clear();
	}

	void CommandBuffer::doSelectVao()const
	{
		m_state.selectedVao = get( m_state.currentGraphicsPipeline )->findGeometryBuffers( m_state.boundVbos, m_state.boundIbo );

		if ( !m_state.selectedVao )
		{
			m_state.selectedVao = &get( m_state.currentGraphicsPipeline )->createGeometryBuffers( m_state.boundVbos, m_state.boundIbo, m_state.indexType ).get();
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
			for ( auto & writes : get( descriptor.second )->getDynamicBuffers() )
			{
				if ( writes->descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC )
				{
					for ( auto & write : writes->writes )
					{
						doProcessMappedBoundBufferOut( write.pBufferInfo->buffer );
					}
				}
			}

			for ( auto & writes : get( descriptor.second )->getStorageBuffers() )
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
			m_cmdList.emplace_back( makeCmd< OpType::eUploadMemory >( buf->getMemoryBinding().getParent() ) );
		}
		else
		{
			m_cmdList.emplace_back( makeCmd< OpType::eDownloadMemory >( buf->getMemoryBinding().getParent() ) );
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
				, get( buf->getMemoryBinding().getParent() )->onDestroy.connect( [this]( GLuint name )
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

	bool CommandBuffer::doIsRtotFbo()const
	{
		return !m_state.stack->hasCurrentFramebuffer()
			|| !get( m_state.stack->getCurrentFramebuffer() )->hasSwapchainImage();
	}

	void CommandBuffer::doCheckPipelineLayoutCompatibility( VkPipelineLayout layout
		, VkPipelineLayout & currentLayout )const
	{
		if ( currentLayout
			&& currentLayout != layout )
		{
			// Check push constants compatibility.
			if ( !areCompatible( get( currentLayout )->getPushConstants()
				, get( layout )->getPushConstants() ) )
			{
				m_state.boundDescriptors.clear();
				m_state.pushConstantBuffers.clear();
			}
			else if ( uint32_t count = areCompatible( get( currentLayout )->getDescriptorsLayouts()
				, get( layout )->getDescriptorsLayouts() ) )
			{
				auto it = m_state.boundDescriptors.begin();

				while ( it != m_state.boundDescriptors.end() && it->first < count )
				{
					++it;
				}

				m_state.boundDescriptors.erase( it, m_state.boundDescriptors.end() );
			}
			else
			{
				m_state.boundDescriptors.clear();
			}
		}

		currentLayout = layout;
	}
}
