/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "Device.hpp"

#include "Renderer.hpp"
#include "SwapChain.hpp"

namespace renderer
{
	Device::Device( Renderer const & renderer
		, Connection && connection )
		: m_device{ renderer.getInstance()
			, renderer.getPhysicalDevice()
			, std::move( connection.getConnection() ) }
		, m_presentQueue{ m_device.getPresentQueue() }
		, m_graphicsQueue{ m_device.getGraphicsQueue() }
		, m_presentCommandPool{ *this, m_device.getPresentCommandPool() }
		, m_graphicsCommandPool{ *this, m_device.getGraphicsCommandPool() }
	{
		m_version = "Vulkan 1.0.0";
	}

	SwapChainPtr Device::createSwapChain( utils::IVec2 const & size )const
	{
		SwapChainPtr result;

		try
		{
			result = std::make_unique< SwapChain >( *this, size );
		}
		catch ( std::exception & exc )
		{
			std::cerr << "Could not create the swap chain:\n" << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Could not create the swap chain:\nUnknown error" << std::endl;
		}

		return result;
	}

	void Device::waitIdle()const
	{
		m_device.waitIdle();
	}
}
