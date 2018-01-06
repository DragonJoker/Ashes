#include "VkSwapChain.hpp"

#include "VkCommandBuffer.hpp"
#include "VkDevice.hpp"
#include "VkFrameBuffer.hpp"
#include "VkRenderPass.hpp"
#include "VkSemaphore.hpp"
#include "VkTexture.hpp"

#include <VkLib/BackBuffer.hpp>
#include <VkLib/LogicalDevice.hpp>

namespace vk_renderer
{
	SwapChain::SwapChain( renderer::Device const & device
		, utils::IVec2 const & size )
		: renderer::SwapChain{ device, size }
		, m_swapChain{ static_cast< Device const & >( device ).getDevice().createSwapChain( size.x, size.y ) }
	{
		m_renderingResources.resize( 3 );

		for ( auto & resource : m_renderingResources )
		{
			resource = std::make_unique< RenderingResources >( device );
		}
	}

	void SwapChain::reset( utils::IVec2 const & size )
	{
		m_dimensions = size;
		doResetSwapChain();
	}

	renderer::FrameBufferPtrArray SwapChain::createFrameBuffers( renderer::RenderPass const & renderPass )const
	{
		auto frameBuffers = m_swapChain->createFrameBuffers( static_cast< RenderPass const & >( renderPass ).getRenderPass() );
		renderer::FrameBufferPtrArray result;
		result.reserve( frameBuffers.size() );

		for ( auto & frameBuffer : frameBuffers )
		{
			result.emplace_back( std::make_shared< FrameBuffer >( std::move( frameBuffer ) ) );
		}

		return result;
	}

	renderer::CommandBufferPtrArray SwapChain::createCommandBuffers()const
	{
		auto commandBuffers = m_swapChain->createCommandBuffers();
		renderer::CommandBufferPtrArray result;
		result.reserve( commandBuffers.size() );

		for ( auto & commandBuffer : commandBuffers )
		{
			result.emplace_back( std::make_unique< CommandBuffer >( std::move( commandBuffer ) ) );
		}

		return result;
	}

	void SwapChain::preRenderCommands( uint32_t index
		, renderer::CommandBuffer const & commandBuffer )const
	{
		m_swapChain->preRenderCommands( index, static_cast< CommandBuffer const & >( commandBuffer ).getCommandBuffer() );
	}

	void SwapChain::postRenderCommands( uint32_t index
		, renderer::CommandBuffer const & commandBuffer )const
	{
		m_swapChain->postRenderCommands( index, static_cast< CommandBuffer const & >( commandBuffer ).getCommandBuffer() );
	}

	renderer::RenderingResources * SwapChain::getResources()
	{
		auto & resources = *m_renderingResources[m_resourceIndex];
		m_resourceIndex = ( m_resourceIndex + 1 ) % m_renderingResources.size();

		if ( resources.waitRecord( vk::FenceTimeout ) )
		{
			uint32_t backBuffer{ 0u };
			auto res = m_swapChain->acquireBackBuffer( static_cast< Semaphore const & >( resources.getImageAvailableSemaphore() ).getSemaphore()
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

	void SwapChain::present( renderer::RenderingResources & resources )
	{
		auto res = m_swapChain->presentBackBuffer( resources.getBackBuffer()
			, static_cast< Semaphore const & >( resources.getRenderingFinishedSemaphore() ).getSemaphore() );
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
		static_cast< Device const & >( m_device ).getDevice().waitIdle();
		auto colour = m_swapChain->getClearColour();
		m_swapChain.reset();
		m_swapChain = static_cast< Device const & >( m_device ).getDevice().createSwapChain( m_dimensions.x, m_dimensions.y );
		m_swapChain->setClearColour( colour );
		onReset();
	}
}
