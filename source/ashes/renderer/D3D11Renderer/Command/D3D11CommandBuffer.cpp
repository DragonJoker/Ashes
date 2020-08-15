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
#include "Command/Commands/D3D11ClearAttachmentsCommand.hpp"
#include "Command/Commands/D3D11ClearColourCommand.hpp"
#include "Command/Commands/D3D11ClearDepthStencilCommand.hpp"
#include "Command/Commands/D3D11CopyBufferCommand.hpp"
#include "Command/Commands/D3D11CopyBufferToImageCommand.hpp"
#include "Command/Commands/D3D11CopyImageCommand.hpp"
#include "Command/Commands/D3D11CopyImageToBufferCommand.hpp"
#include "Command/Commands/D3D11CopyQueryPoolResultsCommand.hpp"
#include "Command/Commands/D3D11DispatchCommand.hpp"
#include "Command/Commands/D3D11DispatchIndirectCommand.hpp"
#include "Command/Commands/D3D11DownloadMemoryCommand.hpp"
#include "Command/Commands/D3D11DrawCommand.hpp"
#include "Command/Commands/D3D11DrawIndexedCommand.hpp"
#include "Command/Commands/D3D11DrawIndexedIndirectCommand.hpp"
#include "Command/Commands/D3D11DrawIndirectCommand.hpp"
#include "Command/Commands/D3D11EndQueryCommand.hpp"
#include "Command/Commands/D3D11EndRenderPassCommand.hpp"
#include "Command/Commands/D3D11EndSubpassCommand.hpp"
#include "Command/Commands/D3D11ExecuteActionsCommand.hpp"
#include "Command/Commands/D3D11ExecuteCommandsCommand.hpp"
#include "Command/Commands/D3D11FillBufferCommand.hpp"
#include "Command/Commands/D3D11GenerateMipsCommand.hpp"
#include "Command/Commands/D3D11MemoryBarrierCommand.hpp"
#include "Command/Commands/D3D11PushConstantsCommand.hpp"
#include "Command/Commands/D3D11ResetEventCommand.hpp"
#include "Command/Commands/D3D11ResetQueryPoolCommand.hpp"
#include "Command/Commands/D3D11ResolveImageCommand.hpp"
#include "Command/Commands/D3D11ScissorCommand.hpp"
#include "Command/Commands/D3D11SetDepthBiasCommand.hpp"
#include "Command/Commands/D3D11SetEventCommand.hpp"
#include "Command/Commands/D3D11SetLineWidthCommand.hpp"
#include "Command/Commands/D3D11UpdateBufferCommand.hpp"
#include "Command/Commands/D3D11UploadMemoryCommand.hpp"
#include "Command/Commands/D3D11ViewportCommand.hpp"
#include "Command/Commands/D3D11WaitEventsCommand.hpp"
#include "Command/Commands/D3D11WriteTimestampCommand.hpp"

#include "ashesd3d11_api.hpp"

#define AshesD3D_UseCommandsList 0

namespace ashes::d3d11
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
		m_mappedResources.clear();
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

	void CommandBuffer::executeCommands( ArrayView< VkCommandBuffer const > commands )const
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

			m_commands.emplace_back( std::make_unique< ExecuteActionsCommand >( m_device
				, dxCommandBuffer.m_afterSubmitActions ) );
		}

#endif
	}

	void CommandBuffer::clearColorImage( VkImage image
		, VkImageLayout imageLayout
		, VkClearColorValue colour
		, ArrayView< VkImageSubresourceRange const > ranges )const
	{
		m_commands.emplace_back( std::make_unique< ClearColourCommand >( m_device
			, image
			, ranges
			, colour ) );
	}

	void CommandBuffer::clearDepthStencilImage( VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, ArrayView< VkImageSubresourceRange const > ranges )const
	{
		m_commands.emplace_back( std::make_unique< ClearDepthStencilCommand >( m_device
			, image
			, ranges
			, value ) );
	}

	void CommandBuffer::clearAttachments( ArrayView< VkClearAttachment const > clearAttachments
		, ArrayView< VkClearRect const > clearRects )
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
			doApplyDynamicStates();
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
		, ArrayView< VkBuffer const > buffers
		, ArrayView< uint64_t const > offsets )const
	{
		assert( buffers.size() == offsets.size() );
		VbosBinding binding;
		binding.startIndex = firstBinding;

		for ( auto i = 0u; i < buffers.size(); ++i )
		{
			binding.buffers.push_back( buffers[i] );
			binding.d3dBuffers.push_back( get( buffers[i] )->getBuffer() );
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
		m_state.newlyBoundIbo = buffer;
	}

	void CommandBuffer::bindDescriptorSets( VkPipelineBindPoint bindingPoint
		, VkPipelineLayout layout
		, uint32_t firstSet
		, ArrayView< VkDescriptorSet const > descriptorSets
		, ArrayView< uint32_t const > dynamicOffsets )const
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
		, ArrayView< VkViewport const > viewports )const
	{
		m_commands.emplace_back( std::make_unique< ViewportCommand >( m_device, firstViewport, viewports ) );
	}

	void CommandBuffer::setScissor( uint32_t firstScissor
		, ArrayView< VkRect2D const > scissors )const
	{
		m_commands.emplace_back( std::make_unique< ScissorCommand >( m_device, firstScissor, scissors ) );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		doBindDynamicStates();
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

		doBindDynamicStates();
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
		doBindDynamicStates();
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
		doBindDynamicStates();
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
		, ArrayView< VkBufferImageCopy const > copyInfos )const
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
		, ArrayView< VkBufferImageCopy const > copyInfos )const
	{
		m_commands.emplace_back( std::make_unique< CopyImageToBufferCommand >( m_device
			, std::move( copyInfos )
			, src
			, dst ) );
	}

	void CommandBuffer::updateBuffer( VkBuffer dstBuffer
		, VkDeviceSize dstOffset
		, ArrayView< uint8_t const > data )
	{
		m_commands.emplace_back( std::make_unique< UpdateBufferCommand >( m_device
			, dstBuffer
			, dstOffset
			, data ) );
	}

	void CommandBuffer::fillBuffer( VkBuffer dstBuffer
		, VkDeviceSize dstOffset
		, VkDeviceSize size
		, uint32_t data )
	{
		m_commands.emplace_back( std::make_unique< FillBufferCommand >( m_device
			, dstBuffer
			, dstOffset
			, size
			, data ) );
	}

	void CommandBuffer::copyBuffer( VkBuffer src
		, VkBuffer dst
		, ArrayView< VkBufferCopy const > copyInfos )const
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
		, ArrayView< VkImageCopy const > copyInfos )const
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
		, ArrayView< VkImageBlit const > regions
		, VkFilter filter )const
	{
		auto & pipeline = doGetBlitPipeline( get( srcImage )->getFormat()
			, get( srcImage )->getFormat() );
		m_commands.emplace_back( std::make_unique< BlitImageCommand >( m_commandPool
			, get( this )
			, m_device
			, pipeline
			, srcImage
			, dstImage
			, regions
			, filter ) );
	}

	void CommandBuffer::resolveImage( VkImage srcImage
		, VkImageLayout srcLayout
		, VkImage dstImage
		, VkImageLayout dstLayout
		, ArrayView< VkImageResolve const > regions )const
	{
		m_commands.emplace_back( std::make_unique< ResolveImageCommand >( m_device
			, srcImage
			, dstImage
			, regions ) );
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

	void CommandBuffer::copyQueryPoolResults( VkQueryPool queryPool
		, uint32_t firstQuery
		, uint32_t queryCount
		, VkBuffer dstBuffer
		, VkDeviceSize dstOffset
		, VkDeviceSize stride
		, VkQueryResultFlags flags )const
	{
		m_commands.emplace_back( std::make_unique< CopyQueryPoolResultsCommand >( m_device
			, queryPool
			, firstQuery
			, queryCount
			, dstBuffer
			, dstOffset
			, stride
			, flags ) );
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
		m_state.lineWidth = width;
		doApplyDynamicStates();
	}

	void CommandBuffer::setDepthBias( float constantFactor
		, float clamp
		, float slopeFactor )const
	{
		m_state.depthBias = DepthBias{ constantFactor, clamp, slopeFactor };
		doApplyDynamicStates();
	}

	void CommandBuffer::setBlendConstants( const float blendConstants[4] )
	{
		m_state.blendConstants = std::array< float, 4u >{};
		m_state.blendConstants.value()[0] = blendConstants[0];
		m_state.blendConstants.value()[1] = blendConstants[1];
		m_state.blendConstants.value()[2] = blendConstants[2];
		m_state.blendConstants.value()[3] = blendConstants[3];
		doApplyDynamicStates();
	}

	void CommandBuffer::setDepthBounds( float minDepthBounds
		, float maxDepthBounds )
	{
		m_state.depthBounds = DepthBounds{ minDepthBounds, maxDepthBounds };
		doApplyDynamicStates();
	}

	void CommandBuffer::setStencilCompareMask( VkStencilFaceFlags faceMask
		, uint32_t compareMask )
	{
		if ( checkFlag( faceMask, VK_STENCIL_FACE_BACK_BIT ) )
		{
			m_state.stencilCompareMask[0] = compareMask;
		}

		if ( checkFlag( faceMask, VK_STENCIL_FACE_FRONT_BIT ) )
		{
			m_state.stencilCompareMask[1] = compareMask;
		}

		doApplyDynamicStates();
	}

	void CommandBuffer::setStencilWriteMask( VkStencilFaceFlags faceMask
		, uint32_t writeMask )
	{
		if ( checkFlag( faceMask, VK_STENCIL_FACE_BACK_BIT ) )
		{
			m_state.stencilWriteMask[0] = writeMask;
		}

		if ( checkFlag( faceMask, VK_STENCIL_FACE_FRONT_BIT ) )
		{
			m_state.stencilWriteMask[1] = writeMask;
		}

		doApplyDynamicStates();
	}

	void CommandBuffer::setStencilReference( VkStencilFaceFlags faceMask
		, uint32_t reference )
	{
		if ( checkFlag( faceMask, VK_STENCIL_FACE_BACK_BIT ) )
		{
			m_state.stencilReference[0] = reference;
		}

		if ( checkFlag( faceMask, VK_STENCIL_FACE_FRONT_BIT ) )
		{
			m_state.stencilReference[1] = reference;
		}

		doApplyDynamicStates();
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

	void CommandBuffer::waitEvents( ArrayView< VkEvent const > events
		, VkPipelineStageFlags srcStageMask
		, VkPipelineStageFlags dstStageMask
		, ArrayView< VkMemoryBarrier const > memoryBarriers
		, ArrayView< VkBufferMemoryBarrier const > bufferMemoryBarriers
		, ArrayView< VkImageMemoryBarrier const > imageMemoryBarriers )const
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
		, ArrayView< VkMemoryBarrier const > memoryBarriers
		, ArrayView< VkBufferMemoryBarrier const > bufferMemoryBarriers
		, ArrayView< VkImageMemoryBarrier const > imageMemoryBarriers )const
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
			, [&command]( Context const & context )
			{
				command.remove( context );
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
				, 0u
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
				, img->getMipmapLevels()
				, uint32_t( img->getMemoryAlignment() ) );
			auto size = getLevelsSize( img->getDimensions()
				, img->getFormat()
				, range.baseMipLevel
				, range.levelCount
				, uint32_t( img->getMemoryAlignment() ) );
			auto offset = img->getMemoryOffset();

			for ( auto layer = range.baseArrayLayer; layer < range.baseArrayLayer + range.layerCount; ++layer )
			{
				doAddMappedResource( &img->getObjectMemory()
					, offset
					, size
					, get( image )->getSubresource( layer )
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
				, 0u
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
				, img->getMipmapLevels()
				, uint32_t( img->getMemoryAlignment() ) );
			auto size = getLevelsSize( img->getDimensions()
				, img->getFormat()
				, range.baseMipLevel
				, range.levelCount
				, uint32_t( img->getMemoryAlignment() ) );
			auto offset = img->getMemoryOffset();

			for ( auto layer = range.baseArrayLayer; layer < range.baseArrayLayer + range.layerCount; ++layer )
			{
				doAddMappedResource( &img->getObjectMemory()
					, offset
					, size
					, get( image )->getSubresource( layer )
					, false );
				offset += layerSize;
			}
		}
	}

	CommandBuffer::ResourceIndex & CommandBuffer::doAddMappedResource( ObjectMemory const * memory
		, VkDeviceSize offset
		, VkDeviceSize range
		, UINT subresource
		, bool isInput )const
	{
		if ( isInput )
		{
			m_commands.emplace_back( std::make_unique< UploadMemoryCommand >( m_device
				, memory
				, offset
				, range
				, subresource ) );
		}
		else
		{
			m_commands.emplace_back( std::make_unique< DownloadMemoryCommand >( m_device
				, memory
				, offset
				, range
				, subresource ) );
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

	BlitPipeline const & CommandBuffer::doGetBlitPipeline( VkFormat src, VkFormat dst )const
	{
		auto hash = makeHash( src, dst );
		auto it = m_blitPipelines.find( hash );

		if ( it == m_blitPipelines.end() )
		{
			it = m_blitPipelines.emplace( hash
				, std::make_unique< BlitPipeline >( *get( m_device )
					, src
					, dst ) ).first;
		}

		return *it->second;
	}

	void CommandBuffer::doApplyDynamicStates()const
	{
		if ( m_state.currentPipeline )
		{
			get( m_state.currentPipeline )->getDynamicStates().setLineWidth( m_state.lineWidth );
			get( m_state.currentPipeline )->getDynamicStates().setDepthBias( m_state.depthBias );
			get( m_state.currentPipeline )->getDynamicStates().setBlendConstants( m_state.blendConstants );
			get( m_state.currentPipeline )->getDynamicStates().setDepthBounds( m_state.depthBounds );
			get( m_state.currentPipeline )->getDynamicStates().setStencilCompareMask( m_state.stencilCompareMask );
			get( m_state.currentPipeline )->getDynamicStates().setStencilWriteMask( m_state.stencilWriteMask );
			get( m_state.currentPipeline )->getDynamicStates().setStencilReference( m_state.stencilReference );
			get( m_state.currentPipeline )->update();
		}
	}

	void CommandBuffer::doBindDynamicStates()const
	{
		assert( m_state.currentPipeline );

		if ( get( m_state.currentPipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK )
			|| get( m_state.currentPipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_WRITE_MASK )
			|| get( m_state.currentPipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_REFERENCE ) )
		{
			m_commands.emplace_back( std::make_unique< BindDepthStencilStateCommand >( m_device
				, m_state.currentPipeline ) );
		}

		if ( get( m_state.currentPipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_DEPTH_BIAS ) )
		{
			assert( m_state.depthBias );
			m_commands.emplace_back( std::make_unique< BindRasterizerStateCommand >( m_device
				, m_state.currentPipeline ) );
		}

		if ( get( m_state.currentPipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_BLEND_CONSTANTS ) )
		{
			assert( m_state.blendConstants );
			m_commands.emplace_back( std::make_unique< BindBlendStateCommand >( m_device
				, m_state.currentPipeline
				, m_state.blendConstants.value() ) );
		}

		if ( get( m_state.currentPipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_DEPTH_BOUNDS ) )
		{
			assert( m_state.depthBounds );
			m_commands.emplace_back( std::make_unique< SetDepthBoundsCommand >( m_device
				, m_state.depthBounds.value() ) );
		}
	}

	//*********************************************************************************************
}
