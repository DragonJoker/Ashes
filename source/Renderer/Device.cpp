/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "Device.hpp"

#include "Renderer.hpp"
#include "SwapChain.hpp"

namespace renderer
{
	Device::Device( Renderer const & renderer
		, Connection & connection )
		: m_device{ renderer.getInstance()
			, renderer.getPhysicalDevice()
			, std::move( connection.getConnection() ) }
	{
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
}
