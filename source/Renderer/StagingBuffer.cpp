#include "StagingBuffer.hpp"

#include "BufferMemoryBarrier.hpp"
#include "ImageMemoryBarrier.hpp"
#include "RenderingResources.hpp"
#include "Texture.hpp"

#include <VkLib/Image.hpp>
#include <VkLib/LogicalDevice.hpp>
#include <VkLib/Queue.hpp>
#include <VkLib/StagingBuffer.hpp>

#include <cstring>

namespace renderer
{
	StagingBuffer::StagingBuffer( Device const & device )
		: m_device{ device }
		, m_stagingBuffer{ device.getDevice().createStagingBuffer( 1000000, 0 ) }
	{
	}

	BufferMemoryBarrier StagingBuffer::makeTransferSource()const
	{
		return BufferMemoryBarrier
		{
			m_currentAccessMask,                              // srcAccessMask
			AccessFlag::eTransferRead,                        // dstAccessMask
			VK_QUEUE_FAMILY_IGNORED,                          // srcQueueFamilyIndex
			VK_QUEUE_FAMILY_IGNORED,                          // dstQueueFamilyIndex
			*this,                                            // buffer
			0,                                                // offset
			VK_WHOLE_SIZE                                     // size
		};
	}

	void StagingBuffer::copyTextureData( CommandBuffer const & commandBuffer
		, vk::ByteArray const & data
		, Texture const & texture )const
	{
		doCopyToStagingBuffer( data.data()
			, uint32_t( data.size() ) );

		if ( commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.memoryBarrier( PipelineStageFlag::eTopOfPipe
				, PipelineStageFlag::eTransfer
				, texture.makeTransferDestination() );
			commandBuffer.copyImage( *this
				, texture );
			commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
				, PipelineStageFlag::eFragmentShader
				, texture.makeShaderInputResource() );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Texture data copy failed: " + vk::getLastError() };
			}

			res = vk::checkError( m_device.getDevice().getGraphicsQueue().submit( { commandBuffer.getCommandBuffer() }
				, {}
				, {}
				, {}
			, nullptr ) );

			if ( !res )
			{
				throw std::runtime_error{ "Texture data copy failed: " + vk::getLastError() };
			}

			m_device.getDevice().waitIdle();
		}
	}

	void StagingBuffer::doCopyToStagingBuffer( uint8_t const * data
		, uint32_t size )const
	{
		auto buffer = m_stagingBuffer->getBuffer().lock( 0
			, size
			, 0 );

		if ( !buffer )
		{
			throw std::runtime_error{ "Staging buffer storage memory mapping failed." };
		}

		std::memcpy( buffer
			, data
			, size );
		m_stagingBuffer->getBuffer().unlock( size
			, true );
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, vk::Buffer const & buffer )const
	{
		if ( commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.getCommandBuffer().copyBuffer( m_stagingBuffer->getBuffer()
				, buffer
				, size
				, offset );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed: " + vk::getLastError() };
			}

			res = vk::checkError( m_device.getDevice().getGraphicsQueue().submit( { commandBuffer.getCommandBuffer() }
				, {}
				, {}
				, {}
			, nullptr ) );

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed: " + vk::getLastError() };
			}

			m_device.getDevice().waitIdle();
		}
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, vk::VertexBuffer const & buffer
		, PipelineStageFlags const & flags )const
	{
		if ( commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.getCommandBuffer().copyBuffer( m_stagingBuffer->getBuffer()
				, buffer.getBuffer()
				, size
				, offset );
			commandBuffer.getCommandBuffer().memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
				, convert( flags )
				, buffer.makeVertexShaderInputResource() );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed: " + vk::getLastError() };
			}

			res = vk::checkError( m_device.getDevice().getGraphicsQueue().submit( { commandBuffer.getCommandBuffer() }
				, {}
				, {}
				, {}
			, nullptr ) );

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed: " + vk::getLastError() };
			}

			m_device.getDevice().waitIdle();
		}
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, vk::UniformBuffer const & buffer
		, PipelineStageFlags const & flags )const
	{
		if ( commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.getCommandBuffer().copyBuffer( m_stagingBuffer->getBuffer()
				, buffer.getBuffer()
				, size
				, offset );
			commandBuffer.getCommandBuffer().memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
				, convert( flags )
				, buffer.makeUniformBufferInput() );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed: " + vk::getLastError() };
			}

			res = vk::checkError( m_device.getDevice().getGraphicsQueue().submit( { commandBuffer.getCommandBuffer() }
				, {}
				, {}
				, {}
			, nullptr ) );

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed: " + vk::getLastError() };
			}

			m_device.getDevice().waitIdle();
		}
	}
}
