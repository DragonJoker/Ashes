#include "VkStagingBuffer.hpp"

#include "VkBufferMemoryBarrier.hpp"
#include "VkCommandBuffer.hpp"
#include "VkDevice.hpp"
#include "VkImageMemoryBarrier.hpp"
#include "VkTexture.hpp"

#include <VkLib/Image.hpp>
#include <VkLib/LogicalDevice.hpp>
#include <VkLib/Queue.hpp>
#include <VkLib/StagingBuffer.hpp>

#include <cstring>

namespace vk_renderer
{
	StagingBuffer::StagingBuffer( renderer::Device const & device
		, uint32_t size )
		: renderer::StagingBuffer{ device, size }
	{
	}

	void StagingBuffer::copyTextureData( renderer::CommandBuffer const & commandBuffer
		, vk::ByteArray const & data
		, renderer::Texture const & texture )const
	{
		doCopyToStagingBuffer( data.data()
			, uint32_t( data.size() ) );

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTopOfPipe
				, renderer::PipelineStageFlag::eTransfer
				, texture.makeTransferDestination() );
			commandBuffer.copyImage( *this
				, texture );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTransfer
				, renderer::PipelineStageFlag::eFragmentShader
				, texture.makeShaderInputResource() );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Texture data copy failed: " + vk::getLastError() };
			}

			res = m_device.getGraphicsQueue().submit( commandBuffer
				, nullptr );

			if ( !res )
			{
				throw std::runtime_error{ "Texture data copy failed: " + vk::getLastError() };
			}

			m_device.getGraphicsQueue().waitIdle();
		}
	}

	void StagingBuffer::doCopyToStagingBuffer( uint8_t const * data
		, uint32_t size )const
	{
		auto buffer = static_cast< BufferBase const & >( getBuffer() ).getBuffer().lock( 0
			, size
			, 0 );

		if ( !buffer )
		{
			throw std::runtime_error{ "Staging buffer storage memory mapping failed." };
		}

		std::memcpy( buffer
			, data
			, size );
		static_cast< BufferBase const & >( getBuffer() ).getBuffer().unlock( size
			, true );
	}

	void StagingBuffer::doCopyFromStagingBuffer( renderer::CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, renderer::BufferBase const & buffer )const
	{
		auto & cb = static_cast< CommandBuffer const & >( commandBuffer );

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			cb.getCommandBuffer().copyBuffer( static_cast< BufferBase const & >( getBuffer() ).getBuffer()
				, static_cast< BufferBase const & >( buffer ).getBuffer()
				, size
				, offset );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed: " + vk::getLastError() };
			}

			res = m_device.getGraphicsQueue().submit( commandBuffer
				, nullptr );

			if ( !res )
			{
				throw std::runtime_error{ "Texture data copy failed: " + vk::getLastError() };
			}

			m_device.getGraphicsQueue().waitIdle();
		}
	}

	void StagingBuffer::doCopyFromStagingBuffer( renderer::CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, renderer::VertexBufferBase const & buffer
		, renderer::PipelineStageFlags const & flags )const
	{
		auto & cb = static_cast< CommandBuffer const & >( commandBuffer );

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			cb.getCommandBuffer().copyBuffer( static_cast< BufferBase const & >( getBuffer() ).getBuffer()
				, static_cast< BufferBase const & >( buffer.getBuffer() ).getBuffer()
				, size
				, offset );
			cb.getCommandBuffer().memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
				, convert( flags )
				, static_cast< BufferBase const & >( buffer.getBuffer() ).getBuffer().makeVertexShaderInputResource() );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed: " + vk::getLastError() };
			}

			res = m_device.getGraphicsQueue().submit( commandBuffer
				, nullptr );

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed: " + vk::getLastError() };
			}

			m_device.getGraphicsQueue().waitIdle();
		}
	}

	void StagingBuffer::doCopyFromStagingBuffer( renderer::CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, renderer::UniformBufferBase const & buffer
		, renderer::PipelineStageFlags const & flags )const
	{
		auto & cb = static_cast< CommandBuffer const & >( commandBuffer );

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			cb.getCommandBuffer().copyBuffer( static_cast< BufferBase const & >( getBuffer() ).getBuffer()
				, static_cast< BufferBase const & >( buffer.getBuffer() ).getBuffer()
				, size
				, offset );
			cb.getCommandBuffer().memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
				, convert( flags )
				, static_cast< BufferBase const & >( buffer.getBuffer() ).getBuffer().makeUniformBufferInput() );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed: " + vk::getLastError() };
			}

			res = m_device.getGraphicsQueue().submit( commandBuffer
				, nullptr );

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed: " + vk::getLastError() };
			}

			m_device.getGraphicsQueue().waitIdle();
		}
	}
}
