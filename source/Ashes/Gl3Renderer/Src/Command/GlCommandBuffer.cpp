/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/GlCommandBuffer.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Buffer/GlGeometryBuffers.hpp"
#include "Buffer/GlUniformBuffer.hpp"
#include "Command/GlCommandPool.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlRenderer.hpp"
#include "Descriptor/GlDescriptorSet.hpp"
#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"
#include "Pipeline/GlComputePipeline.hpp"
#include "Pipeline/GlPipeline.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include "Commands/GlBeginQueryCommand.hpp"
#include "Commands/GlBeginRenderPassCommand.hpp"
#include "Commands/GlBeginSubpassCommand.hpp"
#include "Commands/GlBindComputePipelineCommand.hpp"
#include "Commands/GlBindDescriptorSetCommand.hpp"
#include "Commands/GlBindGeometryBuffersCommand.hpp"
#include "Commands/GlBindPipelineCommand.hpp"
#include "Commands/GlBlitImageCommand.hpp"
#include "Commands/GlClearAttachmentsCommand.hpp"
#include "Commands/GlClearColourCommand.hpp"
#include "Commands/GlClearColourFboCommand.hpp"
#include "Commands/GlClearDepthStencilCommand.hpp"
#include "Commands/GlClearDepthStencilFboCommand.hpp"
#include "Commands/GlCopyBufferCommand.hpp"
#include "Commands/GlCopyBufferToImageCommand.hpp"
#include "Commands/GlCopyImageCommand.hpp"
#include "Commands/GlCopyImageToBufferCommand.hpp"
#include "Commands/GlDispatchCommand.hpp"
#include "Commands/GlDispatchIndirectCommand.hpp"
#include "Commands/GlDrawCommand.hpp"
#include "Commands/GlDrawIndexedCommand.hpp"
#include "Commands/GlDrawIndexedIndirectCommand.hpp"
#include "Commands/GlDrawIndirectCommand.hpp"
#include "Commands/GlEndQueryCommand.hpp"
#include "Commands/GlEndRenderPassCommand.hpp"
#include "Commands/GlEndSubpassCommand.hpp"
#include "Commands/GlGenerateMipmapsCommand.hpp"
#include "Commands/GlMemoryBarrierCommand.hpp"
#include "Commands/GlPushConstantsCommand.hpp"
#include "Commands/GlResetEventCommand.hpp"
#include "Commands/GlResetQueryPoolCommand.hpp"
#include "Commands/GlScissorCommand.hpp"
#include "Commands/GlSetDepthBiasCommand.hpp"
#include "Commands/GlSetEventCommand.hpp"
#include "Commands/GlSetLineWidthCommand.hpp"
#include "Commands/GlViewportCommand.hpp"
#include "Commands/GlWaitEventsCommand.hpp"
#include "Commands/GlWriteTimestampCommand.hpp"

#include <Buffer/StagingBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>

#include <algorithm>

namespace gl_renderer
{
	namespace
	{
		ashes::TextureView const & getView( ashes::WriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.imageInfo.size() );
			return write.imageInfo[index].imageView.value().get();
		}
	}

	CommandBuffer::CommandBuffer( Device const & device
		, ashes::CommandPool const & pool
		, bool primary )
		: ashes::CommandBuffer{ device, pool, primary }
		, m_device{ device }
	{
	}

	void CommandBuffer::applyPostSubmitActions( ContextLock const & context )const
	{
		for ( auto & action : m_afterSubmitActions )
		{
			action( context );
		}
	}

	void CommandBuffer::generateMipmaps( Texture const & texture )const
	{
		m_commands.emplace_back( std::make_unique< GenerateMipmapsCommand >( m_device
			, texture ) );
	}

	void CommandBuffer::begin( ashes::CommandBufferBeginInfo const & info )const
	{
		m_afterSubmitActions.clear();
		m_commands.clear();
		m_state = State{};
		m_state.beginFlags = info.flags;
	}

	void CommandBuffer::end()const
	{
		m_state.pushConstantBuffers.clear();
	}

	void CommandBuffer::reset( ashes::CommandBufferResetFlags flags )const
	{
		m_afterSubmitActions.clear();
		m_commands.clear();
	}

	void CommandBuffer::beginRenderPass( ashes::RenderPassBeginInfo const & beginInfo
		, ashes::SubpassContents contents )const
	{
		m_state.currentRenderPass = static_cast< RenderPass const * >( beginInfo.renderPass );
		m_state.currentFrameBuffer = beginInfo.framebuffer;
		m_state.currentSubpassIndex = 0u;
		m_state.currentSubpass = &m_state.currentRenderPass->getSubpasses()[m_state.currentSubpassIndex++];
		m_commands.emplace_back( std::make_unique< BeginRenderPassCommand >( m_device
			, *beginInfo.renderPass
			, *beginInfo.framebuffer
			, beginInfo.clearValues
			, contents
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
		m_state.boundVbos.clear();
	}

	void CommandBuffer::endRenderPass()const
	{
		m_commands.emplace_back( std::make_unique< EndSubpassCommand >( m_device
			, *m_state.currentFrameBuffer
			, *m_state.currentSubpass ) );
		m_commands.emplace_back( std::make_unique< EndRenderPassCommand >( m_device ) );
		m_state.boundVbos.clear();
	}

	void CommandBuffer::executeCommands( ashes::CommandBufferCRefArray const & commands )const
	{
		for ( auto & commandBuffer : commands )
		{
			auto & glCommandBuffer = static_cast< CommandBuffer const & >( commandBuffer.get() );
			glCommandBuffer.initialiseGeometryBuffers();

			for ( auto & command : glCommandBuffer.getCommands() )
			{
				m_commands.emplace_back( command->clone() );
			}

			m_afterSubmitActions.insert( m_afterSubmitActions.end()
				, glCommandBuffer.m_afterSubmitActions.begin()
				, glCommandBuffer.m_afterSubmitActions.end() );
		}
	}

	void CommandBuffer::clear( ashes::TextureView const & image
		, ashes::ClearColorValue const & colour )const
	{
		if ( !m_device.getRenderer().getFeatures().hasClearTexImage )
		{
			m_commands.emplace_back( std::make_unique< ClearColourFboCommand >( m_device
				, image
				, colour ) );
		}
		else
		{
			m_commands.emplace_back( std::make_unique< ClearColourCommand >( m_device
				, image
				, colour ) );
		}
	}

	void CommandBuffer::clear( ashes::TextureView const & image
		, ashes::DepthStencilClearValue const & value )const
	{
		if ( !m_device.getRenderer().getFeatures().hasClearTexImage )
		{
			m_commands.emplace_back( std::make_unique< ClearDepthStencilFboCommand >( m_device
				, image
				, value ) );
		}
		else
		{
			m_commands.emplace_back( std::make_unique< ClearDepthStencilCommand >( m_device
				, image
				, value ) );
		}
	}

	void CommandBuffer::clearAttachments( ashes::ClearAttachmentArray const & clearAttachments
		, ashes::ClearRectArray const & clearRects )
	{
		m_commands.emplace_back( std::make_unique< ClearAttachmentsCommand >( m_device
			, clearAttachments
			, clearRects ) );
	}

	void CommandBuffer::bindPipeline( ashes::Pipeline const & pipeline
		, ashes::PipelineBindPoint bindingPoint )const
	{
		if ( m_state.currentPipeline )
		{
			auto src = m_state.currentPipeline->getVertexInputStateHash();
			auto dst = static_cast< Pipeline const & >( pipeline ).getVertexInputStateHash();

			if ( src != dst )
			{
				IboBinding empty{};
				m_state.boundIbo.swap( empty );
				m_state.boundVbos.clear();
			}
		}

		m_state.currentPipeline = &static_cast< Pipeline const & >( pipeline );
		m_commands.emplace_back( std::make_unique< BindPipelineCommand >( m_device
			, pipeline
			, bindingPoint ) );

		for ( auto & pcb : m_state.pushConstantBuffers )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, pcb.second ) );
		}

		m_state.pushConstantBuffers.clear();

		m_afterSubmitActions.insert( m_afterSubmitActions.begin()
			, [this]( ContextLock const & context )
			{
				glLogCall( context
					, glUseProgram
					, 0u );
			} );
	}

	void CommandBuffer::bindPipeline( ashes::ComputePipeline const & pipeline
		, ashes::PipelineBindPoint bindingPoint )const
	{
		if ( m_device.getRenderer().getFeatures().hasComputeShaders )
		{
			m_state.currentComputePipeline = &static_cast< ComputePipeline const & >( m_device
				, pipeline );
			m_commands.emplace_back( std::make_unique< BindComputePipelineCommand >( m_device
				, pipeline
				, bindingPoint ) );

			for ( auto & pcb : m_state.pushConstantBuffers )
			{
				m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
					, pcb.second ) );
			}

			m_state.pushConstantBuffers.clear();

			m_afterSubmitActions.insert( m_afterSubmitActions.begin()
				, [this]( ContextLock const & context )
				{
					glLogCall( context
						, glUseProgram
						, 0u );
				} );
		}
		else
		{
			ashes::Logger::logWarning( "Compute shaders are not supported" );
		}
	}

	void CommandBuffer::bindVertexBuffers( uint32_t firstBinding
		, ashes::BufferCRefArray const & buffers
		, ashes::UInt64Array offsets )const
	{
		assert( buffers.size() == offsets.size() );
		uint32_t binding = firstBinding;

		for ( auto i = 0u; i < buffers.size(); ++i )
		{
			auto & glBuffer = static_cast< Buffer const & >( buffers[i].get() );
			m_state.boundVbos[binding] = { glBuffer.getBuffer(), offsets[i], &glBuffer };
			++binding;
		}

		m_state.boundVao = nullptr;
	}

	void CommandBuffer::bindIndexBuffer( ashes::BufferBase const & buffer
		, uint64_t offset
		, ashes::IndexType indexType )const
	{
		auto & glBuffer = static_cast< Buffer const & >( buffer );
		m_state.boundIbo = BufferObjectBinding{ glBuffer.getBuffer(), offset, &glBuffer };
		m_state.indexType = indexType;
		m_state.boundVao = nullptr;
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

			auto & glDescriptorSet = static_cast< DescriptorSet const & >( descriptorSet.get() );

			for ( auto & write : glDescriptorSet.getCombinedTextureSamplers() )
			{
				for ( auto i = 0u; i < write.imageInfo.size(); ++i )
				{
					uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
					auto & view = getView( write, i );
					auto type = convert( view.getType() );
					m_afterSubmitActions.insert( m_afterSubmitActions.begin()
						, [this, type, i, bindingIndex]( ContextLock const & context )
						{
							glLogCall( context
								, glActiveTexture
								, GlTextureUnit( GL_TEXTURE0 + bindingIndex ) );
							glLogCall( context
								, glBindTexture
								, type
								, 0u );
							glLogCall( context
								, glBindSampler
								, bindingIndex
								, 0u );
						} );
				}
			}

			for ( auto & write : glDescriptorSet.getSampledTextures() )
			{
				for ( auto i = 0u; i < write.imageInfo.size(); ++i )
				{
					uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
					auto & view = getView( write, i );
					auto type = convert( view.getType() );
					m_afterSubmitActions.insert( m_afterSubmitActions.begin()
						, [this, type, i, bindingIndex]( ContextLock const & context )
						{
							glLogCall( context
								, glActiveTexture
								, GlTextureUnit( GL_TEXTURE0 + bindingIndex ) );
							glLogCall( context
								, glBindTexture
								, type
								, 0u );
						} );
				}
			}
		}
	}

	void CommandBuffer::setViewport( uint32_t firstViewport
		, ashes::ViewportArray const & viewports )const
	{
		m_commands.emplace_back( std::make_unique< ViewportCommand >( m_device
			, firstViewport
			, viewports ) );
	}

	void CommandBuffer::setScissor( uint32_t firstScissor
		, ashes::ScissorArray const & scissors )const
	{
		m_commands.emplace_back( std::make_unique< ScissorCommand >( m_device
			, firstScissor
			, scissors ) );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		if ( !m_state.currentPipeline->hasVertexLayout() )
		{
			bindIndexBuffer( m_device.getEmptyIndexedVaoIdx(), 0u, ashes::IndexType::eUInt32 );
			m_state.boundVao = &m_device.getEmptyIndexedVao();
			m_commands.emplace_back( std::make_unique< BindGeometryBuffersCommand >( m_device
				, *m_state.boundVao ) );
			m_commands.emplace_back( std::make_unique< DrawIndexedCommand >( m_device
				, vtxCount
				, instCount
				, 0u
				, firstVertex
				, firstInstance
				, m_state.currentPipeline->getInputAssemblyState().topology
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
				, m_state.currentPipeline->getInputAssemblyState().topology ) );
		}

		m_afterSubmitActions.insert( m_afterSubmitActions.begin()
			, [this]( ContextLock const & context )
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
		if ( !m_state.currentPipeline->hasVertexLayout() )
		{
			bindIndexBuffer( m_device.getEmptyIndexedVaoIdx(), 0u, ashes::IndexType::eUInt32 );
			m_state.boundVao = &m_device.getEmptyIndexedVao();
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
			, m_state.currentPipeline->getInputAssemblyState().topology
			, m_state.indexType ) );

		m_afterSubmitActions.insert( m_afterSubmitActions.begin()
			, [this]( ContextLock const & context )
			{
				glLogCall( context
					, glBindVertexArray
					, 0u );
			} );
	}

	void CommandBuffer::drawIndirect( ashes::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		if ( !m_device.getPhysicalDevice().getFeatures().multiDrawIndirect )
		{
			throw std::runtime_error( "Draw indirect is not supported" );
		}

		if ( !m_state.boundVao )
		{
			doBindVao();
		}

		m_commands.emplace_back( std::make_unique< DrawIndirectCommand >( m_device
			, buffer
			, offset
			, drawCount
			, stride
			, m_state.currentPipeline->getInputAssemblyState().topology ) );

		m_afterSubmitActions.insert( m_afterSubmitActions.begin()
			, [this]( ContextLock const & context )
			{
				glLogCall( context
					, glBindVertexArray
					, 0u );
			} );
	}

	void CommandBuffer::drawIndexedIndirect( ashes::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride )const
	{
		if ( !m_device.getPhysicalDevice().getFeatures().multiDrawIndirect )
		{
			throw std::runtime_error( "Draw indirect is not supported" );
		}

		if ( !m_state.currentPipeline->hasVertexLayout() )
		{
			bindIndexBuffer( m_device.getEmptyIndexedVaoIdx(), 0u, ashes::IndexType::eUInt32 );
			m_state.boundVao = &m_device.getEmptyIndexedVao();
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
			, m_state.currentPipeline->getInputAssemblyState().topology
			, m_state.indexType ) );

		m_afterSubmitActions.insert( m_afterSubmitActions.begin()
			, [this]( ContextLock const & context )
			{
				glLogCall( context
					, glBindVertexArray
					, 0u );
			} );
	}

	void CommandBuffer::copyToImage( ashes::BufferImageCopyArray const & copyInfo
		, ashes::BufferBase const & src
		, ashes::Texture const & dst )const
	{
		m_commands.emplace_back( std::make_unique< CopyBufferToImageCommand >( m_device
			, copyInfo
			, src
			, dst ) );
	}

	void CommandBuffer::copyToBuffer( ashes::BufferImageCopyArray const & copyInfo
		, ashes::Texture const & src
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
		, ashes::Texture const & src
		, ashes::ImageLayout srcLayout
		, ashes::Texture const & dst
		, ashes::ImageLayout dstLayout )const
	{
		m_commands.emplace_back( std::make_unique< CopyImageCommand >( m_device
			, copyInfo
			, src
			, dst ) );
	}

	void CommandBuffer::blitImage( ashes::Texture const & srcImage
		, ashes::ImageLayout srcLayout
		, ashes::Texture const & dstImage
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
			{},
			{ reinterpret_cast< uint8_t const * >( data ), reinterpret_cast< uint8_t const * >( data ) + size }
		};

		if ( m_state.currentPipeline )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, m_state.currentPipeline->findPushConstantBuffer( desc ) ) );
		}
		else if ( m_state.currentComputePipeline )
		{
			m_commands.emplace_back( std::make_unique< PushConstantsCommand >( m_device
				, m_state.currentComputePipeline->findPushConstantBuffer( desc ) ) );
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
		if ( m_device.getRenderer().getFeatures().hasComputeShaders )
		{
			m_commands.emplace_back( std::make_unique< DispatchCommand >( m_device
				, groupCountX
				, groupCountY
				, groupCountZ ) );
		}
		else
		{
			ashes::Logger::logWarning( "Compute shaders are not supported" );
		}
	}

	void CommandBuffer::dispatchIndirect( ashes::BufferBase const & buffer
		, uint32_t offset )const
	{
		if ( m_device.getRenderer().getFeatures().hasComputeShaders )
		{
			m_commands.emplace_back( std::make_unique< DispatchIndirectCommand >( m_device
				, buffer
				, offset ) );
		}
		else
		{
			ashes::Logger::logWarning( "Compute shaders are not supported" );
		}
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
		, ashes::ImageMemoryBarrierArray const & imageMemoryBarriers )const
	{
		m_commands.emplace_back( std::make_unique< WaitEventsCommand >( m_device
			, events
			, srcStageMask
			, dstStageMask
			, bufferMemoryBarriers
			, imageMemoryBarriers ) );
	}

	void CommandBuffer::initialiseGeometryBuffers()const
	{
		for ( auto & vao : m_state.vaos )
		{
			vao.get().initialise();
		}

		m_state.vaos.clear();
	}

	void CommandBuffer::pipelineBarrier( ashes::PipelineStageFlags after
		, ashes::PipelineStageFlags before
		, ashes::DependencyFlags dependencyFlags
		, ashes::MemoryBarrierArray const & memoryBarriers
		, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
		, ashes::ImageMemoryBarrierArray const & imageMemoryBarriers )const
	{
		if ( m_device.getRenderer().getFeatures().hasImageTexture )
		{
			m_commands.emplace_back( std::make_unique< MemoryBarrierCommand >( m_device
				, after
				, before
				, dependencyFlags
				, memoryBarriers
				, bufferMemoryBarriers
				, imageMemoryBarriers ) );
		}
	}

	void CommandBuffer::doBindVao()const
	{
		m_state.boundVao = m_state.currentPipeline->findGeometryBuffers( m_state.boundVbos, m_state.boundIbo );

		if ( !m_state.boundVao )
		{
			m_state.boundVao = &m_state.currentPipeline->createGeometryBuffers( m_state.boundVbos, m_state.boundIbo, m_state.indexType ).get();
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
