/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkCommandBuffer.hpp"

#include "VkBuffer.hpp"
#include "VkBufferMemoryBarrier.hpp"
#include "VkClearValue.hpp"
#include "VkCommandPool.hpp"
#include "VkDescriptorSet.hpp"
#include "VkDevice.hpp"
#include "VkFrameBuffer.hpp"
#include "VkGeometryBuffers.hpp"
#include "VkImageMemoryBarrier.hpp"
#include "VkImageSubresourceRange.hpp"
#include "VkPipeline.hpp"
#include "VkPipelineLayout.hpp"
#include "VkRenderingResources.hpp"
#include "VkRenderPass.hpp"
#include "VkScissor.hpp"
#include "VkTexture.hpp"
#include "VkTextureView.hpp"
#include "VkUniformBuffer.hpp"
#include "VkViewport.hpp"

#include <Buffer/StagingBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>

namespace vk_renderer
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
	}

	CommandBuffer::CommandBuffer( Device const & device
		, CommandPool const & pool
		, bool primary )
		: m_device{ device }
		, m_pool{ pool }
	{
		VkCommandBufferAllocateInfo cmdAllocInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			nullptr,
			m_pool,                                   // commandPool
			primary                                   // level
				? VK_COMMAND_BUFFER_LEVEL_PRIMARY
				: VK_COMMAND_BUFFER_LEVEL_SECONDARY,
			1                                         // commandBufferCount
		};
		DEBUG_DUMP( cmdAllocInfo );
		auto res = AllocateCommandBuffers( m_device, &cmdAllocInfo, &m_commandBuffer );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "CommandBuffer creation failed: " + getLastError() };
		}
	}

	CommandBuffer::~CommandBuffer()
	{
		FreeCommandBuffers( m_device, m_pool, 1, &m_commandBuffer );
	}

	void CommandBuffer::copyImage( VkBufferImageCopy const & copyInfo
		, Buffer const & src
		, Texture const & dst )const
	{
		DEBUG_DUMP( copyInfo );
		CmdCopyBufferToImage( m_commandBuffer
			, src
			, dst
			, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			, 1
			, &copyInfo );
	}


	void CommandBuffer::copyImage( Texture const & src
		, Texture const & dst )const
	{
		auto const & srcRange = src.getView().getSubResourceRange();
		auto const & dstRange = dst.getView().getSubResourceRange();
		copyImage( VkImageCopy
			{
				{                                                   // srcSubresource
					VK_IMAGE_ASPECT_COLOR_BIT,                          // aspectMask
					srcRange.getBaseMipLevel(),                         // mipLevel
					srcRange.getBaseArrayLayer(),                       // baseArrayLayer
					srcRange.getLayerCount()                            // layerCount
				},
				{                                                   // srcOffset
					0,                                                  // x
					0,                                                  // y
					0                                                   // z
				},
				{                                                   // dstSubresource
					VK_IMAGE_ASPECT_COLOR_BIT,                          // aspectMask
					dstRange.getBaseMipLevel(),                         // mipLevel
					dstRange.getBaseArrayLayer(),                       // baseArrayLayer
					dstRange.getLayerCount()                            // layerCount
				},
				{                                                   // dstOffset
					0,                                                  // x
					0,                                                  // y
					0                                                   // z
				},
				{                                                   // extent
					uint32_t( dst.getDimensions().x ),                  // width
					uint32_t( dst.getDimensions().y ),                  // height
					1u                                                  // depth
				}
			}
			, src
			, dst );
	}

	void CommandBuffer::copyImage( VkImageCopy const & copyInfo
		, Texture const & src
		, Texture const & dst )const
	{
		DEBUG_DUMP( copyInfo );
		CmdCopyImage( m_commandBuffer
			, src
			, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
			, dst
			, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			, 1
			, &copyInfo );
	}

	bool CommandBuffer::begin( renderer::CommandBufferUsageFlags flags )const
	{
		VkCommandBufferBeginInfo cmdBufInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			convert( flags ),                            // flags
			nullptr                                      // pInheritanceInfo
		};
		DEBUG_DUMP( cmdBufInfo );
		auto res = BeginCommandBuffer( m_commandBuffer, &cmdBufInfo );
		m_currentPipeline = nullptr;
		return checkError( res );
	}

	bool CommandBuffer::begin( renderer::CommandBufferUsageFlags flags
		, renderer::RenderPass const & renderPass
		, uint32_t subpass
		, renderer::FrameBuffer const & frameBuffer
		, bool occlusionQueryEnable
		, renderer::QueryControlFlags queryFlags
		, renderer::QueryPipelineStatisticFlags pipelineStatistics )const
	{
		m_inheritanceInfo = VkCommandBufferInheritanceInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,  // sType;
			nullptr,                                            // pNext;
			static_cast< RenderPass const & >( renderPass ),    // renderPass;
			subpass,                                            // subpass;
			static_cast< FrameBuffer const & >( frameBuffer ),  // framebuffer;
			occlusionQueryEnable,                               // occlusionQueryEnable;
			queryFlags,                                         // queryFlags;
			pipelineStatistics                                  // pipelineStatistics;
		};
		VkCommandBufferBeginInfo cmdBufInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			flags,                                       // flags
			&m_inheritanceInfo                           // pInheritanceInfo
		};
		DEBUG_DUMP( cmdBufInfo );
		auto res = BeginCommandBuffer( m_commandBuffer, &cmdBufInfo );
		m_currentPipeline = nullptr;
		return checkError( res );
	}

	bool CommandBuffer::end()const
	{
		auto res = EndCommandBuffer( m_commandBuffer );
		m_currentPipeline = nullptr;
		return checkError( res );
	}

	bool CommandBuffer::reset( renderer::CommandBufferResetFlags flags )const
	{
		auto res = ResetCommandBuffer( m_commandBuffer, flags );
		return checkError( res );
	}

	void CommandBuffer::beginRenderPass( renderer::RenderPass const & renderPass
		, renderer::FrameBuffer const & frameBuffer
		, renderer::ClearValueArray const & clearValues
		, renderer::SubpassContents contents )const
	{
		auto & vkclearValues = static_cast< RenderPass const & >( renderPass ).getClearValues( clearValues );
		//auto vkclearValues = convert< VkClearValue >( clearValues );
		auto & vkfbo = static_cast< FrameBuffer const & >( frameBuffer );
		VkRenderPassBeginInfo beginInfo
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			nullptr,
			static_cast< RenderPass const & >( renderPass ),    // renderPass
			vkfbo,                                              // framebuffer
			{                                                   // renderArea
				{                                                 // offset
					0,                                                // x
					0                                                 // y
				},
				{                                                 // extent
					uint32_t( vkfbo.getDimensions().x ),              // width
					uint32_t( vkfbo.getDimensions().y ),              // height
				}
			},
			uint32_t( vkclearValues.size() ),                   // clearValueCount
			vkclearValues.data()                                // pClearValues
		};
		DEBUG_DUMP( beginInfo );
		CmdBeginRenderPass( m_commandBuffer
			, &beginInfo
			, convert( contents ) );
	}

	void CommandBuffer::nextSubpass( renderer::SubpassContents contents )const
	{
		CmdNextSubpass( m_commandBuffer, convert( contents ) );
	}

	void CommandBuffer::endRenderPass()const
	{
		CmdEndRenderPass( m_commandBuffer );
	}

	void CommandBuffer::executeCommands( renderer::CommandBufferCRefArray const & commands )const
	{
		auto vkCommands = makeVkArray< VkCommandBuffer >( convert( commands ) );
		CmdExecuteCommands( m_commandBuffer
			, uint32_t( vkCommands.size() )
			, vkCommands.data() );
	}

	void CommandBuffer::clear( renderer::Texture const & image
		, renderer::RgbaColour const & colour )const
	{
		auto vkcolour = convert( colour );
		auto vksubresourceRange = convert( static_cast< Texture const & >( image ).getView().getSubResourceRange() );
		CmdClearColorImage( m_commandBuffer
			, static_cast< Texture const & >( image )
			, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			, &vkcolour
			, 1
			, &vksubresourceRange );
	}

	void CommandBuffer::bindPipeline( renderer::Pipeline const & pipeline
		, renderer::PipelineBindPoint bindingPoint )const
	{
		CmdBindPipeline( m_commandBuffer
			, convert( bindingPoint )
			, static_cast< Pipeline const & >( pipeline ) );
		m_currentPipeline = &static_cast< Pipeline const & >( pipeline );
	}

	void CommandBuffer::bindGeometryBuffers( renderer::GeometryBuffers const & geometryBuffers )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
		std::vector< std::reference_wrapper< Buffer const > > buffers;
		std::vector< uint64_t > offsets;

		for ( auto & vbo : geometryBuffers.getVbos() )
		{
			buffers.emplace_back( static_cast< Buffer const & >( vbo.vbo.getBuffer() ) );
			offsets.emplace_back( vbo.offset );
		}

		CmdBindVertexBuffers( m_commandBuffer
			, 0u
			, uint32_t( buffers.size() )
			, makeVkArray< VkBuffer >( buffers ).data()
			, offsets.data() );

		if ( geometryBuffers.hasIbo() )
		{
			auto & ibo = geometryBuffers.getIbo();
			CmdBindIndexBuffer( m_commandBuffer
				, static_cast< Buffer const & >( ibo.buffer )
				, ibo.offset
				, convert( ibo.type ) );
		}
	}

	void CommandBuffer::memoryBarrier( renderer::PipelineStageFlags after
		, renderer::PipelineStageFlags before
		, renderer::BufferMemoryBarrier const & transitionBarrier )const
	{
		auto vkafter = convert( after );
		auto vkbefore = convert( before );
		auto vktb = convert( transitionBarrier );
		CmdPipelineBarrier( m_commandBuffer
			, vkbefore
			, vkafter
			, 0
			, 0u
			, nullptr
			, 1u
			, &vktb
			, 0u
			, nullptr );
	}

	void CommandBuffer::memoryBarrier( renderer::PipelineStageFlags after
		, renderer::PipelineStageFlags before
		, renderer::ImageMemoryBarrier const & transitionBarrier )const
	{
		auto vkafter = convert( after );
		auto vkbefore = convert( before );
		auto vktb = convert( transitionBarrier );
		CmdPipelineBarrier( m_commandBuffer
			, vkbefore
			, vkafter
			, 0
			, 0u
			, nullptr
			, 0u
			, nullptr
			, 1u
			, &vktb );
	}

	void CommandBuffer::bindDescriptorSet( renderer::DescriptorSet const & descriptorSet
		, renderer::PipelineLayout const & layout
		, renderer::PipelineBindPoint bindingPoint )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
		VkDescriptorSet set{ static_cast< DescriptorSet const & >( descriptorSet ) };
		CmdBindDescriptorSets( m_commandBuffer
			, convert( bindingPoint )
			, static_cast< PipelineLayout const & >( layout )
			, 0u
			, 1u
			, &set
			, 0u
			, nullptr );
	}

	void CommandBuffer::setViewport( renderer::Viewport const & viewport )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
		auto vkviewport = convert( viewport );
		CmdSetViewport( m_commandBuffer
			, 0u
			, 1u
			, &vkviewport );
	}

	void CommandBuffer::setScissor( renderer::Scissor const & scissor )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
		auto vkscissor = convert( scissor );
		CmdSetScissor( m_commandBuffer
			, 0u
			, 1u
			, &vkscissor );
	}

	void CommandBuffer::draw( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
		CmdDraw( m_commandBuffer
			, vtxCount
			, instCount
			, firstVertex
			, firstInstance );
	}

	void CommandBuffer::drawIndexed( uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance )const
	{
		assert( m_currentPipeline && "No pipeline bound." );
		CmdDrawIndexed( m_commandBuffer
			, indexCount
			, instCount
			, firstIndex
			, vertexOffset
			, firstInstance );
	}

	void CommandBuffer::copyBuffer( renderer::BufferBase const & src
		, renderer::BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		VkBufferCopy copyInfo
		{
			0,                                                // srcOffset
			offset,                                           // dstOffset
			size                                              // size
		};
		DEBUG_DUMP( copyInfo );
		CmdCopyBuffer( m_commandBuffer
			, static_cast< Buffer const & >( src )
			, static_cast< Buffer const & >( dst )
			, 1u
			, &copyInfo );
	}

	void CommandBuffer::copyBuffer( renderer::BufferBase const & src
		, renderer::VertexBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		copyBuffer( src
			, dst.getBuffer()
			, size
			, offset );
	}

	void CommandBuffer::copyBuffer( renderer::VertexBufferBase const & src
		, renderer::BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		copyBuffer( src.getBuffer()
			, dst
			, size
			, offset );
	}

	void CommandBuffer::copyBuffer( renderer::VertexBufferBase const & src
		, renderer::VertexBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		copyBuffer( src.getBuffer()
			, dst.getBuffer()
			, size
			, offset );
	}

	void CommandBuffer::copyBuffer( renderer::BufferBase const & src
		, renderer::UniformBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		copyBuffer( src
			, dst.getBuffer()
			, size
			, offset );
	}

	void CommandBuffer::copyBuffer( renderer::UniformBufferBase const & src
		, renderer::BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		copyBuffer( src.getBuffer()
			, dst
			, size
			, offset );
	}

	void CommandBuffer::copyBuffer( renderer::UniformBufferBase const & src
		, renderer::UniformBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		copyBuffer( src.getBuffer()
			, dst.getBuffer()
			, size
			, offset );
	}

	void CommandBuffer::copyImage( renderer::BufferBase const & src
		, renderer::Texture const & dst )const
	{
		auto const & texture = static_cast< Texture const & >( dst );
		auto const & buffer = static_cast< Buffer const & >( src );
		auto const & range = texture.getView().getSubResourceRange();
		copyImage( VkBufferImageCopy
			{
				0,                                                  // bufferOffset
				0,                                                  // bufferRowLength
				0,                                                  // bufferImageHeight
				{                                                   // imageSubresource
					VK_IMAGE_ASPECT_COLOR_BIT,                          // aspectMask
					range.getBaseMipLevel(),                            // mipLevel
					range.getBaseArrayLayer(),                          // baseArrayLayer
					range.getLayerCount()                               // layerCount
				},
				{                                                   // imageOffset
					0,                                                  // x
					0,                                                  // y
					0                                                   // z
				},
				{                                                   // imageExtent
					uint32_t( dst.getDimensions().x ),                  // width
					uint32_t( dst.getDimensions().y ),                  // height
					1u                                                  // depth
				}
			}
			, buffer
			, texture );
	}

	void CommandBuffer::copyImage( renderer::StagingBuffer const & src
		, renderer::Texture const & dst )const
	{
		copyImage( src.getBuffer()
			, dst );
	}
}
