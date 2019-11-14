/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/TestCommandBuffer.hpp"

#include "Buffer/TestBuffer.hpp"
#include "Command/TestCommandPool.hpp"
#include "Core/TestDevice.hpp"
#include "Descriptor/TestDescriptorSet.hpp"
#include "Image/TestImage.hpp"
#include "Image/TestImageView.hpp"
#include "Miscellaneous/TestQueryPool.hpp"
#include "Pipeline/TestPipeline.hpp"
#include "Pipeline/TestPipelineLayout.hpp"
#include "RenderPass/TestFrameBuffer.hpp"
#include "RenderPass/TestRenderPass.hpp"

#include "Command/Commands/TestBeginQueryCommand.hpp"
#include "Command/Commands/TestBeginRenderPassCommand.hpp"
#include "Command/Commands/TestBeginSubpassCommand.hpp"
#include "Command/Commands/TestBindComputePipelineCommand.hpp"
#include "Command/Commands/TestBindDescriptorSetCommand.hpp"
#include "Command/Commands/TestBindIndexBufferCommand.hpp"
#include "Command/Commands/TestBindVertexBuffersCommand.hpp"
#include "Command/Commands/TestBindPipelineCommand.hpp"
#include "Command/Commands/TestClearAttachmentsCommand.hpp"
#include "Command/Commands/TestClearColourCommand.hpp"
#include "Command/Commands/TestClearDepthStencilCommand.hpp"
#include "Command/Commands/TestCopyBufferCommand.hpp"
#include "Command/Commands/TestCopyBufferToImageCommand.hpp"
#include "Command/Commands/TestCopyImageCommand.hpp"
#include "Command/Commands/TestCopyImageToBufferCommand.hpp"
#include "Command/Commands/TestDispatchCommand.hpp"
#include "Command/Commands/TestDispatchIndirectCommand.hpp"
#include "Command/Commands/TestDownloadMemoryCommand.hpp"
#include "Command/Commands/TestDrawCommand.hpp"
#include "Command/Commands/TestDrawIndexedCommand.hpp"
#include "Command/Commands/TestDrawIndexedIndirectCommand.hpp"
#include "Command/Commands/TestDrawIndirectCommand.hpp"
#include "Command/Commands/TestEndQueryCommand.hpp"
#include "Command/Commands/TestEndRenderPassCommand.hpp"
#include "Command/Commands/TestEndSubpassCommand.hpp"
#include "Command/Commands/TestExecuteActionsCommand.hpp"
#include "Command/Commands/TestExecuteCommandsCommand.hpp"
#include "Command/Commands/TestGenerateMipsCommand.hpp"
#include "Command/Commands/TestMemoryBarrierCommand.hpp"
#include "Command/Commands/TestPushConstantsCommand.hpp"
#include "Command/Commands/TestResetEventCommand.hpp"
#include "Command/Commands/TestResetQueryPoolCommand.hpp"
#include "Command/Commands/TestScissorCommand.hpp"
#include "Command/Commands/TestSetDepthBiasCommand.hpp"
#include "Command/Commands/TestSetEventCommand.hpp"
#include "Command/Commands/TestSetLineWidthCommand.hpp"
#include "Command/Commands/TestUploadMemoryCommand.hpp"
#include "Command/Commands/TestViewportCommand.hpp"
#include "Command/Commands/TestWaitEventsCommand.hpp"
#include "Command/Commands/TestWriteTimestampCommand.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	//*********************************************************************************************

	namespace
	{
		uint64_t makeHash( VkFormat src
			, VkFormat dst )
		{
			return ( uint64_t( src ) << 32 )
				| ( uint64_t( dst ) << 0 );
		}
	}

	//*********************************************************************************************

	CommandBuffer::CommandBuffer( VkDevice device
		, VkCommandPool commandPool
		, bool primary )
		: m_device{ device }
		, m_commandPool{ commandPool }
	{
		get( commandPool )->registerCommands( get( this ) );
	}

	CommandBuffer::~CommandBuffer()
	{
	}

	void CommandBuffer::execute()const
	{
		for ( auto & command : m_commands )
		{
			command->apply();
		}

		for ( auto & action : m_afterSubmitActions )
		{
			action();
		}
	}

	VkResult CommandBuffer::begin( VkCommandBufferBeginInfo info )const
	{
		m_commands.clear();
		m_mappedResources.clear();
		m_afterSubmitActions.clear();
		m_state = State{};
		m_state.beginInfo = std::move( info );
		return VK_SUCCESS;
	}

	VkResult CommandBuffer::end()const
	{
		m_state.pushConstantBuffers.clear();
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
		for ( auto & commandBuffer : commands )
		{
			auto & dxCommandBuffer = *get( commandBuffer );

			for ( auto & command : dxCommandBuffer.getCommands() )
			{
				m_commands.emplace_back( command->clone() );
			}

			m_commands.emplace_back( std::make_unique< ExecuteActionsCommand >( m_device
				, dxCommandBuffer.m_afterSubmitActions ) );
		}
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
					, pcb.second ) );
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
					, pcb.second ) );
				doAddAfterSubmitAction();
			}

			m_state.pushConstantBuffers.clear();
		}

		auto it = m_state.boundDescriptors.end();

		for ( auto & layout : get( pipeline )->getDescriptorsLayouts() )
		{
			it = std::remove_if( m_state.boundDescriptors.begin()
				, it
				, [&layout]( VkDescriptorSet lookup )
				{
					return get( lookup )->getLayout() == layout;
				} );
		}

		if ( it != m_state.boundDescriptors.begin() )
		{
			m_state.boundDescriptors.erase( m_state.boundDescriptors.begin(), it );
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
			binding.buffers.push_back( buffers[i] );
			binding.offsets.push_back( uint32_t( offsets[i] ) );
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
		m_state.newlyBoundIbo = buffer;
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
			doProcessMappedBoundDescriptorResourcesIn( descriptorSet );
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
			doProcessMappedBoundVaoBuffersIn();
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
			doProcessMappedBoundVaoBuffersIn();
			m_commands.emplace_back( std::make_unique< DrawCommand >( m_device
				, vtxCount
				, instCount
				, firstVertex
				, firstInstance
				, get( m_state.currentPipeline )->getInputAssemblyState().topology
				, m_state.vbos ) );
		}

		doProcessMappedBoundDescriptorsResourcesOut();
	}

	void CommandBuffer::drawIndexed( uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance )const
	{
		if ( !get( m_state.currentPipeline )->hasVertexLayout()
			&& m_state.newlyBoundIbo == VK_NULL_HANDLE )
		{
			bindIndexBuffer( get( m_device )->getEmptyIndexedVaoIdx(), 0u, VK_INDEX_TYPE_UINT32 );
		}

		doFillVboStrides();
		doProcessMappedBoundVaoBuffersIn();
		m_commands.emplace_back( std::make_unique< DrawIndexedCommand >( m_device
			, indexCount
			, instCount
			, firstIndex
			, vertexOffset
			, firstInstance
			, get( m_state.currentPipeline )->getInputAssemblyState().topology
			, m_state.indexType
			, m_state.vbos ) );
		doProcessMappedBoundDescriptorsResourcesOut();
		m_state.newlyBoundIbo = VK_NULL_HANDLE;
	}

	void CommandBuffer::drawIndirect( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		doFillVboStrides();
		doProcessMappedBoundVaoBuffersIn();
		m_commands.emplace_back( std::make_unique< DrawIndirectCommand >( m_device
			, buffer
			, offset
			, drawCount
			, stride
			, get( m_state.currentPipeline )->getInputAssemblyState().topology
			, m_state.vbos ) );
		doProcessMappedBoundDescriptorsResourcesOut();
	}

	void CommandBuffer::drawIndexedIndirect( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		if ( !get( m_state.currentPipeline )->hasVertexLayout()
			&& m_state.newlyBoundIbo == VK_NULL_HANDLE )
		{
			bindIndexBuffer( get( m_device )->getEmptyIndexedVaoIdx(), 0u, VK_INDEX_TYPE_UINT32 );
		}

		doFillVboStrides();
		doProcessMappedBoundVaoBuffersIn();
		m_commands.emplace_back( std::make_unique< DrawIndexedIndirectCommand >( m_device
			, buffer
			, offset
			, drawCount
			, stride
			, get( m_state.currentPipeline )->getInputAssemblyState().topology
			, m_state.indexType
			, m_state.vbos ) );
		doProcessMappedBoundDescriptorsResourcesOut();
		m_state.newlyBoundIbo = VK_NULL_HANDLE;
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
		m_commands.emplace_back( std::make_unique< BlitImageCommand >( m_commandPool
			, get( this )
			, m_device
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
				, desc ) );
			doAddAfterSubmitAction();
		}
		else if ( m_state.currentComputePipeline )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, desc ) );
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
		doProcessMappedBoundDescriptorsResourcesOut();
	}

	void CommandBuffer::dispatchIndirect( VkBuffer buffer
		, VkDeviceSize offset )const
	{
		m_commands.emplace_back( std::make_unique< DispatchIndirectCommand >( m_device
			, buffer
			, offset ) );
		doProcessMappedBoundDescriptorsResourcesOut();
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
			, [&command]()
			{
				command.remove();
			} );
	}

	void CommandBuffer::doProcessMappedBoundDescriptorResourcesIn( VkDescriptorSet descriptor )const
	{
		for ( auto & writes : get( descriptor )->getDynamicBuffers() )
		{
			for ( auto & write : writes->writes )
			{
				for ( auto & info : makeArrayView( write.pBufferInfo, write.descriptorCount ) )
				{
					doProcessMappedBoundResourceIn( info.buffer, info.offset, info.range );
				}
			}
		}

		for ( auto & writes : get( descriptor )->getStorageBuffers() )
		{
			for ( auto & write : writes->writes )
			{
				for ( auto & info : makeArrayView( write.pBufferInfo, write.descriptorCount ) )
				{
					doProcessMappedBoundResourceIn( info.buffer, info.offset, info.range );
				}
			}
		}
		
		for ( auto & writes : get( descriptor )->getStorageTextures() )
		{
			for ( auto & write : writes->writes )
			{
				for ( auto & info : makeArrayView( write.pImageInfo, write.descriptorCount ) )
				{
					doProcessMappedBoundResourceIn( info.imageView );
				}
			}
		}

		for ( auto & writes : get( descriptor )->getUniformBuffers() )
		{
			for ( auto & write : writes->writes )
			{
				for ( auto & info : makeArrayView( write.pBufferInfo, write.descriptorCount ) )
				{
					doProcessMappedBoundResourceIn( info.buffer, info.offset, info.range );
				}
			}
		}
	}

	void CommandBuffer::doProcessMappedBoundDescriptorsResourcesOut()const
	{
		for ( auto descriptor : m_state.boundDescriptors )
		{
			for ( auto & writes : get( descriptor )->getDynamicStorageBuffers() )
			{
				for ( auto & write : writes->writes )
				{
					for ( auto & info : makeArrayView( write.pBufferInfo, write.descriptorCount ) )
					{
						doProcessMappedBoundResourceOut( info.buffer, info.offset, info.range );
					}
				}
			}

			for ( auto & writes : get( descriptor )->getStorageBuffers() )
			{
				for ( auto & write : writes->writes )
				{
					for ( auto & info : makeArrayView( write.pBufferInfo, write.descriptorCount ) )
					{
						doProcessMappedBoundResourceOut( info.buffer, info.offset, info.range );
					}
				}
			}

			for ( auto & writes : get( descriptor )->getStorageTextures() )
			{
				for ( auto & write : writes->writes )
				{
					for ( auto & info : makeArrayView( write.pImageInfo, write.descriptorCount ) )
					{
						doProcessMappedBoundResourceOut( info.imageView );
					}
				}
			}
		}
	}

	void CommandBuffer::doProcessMappedBoundVaoBuffersIn()const
	{
		for ( auto & vbo : m_state.vbos )
		{
			for ( auto & buffer : vbo.buffers )
			{
				doProcessMappedBoundResourceIn( buffer
					, 0u
					, get( buffer )->getSize() );
			}
		}
	}

	void CommandBuffer::doProcessMappedBoundResourceIn( VkBuffer buffer
		, VkDeviceSize offset
		, VkDeviceSize range )const
	{
		auto buf = get( buffer );

		if ( get( buf->getMemory() )->isMapped() )
		{
			doAddMappedResource( &buf->getObjectMemory()
				, offset
				, range
				, true );
		}
	}

	void CommandBuffer::doProcessMappedBoundResourceIn( VkImageView image )const
	{
		auto img = get( get( image )->getImage() );

		//if ( get( img->getMemory() )->isMapped() )
		{
			auto & range = get( image )->getSubResourceRange();
			auto layerSize = getLevelsSize( img->getDimensions()
				, img->getFormat()
				, 0u
				, img->getMipmapLevels() );
			auto size = getLevelsSize( img->getDimensions()
				, img->getFormat()
				, range.baseMipLevel
				, range.levelCount );
			auto offset = img->getMemoryOffset();

			for ( auto layer = range.baseArrayLayer; layer < range.baseArrayLayer + range.layerCount; ++layer )
			{
				doAddMappedResource( &img->getObjectMemory()
					, offset
					, size
					, true );
				offset += layerSize;
			}
		}
	}

	void CommandBuffer::doProcessMappedBoundResourceOut( VkBuffer buffer
		, VkDeviceSize offset
		, VkDeviceSize range )const
	{
		auto buf = get( buffer );

		if ( get( buf->getMemory() )->isMapped() )
		{
			doAddMappedResource( &buf->getObjectMemory()
				, offset
				, range
				, false );
		}
	}

	void CommandBuffer::doProcessMappedBoundResourceOut( VkImageView image )const
	{
		auto img = get( get( image )->getImage() );

		//if ( get( img->getMemory() )->isMapped() )
		{
			auto & range = get( image )->getSubResourceRange();
			auto layerSize = getLevelsSize( img->getDimensions()
				, img->getFormat()
				, 0u
				, img->getMipmapLevels() );
			auto size = getLevelsSize( img->getDimensions()
				, img->getFormat()
				, range.baseMipLevel
				, range.levelCount );
			auto offset = img->getMemoryOffset();

			for ( auto layer = range.baseArrayLayer; layer < range.baseArrayLayer + range.layerCount; ++layer )
			{
				doAddMappedResource( &img->getObjectMemory()
					, offset
					, size
					, false );
				offset += layerSize;
			}
		}
	}

	CommandBuffer::ResourceIndex & CommandBuffer::doAddMappedResource( ObjectMemory const * memory
		, VkDeviceSize offset
		, VkDeviceSize range
		, bool isInput )const
	{
		if ( isInput )
		{
			m_commands.emplace_back( std::make_unique< UploadMemoryCommand >( m_device
				, memory
				, offset
				, range ) );
		}
		else
		{
			m_commands.emplace_back( std::make_unique< DownloadMemoryCommand >( m_device
				, memory
				, offset
				, range ) );
		}

		return doAddMappedResource( memory );
	}

	CommandBuffer::ResourceIndex & CommandBuffer::doAddMappedResource( ObjectMemory const * memory )const
	{
		auto it = std::find_if( m_mappedResources.begin()
			, m_mappedResources.end()
			, [memory]( ResourceIndex const & lookup )
		{
			return lookup.memory == memory;
		} );

		ResourceIndex * result{ nullptr };

		if ( it == m_mappedResources.end() )
		{
			result = &m_mappedResources.emplace_back( memory
				, m_commands.size() - 1u
				, get( memory->deviceMemory )->onDestroy.connect( [this, memory]( VkDeviceMemory deviceMemory )
					{
						doRemoveMappedResource( memory );
					} ) );
		}
		else
		{
			result = &( *it );
		}

		return *result;
	}

	void CommandBuffer::doRemoveMappedResource( ObjectMemory const * memory )const
	{
		auto it = std::find_if( m_mappedResources.begin()
			, m_mappedResources.end()
			, [memory]( ResourceIndex const & lookup )
		{
			return lookup.memory == memory;
		} );

		if ( it != m_mappedResources.end() )
		{
			auto index = it->index;
			m_commands.erase( m_commands.begin() + index );
			it = m_mappedResources.erase( it );

			while ( it != m_mappedResources.end() )
			{
				it->index--;
				++it;
			}
		}
	}

	//*********************************************************************************************
}
