#include "SwapChain.hpp"

#include "Device.hpp"
#include "RenderPass.hpp"
#include "Texture.hpp"

#include <VkLib/BackBuffer.hpp>
#include <VkLib/LogicalDevice.hpp>

namespace renderer
{
	SwapChain::SwapChain( Device const & device
		, utils::IVec2 const & size )
		: m_device{ device }
		, m_dimensions{ size }
		, m_swapChain{ device.getDevice().createSwapChain( size.x, size.y ) }
	{
		m_renderingResources.resize( 3 );

		for ( auto & resource : m_renderingResources )
		{
			resource = std::make_unique< renderer::RenderingResources >( device );
		}
	}

	void SwapChain::reset( utils::IVec2 const & size )
	{
		m_dimensions = size;
		doResetSwapChain();
	}

	FrameBufferPtrArray SwapChain::createFrameBuffers( RenderPass const & renderPass )const
	{
		auto frameBuffers = m_swapChain->createFrameBuffers( renderPass.getRenderPass() );
		FrameBufferPtrArray result;
		result.reserve( frameBuffers.size() );

		for ( auto & frameBuffer : frameBuffers )
		{
			result.emplace_back( std::make_shared< FrameBuffer >( std::move( frameBuffer ) ) );
		}

		return result;
	}

	CommandBufferPtrArray SwapChain::createCommandBuffers()const
	{
		auto commandBuffers = m_swapChain->createCommandBuffers();
		CommandBufferPtrArray result;
		result.reserve( commandBuffers.size() );

		for ( auto & commandBuffer : commandBuffers )
		{
			result.emplace_back( std::make_unique< CommandBuffer >( std::move( commandBuffer ) ) );
		}

		return result;
	}

	void SwapChain::preRenderCommands( uint32_t index
		, CommandBuffer const & commandBuffer )const
	{
		m_swapChain->preRenderCommands( index, commandBuffer.getCommandBuffer() );
	}

	void SwapChain::postRenderCommands( uint32_t index
		, CommandBuffer const & commandBuffer )const
	{
		m_swapChain->postRenderCommands( index, commandBuffer.getCommandBuffer() );
	}

	RenderingResources * SwapChain::getResources()
	{
		auto & resources = *m_renderingResources[m_resourceIndex];
		m_resourceIndex = ( m_resourceIndex + 1 ) % m_renderingResources.size();

		if ( resources.waitRecord( vk::FenceTimeout ) )
		{
			uint32_t backBuffer{ 0u };
			auto res = m_swapChain->acquireBackBuffer( resources.getImageAvailableSemaphore().getSemaphore()
				, backBuffer );

			if ( doCheckNeedReset( res, true, "Swap chain image acquisition" ) )
			{
				resources.setBackBuffer( backBuffer );
				return &resources;
			}

			return nullptr;
		}

		std::cerr << "Can't render: " << vk::getLastError() << std::endl;
		return nullptr;
	}

	void SwapChain::present( RenderingResources & resources )
	{
		auto res = m_swapChain->presentBackBuffer( resources.getBackBuffer()
			, resources.getRenderingFinishedSemaphore().getSemaphore() );
		doCheckNeedReset( res, false, "Image presentation" );
		resources.setBackBuffer( ~0u );
	}

	bool SwapChain::doCheckNeedReset( VkResult errCode
		, bool acquisition
		, char const * const action )
	{
		bool result{ false };

		switch ( errCode )
		{
		case VK_SUCCESS:
			result = true;
			break;

		case VK_ERROR_OUT_OF_DATE_KHR:
			if ( !acquisition )
			{
				doResetSwapChain();
			}
			else
			{
				result = true;
			}
			break;

		case VK_SUBOPTIMAL_KHR:
			doResetSwapChain();
			break;

		default:
			std::cerr << action << " failed: " << vk::getLastError() << std::endl;
			break;
		}

		return result;
	}

	void SwapChain::doResetSwapChain()
	{
		m_device.getDevice().waitIdle();
		auto colour = m_swapChain->getClearColour();
		m_swapChain.reset();
		m_swapChain = m_device.getDevice().createSwapChain( m_dimensions.x, m_dimensions.y );
		m_swapChain->setClearColour( colour );
		onReset();
	}
}
