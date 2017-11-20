#include "RenderingResources.hpp"

#include <VkLib/CommandPool.hpp>
#include <VkLib/Image.hpp>
#include <VkLib/LogicalDevice.hpp>
#include <VkLib/Queue.hpp>
#include <VkLib/StagingBuffer.hpp>

namespace renderer
{
	RenderingResources::RenderingResources( vk::LogicalDevice const & device
		, vk::CommandPool const & commandPool )
		: m_device{ device }
		, m_commandBuffer{ commandPool.createCommandBuffer() }
		, m_imageAvailableSemaphore{ device.createSemaphore() }
		, m_finishedRenderingSemaphore{ device.createSemaphore() }
		, m_fence{ device, VK_FENCE_CREATE_SIGNALED_BIT }
		, m_stagingBuffer{ device.createStagingBuffer( 1000000, 0 ) }
	{
	}

	bool RenderingResources::waitRecord( uint32_t timeout )
	{
		bool res = m_fence.wait( timeout ) == VK_SUCCESS;

		if ( res )
		{
			m_fence.reset();
		}

		return res;
	}

	void RenderingResources::copyTextureData( vk::ByteArray const & data
		, vk::Image & texture )const
	{
		doCopyToStagingBuffer( data.data()
			, uint32_t( data.size() ) );
		auto & commandBuffer = getCommandBuffer();

		if ( commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) )
		{
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, VK_PIPELINE_STAGE_TRANSFER_BIT
				, texture.makeTransferDestination() );
			commandBuffer.copyImage( m_stagingBuffer->getBuffer()
				, texture );
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
				, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
				, texture.makeShaderInputResource() );
			auto res = commandBuffer.end();

			if ( !vk::checkError( res ) )
			{
				throw std::runtime_error{ "Texture data copy failed: " + vk::getLastError() };
			}

			res = m_device.getGraphicsQueue().submit( { commandBuffer }
				, {}
				, {}
				, {}
			, nullptr );

			if ( !vk::checkError( res ) )
			{
				throw std::runtime_error{ "Texture data copy failed: " + vk::getLastError() };
			}

			m_device.waitIdle();
		}
	}

	void RenderingResources::doCopyToStagingBuffer( uint8_t const * data
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
}
