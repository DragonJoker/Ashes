/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/GlQueue.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Command/Commands/GlCommandBase.hpp"
#include "Core/GlDevice.hpp"
#include "Sync/GlFence.hpp"
#include "Sync/GlSemaphore.hpp"
#include "Core/GlSwapChain.hpp"

namespace gl_renderer
{
	Queue::Queue( Device const & device
		, ashes::DeviceQueueCreateInfo createInfo
		, uint32_t index )
		: ashes::Queue{ device, std::move( createInfo ), index }
		, m_device{ device }
	{
	}

	void Queue::submit( ashes::CommandBufferCRefArray const & commandBuffers
		, ashes::SemaphoreCRefArray const & semaphoresToWait
		, ashes::PipelineStageFlagsArray const & semaphoresStage
		, ashes::SemaphoreCRefArray const & semaphoresToSignal
		, ashes::Fence const * fence )const
	{
		auto context = m_device.getContext();

		for ( auto & commandBuffer : commandBuffers )
		{
			auto & glCommandBuffer = static_cast< CommandBuffer const & >( commandBuffer.get() );
			glCommandBuffer.initialiseGeometryBuffers();

			for ( auto & command : glCommandBuffer.getCommands() )
			{
				command->apply( context );
			}

			glCommandBuffer.applyPostSubmitActions( context );
		}
	}

	ashes::ResultArray Queue::present( ashes::SwapChainCRefArray const & swapChains
		, ashes::UInt32Array const & imagesIndex
		, ashes::SemaphoreCRefArray const & semaphoresToWait )const
	{
		ashes::ResultArray result{ swapChains.size(), ashes::Result::eSuccess };

		for ( auto & swapChain : swapChains )
		{
			static_cast< Device const & >( swapChain.get().getDevice() ).swapBuffers();
		}

		return result;
	}

	void Queue::waitIdle()const
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glFinish );
	}
}
