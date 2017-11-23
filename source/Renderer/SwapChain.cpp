#include "SwapChain.hpp"

#include "Device.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace renderer
{
	SwapChain::SwapChain( Device const & device
		, utils::IVec2 const & size )
		: m_device{ device }
		, m_dimensions{ size }
		, m_swapChain{ device.getDevice().createSwapChain( size.x, size.y ) }
	{
		m_swapChain->setClearColour( VkClearColorValue{ { 1.0f, 0.8f, 0.4f, 0.0f } } );
		m_renderingResources.resize( 3 );

		for ( auto & resource : m_renderingResources )
		{
			resource = std::make_unique< renderer::RenderingResources >( device );
		}
	}

	RenderingResources * SwapChain::getResources()
	{
		auto & resources = *m_renderingResources[m_resourceIndex];
		m_resourceIndex = ( m_resourceIndex + 1 ) % m_renderingResources.size();

		if ( resources.waitRecord( vk::FenceTimeout ) )
		{
			auto res = m_swapChain->acquireBackBuffer( resources.getImageAvailableSemaphore()
				, m_backBuffer );

			if ( doCheckNeedReset( res, true, "Swap chain image acquisition" ) )
			{
				return &resources;
			}

			return nullptr;
		}

		std::cerr << "Can't render: " << vk::getLastError() << std::endl;
		return nullptr;
	}

	void SwapChain::present( RenderingResources & resources )
	{
		auto res = m_device.getDevice().submitToGraphicsQueue( resources.getCommandBuffer().getCommandBuffer()
			, resources.getImageAvailableSemaphore()
			, resources.getRenderingFinishedSemaphore()
			, &resources.getFence() );

		if ( vk::checkError( res ) )
		{
			res = m_swapChain->presentBackBuffer( *m_backBuffer
				, resources.getRenderingFinishedSemaphore() );
			doCheckNeedReset( res, false, "Image presentation" );
		}

		m_backBuffer = nullptr;
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
		m_swapChain.reset();
		m_swapChain = m_device.getDevice().createSwapChain( m_dimensions.x, m_dimensions.y );
	}
}
