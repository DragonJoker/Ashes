#include "StagingBuffer.hpp"

#include "BufferMemoryBarrier.hpp"
#include "ImageMemoryBarrier.hpp"
#include "RenderingResources.hpp"
#include "Texture.hpp"

#include <VkLib/Image.hpp>
#include <VkLib/LogicalDevice.hpp>
#include <VkLib/Queue.hpp>
#include <VkLib/StagingBuffer.hpp>

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

	void StagingBuffer::copyTextureData( CommandBuffer const & cb
		, vk::ByteArray const & data
		, Texture const & texture )const
	{
		doCopyToStagingBuffer( cb
			, data.data()
			, uint32_t( data.size() ) );
		auto & commandBuffer = cb.getCommandBuffer();

		if ( commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) )
		{
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, VK_PIPELINE_STAGE_TRANSFER_BIT
				, m_stagingBuffer->getBuffer().makeTransferSource() );
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, VK_PIPELINE_STAGE_TRANSFER_BIT
				, texture.getImage().makeTransferDestination() );
			commandBuffer.copyImage( m_stagingBuffer->getBuffer()
				, texture.getImage() );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Texture data copy failed: " + vk::getLastError() };
			}

			res = vk::checkError( m_device.getDevice().getGraphicsQueue().submit( { commandBuffer }
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

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & cb
		, uint8_t const * data
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

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & cb
		, uint32_t size
		, uint32_t offset
		, vk::Buffer const & buffer )const
	{
		auto & commandBuffer = cb.getCommandBuffer();

		if ( commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) )
		{
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, VK_PIPELINE_STAGE_TRANSFER_BIT
				, m_stagingBuffer->getBuffer().makeTransferSource() );
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, VK_PIPELINE_STAGE_TRANSFER_BIT
				, buffer.makeTransferDestination() );
			commandBuffer.copyBuffer( m_stagingBuffer->getBuffer()
				, buffer
				, size
				, offset );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed: " + vk::getLastError() };
			}

			res = vk::checkError( m_device.getDevice().getGraphicsQueue().submit( { commandBuffer }
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
